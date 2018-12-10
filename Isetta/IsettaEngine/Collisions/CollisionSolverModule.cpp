/*
 * Copyright (c) 2018 Isetta
 */

#include "CollisionSolverModule.h"
#include "Collisions/CollisionUtil.h"
#include "Collisions/CollisionsModule.h"
#include "Scene/Entity.h"

#include "Collisions/BoxCollider.h"
#include "Collisions/CapsuleCollider.h"
#include "Collisions/Collider.h"
#include "Collisions/SphereCollider.h"

#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"

#include "Core/Debug/DebugDraw.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {

using Collision = CollisionSolverModule::Collision;

CollisionsModule* CollisionSolverModule::collisionsModule{nullptr};

void CollisionSolverModule::StartUp(){};
void CollisionSolverModule::ShutDown(){};

Collision CollisionSolverModule::Solve(Collider* collider, Collider* other) {
  PROFILE
  Collision collision;

  Math::Vector3 point;
  bool otherOnEdge = false;

  // Project the "center point" to the appropriate position depending on the
  // other collider
  switch (other->GetType()) {
    case Collider::ColliderType::BOX: {
      BoxCollider* box = static_cast<BoxCollider*>(other);

      Math::Vector3 p =
          box->transform->LocalPosFromWorldPos(collider->GetWorldCenter()) -
          box->center;  // TODO(Caleb): Check if this is supposed
                        // to be converted from world space

      Math::Vector3 extents = box->size * .5;
      Math::Vector3 scale = box->transform->GetWorldScale();

      int numEdges = 0;
      int maxAxis = 0;
      int minAxis = 0;
      float maxDist = (extents[0] - Math::Util::Abs(p[0])) * scale[0];
      float minDist = (extents[0] - Math::Util::Abs(p[0])) * scale[0];

      for (int i = 0; i < Math::Vector3::ELEMENT_COUNT; ++i) {
        if ((extents[i] - Math::Util::Abs(p[i])) * scale[i] < minDist) {
          minDist = (extents[i] - Math::Util::Abs(p[i])) * scale[i];
          minAxis = i;
        }

        if ((extents[i] - Math::Util::Abs(p[i])) * scale[i] > maxDist) {
          maxDist = (extents[i] - Math::Util::Abs(p[i])) * scale[i];
          maxAxis = i;
        }

        if (Math::Util::Abs(p[i]) >= extents[i]) ++numEdges;
      }

      // If we're hitting an edge, then we should push based off of object
      // center anyways
      if (numEdges > 1) {
        Math::Vector3 multiplier =
            Math::Vector3{1.0f - (maxAxis == 0), 1.0f - (maxAxis == 1),
                          1.0f - (maxAxis == 2)};
        Math::Vector3 nMultiplier = Math::Vector3{
            1.0f - multiplier[0], 1.0f - multiplier[1], 1.0f - multiplier[2]};

        point = Math::Vector3::Scale(
            box->GetWorldCenter(),
            multiplier) + Math::Vector3::Scale(collider->GetWorldCenter(), nMultiplier);
      } else {
        Math::Vector3 axis = box->transform->GetAxis(minAxis);
        axis.x = Math::Util::Abs(axis.x);
        axis.y = Math::Util::Abs(axis.y);
        axis.z = Math::Util::Abs(axis.z);
        point = collider->GetWorldCenter() +
                Math::Vector3::Scale(
                    box->GetWorldCenter() - collider->GetWorldCenter(), axis);
      }
      break;
    }
    default: {
      point = other->GetWorldCenter();
      break;
    }
  }

  switch (collider->GetType()) {
    case Collider::ColliderType::BOX: {
      BoxCollider* box = static_cast<BoxCollider*>(collider);

      Math::Vector3 p = box->transform->LocalPosFromWorldPos(point) -
                        box->center;  // TODO(Caleb): Check if this is supposed
                                      // to be converted from world space

      Math::Vector3 extents = box->size * .5;
      Math::Vector3 hitPoint = Math::Vector3::zero;
      Math::Vector3 scale = box->transform->GetWorldScale();

      int numEdges = 0;
      int minAxis = 0;
      float minDist = (extents[0] - Math::Util::Abs(p[0])) * scale[0];

      for (int i = 0; i < Math::Vector3::ELEMENT_COUNT; ++i) {
        hitPoint[i] =
            Math::Util::Min(Math::Util::Max(p[i], -extents[i]), extents[i]);

        if ((extents[i] - Math::Util::Abs(p[i])) * scale[i] < minDist) {
          minDist = (extents[i] - Math::Util::Abs(p[i])) * scale[i];
          minAxis = i;
        }

        if (Math::Util::Abs(hitPoint[i]) >= extents[i]) ++numEdges;
      }

      hitPoint[minAxis] =
          Math::Util::Sign(hitPoint[minAxis]) * extents[minAxis];

      collision.hitPoint =
          box->transform->WorldPosFromLocalPos(hitPoint + box->center);
      collision.pushDir = Math::Vector3::zero;
      collision.pushDir[minAxis] = 1;
      collision.pushDir =
          box->transform->WorldDirFromLocalDir(collision.pushDir).Normalized();
      collision.onEdge = numEdges > 1;
      break;
    }
    case Collider::ColliderType::CAPSULE: {
      CapsuleCollider* capsule = static_cast<CapsuleCollider*>(collider);

      // Get the line segment of the capsule
      Math::Matrix4 rot, scale;
      Math::Vector3 cp0, cp1, cdir;
      float radiusScale = capsule->GetWorldCapsule(&rot, &scale);
      cdir = (Math::Vector3)(rot * scale *
                             Math::Matrix4::Scale(Math::Vector3{
                                 capsule->height - 2 * capsule->radius}) *
                             Math::Vector4{0, 1, 0, 0}) *
             .5;
      cp0 = capsule->GetWorldCenter() - cdir;
      cp1 = capsule->GetWorldCenter() + cdir;

      // Get the closest point on the segment to our first collider's
      // center then push that point out
      float t;
      collision.hitPoint =
          collisionsModule->ClosestPtPointSegment(point, cp0, cp1, &t);

      Math::Vector3 radialPoint = (point - collision.hitPoint).Normalized();

      collision.hitPoint =
          collision.hitPoint + radialPoint * capsule->radius * radiusScale;
      collision.pushDir = radialPoint;

      break;
    }
    case Collider::ColliderType::SPHERE:
      SphereCollider* sphere = static_cast<SphereCollider*>(collider);
      collision.hitPoint = collision.hitPoint =
          sphere->GetWorldCenter() +
          (point - sphere->GetWorldCenter()).Normalized() *
              sphere->GetWorldRadius();
      collision.pushDir = (point - collision.hitPoint).Normalized();
      break;
  }

#if _EDITOR
  DebugDraw::Point(collision.hitPoint, Color::white, 5, .1);
#endif
  return collision;
}

