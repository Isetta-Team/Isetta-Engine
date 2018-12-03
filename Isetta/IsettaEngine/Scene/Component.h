/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <bitset>
#include <set>
#include <typeindex>
#include <unordered_map>
#include "ISETTA_API.h"

#define DEFINE_COMPONENT(NAME, BASE, UNIQUE)                   \
  template <bool Unique>                                       \
  class ISETTA_API_DECLARE                                     \
      Isetta::ComponentRegistry<class NAME, BASE, Unique> {    \
   protected:                                                  \
    static bool NAME##Registered;                              \
  };                                                           \
  class ISETTA_API_DECLARE NAME                                \
      : public BASE,                                           \
        public Isetta::ComponentRegistry<NAME, BASE, UNIQUE> { \
   protected:                                                  \
    static bool isRegistered() { return NAME##Registered; }    \
                                                               \
   private:

#define DEFINE_COMPONENT_END(NAME, BASE)                                 \
  }                                                                      \
  ;                                                                      \
  template <bool Unique>                                                 \
  bool Isetta::ComponentRegistry<NAME, BASE, Unique>::NAME##Registered = \
      Component::RegisterComponent(std::type_index(typeid(NAME)),        \
                                   std::type_index(typeid(BASE)), Unique);

#define REGISTER_COMPONENT(NAME, BASE, UNIQUE)                           \
  bool Isetta::ComponentRegistry<NAME, BASE, UNIQUE>::NAME##Registered = \
      Component::RegisterComponent(std::type_index(typeid(NAME)),        \
                                   std::type_index(typeid(BASE)), UNIQUE);

namespace Isetta {

template <typename Curr, typename Base, bool Exclude>
struct ISETTA_API_DECLARE ComponentRegistry {};

class ISETTA_API Component {
  friend class Entity;

  std::bitset<7> attributes;

  static std::unordered_map<std::type_index, std::list<std::type_index>>&
  childrenTypes() {
    static std::unordered_map<std::type_index, std::list<std::type_index>>
        children{};
    return children;
  }

  static std::set<std::type_index>& uniqueComponents() {
    static std::set<std::type_index> uniques{};
    return uniques;
  }

  static void FlattenComponentList();
  static void FlattenHelper(std::type_index parent, std::type_index curr);
  static bool isFlattened;

 protected:
  enum class ComponentAttributes {
    IS_ACTIVE,
    HAS_AWAKEN,
    NEED_DESTROY,
    NEED_GUI_UPDATE,
    NEED_UPDATE,
    NEED_LATE_UPDATE,
    NEED_FIXED_UPDATE,
  };

  Component();

  void SetAttribute(ComponentAttributes attr, bool value);

  bool GetAttribute(ComponentAttributes attr) const;

 public:
  virtual ~Component() = default;
  void SetActive(bool value);
  bool GetActive() const;
  class Entity* const entity;
  class Transform* const transform;

  /**
   * \brief Awake is called once, immediately when the component is first
   * created and enabled
   */
  virtual void Awake() {}
  /**
   * \brief OnEnable is called immediately each time the component becomes
   * active, including after creation
   */
  virtual void OnEnable() {}
  /**
   * \brief Start is called once, on the first update frame after the component
   * is created and enabled
   */
  virtual void Start() {}
  /**
   * \brief GuiUpdate is called each frame (variable delta time), GUI can only be called in GuiUpdate
   */
  virtual void GuiUpdate() {
    SetAttribute(ComponentAttributes::NEED_GUI_UPDATE, false);
  }
  /**
   * \brief Update is called each frame (variable delta time)
   */
  virtual void Update() {
    SetAttribute(ComponentAttributes::NEED_UPDATE, false);
  }
  /**
   * \brief LateUpdate is called each frame (variable delta time)
   */
  virtual void LateUpdate() {
    SetAttribute(ComponentAttributes::NEED_LATE_UPDATE, false);
  }
  /**
   * \brief FixedUpdate is called on fixed time (constant delta time)
   */
  virtual void FixedUpdate() {
    SetAttribute(ComponentAttributes::NEED_FIXED_UPDATE, false);
  }
  /**
   * \brief OnDestroy is called once when the component is destroyed
   */
  virtual void OnDestroy() {}
  /**
   * \brief OnDisable is called immediately each time the component becomes inactive
   */
  virtual void OnDisable() {}

  static bool RegisterComponent(std::type_index curr, std::type_index base,
                                bool isExclude);
};

}  // namespace Isetta