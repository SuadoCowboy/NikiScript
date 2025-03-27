#pragma once

#ifdef BUILD_SHARED
#define NIKIAPI __declspec(dllexport)
#else
#define NIKIAPI __declspec(dllimport)
#endif