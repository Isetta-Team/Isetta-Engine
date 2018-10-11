/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/Transform.h"
#include "Core/Debug/Logger.h"
#include "Core/Math/Matrix3.h"
#include "Scene/Entity.h"
#include "Util.h"

namespace Isetta {

Math::Vector4 Transform::sharedV4{};

Transform::Transform(Entity* entity) : entity(entity) {}

// TODO(YIDI): Test this
Math::Vector3 Transform::GetWorldPos() {
  return GetLocalToWorldMatrix().GetCol(3).GetVector3();
}

Math::Vector3 Transform::GetLocalPos() const { return localPos; }

// TODO(YIDI): test this
void Transform::SetWorldPos(const Math::Vector3& newWorldPos) {
  SetDirty();

  if (parent == nullptr) {
    localPos = newWorldPos;
  } else {
    sharedV4.Set(newWorldPos, 1);
    localPos =
        (parent->GetLocalToWorldMatrix().Inverse() * sharedV4).GetVector3();
  }
}

// TODO(YIDI): test this
void Transform::SetLocalPos(const Math::Vector3& newLocalPos) {
  localPos = newLocalPos;
  SetDirty();
}

// TODO(YIDI): test this
void Transform::TranslateWorld(const Math::Vector3& delta) {
  SetWorldPos(GetWorldPos() + delta);
}

// TODO(YIDI): test this
void Transform::TranslateLocal(const Math::Vector3& delta) {
  SetLocalPos(localPos + delta);
}

// TODO(YIDI): test this
Math::Quaternion Transform::GetWorldRot() {
  if (parent == nullptr) {
    worldRot = localRot;
  } else {
    worldRot = parent->GetWorldRot() * localRot;
  }

  return worldRot;
}

// TODO(YIDI): test this
Math::Quaternion Transform::GetLocalRot() const { return localRot; }

// TODO(YIDI): test this
Math::Vector3 Transform::GetWorldEulerAngles() {
  return GetWorldRot().GetEulerAngles();
}

// TODO(YIDI): test this
Math::Vector3 Transform::GetLocalEulerAngles() const {
  return localRot.GetEulerAngles();
}

void Transform::SetWorldRot(const Math::Quaternion& newWorldRot) {
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
  SetLocalRot(Math::Quaternion::FromEulerAngles(eulerAngles) * localRot);
}

void Transform::RotateLocal(const Math::Vector3& axis, const float angle) {
  SetLocalRot(Math::Quaternion::FromAngleAxis(axis, angle) * localRot);
}

Math::Vector3 Transform::GetWorldScale() const {
  // TODO(YIDI):  implement this
  return localScale;
}

Math::Vector3 Transform::GetLocalScale() const { return localScale; }

void Transform::SetLocalScale(const Math::Vector3& newScale) {
  localScale = newScale;
  SetDirty();
}

// TODO(YIDI): Test this
void Transform::SetParent(Transform* transform) {
  if (parent == transform) {
    LOG_ERROR(Debug::Channel::Graphics,
              "You are trying to set (%s)'s parent to (%s), whose is already "
              "their parent",
              GetName().c_str(), transform->GetName().c_str());
    return;
  }

  if (parent != nullptr) {
    parent->RemoveChild(this);
  }
  if (transform != nullptr) {
    transform->AddChild(this);
  }
  parent = transform;
  SetDirty();
}

// TODO(YIDI): test this
Math::Vector3 Transform::GetForward() {
  return GetLocalToWorldMatrix().GetCol(2).GetVector3();
}

// TODO(YIDI): test this
Math::Vector3 Transform::GetUp() {
  return GetLocalToWorldMatrix().GetCol(1).GetVector3();
}

// TODO(YIDI): test this
Math::Vector3 Transform::GetRight() {
  return GetLocalToWorldMatrix().GetCol(0).GetVector3();
}

void Transform::LookAt(const Math::Vector3& target,
                       const Math::Vector3& worldUp) {
  Math::Vector3 forwardDir = target - GetLocalPos();
  Math::Vector3 rightDir =
      Math::Vector3::Cross(forwardDir, worldUp).Normalized() * -1;
  Math::Vector3 upDir = Math::Vector3::Cross(forwardDir, rightDir);

  SetLocalRot(Math::Quaternion::FromLookRotation(forwardDir, upDir));
}

// TODO(YIDI): Test this
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
  sharedV4.Set(localPoint, 1);
  return (GetLocalToWorldMatrix() * sharedV4).GetVector3();
}