Math::Vector3 CollisionSolverModule::Resolve(Collider* collider1,
                                             Collision collision1,
                                             Collider* collider2,
                                             Collision collision2) {
  // Calculate the solve point based on the collisions
  int solveType = static_cast<int>(collision1.onEdge) +
                  2 * static_cast<int>(collision2.onEdge);
  bool collider1IsDominant =
      collider1->GetType() == Collider::ColliderType::BOX;

  Math::Vector3 responseDirection;

  if (!collision1.onEdge && !collision2.onEdge && collider1IsDominant ||
      collision2.onEdge) {
    responseDirection = collision1.pushDir;
  } else {
    responseDirection = collision2.pushDir;
  }

  float push = Math::Vector3::Dot((collision2.hitPoint - collision1.hitPoint),
                                  responseDirection);
  return (push + Math::Util::Sign(push) * EPS) * responseDirection;
}

void CollisionSolverModule::Update() {
  BROFILER_CATEGORY("Collision Solver Update", Profiler::Color::Aqua);

  CollisionUtil::ColliderPairSet collidingPairs =
      collisionsModule->collidingPairs;

  for (int i = 0; i < 3;
       ++i) {  // TODO(Caleb): Don't hardcode number of iterations

    for (const auto& pair : collidingPairs) {
      Collider* collider1 = pair.first;
      Collider* collider2 = pair.second;

      // Look for a reason to skip this collision solve
      if (((collider1->entity->isStatic || collider1->mass == 0) &&
           (collider1->entity->isStatic || collider2->mass == 0)) ||
          collider1->isTrigger || collider2->isTrigger) {
        continue;
      }

      // Only test intersections after first test because we can use cached
      // CollisionModule tests
      if (i > 0 && !collider1->Intersection(collider2)) {
        continue;
      }

      Collision collision1, collision2;
      collision1 = Solve(collider1, collider2);
      collision2 = Solve(collider2, collider1);

      Math::Vector3 response =
          Resolve(collider1, collision1, collider2, collision2);

      bool c1Static = collider1->entity->isStatic || collider1->mass == 0;
      bool c2Static = collider2->entity->isStatic || collider2->mass == 0;
      int staticSwitch =
          static_cast<int>(c1Static) + 2 * static_cast<int>(c2Static);

      switch (staticSwitch) {
        case 0: {  // Neither collider is static

          float massRatio =
              collider2->mass / (collider1->mass + collider2->mass);

          collider1->transform->TranslateWorld(response * massRatio);
          collider2->transform->TranslateWorld(-response * (1 - massRatio));
#if _EDITOR
          DebugDraw::Line(collision1.hitPoint,
                          collision1.hitPoint + response * massRatio,
                          Color::cyan, 3, .1);
          DebugDraw::Line(collision2.hitPoint,
                          collision2.hitPoint - response * (1 - massRatio),
                          Color::cyan, 3, .1);
#endif
        } break;

        case 1:  // Collider 1 is static
          collider2->transform->TranslateWorld(-response);
#if _EDITOR
          DebugDraw::Line(collision2.hitPoint, collision2.hitPoint - response,
                          Color::cyan, 3, .1);
#endif
          break;

        case 2:  // Collider 2 is static
          collider1->transform->TranslateWorld(response);
#if _EDITOR
          DebugDraw::Line(collision1.hitPoint, collision1.hitPoint + response,
                          Color::cyan, 3, .1);
#endif
          break;
      }
    }
  }
}
}  // namespace Isetta
