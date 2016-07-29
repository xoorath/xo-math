#pragma once
#ifndef XO_GAME_MATH
#define XO_GAME_MATH

////////////////////////////////////////////////////////////////////////// XOMATH_BEGIN_XO_NS, XOMATH_END_XO_NS
#ifdef SINGLE_XO_NS
#   define XOMATH_BEGIN_XO_NS  namespace xo {
#   define XOMATH_END_XO_NS    }
#elif defined(SIMPLE_XO_NS)
#   define XOMATH_BEGIN_XO_NS  namespace xomath {
#   define XOMATH_END_XO_NS    }
#elif defined(NO_XO_NS)
#   define XOMATH_BEGIN_XO_NS
#   define XOMATH_END_XO_NS
#else
#   define XOMATH_BEGIN_XO_NS  namespace xo { namespace math {
#   define XOMATH_END_XO_NS    } }
#endif

////////////////////////////////////////////////////////////////////////// Dependencies for xo-math headers

#include <math.h>
#include <ostream>
#if defined(_MSC_VER)
#   include <xmmintrin.h>
#else
#   include <x86intrin.h>
#endif

#if defined(XOMATH_INTERNAL_MACRO_WARNING) || defined(XOMATH_INTERNAL)
static_assert(false, "Xomath found an internal macro where it shouldn't have.");
#else
#define XOMATH_INTERNAL_MACRO_WARNING static_assert(false, "Xomath found an internal macro where it shouldn't have.");
#endif

#define XOMATH_INTERNAL 1

#if defined(XOMATH_INLINE)
XOMATH_INTERNAL_MACRO_WARNING
#else
#   ifdef NO_XO_INLINE
        // I've found at least on msvc that inline will improve execution time in our test main. on 100k iterations I saw 0.51s without inline and 0.46s with inline.
#       define XOMATH_INLINE
#   elif defined(_MSC_VER)
#       define XOMATH_INLINE __forceinline
#   else
#       define XOMATH_INLINE inline
#   endif
#endif

#if defined(XOMATH_FAST)
XOMATH_INTERNAL_MACRO_WARNING
#else // XOMATH_FAST
#   if defined(NO_XO_FAST)
        // I've found at least on msvc that fastcall doesn't effect our execution time in our test main. on 100k iterations.
#       define XOMATH_FAST(func) func
#   elif defined(_MSC_VER)
#       define XOMATH_FAST(func) __fastcall func
#   elif defined(__GNUC__)
#       if defined(__x86_64__)
#           define XOMATH_FAST(func) func // attribute fast call has no effect on x86_64, as clang would warn us.
#       else
#           define XOMATH_FAST(func) __attribute__((fastcall)) func
#       endif
#   else
#       define XOMATH_FAST(func) func
#   endif
#endif // XOMATH_FAST

XOMATH_BEGIN_XO_NS
constexpr const float PI = 3.141592653589793238462643383279502884197169399375105820f;
constexpr const float PIx2 = 2.0f * PI;
constexpr const float TAU = PIx2;
constexpr const float HalfPI = PI / 2.0f;

constexpr const float FloatEpsilon = 1.192092896e-07F;

constexpr const float Rad2Deg = 360.0f / (PI*2.0f);

// wrap for now, so we have the option to make a faster version later.
XOMATH_INLINE float XOMATH_FAST(Sqrt(float f)) { return sqrtf(f); }
XOMATH_INLINE float XOMATH_FAST(Sin(float f)) { return sinf(f); }
XOMATH_INLINE float XOMATH_FAST(Cos(float f)) { return cosf(f); }
XOMATH_INLINE float XOMATH_FAST(Tan(float f)) { return tanf(f); }
XOMATH_INLINE float XOMATH_FAST(ASin(float f)) { return asinf(f); }
XOMATH_INLINE float XOMATH_FAST(ACos(float f)) { return acosf(f); }
XOMATH_INLINE float XOMATH_FAST(ATan(float f)) { return atanf(f); }
XOMATH_INLINE float XOMATH_FAST(Atan2(float y, float x)) { return atan2f(y, x); }
XOMATH_END_XO_NS

////////////////////////////////////////////////////////////////////////// Math type macros
// used to condense the math type code and make it easier to read

#if defined(VEC2D_SIMPLE_OP) || defined(VEC2D_SIMPLE_OP_ADD) || defined(VEC2_COMPARE_OP) || defined(VEC2_COMPARE_CLOSE_OP)
XOMATH_INTERNAL_MACRO_WARNING
#else
#   define VEC2D_SIMPLE_OP(op) \
    Vector2 operator op (const Vector2& v) const    { return Vector2(x op v[0], y op v[1]); } \
    Vector2 operator op (float v) const             { return Vector2(x op v, y op v); } \
    Vector2 operator op (double v) const            { return Vector2(x op (float)v, y op (float)v); } \
    Vector2 operator op (int v) const               { return Vector2(x op (float)v, y op (float)v); }
#   define VEC2D_SIMPLE_OP_ADD(op) \
    const Vector2& operator op (const Vector2& v)   { x op v[0]; y op v[1]; return *this; } \
    const Vector2& operator op (float v)            { x op v; y op v; return *this; } \
    const Vector2& operator op (double v)           { x op (float)v; y op (float)v; return *this; } \
    const Vector2& operator op (int v)              { x op (float)v; y op (float)v; return *this; }
