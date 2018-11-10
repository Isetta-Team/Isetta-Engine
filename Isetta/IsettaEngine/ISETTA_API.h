/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#if defined(IN_ENGINE) || defined(IN_TEST)
#define ISETTA_API
#define ISETTA_API_DECLARE
#else
#ifdef ISETTA_EXPORTS
#define ISETTA_API __declspec(dllexport)
#define ISETTA_API_DECLARE __declspec(dllexport)
#else
#define ISETTA_API __declspec(dllimport)
#define ISETTA_API_DECLARE
#endif
#endif
