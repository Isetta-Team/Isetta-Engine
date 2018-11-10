/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Debug/Logger.h"

#ifdef _DEBUG
/**
 * @brief ASSERT must evaluate to true otherwise will break
 * 
 */
#define ASSERT(expr) \
  if (expr) {        \
  } else {           \
    __debugbreak();  \
  }

/**
 * @brief ASSERT_LOG must evaluate to true otherwise will log error and break
 * 
 */
#define ASSERT_LOG(expr, channel, msg) \
  if (expr) {                          \
  } else {                             \
    LOG_ERROR(channel, msg);           \
    __debugbreak();                    \
  }
#else
#define ASSERT(expr) ((void)0)
#define ASSERT_LOG(expr)     \
  if (expr) {                \
  } else {                   \
    LOG_ERROR(channel, msg); \
  }
#endif
