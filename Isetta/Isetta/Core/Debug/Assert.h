/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#ifdef _DEBUG
#define ASSERT(expr) \
  if (expr) {        \
  } else {           \
    __debugbreak();  \
  }
#else
#define ASSERT(expr) ((void)0)
#endif