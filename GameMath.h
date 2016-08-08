#pragma once
#ifndef XO_GAME_MATH
#define XO_GAME_MATH

////////////////////////////////////////////////////////////////////////// XOMATH_BEGIN_XO_NS, XOMATH_END_XO_NS
#ifdef XO_SINGLE_NS
#   define XOMATH_BEGIN_XO_NS  namespace xo {
#   define XOMATH_END_XO_NS    }
#elif defined(XO_SIMPLE_NS)
#   define XOMATH_BEGIN_XO_NS  namespace xomath {
#   define XOMATH_END_XO_NS    }
#elif defined(XO_NO_NS)
#   define XOMATH_BEGIN_XO_NS
#   define XOMATH_END_XO_NS
#else
#   define XOMATH_BEGIN_XO_NS  namespace xo { namespace math {
#   define XOMATH_END_XO_NS    } }
#endif

////////////////////////////////////////////////////////////////////////// Dependencies for xo-math headers

#include <math.h>
#include <ostream>
#include <random>
#include <thread>
#if defined(_MSC_VER)
#   include <xmmintrin.h>
#else
#   include <x86intrin.h>
#endif

#ifndef _MM_ALIGN16
#   define _MM_ALIGN16 __attribute__((aligned(16)))
#endif

#if defined(_XOMATH_INTERNAL_MACRO_WARNING) || defined(XOMATH_INTERNAL)
static_assert(false, "Xomath found an internal macro where it shouldn't have.");
#else
#define _XOMATH_INTERNAL_MACRO_WARNING static_assert(false, "Xomath found an internal macro where it shouldn't have.");
#endif

#define XOMATH_INTERNAL 1

#if defined(_XOINL)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   ifdef NO_XO_INLINE
        // I've found at least on msvc that inline will improve execution time in our test main. on 100k iterations I saw 0.51s without inline and 0.46s with inline.
#       define _XOINL
#   elif defined(_MSC_VER)
#       define _XOINL __forceinline
#   else
#       define _XOINL inline
#   endif
#endif

#if defined(_XOTLS)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   ifdef __APPLE__
        // todo: find a way to get thread local storage out of xcode clang which allows for
        // non const-expr initialization
#       define _XOTLS
#   else
#       define _XOTLS thread_local
#   endif
#endif

XOMATH_BEGIN_XO_NS
constexpr const float PI = 3.141592653589793238462643383279502884197169399375105820f;
constexpr const float PIx2 = 2.0f * PI;
constexpr const float TAU = PIx2;
constexpr const float HalfPI = PI / 2.0f;

constexpr const float FloatEpsilon = 0.0000001192092896f;

constexpr const float Rad2Deg = 360.0f / TAU;
constexpr const float Deg2Rad = TAU / 360.0f;

#if XO_SSE
namespace SSE {
#   if XO_SSE2
    static const __m128 AbsMask = _mm_castsi128_ps(_mm_srli_epi32(_mm_set1_epi32(-1), 1));

    __m128 Abs(__m128 v) {
        return _mm_and_ps(AbsMask, v);
    }
#   endif

    // the quoted error on _mm_rcp_ps documentation
    constexpr const float SSEFloatEpsilon = 0.000366210938f;

    static const __m128 Zero = _mm_setzero_ps();
    static const __m128 One = _mm_set1_ps(1.0f);
    static const __m128 Epsilon = _mm_set_ps1(SSEFloatEpsilon);
}
#endif

// wrap for now, so we have the option to make a faster version later.
_XOINL 
float Sqrt(float f) { return sqrtf(f); }

_XOINL 
float Sin(float f) { return sinf(f); }

_XOINL 
float Cos(float f) { return cosf(f); }

_XOINL 
float Tan(float f) { return tanf(f); }

_XOINL 
float ASin(float f) { return asinf(f); }

_XOINL 
float ACos(float f) { return acosf(f); }

_XOINL 
float ATan(float f) { return atanf(f); }

_XOINL 
float Atan2(float y, float x) { return atan2f(y, x); }

_XOINL 
bool CloseEnough(float x, float y, float tolerance = FloatEpsilon) { return fabs(y - x) < tolerance; }

template<typename T>
constexpr _XOINL 
T Square(const T& t) {
    return t*t;
}

_XOINL 
bool RandomBool() {
    static _XOTLS std::mt19937 engine((unsigned)time(nullptr));
    std::uniform_int_distribution<int> dist(0, 1);
    return dist(engine) == 1;
}

