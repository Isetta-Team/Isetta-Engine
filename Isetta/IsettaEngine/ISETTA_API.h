/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#ifdef IN_ENGINE
#define ISETTA_API
#else
#ifdef ISETTA_EXPORTS
#define ISETTA_API __declspec(dllexport)
#else
#define ISETTA_API __declspec(dllimport)
#endif
#endif
