/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/IsettaAlias.h"
#include "Core/Math/Matrix4.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Vector3.h"

namespace Isetta {
class ISETTA_API_DECLARE Transform {
  friend class Entity;

 public:
  // constructors
  Transform() = delete;
  explicit Transform(class Entity* const entity);
  ~Transform() = default;

  // position
  Math::Vector3 GetWorldPos();
  Math::Vector3 GetLocalPos() const;
  void SetWorldPos(const Math::Vector3& newWorldPos);
  void SetLocalPos(const Math::Vector3& newLocalPos);

  /**
   * \brief Translate this transform with the given delta in world space
   */
  void TranslateWorld(const Math::Vector3& delta);
  /**
   * \brief Translate this transform with the given delta in local space
   */
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

  /**
   * \brief Rotate the given euler angles in world space
   * \param eulerAngles Angles in degree around x, y, z axes
   */
  void RotateWorld(const Math::Vector3& eulerAngles);
  /**
   * \brief Rotate around the given axis for the given angle in degree
   */
  void RotateWorld(const Math::Vector3& axis, float angle);
  void RotateLocal(const Math::Vector3& eulerAngles);
  void RotateLocal(const Math::Vector3& axisWorldSpace, float angle);

  // scale
  Math::Vector3 GetWorldScale();
  Math::Vector3 GetLocalScale() const;
  void SetLocalScale(const Math::Vector3& newScale);
  void SetWorldScale(const Math::Vector3& newWorldScale);

  // hierarchy
  void SetParent(Transform* const transform);
  Transform* GetParent() const { return parent; }

  // helper
  Math::Vector3 GetForward();
  Math::Vector3 GetUp();
  Math::Vector3 GetLeft();
  Math::Vector3 GetAxis(int i);
  /**
   * \brief Get a Matrix4 that transform objects from local space to world space
   */
  const Math::Matrix4& GetLocalToWorldMatrix();
  /**
   * \brief Get a Matrix4 that transform objects from world space to local space
   */
  const Math::Matrix4& GetWorldToLocalMatrix();
  void SetLocalToWorldMatrix(const Math::Matrix4& newMatrix);

  // other
  /**
   * \brief Warning: you should never look at something that's your child
   */
  void LookAt(const Math::Vector3& target,
              const Math::Vector3& worldUp = Math::Vector3::up);
  void LookAt(Transform& target,
              const Math::Vector3& worldUp = Math::Vector3::up);
  Size GetChildCount() const { return children.Size(); }
  Transform* GetChild(U16 childIndex);
  inline std::string GetName() const;

  // utilities
  /**
   * \brief Transform a position from world space to local space
   */
  Math::Vector3 WorldPosFromLocalPos(const Math::Vector3& localPoint);
  /**
   * \brief Transform a position from local space to world space
   */
  Math::Vector3 LocalPosFromWorldPos(const Math::Vector3& worldPoint);
  /**
   * \brief Transform a direction from world space to local space
   */
  Math::Vector3 WorldDirFromLocalDir(const Math::Vector3& localDirection);
  /**
   * \brief Transform a direction from local space to world space
   */
  Math::Vector3 LocalDirFromWorldDir(const Math::Vector3& worldDirection);

  /**
   * \brief Execute the action on all immediate children of this transform
   */
  void ForChildren(const Action<Transform*>& action);
  /**
   * \brief Execute the action on all descendants (not including self) of this
   * transform
   */
  void ForDescendants(const Action<Transform*>& action);

  /**
   * \brief Set world position, world rotation, and world scale of this
   * transform 
   * 
   * \param inPosition World Position 
   * \param inEulerAngles World Rotation 
   * \param inScale World Scale
   */
  void SetWorldTransform(const Math::Vector3& inPosition,
                         const Math::Vector3& inEulerAngles,
                         const Math::Vector3& inScale);

  // more utilities
  typedef int H3DNode;

  /**
   * \brief Used internally to update horde3d node's transformation matrix using
   * this transform
   */
  static void SetH3DNodeTransform(int node, Transform& transform);

  // iterator
  typedef Array<Transform*>::iterator iterator;
  typedef Array<Transform*>::const_iterator const_iterator;

  iterator begin() { return children.begin(); }
  const_iterator begin() const { return children.begin(); }
  iterator end() { return children.end(); }
  const_iterator end() const { return children.end(); }

  // both called by SetParent
  void AddChild(Transform* transform);
  void RemoveChild(Transform* transform);
  /// Points to the entity that owns
  class Entity* const entity{nullptr};

 private:
  void RecalculateLocalToWorldMatrix();

  // cached results
  Math::Quaternion worldRot;
  Math::Vector3 worldScale;
  Math::Matrix4 localToWorldMatrix{};
  Math::Matrix4 worldToLocalMatrix{};
  // union
  Math::Vector3 axis[3];
  Math::Vector3& left = axis[0];
  Math::Vector3& up = axis[1];
  Math::Vector3& forward = axis[2];

  // local storage
  Math::Vector3 localPos{Math::Vector3::zero};
  Math::Quaternion localRot{Math::Quaternion::identity};
  Math::Vector3 localScale{Math::Vector3::one};

  // marked when anything local changed
  // cleared when matrix recalculated
  void SetDirty();
  bool isDirty{true};
  bool isWorldToLocalDirty{true};

  Transform* parent{nullptr};
  Array<Transform*> children;
};
}  // namespace Isetta