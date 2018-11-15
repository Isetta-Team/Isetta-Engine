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
CollisionsModule* CollisionSolverModule::collisionsModule{nullptr};

void CollisionSolverModule::StartUp(){};
void CollisionSolverModule::ShutDown(){};

Math::Vector3 CollisionSolverModule::Solve(Collider* collider,
                                           Math::Vector3 point) {
  PROFILE
  Math::Vector3 closestPoint;

  switch (collider->GetType()) {
    case Collider::ColliderType::BOX: {
      BoxCollider* box = static_cast<BoxCollider*>(collider);
      closestPoint = collisionsModule->ClosestPtPointOBB(point, *box);
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
      closestPoint =
          collisionsModule->ClosestPtPointSegment(point, cp0, cp1, &t);
      U32 tInt = reinterpret_cast<U32&>(t);

      // TODO(Caleb): Make this float reinterpretation platform agnostic
      // (i.e. can't use just IEEE)
      Math::Vector3 radialPoint = (point - closestPoint).Normalized();
      switch (tInt) {
        case 0x3f800000:  // 1: Can extend the radius "anywhere"
        case 0x00000000:  // 0
          closestPoint =
              closestPoint + radialPoint * capsule->radius * radiusScale;
          break;

        default:  // anything else: Can only extend the radius along the
                  // circular cross-section of the capsule
          Math::Vector3 capsuleDir = (cp0 - cp1).Normalized();
          Math::Vector3 crossProd =
              Math::Vector3::Cross(radialPoint, capsuleDir);
          Math::Vector3 projLine = Math::Vector3::Cross(capsuleDir, crossProd);
          closestPoint =
              closestPoint + projLine * capsule->radius * radiusScale;
      }

      break;
    }
    case Collider::ColliderType::SPHERE:
      SphereCollider* sphere = static_cast<SphereCollider*>(collider);
      closestPoint = sphere->GetWorldCenter();
      closestPoint = closestPoint + (point - closestPoint).Normalized() *
                                        sphere->GetWorldRadius();
      break;
  }

#if _EDITOR
  DebugDraw::Point(closestPoint, Color::white, 5, .1);
#endif
  return closestPoint;
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
      if ((collider1->GetProperty(Collider::Property::IS_STATIC) &&
           (collider2->GetProperty(Collider::Property::IS_STATIC))) ||
          collider1->GetProperty(Collider::Property::IS_TRIGGER) ||
          collider2->GetProperty(Collider::Property::IS_TRIGGER)) {
        continue;
      }

      // Only test intersections after first test because we can use cached
      // CollisionModule tests
      if (i > 0 && !collider1->Intersection(collider2)) {
        continue;
      }

      Math::Vector3 closestPoint1, closestPoint2;
      closestPoint1 = Solve(collider1, collider2->GetWorldCenter());
      closestPoint2 = Solve(collider2, collider1->GetWorldCenter());

      Math::Vector3 halfDistanceVector = (closestPoint2 - closestPoint1) * .5;

      bool c1Static = collider1->GetProperty(Collider::Property::IS_STATIC);
      bool c2Static = collider2->GetProperty(Collider::Property::IS_STATIC);
      int staticSwitch =
          static_cast<int>(c1Static) + 2 * static_cast<int>(c2Static);

      switch (staticSwitch) {
        case 0:  // Neither collider is static
          collider1->GetTransform()->TranslateWorld(halfDistanceVector);
          collider2->GetTransform()->TranslateWorld(-halfDistanceVector);
#if _EDITOR
          DebugDraw::Line(closestPoint1, closestPoint1 + halfDistanceVector,
                          Color::cyan, 3, .1);
          DebugDraw::Line(closestPoint2, closestPoint2 - halfDistanceVector,
                          Color::cyan, 3, .1);
#endif
          break;

        case 1:  // Collider 1 is static
          collider2->GetTransform()->TranslateWorld(-halfDistanceVector * 2);
#if _EDITOR
          DebugDraw::Line(closestPoint2, closestPoint2 - halfDistanceVector * 2,
                          Color::cyan, 3, .1);
#endif
          break;

        case 2:  // Collider 2 is static
          collider1->GetTransform()->TranslateWorld(halfDistanceVector * 2);
#if _EDITOR
          DebugDraw::Line(closestPoint1, closestPoint1 + halfDistanceVector * 2,
                          Color::cyan, 3, .1);
#endif
          break;
      }
    }
  }
}
}  // namespace Isetta
