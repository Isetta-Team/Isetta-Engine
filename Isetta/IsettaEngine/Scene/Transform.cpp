/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/Transform.h"
#include "Core/Debug/DebugDraw.h"
#include "Core/Debug/Logger.h"
#include "Core/Math/Matrix3.h"
#include "Core/Math/Vector4.h"
#include "Horde3D/Horde3D/Bindings/C++/Horde3D.h"
#include "Scene/Entity.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"
#include "Util.h"

namespace Isetta {
Transform::Transform(Entity* const entity) : entity(entity) {}

void Transform::SetLocalToWorldMatrix(const Math::Matrix4& newMatrix) {
  if (!entity->IsMoveable()) return;

  localToWorldMatrix = newMatrix;
  for (int i = 0; i < Math::Matrix3::ROW_COUNT; ++i) {
    axis[i] = localToWorldMatrix.GetCol(i).GetVector3().Normalized();
  }
  SetDirty();
}

Math::Vector3 Transform::GetWorldPos() {
  return GetLocalToWorldMatrix().GetCol(3).GetVector3();
}

Math::Vector3 Transform::GetLocalPos() const { return localPos; }

void Transform::SetWorldPos(const Math::Vector3& newWorldPos) {
  if (!entity->IsMoveable()) return;
  SetDirty();

  if (parent == nullptr) {
    localPos = newWorldPos;
  } else {
    localPos = (parent->GetWorldToLocalMatrix() * Math::Vector4{newWorldPos, 1})
                   .GetVector3();
  }
}

void Transform::SetLocalPos(const Math::Vector3& newLocalPos) {
  if (!entity->IsMoveable()) return;

  localPos = newLocalPos;
  SetDirty();
}

void Transform::TranslateWorld(const Math::Vector3& delta) {
  SetWorldPos(GetWorldPos() + delta);
}

void Transform::TranslateLocal(const Math::Vector3& delta) {
  SetLocalPos(localPos + delta);
}

Math::Quaternion Transform::GetWorldRot() {
  if (parent == nullptr) {
    worldRot = localRot;
  } else {
    worldRot = parent->GetWorldRot() * localRot;
  }

  return worldRot;
}

Math::Quaternion Transform::GetLocalRot() const { return localRot; }

Math::Vector3 Transform::GetWorldEulerAngles() {
  return GetWorldRot().GetEulerAngles();
}

Math::Vector3 Transform::GetLocalEulerAngles() const {
  return localRot.GetEulerAngles();
}

void Transform::SetWorldRot(const Math::Quaternion& newWorldRot) {
  if (!entity->IsMoveable()) return;

  worldRot = newWorldRot;
  SetDirty();

  if (parent == nullptr) {
    localRot = worldRot;
  } else {
    localRot = parent->GetWorldRot().GetInverse() * worldRot;
  }
}

void Transform::SetWorldRot(const Math::Vector3& worldEulers) {
  SetWorldRot(Math::Quaternion::FromEulerAngles(worldEulers));
}

void Transform::SetLocalRot(const Math::Quaternion& newLocalRot) {
  if (!entity->IsMoveable()) return;

  localRot = newLocalRot;
  SetDirty();
}

void Transform::SetLocalRot(const Math::Vector3& localEulers) {
  SetLocalRot(Math::Quaternion::FromEulerAngles(localEulers));
}

void Transform::RotateWorld(const Math::Vector3& eulerAngles) {
  SetWorldRot(Math::Quaternion::FromEulerAngles(eulerAngles) * GetWorldRot());
}

void Transform::RotateWorld(const Math::Vector3& axis, const float angle) {
  SetWorldRot(Math::Quaternion::FromAngleAxis(axis, angle) * GetWorldRot());
}

void Transform::RotateLocal(const Math::Vector3& eulerAngles) {
  // first, get the basis vectors in local space
  bool hasParent = GetParent() != nullptr;
  Math::Vector3 left, up, forward;
  if (hasParent) {
    Transform* parent = GetParent();
    left = parent->LocalDirFromWorldDir(GetLeft());
    up = parent->LocalDirFromWorldDir(GetUp());
    forward = parent->LocalDirFromWorldDir(GetForward());
  } else {
    left = GetLeft();
    up = GetUp();
    forward = GetForward();
  }
  // then, map euler angles and basis
  // i.e. Transform the euler angles to local space
  SetLocalRot(Math::Quaternion::FromEulerAngles(left * eulerAngles.x +
                                                up * eulerAngles.y +
                                                forward * eulerAngles.z) *
              localRot);
}

void Transform::RotateLocal(const Math::Vector3& axisWorldSpace,
                            const float angle) {
  // transform the axis from world space to the space that
  // this object sits in, so it can understand the axis correctly
  Math::Vector3 localAxis =
      GetParent() != nullptr ? GetParent()->LocalDirFromWorldDir(axisWorldSpace)
                             : axisWorldSpace;
  SetLocalRot(Math::Quaternion::FromAngleAxis(localAxis, angle) * localRot);
}

Math::Vector3 Transform::GetWorldScale() {
  if (parent == nullptr) {
    worldScale = localScale;
  } else {
    worldScale = Math::Vector3::Scale(parent->GetWorldScale(), localScale);
  }

  return worldScale;
}

Math::Vector3 Transform::GetLocalScale() const { return localScale; }

void Transform::SetLocalScale(const Math::Vector3& newScale) {
  if (!entity->IsMoveable()) return;

  localScale = newScale;
  SetDirty();
}

void Transform::SetWorldScale(const Math::Vector3& newWorldScale) {
  if (!entity->IsMoveable()) return;

  worldScale = newWorldScale;
  SetDirty();

  if (parent == nullptr) {
    localScale = worldScale;
  } else {
    localScale =
        Math::Vector3::ReverseScale(worldScale, parent->GetWorldScale());
  }
}

void Transform::SetParent(Transform* const transform, bool inheritTransform) {
  if (!entity->IsMoveable()) return;

  Transform* targetTransform = transform;
  if (transform == nullptr) {
    targetTransform =
        LevelManager::Instance().loadedLevel->levelRoot->transform;
  }
  if (parent == targetTransform) {
    LOG_WARNING(Debug::Channel::Graphics,
                "You are trying to set (%s)'s parent to (%s), whose is already "
                "their parent",
                GetName().c_str(), targetTransform->GetName().c_str());
    return;
  }
  Math::Vector3 originalPos, originalScale;
  Math::Quaternion originalRot;
  if (!inheritTransform) {
    originalPos = GetWorldPos();
    originalRot = GetWorldRot();
    originalScale = GetWorldScale();
  }

  if (parent != nullptr) {
    parent->RemoveChild(this);
  }
  if (targetTransform != nullptr) {
    targetTransform->AddChild(this);
  }
  parent = targetTransform;
  if (!inheritTransform) {
    SetWorldPos(originalPos);
    SetWorldRot(originalRot);
    SetWorldScale(originalScale);
  }
  SetDirty();
}

Math::Vector3 Transform::GetForward() {
  GetLocalToWorldMatrix();
  return axis[2];
}

Math::Vector3 Transform::GetUp() {
  GetLocalToWorldMatrix();
  return axis[1];
}

Math::Vector3 Transform::GetLeft() {
  GetLocalToWorldMatrix();
  return axis[0];
}

Math::Vector3 Transform::GetAxis(const int i) {
  GetLocalToWorldMatrix();
  return axis[i];
}

void Transform::LookAt(const Math::Vector3& target,
                       const Math::Vector3& worldUp) {
  Math::Vector3 forwardDir = (target - GetLocalPos()).Normalized();
  Math::Vector3 rightDir =
      Math::Vector3::Cross(forwardDir, worldUp).Normalized();
  // upDir is guaranteed to be of unit length
  // cause |upDir| = |forwardDir| * |rightDir| * sin();
  Math::Vector3 upDir = Math::Vector3::Cross(rightDir, forwardDir);
  // localToWorldMatrix.SetCol(0, rightDir, 0);
  // localToWorldMatrix.SetCol(1, upDir, 0);
  // localToWorldMatrix.SetCol(2, forwardDir, 0);
  SetLocalRot(Math::Quaternion::FromLookRotation(forwardDir, upDir));
}

void Transform::LookAt(Transform& target, const Math::Vector3& worldUp) {
  LookAt(target.GetWorldPos(), worldUp);
}

Transform* Transform::GetChild(const U16 childIndex) {
  if (childIndex >= GetChildCount()) {
    throw std::exception{
        Util::StrFormat("Transform::GetChild => transform of (%s) only has %d "
                        "children but you are asking for the %dth one",
                        GetName().c_str(), GetChildCount(), childIndex)};
  }
  return children[childIndex];
}

std::string Transform::GetName() const { return entity->GetName(); }

Math::Vector3 Transform::WorldPosFromLocalPos(const Math::Vector3& localPoint) {
  return (GetLocalToWorldMatrix() * Math::Vector4{localPoint, 1}).GetVector3();
}

Math::Vector3 Transform::LocalPosFromWorldPos(const Math::Vector3& worldPoint) {
  return (GetWorldToLocalMatrix() * Math::Vector4{worldPoint, 1}).GetVector3();
}

Math::Vector3 Transform::WorldDirFromLocalDir(
    const Math::Vector3& localDirection) {
  return (GetLocalToWorldMatrix() * Math::Vector4{localDirection, 0})
      .GetVector3();
}

Math::Vector3 Transform::LocalDirFromWorldDir(
    const Math::Vector3& worldDirection) {
  return (GetWorldToLocalMatrix() * Math::Vector4{worldDirection, 0})
      .GetVector3();
}

void Transform::ForChildren(const Action<Transform*>& action) {
  for (auto& child : children) {
    action(child);
  }
}

void Transform::ForDescendants(const Action<Transform*>& action) {
  for (auto& child : children) {
    action(child);
    child->ForDescendants(action);
  }
}

void Transform::SetWorldTransform(const Math::Vector3& inPosition,
                                  const Math::Vector3& inEulerAngles,
                                  const Math::Vector3& inScale) {
  if (!entity->IsMoveable()) return;

  SetWorldPos(inPosition);
  SetWorldRot(inEulerAngles);
  SetLocalScale(inScale);
}

void Transform::SetH3DNodeTransform(const H3DNode node, Transform& transform) {
  h3dSetNodeTransMat(node, transform.GetLocalToWorldMatrix().Transpose().data);
}

const Math::Matrix4& Transform::GetLocalToWorldMatrix() {
  if (isDirty) {
    RecalculateLocalToWorldMatrix();
    isDirty = false;
  }
  return localToWorldMatrix;
}

const Math::Matrix4& Transform::GetWorldToLocalMatrix() {
  if (isWorldToLocalDirty) {
    worldToLocalMatrix = GetLocalToWorldMatrix().Inverse();
    isWorldToLocalDirty = false;
  }
  return worldToLocalMatrix;
}
void Transform::RecalculateLocalToWorldMatrix() {
  Math::Matrix4 localToParentMatrix{};
  localToParentMatrix.SetTopLeftMatrix3(localRot.GetMatrix3());  // rotation
  Math::Matrix4 temp;
  temp.SetDiagonal(localScale.x, localScale.y, localScale.z, 1);
  localToParentMatrix = localToParentMatrix * temp;  // scale
  localToParentMatrix.SetCol(3, localPos, 1);

  if (parent != nullptr) {
    localToWorldMatrix = parent->GetLocalToWorldMatrix() * localToParentMatrix;
  } else {
    localToWorldMatrix = localToParentMatrix;
  }
  for (int i = 0; i < Math::Matrix3::ROW_COUNT; ++i) {
    axis[i] = localToWorldMatrix.GetCol(i).GetVector3().Normalized();
  }
}

void Transform::AddChild(Transform* transform) {
  // duplicate child check is in SetParent
  children.PushBack(transform);
}

void Transform::RemoveChild(Transform* transform) {
  for (auto it = children.begin(); it != children.end(); ++it) {
    if (*it == transform) {
      children.Erase(it);
      return;
    }
  }

  throw std::exception{
      Util::StrFormat("Transform::RemoveChild => child (%s) doesn't exist!",
                      transform->GetName().c_str())};
}

void Transform::SetDirty() {
  // TODO(YIDI): Don't need to traverse all children, if one child is dirty, all
  // children all also dirty
  isDirty = true;
  isWorldToLocalDirty = true;
  ForDescendants([](Transform* trans) {
    trans->isDirty = true;
    trans->isWorldToLocalDirty = true;
  });
}

}  // namespace Isetta
