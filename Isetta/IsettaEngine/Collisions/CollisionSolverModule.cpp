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

Collision CollisionSolverModule::Solve(Collider* collider,
                                       Math::Vector3 point) {
  const float EPS = 1e-6;

  PROFILE
  Collision collision;

  switch (collider->GetType()) {
    case Collider::ColliderType::BOX: {
      BoxCollider* box = static_cast<BoxCollider*>(collider);

      Math::Vector3 p = box->transform->LocalPosFromWorldPos(point) -
                        box->center;  // TODO(Caleb): Check if this is supposed
                                      // to be converted from world space

      Math::Vector3 extents = box->size * .5;
      Math::Vector3 hitPoint = Math::Vector3::zero;

      int numEdges = 0;
      int maxAxis = 0;
      float maxDist = Math::Util::Abs(p[0]);

      for (int i = 0; i < Math::Vector3::ELEMENT_COUNT; ++i) {
        hitPoint[i] =
            Math::Util::Min(Math::Util::Max(p[i], -extents[i]), extents[i]);

        if (Math::Util::Abs(p[i]) > maxDist) {
          maxDist = Math::Util::Abs(p[i]);
          maxAxis = i;
        }

        if (Math::Util::Abs(hitPoint[i]) >= extents[i]) ++numEdges;
      }

      hitPoint[maxAxis] =
          Math::Util::Sign(hitPoint[maxAxis]) * (extents[maxAxis] + EPS);

      collision.hitPoint = box->transform->WorldPosFromLocalPos(hitPoint);
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
                             Math::Vector4{0, 1, 0, 0});
      cp0 = capsule->GetWorldCenter() - cdir;
      cp1 = capsule->GetWorldCenter() + cdir;

      // Get the closest point on the segment to our first collider's
      // center then push that point out
      float t;
      collision.hitPoint =
          collisionsModule->ClosestPtPointSegment(point, cp0, cp1, &t);

      // TODO(Caleb): Make this float reinterpretation platform agnostic
      // (i.e. can't use just IEEE)
      Math::Vector3 radialPoint = (point - collision.hitPoint).Normalized();

      collision.hitPoint =
          collision.hitPoint + radialPoint * capsule->radius * radiusScale;

      break;
    }
    case Collider::ColliderType::SPHERE:
      SphereCollider* sphere = static_cast<SphereCollider*>(collider);
      collision.hitPoint = sphere->GetWorldCenter();
      collision.hitPoint =
          collision.hitPoint +
          (point - collision.hitPoint).Normalized() * sphere->GetWorldRadius();
      break;
  }

#if _EDITOR
  DebugDraw::Point(collision.hitPoint, Color::white, 5, .1);
#endif
  return collision;
}

Math::Vector3 CollisionSolverModule::GetPushDirection(Collider* collider,
                                                      Math::Vector3 point) {
  switch (collider->GetType()) {
    case Collider::ColliderType::BOX: {
      BoxCollider* box = static_cast<BoxCollider*>(collider);
      Math::Vector3 d = point - box->GetWorldCenter();

      int strongest = 0;
      float maxMagnitude =
          Math::Util::Abs(Math::Vector3::Dot(box->transform->GetAxis(0), d));

      for (int i = 1; i < Math::Vector3::ELEMENT_COUNT; ++i) {
        float magnitude =
            Math::Util::Abs(Math::Vector3::Dot(box->transform->GetAxis(i), d));
        if (magnitude > maxMagnitude) {
          strongest = i;
          maxMagnitude = magnitude;
        }
      }

      return box->transform->GetAxis(strongest);
    } break;

    case Collider::ColliderType::CAPSULE: {
      return (point - collider->GetWorldCenter())
          .Normalized();  // TODO(Caleb): Make an actual capsule force vector
                          // lol
    } break;

    case Collider::ColliderType::SPHERE:
    default: {
      return (point - collider->GetWorldCenter()).Normalized();
    } break;
  }
}

Math::Vector3 CollisionSolverModule::Resolve(Collider* collider1,
                                             Collision collision1,
                                             Collider* collider2,
                                             Collision collision2) {
  // Calculate the solve point based on the collisions
  int solveType = static_cast<int>(collision1.onEdge) +
                  2 * static_cast<int>(collision2.onEdge);
  bool collider1IsDominant = collider1->GetType() == Collider::ColliderType::BOX;

  Math::Vector3 responseDirection;

  if (!collision1.onEdge && !collision2.onEdge && collider1IsDominant || collision2.onEdge) {
    responseDirection = GetPushDirection(collider1, collision1.hitPoint);
  } else {
    responseDirection = GetPushDirection(collider2, collision2.hitPoint);
  }

  return Math::Vector3::Dot((collision2.hitPoint - collision1.hitPoint),
                            responseDirection) *
         responseDirection;
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
      if ((collider1->entity->isStatic && collider1->entity->isStatic) ||
          collider1->isTrigger || collider2->isTrigger) {
        continue;
      }

      // Only test intersections after first test because we can use cached
      // CollisionModule tests
      if (i > 0 && !collider1->Intersection(collider2)) {
        continue;
      }

      Collision collision1, collision2;
      collision1 = Solve(collider1, collider2->GetWorldCenter());
      collision2 = Solve(collider2, collider1->GetWorldCenter());

      Math::Vector3 response =
          Resolve(collider1, collision1, collider2, collision2);

      bool c1Static = collider1->entity->isStatic;
      bool c2Static = collider2->entity->isStatic;
      int staticSwitch =
          static_cast<int>(c1Static) + 2 * static_cast<int>(c2Static);

      switch (staticSwitch) {
        case 0:  // Neither collider is static
          collider1->transform->TranslateWorld(response * .5);
          collider2->transform->TranslateWorld(-response * .5);
#if _EDITOR
          DebugDraw::Line(collision1.hitPoint,
                          collision1.hitPoint + response * .5, Color::cyan, 3,
                          .1);
          DebugDraw::Line(collision2.hitPoint,
                          collision2.hitPoint - response * .5, Color::cyan, 3,
                          .1);
#endif
          break;

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