#   define VEC2_COMPARE_OP(op) \
    bool operator op (const Vector2& v) const       { return MagnitudeSquared() op v.MagnitudeSquared(); } \
    bool operator op (float v) const                { return MagnitudeSquared() op (v*v); } \
    bool operator op (double v) const               { return MagnitudeSquared() op (float)(v*v); } \
    bool operator op (int v) const                  { return MagnitudeSquared() op (float)(v*v); }
#   define VEC2_COMPARE_CLOSE_OP(op) \
    bool operator op (const Vector2& v) const       { return (*this - v).MagnitudeSquared() <= Epsilon; } \
    bool operator op (float v) const                { return MagnitudeSquared() - (v*v) <= Epsilon; } \
    bool operator op (double v) const               { return MagnitudeSquared() - (float)(v*v) <= Epsilon; } \
    bool operator op (int v) const                  { return MagnitudeSquared() - (float)(v*v) <= Epsilon; }
#endif

#if defined(VEC3_COMPARE_OP) || defined(VEC3D_SIMPLE_OP) || defined(VEC3D_SIMPLE_OP_ADD)
XOMATH_INTERNAL_MACRO_WARNING
#else
#   define VEC3D_SIMPLE_OP(op, simd_command) \
    XOMATH_INLINE Vector3 XOMATH_FAST(operator op (const Vector3& v) const)    { auto ret = Vector3(simd_command(m, v.m)); return ret;} \
    XOMATH_INLINE Vector3 XOMATH_FAST(operator op (float v) const)             { auto ret = Vector3(simd_command(m, _mm_set1_ps(v))); return ret; } \
    XOMATH_INLINE Vector3 XOMATH_FAST(operator op (double v) const)            { auto ret = Vector3(simd_command(m, _mm_set1_ps((float)v))); return ret; } \
    XOMATH_INLINE Vector3 XOMATH_FAST(operator op (int v) const)               { auto ret = Vector3(simd_command(m, _mm_set1_ps((float)v))); return ret; }
#   define VEC3D_SIMPLE_OP_ADD(op, simd_command) \
    XOMATH_INLINE const Vector3& XOMATH_FAST(operator op (const Vector3& v))   { m = simd_command(m, v.m); return *this; } \
    XOMATH_INLINE const Vector3& XOMATH_FAST(operator op (float v))            { m = simd_command(m, _mm_set1_ps(v)); return *this; } \
    XOMATH_INLINE const Vector3& XOMATH_FAST(operator op (double v))           { m = simd_command(m, _mm_set1_ps((float)v)); return *this; } \
    XOMATH_INLINE const Vector3& XOMATH_FAST(operator op (int v))              { m = simd_command(m, _mm_set1_ps((float)v)); return *this; }
#   define VEC3_COMPARE_OP(op) \
    XOMATH_INLINE bool XOMATH_FAST(operator op (const Vector3& v) const)       { return MagnitudeSquared() op v.MagnitudeSquared(); } \
    XOMATH_INLINE bool XOMATH_FAST(operator op (float v) const)                { return MagnitudeSquared() op (v*v); } \
    XOMATH_INLINE bool XOMATH_FAST(operator op (double v) const)               { return MagnitudeSquared() op (float)(v*v); } \
    XOMATH_INLINE bool XOMATH_FAST(operator op (int v) const)                  { return MagnitudeSquared() op (float)(v*v); }
#   define VEC3_COMPARE_CLOSE_OP(op) \
    XOMATH_INLINE bool XOMATH_FAST(operator op (const Vector3& v) const)       { return (_mm_movemask_ps(_mm_cmpeq_ps(m, v.m)) & 0b0111) op 0b0111; } \
    XOMATH_INLINE bool XOMATH_FAST(operator op (float v) const)                { return MagnitudeSquared() - (v*v) <= Epsilon; } \
    XOMATH_INLINE bool XOMATH_FAST(operator op (double v) const)               { return MagnitudeSquared() - (float)(v*v) <= Epsilon; } \
    XOMATH_INLINE bool XOMATH_FAST(operator op (int v) const)                  { return MagnitudeSquared() - (float)(v*v) <= Epsilon; }
#endif

#include "Vector2.h"
#include "Vector3.h"

////////////////////////////////////////////////////////////////////////// Type dependent implementations
XOMATH_BEGIN_XO_NS
//Vector3::Vector3(const Vector2& v) : m(_mm_set_ps(0.0f, v.y, v.x)) { }
//Vector2::Vector2(const Vector3& v) : x(v[0]), y(v[1]) { }
//
//Vector3 Vector3::operator + (const Vector2& v) const { return Vector3(x + v[0], y + v[1], z); }
//Vector2 Vector2::operator + (const Vector3& v) const { return Vector2(x + v[0], y + v[1]); }
XOMATH_END_XO_NS

////////////////////////////////////////////////////////////////////////// Remove internal macros

#undef XOMATH_INLINE

#undef VEC2D_SIMPLE_OP
#undef VEC2D_SIMPLE_OP_ADD
#undef VEC2_COMPARE_OP
#undef VEC2_COMPARE_CLOSE_OP

#undef VEC3D_SIMPLE_OP
#undef VEC3D_SIMPLE_OP_ADD
#undef VEC3_COMPARE_OP
#undef VEC3_COMPARE_CLOSE_OP

#undef XOMATH_INTERNAL_MACRO_WARNING
#undef XOMATH_INTERNAL
#undef XOMATH_BEGIN_XO_NS
#undef XOMATH_END_XO_NS

#endif // XO_GAME_MATH