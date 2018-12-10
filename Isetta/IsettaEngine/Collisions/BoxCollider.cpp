/*
 * Copyright (c) 2018 Isetta
 */
#include "Collisions/BoxCollider.h"
#include "Collisions/CapsuleCollider.h"
#include "Collisions/CollisionsModule.h"
#include "Collisions/SphereCollider.h"

#include "Core/Debug/DebugDraw.h"
#include "Core/Geometry/Ray.h"
#include "Core/Math/Matrix4.h"
#include "Scene/Transform.h"

namespace Isetta {
#if _EDITOR
void BoxCollider::Update() {
  DebugDraw::WireCube(
      Math::Matrix4::Translate(GetWorldCenter()) *
          (Math::Matrix4)transform->GetWorldRot() *
          Math::Matrix4::Scale(
              Math::Vector3::Scale(transform->GetWorldScale(), size)),
      debugColor);
}
#endif

bool BoxCollider::Raycast(const Ray& ray, RaycastHit* const hitInfo,
                          float maxDistance) {
  maxDistance = maxDistance <= 0 ? INFINITY : maxDistance;
  float tmin = -INFINITY, tmax = INFINITY;
  Math::Vector3 e = GetWorldExtents();
  Math::Vector3 o = transform->LocalPosFromWorldPos(ray.GetOrigin());
  Math::Vector3 d = transform->LocalDirFromWorldDir(ray.GetDirection());
  Math::Vector3 invD = 1.0f / d;

  float t[6];
  for (int i = 0; i < 3; ++i) {
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
  if (tmin > maxDistance) return false;

  Math::Vector3 pt = transform->WorldPosFromLocalPos(o) +
                     transform->WorldDirFromLocalDir(d) * tmin;
  Math::Vector3 to = pt - GetWorldCenter();
  Math::Vector3 n;
  tmax = 0;
  for (int i = 0; i < Math::Vector3::ELEMENT_COUNT; ++i) {
    float tmp = Math::Vector3::Dot(to, transform->GetAxis(i));
    if (tmp > tmax) {
      tmax = tmp;
      n = Math::Util::Sign(tmp) * transform->GetAxis(i);
    }
  }

  RaycastHitCtor(hitInfo, tmin, pt, n);
  return true;
}

AABB BoxCollider::GetFatAABB() {
  AABB aabb = GetAABB();
  aabb.Expand(1.f + fatFactor);
  return aabb;
}
AABB BoxCollider::GetAABB() {
  const Math::Vector3 size = GetWorldSize();
  Math::Vector3 aabbSize = size;
  for (int i = 0; i < Math::Vector3::ELEMENT_COUNT; ++i)
    aabbSize[i] = Math::Util::Abs(transform->GetLeft()[i]) * size.x +
                  Math::Util::Abs(transform->GetUp()[i]) * size.y +
                  Math::Util::Abs(transform->GetForward()[i]) * size.z;
  return AABB{GetWorldCenter(), aabbSize};
}

INTERSECTION_TEST(BoxCollider)
}  // namespace Isetta