Math::Vector3 Transform::LocalPosFromWorldPos(const Math::Vector3& worldPoint) {
  sharedV4.Set(worldPoint, 1);
  return (GetLocalToWorldMatrix().Inverse() * sharedV4).GetVector3();
}

Math::Vector3 Transform::WorldDirFromLocalDir(
    const Math::Vector3& localDirection) {
  sharedV4.Set(localDirection, 0);
  return (GetLocalToWorldMatrix() * sharedV4).GetVector3();
}

Math::Vector3 Transform::LocalDirFromWorldDir(
    const Math::Vector3& worldDirection) {
  sharedV4.Set(worldDirection, 0);
  return (GetLocalToWorldMatrix().Inverse() * sharedV4).GetVector3();
}

// TODO(YIDI): Test this
void Transform::ForChildren(Action<Transform*> action) {
  for (auto& child : children) {
    action(child);
  }
}

// TODO(YIDI): test this
void Transform::ForDescendents(Action<Transform*> action) {
  for (auto& child : children) {
    action(child);
    child->ForDescendents(action);
  }
}

void Transform::ForSelfAndDescendents(Action<Transform*> action) {
  action(this);
  ForDescendents(action);
}

// TODO(YIDI): test this
void Transform::SetWorldTransform(const Math::Vector3& inPosition,
                                  const Math::Vector3& inEulerAngles,
                                  const Math::Vector3& inScale) {
  SetWorldPos(inPosition);
  SetWorldRot(inEulerAngles);
  SetLocalScale(inScale);  // TODO(YIDI): fix this ,
}

// TODO(YIDI): test this
void Transform::SetH3DNodeTransform(const H3DNode node, Transform& transform) {
  Math::Vector3 worldPos = transform.GetWorldPos();
  Math::Vector3 worldEuler = transform.GetWorldEulerAngles();
  Math::Vector3 worldScale = transform.GetWorldScale();

  h3dSetNodeTransform(node, worldPos.x, worldPos.y, worldPos.z, worldEuler.x,
                      worldEuler.y, worldEuler.z, worldScale.x, worldScale.y,
                      worldScale.z);
}

void Transform::Print() {
  LOG_INFO(Debug::Channel::Graphics,
           "\nName [%s]"
           "\nWorldPos %s"
           "\nLocalPos %s"
           "\nWorldRot %s"
           "\nLocalRot %s"
           "\nLocalQuat %s"
           "\nWorldScale %s",
           GetName().c_str(), GetWorldPos().ToString().c_str(),
           GetLocalPos().ToString().c_str(),
           GetWorldRot().GetEulerAngles().ToString().c_str(),
           GetLocalRot().GetEulerAngles().ToString().c_str(),
           GetLocalRot().ToString().c_str(),
           GetWorldScale().ToString().c_str());
}

// TODO(YIDI): test this
const Math::Matrix4& Transform::GetLocalToWorldMatrix() {
  if (isMatrixDirty) {
    RecalculateLocalToWorldMatrix();
    isMatrixDirty = false;
  }
  return localToWorldMatrix;
}

void Transform::RecalculateLocalToWorldMatrix() {
  Math::Matrix4 localToParentMatrix{};
  localToParentMatrix.SetCol(3, localPos, 1);                    // translation
  localToParentMatrix.SetTopLeftMatrix3(localRot.GetMatrix3());  // rotation

  Math::Matrix4 temp;
  temp.SetDiagonal(localScale.x, localScale.y, localScale.z, 1);
  localToParentMatrix = temp * localToParentMatrix;  // scale

  if (parent != nullptr) {
    localToWorldMatrix = parent->GetLocalToWorldMatrix() * localToParentMatrix;
  } else {
    localToWorldMatrix = localToParentMatrix;
  }
}

// TODO(YIDI): Test this
void Transform::AddChild(Transform* transform) {
  // duplicate child check is in SetParent
  children.push_back(transform);
}

// TODO(YIDI): test this
void Transform::RemoveChild(Transform* transform) {
  for (auto it = children.begin(); it != children.end(); ++it) {
    if (*it == transform) {
      children.erase(it);
      return;
    }
  }

  throw std::exception{
      Util::StrFormat("Transform::RemoveChild => child (%s) doesn't exist!",
                      transform->GetName().c_str())};
}

void Transform::SetDirty() {
  ForSelfAndDescendents([](Transform* trans) { trans->isMatrixDirty = true; });
}

#if _DEBUG
// TODO(YIDI): test this
#endif

}  // namespace Isetta
