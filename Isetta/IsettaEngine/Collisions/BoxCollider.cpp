/*
 * Copyright (c) 2018 Isetta
 */
#include "Collisions/BoxCollider.h"
#include "Collisions/CapsuleCollider.h"
#include "Collisions/CollisionsModule.h"
#include "Collisions/SphereCollider.h"

#include "Collisions/Ray.h"
#include "Core/Debug/DebugDraw.h"
#include "Core/Math/Matrix4.h"
#include "Scene/Transform.h"

namespace Isetta {
void BoxCollider::Update() {
  DebugDraw::WireCube(
      Math::Matrix4::Translate(GetTransform()->GetWorldPos() + center) *
          Math::Matrix4::Scale(
              Math::Vector3::Scale(GetTransform()->GetWorldScale(), size)) *
          (Math::Matrix4)GetTransform()->GetWorldRot(),
      debugColor);
}

bool BoxCollider::Raycast(const Ray& ray, RaycastHit* const hitInfo,
                          float maxDistance) {
  float tmin = -INFINITY, tmax = maxDistance > 0 ? maxDistance : INFINITY;
  Math::Vector3 e = GetWorldExtents();
  Math::Vector3 o = GetTransform()->LocalPosFromWorldPos(ray.GetOrigin());
  Math::Vector3 d = GetTransform()->LocalDirFromWorldDir(ray.GetDirection());
  Math::Vector3 invD = 1.0f / d;

  float t[6];
  for (int i = 0; i < 3; i++) {
    t[2 * i] = -(e[i] + o[i]) * invD[i];
    t[2 * i + 1] = (e[i] - o[i]) * invD[i];
  }
  tmin =
      Math::Util::Max({Math::Util::Min(t[0], t[1]), Math::Util::Min(t[2], t[3]),
                       Math::Util::Min(t[4], t[5])});
  tmax =
      Math::Util::Min({Math::Util::Max(t[0], t[1]), Math::Util::Max(t[2], t[3]),
                       Math::Util::Max(t[4], t[5])});
  if (tmax < 0 || tmin > tmax) return false;
  if (tmin < 0) tmin = tmax;

  Math::Vector3 n;  // TODO (normal)
  RaycastHitCtor(hitInfo, tmin,
                 GetTransform()->WorldPosFromLocalPos(o) +
                     GetTransform()->WorldDirFromLocalDir(d) * tmin,
                 n);
  return true;
}

INTERSECTION_TEST(BoxCollider)
}  // namespace Isetta