_XOINL 
int RandomRange(int low, int high) {
    static _XOTLS std::mt19937 engine((unsigned)time(nullptr));
    std::uniform_int_distribution<int> dist(low, high);
    return dist(engine);
}

_XOINL 
float RandomRange(float low, float high) {
    static _XOTLS std::mt19937 engine((unsigned)time(nullptr));
    std::uniform_real_distribution<float> dist(low, high);
    return dist(engine);
}

XOMATH_END_XO_NS

////////////////////////////////////////////////////////////////////////// Math type macros
// used to condense the math type code and make it easier to read

#if defined(VEC2D_SIMPLE_OP) || defined(VEC2D_SIMPLE_OP_ADD) || defined(VEC2_COMPARE_OP) || defined(VEC2_COMPARE_CLOSE_OP)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   define VEC2D_SIMPLE_OP(op) \
    _XOINL Vector2 operator op (const Vector2& v) const    { return Vector2(x op v[0], y op v[1]); } \
    _XOINL Vector2 operator op (float v) const             { return Vector2(x op v, y op v); } \
    _XOINL Vector2 operator op (double v) const            { return Vector2(x op (float)v, y op (float)v); } \
    _XOINL Vector2 operator op (int v) const               { return Vector2(x op (float)v, y op (float)v); } \
    _XOINL Vector2 operator op (const class Vector3& v) const; \
    _XOINL Vector2 operator op (const class Vector4& v) const;
#   define VEC2D_SIMPLE_OP_ADD(op) \
    _XOINL const Vector2& operator op (const Vector2& v)   { x op v[0]; y op v[1]; return *this; } \
    _XOINL const Vector2& operator op (float v)            { x op v; y op v; return *this; } \
    _XOINL const Vector2& operator op (double v)           { x op (float)v; y op (float)v; return *this; } \
    _XOINL const Vector2& operator op (int v)              { x op (float)v; y op (float)v; return *this; }\
    _XOINL const Vector2& operator op (const class Vector3& v); \
    _XOINL const Vector2& operator op (const class Vector4& v);
#   define VEC2_COMPARE_OP(op) \
    _XOINL bool operator op (const Vector2& v) const       { return MagnitudeSquared() op v.MagnitudeSquared(); } \
    _XOINL bool operator op (float v) const                { return MagnitudeSquared() op (v*v); } \
    _XOINL bool operator op (double v) const               { return MagnitudeSquared() op (float)(v*v); } \
    _XOINL bool operator op (int v) const                  { return MagnitudeSquared() op (float)(v*v); } \
    _XOINL bool operator op (const class Vector3& v) const; \
    _XOINL bool operator op (const class Vector4& v) const;
#   define VEC2_COMPARE_CLOSE_OP(op) \
    _XOINL bool operator op (const Vector2& v) const       { return (*this - v).MagnitudeSquared() <= Epsilon; } \
    _XOINL bool operator op (float v) const                { return MagnitudeSquared() - (v*v) <= Epsilon; } \
    _XOINL bool operator op (double v) const               { return MagnitudeSquared() - (float)(v*v) <= Epsilon; } \
    _XOINL bool operator op (int v) const                  { return MagnitudeSquared() - (float)(v*v) <= Epsilon; } \
    _XOINL bool operator op (const class Vector3& v) const; \
    _XOINL bool operator op (const class Vector4& v) const;
#endif

#if defined(VEC3_COMPARE_OP) || defined(VEC3D_SIMPLE_OP) || defined(VEC3D_SIMPLE_OP_ADD)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   define VEC3_COMPARE_OP(op) \
    _XOINL bool operator op (const Vector3& v) const       { return MagnitudeSquared() op v.MagnitudeSquared(); } \
    _XOINL bool operator op (float v) const                { return MagnitudeSquared() op (v*v); } \
    _XOINL bool operator op (double v) const               { return (*this) op (float)v; } \
    _XOINL bool operator op (int v) const                  { return (*this) op (float)v; } \
    _XOINL bool operator op (const class Vector2& v) const; \
    _XOINL bool operator op (const class Vector4& v) const;
#   if XO_SSE
#       define VEC3_COMPARE_CLOSE_OP(op, andor) \
        _XOINL bool operator op (const Vector3& v) const { \
            return (_mm_movemask_ps(_mm_cmpeq_ps(m, v.m)) & 0b0111) op 0b0111; \
        } \
        _XOINL bool operator op (float v) const                { return MagnitudeSquared() - (v*v) op Epsilon; } \
        _XOINL bool operator op (double v) const               { return (*this) op (float)v; } \
        _XOINL bool operator op (int v) const                  { return (*this) op (float)v; } \
        _XOINL bool operator op (const class Vector2& v) const; \
        _XOINL bool operator op (const class Vector4& v) const;
