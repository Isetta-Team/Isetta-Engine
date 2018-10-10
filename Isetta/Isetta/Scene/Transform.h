/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "Core/Math/Matrix4.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Graphics/RenderNode.h"

namespace Isetta {
class Transform {
 public:
  // constructors
  Transform() = delete;
  explicit Transform(class Entity* entity);
  ~Transform() = default;

  // position
  Math::Vector3 GetWorldPos();
  Math::Vector3 GetLocalPos() const;
  void SetWorldPos(const Math::Vector3& newWorldPos);
  void SetLocalPos(const Math::Vector3& newLocalPos);

  void TranslateWorld(const Math::Vector3& delta);
  void TranslateLocal(const Math::Vector3& delta);

  // rotation
  Math::Quaternion GetWorldRot();
  Math::Quaternion GetLocalRot() const;
  Math::Vector3 GetWorldEulerAngles();
  Math::Vector3 GetLocalEulerAngles() const;
  void SetWorldRot(const Math::Quaternion& newWorldRot);
  void SetWorldRot(const Math::Vector3& worldEulers);
  void SetLocalRot(const Math::Quaternion& newLocalRot);
  void SetLocalRot(const Math::Vector3& localEulers);

  void RotateWorld(const Math::Vector3& eulerAngles);
  void RotateWorld(const Math::Vector3& axis, float angle);
  void RotateLocal(const Math::Vector3& eulerAngles);
  void RotateLocal(const Math::Vector3& axis, float angle);

  // scale
  Math::Vector3 GetWorldScale() const;
  Math::Vector3 GetLocalScale() const;
  void SetLocalScale(const Math::Vector3& newScale);

  // hierarchy
  void SetParent(Transform* transform);
  Transform* GetParent() const { return parent; }

  // helper
  Math::Vector3 GetForward();
  Math::Vector3 GetUp();
  Math::Vector3 GetRight();

  // other
  void LookAt(const Math::Vector3& target,
              const Math::Vector3& worldUp = Math::Vector3::up);
  class Entity* GetEntity() const { return entity; }
  Size GetChildCount() const { return children.size(); }
  Transform* GetChild(U16 childIndex);
  inline std::string GetName() const;

  // utilities
  Math::Vector3 WorldPosFromLocalPos(const Math::Vector3& localPoint);
  Math::Vector3 LocalPosFromWorldPos(const Math::Vector3& worldPoint);
  Math::Vector3 WorldDirFromLocalDir(const Math::Vector3& localDirection);
  Math::Vector3 LocalDirFromWorldDir(const Math::Vector3& worldDirection);

  void ForChildren(Action<Transform*> action);
  void ForDescendents(Action<Transform*> action);
  void ForSelfAndDescendents(Action<Transform*> action);

  void SetWorldTransform(const Math::Vector3& inPosition,
                         const Math::Vector3& inEulerAngles,
                         const Math::Vector3& inScale);

  // more utilities
  // TODO(YIDI): Decide if they should stay here
  static void SetH3DNodeTransform(H3DNode node, Transform& transform);

#if _DEBUG
  void Print();
#endif

 private:
  // to be used by children
  const Math::Matrix4& GetLocalToWorldMatrix();
  void RecalculateLocalToWorldMatrix();
  // const Math::Matrix4& GetWorldToLocalMatrix();

  // both called by SetParent
  void AddChild(Transform* transform);
  void RemoveChild(Transform* transform);

  Math::Quaternion worldRot;  // only for query

  Math::Matrix4 localToWorldMatrix{};
  Math::Vector3 localPos{Math::Vector3::zero};  // part of local storage
  Math::Quaternion localRot{
      Math::Quaternion::identity};               // part of local storage
  Math::Vector3 localScale{Math::Vector3::one};  // part of local storage

  // marked when anything local changed
  // cleared when matrix recalculated
  void SetDirty();
  bool isMatrixDirty{true};

  class Entity* entity{nullptr};
  Transform* root{nullptr};
  Transform* parent{nullptr};
  std::vector<Transform*> children;

  static Math::Vector4 sharedV4;
};
}  // namespace Isetta