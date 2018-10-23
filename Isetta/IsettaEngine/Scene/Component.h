/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <bitset>
#include "ISETTA_API.h"

namespace Isetta {
class ISETTA_API Component {
  friend class Entity;

  std::bitset<4> attributes;

 protected:
  class Entity* entity;

  enum class ComponentAttributes {
    IS_ACTIVE,
    HAS_STARTED,
    NEED_DESTROY,
    NEED_UPDATE
  };

  Component();

  void SetAttribute(ComponentAttributes attr, bool value);

  bool GetAttribute(ComponentAttributes attr) const;

 public:
  virtual ~Component() = default;
  void SetActive(bool value);
  bool GetActive() const;
  class Transform& GetTransform() const;
  class Entity* GetEntity() const;
  // TODO(Jacob) possibly remove?
  template <typename CheckType, typename InstanceType>
  bool IsInstanceOf(const InstanceType& instance) {
    return (dynamic_cast<CheckType*>(&instance) != NULL);
  }

  // TODO(Jacob) do we want?
  // template <typename T>
  // T* GetComponent();
  // template <typename T>
  // std::vector<T*> GetComponents();
  // template <typename T>
  // T* GetComponentInParent();
  // template <typename T>
  // std::vector<T*> GetComponentsInParent();
  // template <typename T>
  // T* GetComponentInChildren();
  // template <typename T>
  // std::vector<T*> GetComponentsInChildren();
  // template <typename T>
  // T* GetComponentInDescendant();
  // template <typename T>
  // std::vector<T*> GetComponentsInDescendant();

  virtual void OnEnable() {}
  virtual void Start() {}
  virtual void GuiUpdate() {}
  virtual void Update() {}
  virtual void LateUpdate() {}
  virtual void FixedUpdate() {}
  virtual void OnDestroy() {}
  virtual void OnDisable() {}
};
}  // namespace Isetta