#   else
#       define VEC3_COMPARE_CLOSE_OP(op, andor) \
        _XOINL bool operator op (const Vector3& v) const { \
            return x op v.x andor y op v.y andor z op v.z; \
        } \
        _XOINL bool operator op (float v) const                { return MagnitudeSquared() - (v*v) op Epsilon; } \
        _XOINL bool operator op (double v) const               { return (*this) op (float)v; } \
        _XOINL bool operator op (int v) const                  { return (*this) op (float)v; } \
        _XOINL bool operator op (const class Vector2& v) const; \
        _XOINL bool operator op (const class Vector4& v) const;
#   endif
#endif

#if defined(VEC4_COMPARE_OP) || defined(VEC4D_SIMPLE_OP) || defined(VEC4D_SIMPLE_OP_ADD)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   if XO_SSE
#       define VEC4D_SIMPLE_OP(op, simd_command) \
            _XOINL Vector4 operator op (const Vector4& v) const { \
                return Vector4(simd_command(m, v.m)); \
            } \
            _XOINL Vector4 operator op (float v) const { \
                return Vector4(simd_command(m, _mm_set1_ps(v))); \
            } \
            _XOINL Vector4 operator op (double v) const            { return (*this) op (float)v; } \
            _XOINL Vector4 operator op (int v) const               { return (*this) op (float)v; } \
            _XOINL Vector4 operator op (const class Vector2& v) const; \
            _XOINL Vector4 operator op (const class Vector3& v) const;
#   else
#       define VEC4D_SIMPLE_OP(op, simd_command) \
            _XOINL Vector4 operator op (const Vector4& v) const { \
                return Vector4(x op v.x, y op v.y, z op v.z, w op v.w);\
            } \
            _XOINL Vector4 operator op (float v) const { \
                return Vector4(x op v, y op v, z op v, z op v); \
            } \
            _XOINL Vector4 operator op (double v) const            { return (*this) op (float)v; } \
            _XOINL Vector4 operator op (int v) const               { return (*this) op (float)v; } \
            _XOINL Vector4 operator op (const class Vector2& v) const; \
            _XOINL Vector4 operator op (const class Vector3& v) const;
#   endif
#   define VEC4D_SIMPLE_OP_ADD(op, simple_op, simd_command) \
    _XOINL const Vector4& operator op (const Vector4& v)   { return (*this) = (*this) simple_op v; } \
    _XOINL const Vector4& operator op (float v)            { return (*this) = (*this) simple_op v; } \
    _XOINL const Vector4& operator op (double v)           { return (*this) = (*this) simple_op v; } \
    _XOINL const Vector4& operator op (int v)              { return (*this) = (*this) simple_op v; } \
    _XOINL const Vector4& operator op (const class Vector2& v); \
    _XOINL const Vector4& operator op (const class Vector3& v);
#   define VEC4_COMPARE_OP(op) \
    _XOINL bool operator op (const Vector4& v) const       { return MagnitudeSquared() op v.MagnitudeSquared(); } \
    _XOINL bool operator op (float v) const                { return MagnitudeSquared() op (v*v); } \
    _XOINL bool operator op (double v) const               { return (*this) op (float)v; } \
    _XOINL bool operator op (int v) const                  { return (*this) op (float)v; } \
    _XOINL bool operator op (const class Vector2& v) const; \
    _XOINL bool operator op (const class Vector3& v) const;
#   if XO_SSE
#   define VEC4_COMPARE_CLOSE_OP(op, andor) \
        _XOINL bool operator op (const Vector4& v) const { \
            return (_mm_movemask_ps(_mm_cmpeq_ps(m, v.m)) & 0b1111) op 0b1111; \
        } \
        _XOINL bool operator op (float v) const                { return MagnitudeSquared() - (v*v) op Epsilon; } \
        _XOINL bool operator op (double v) const               { return (*this) op (float)v; } \
        _XOINL bool operator op (int v) const                  { return (*this) op (float)v; } \
        _XOINL bool operator op (const class Vector2& v) const; \
        _XOINL bool operator op (const class Vector3& v) const;
