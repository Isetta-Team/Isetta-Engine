/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include <unordered_map>
#include "ISETTA_API.h"
#include "SID/sid.h"

namespace Isetta {
/**
 * @brief Base class of console variables
 *
 */
class ISETTA_API ICVar {
 public:
  /// Key name of CVar
  const std::string name;
  /// StringId of CVar
  const StringId nameID;

  /**
   * @brief Set the Val object, abstract
   *
   * @param strVal convert from this string
   */
  virtual void SetVal(const std::string_view strVal) = 0;
  // virtual StringId GetType() const { return typeID; }

 protected:
  // const StringId typeID;
  /**
   * @brief Construct a new ICVar object, cannot publicly create ICVar
   *
   * @param name
   */
  explicit ICVar(std::string_view name)
      : name{name}, nameID{SID(name.data())} {}
  /**
   * @brief Destroy the ICVar object, cannot instance ICVar
   *
   */
  virtual ~ICVar() {}
};
}  // namespace Isetta
