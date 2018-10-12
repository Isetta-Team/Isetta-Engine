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
  class Entity* owner;

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