#   else
#   define VEC4_COMPARE_CLOSE_OP(op, andor) \
        _XOINL bool operator op (const Vector4& v) const { \
            return x op v.x andor y op v.y andor z op v.z andor w op v.w; \
        } \
        _XOINL bool operator op (float v) const                { return MagnitudeSquared() - (v*v) op Epsilon; } \
        _XOINL bool operator op (double v) const               { return (*this) op (float)v; } \
        _XOINL bool operator op (int v) const                  { return (*this) op (float)v; } \
        _XOINL bool operator op (const class Vector2& v) const; \
        _XOINL bool operator op (const class Vector3& v) const;
#   endif
#endif

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#include "Vector2Operators.h"
#include "Vector2Methods.h"
#include "Vector3Operators.h"
#include "Vector3Methods.h"
#include "Vector4Operators.h"
#include "Vector4Methods.h"

////////////////////////////////////////////////////////////////////////// Type dependent implementations
XOMATH_BEGIN_XO_NS
Vector2::Vector2(const Vector3& v) : x(v.x), y(v.y) { }
Vector2::Vector2(const Vector4& v) : x(v.x), y(v.y) { }

Vector4::Vector4(const Vector2& v) :
#if XO_SSE
    m(_mm_set_ps(0.0f, 0.0f, v.y, v.x)) { }
#else
    x(v.x), y(v.y), z(0.0f), w(0.0f) { }
#endif

Vector4::Vector4(const Vector3& v) :
#if XO_SSE
        m(v.m) { }
#else
        x(v.x), y(v.y), z(v.z), w(0.0f) { }
#endif

#define XOMATH_CONVERT_OP(ourtype, thiertype) \
    _XOINL ourtype ourtype ::operator + (const thiertype& v) const { return (*this) + ourtype(v); } \
    _XOINL ourtype ourtype ::operator - (const thiertype& v) const { return (*this) - ourtype(v); } \
    _XOINL ourtype ourtype ::operator * (const thiertype& v) const { return (*this) * ourtype(v); } \
    _XOINL ourtype ourtype ::operator / (const thiertype& v) const { return (*this) / ourtype(v); } \
    _XOINL const ourtype & ourtype ::operator += (const thiertype& v) { return (*this) += ourtype(v); } \
    _XOINL const ourtype & ourtype ::operator -= (const thiertype& v) { return (*this) -= ourtype(v); } \
    _XOINL const ourtype & ourtype ::operator *= (const thiertype& v) { return (*this) *= ourtype(v); } \
    _XOINL const ourtype & ourtype ::operator /= (const thiertype& v) { return (*this) /= ourtype(v); } \
    _XOINL bool ourtype ::operator == (const thiertype& v) const { return MagnitudeSquared() == ourtype(v).MagnitudeSquared(); } \
    _XOINL bool ourtype ::operator != (const thiertype& v) const { return MagnitudeSquared() != ourtype(v).MagnitudeSquared(); } \
    _XOINL bool ourtype ::operator < (const thiertype& v) const { return MagnitudeSquared() < ourtype(v).MagnitudeSquared(); } \
    _XOINL bool ourtype ::operator <= (const thiertype& v) const { return MagnitudeSquared() <= ourtype(v).MagnitudeSquared(); } \
    _XOINL bool ourtype ::operator > (const thiertype& v) const { return MagnitudeSquared() > ourtype(v).MagnitudeSquared(); } \
    _XOINL bool ourtype ::operator >= (const thiertype& v) const { return MagnitudeSquared() >= ourtype(v).MagnitudeSquared(); }

XOMATH_CONVERT_OP(Vector2, Vector3);
XOMATH_CONVERT_OP(Vector2, Vector4);

XOMATH_CONVERT_OP(Vector4, Vector2);
XOMATH_CONVERT_OP(Vector4, Vector3);

XOMATH_END_XO_NS

////////////////////////////////////////////////////////////////////////// Remove internal macros

#undef _XOINL
#undef _XOTLS

#undef _XOMATH_INTERNAL_MACRO_WARNING

#undef VEC2D_SIMPLE_OP
#undef VEC2D_SIMPLE_OP_ADD
#undef VEC2_COMPARE_OP
#undef VEC2_COMPARE_CLOSE_OP

#undef VEC3D_SIMPLE_OP
#undef VEC3D_SIMPLE_OP_ADD
#undef VEC3_COMPARE_OP
#undef VEC3_COMPARE_CLOSE_OP

#undef XOMATH_INTERNAL
#undef XOMATH_BEGIN_XO_NS
#undef XOMATH_END_XO_NS

#endif // XO_GAME_MATH