#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#define M_PI_F (static_cast<float>(M_PI))

#define DEG2RAD(x) ((x) * (M_PI_F / 180.0f))
#define RAD2DEG(x) ((x) * (180.0f / M_PI_F))
