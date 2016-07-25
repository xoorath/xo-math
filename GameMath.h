#pragma once
#ifndef XO_GAME_MATH
#define XO_GAME_MATH

////////////////////////////////////////////////////////////////////////// XOMATH_BEGIN_XO_NS, XOMATH_END_XO_NS
#ifdef SINGLE_XO_NS
  #define XOMATH_BEGIN_XO_NS  namespace xo {
  #define XOMATH_END_XO_NS    }
#elif defined(SIMPLE_XO_NS)
  #define XOMATH_BEGIN_XO_NS  namespace xomath {
  #define XOMATH_END_XO_NS    }
#elif defined(NO_XO_NS)
  #define XOMATH_BEGIN_XO_NS
  #define XOMATH_END_XO_NS
#else
  #define XOMATH_BEGIN_XO_NS  namespace xo { namespace math {
  #define XOMATH_END_XO_NS    } }
#endif

////////////////////////////////////////////////////////////////////////// Dependencies for xo-math headers

// Use XO_GAME_MATH_INTERNAL to sanity check inside other xo-math headers
// it's kind of overkill, but xo-math is also easy to misuse, it's so
// darn tempting to just #include "Vector3.h" when that's what you need.
// Doesn't work that way, Homie. The types in here are dependent.
#define XO_GAME_MATH_INTERNAL 1

XOMATH_BEGIN_XO_NS

constexpr const float PI = 3.141592653589793238462643383279502884197169399375105820f;
constexpr const float PIx2 = 2.0f * PI;
constexpr const float TAU = PIx2;
constexpr const float HalfPI = PI / 2.0f;

constexpr const float FloatEpsilon = 1.192092896e-07F;

constexpr const float Rad2Deg = 360.0f / (PI*2.0f);
XOMATH_END_XO_NS

#include <math.h>
#include <cfloat>

#include "Vector2.h"
#include "Vector3.h"

////////////////////////////////////////////////////////////////////////// Type dependent implementations
XOMATH_BEGIN_XO_NS
Vector3::Vector3(const Vector2& v) : x(v[0]), y(v[1]), z(0.0f) { }
Vector2::Vector2(const Vector3& v) : x(v[0]), y(v[1]) { }

Vector3 Vector3::operator + (const Vector2& v) const { return Vector3(x + v[0], y + v[1], z); }
Vector2 Vector2::operator + (const Vector3& v) const { return Vector2(x + v[0], y + v[1]); }
XOMATH_END_XO_NS

////////////////////////////////////////////////////////////////////////// Remove internal macros
#undef XO_GAME_MATH_INTERNAL
#undef XOMATH_BEGIN_XO_NS
#undef XOMATH_END_XO_NS

#endif XO_GAME_MATH