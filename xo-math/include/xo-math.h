// The MIT License (MIT)
//
// Copyright (c) 2016 Jared Thomson
//
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included 
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT 
// OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
// THE USE OR OTHER DEALINGS IN THE SOFTWARE.


// xo-math:
// TODO:
//  * Ensure macros are consistently named.
//  * Remove internal macro warning. Users are probably not stupid.
//  * Selectively start moving methods away from inline, rather than using it everywhere.
//  * Support NEON
//  * Add transformation object.
//  * Matrix
//  *   IsUnitary()
//  *   HasOrthonormalBases()
//  *   HasInverse()
//  *   SafeInverse() or TryInverse()
//  * xo-math
//  *   fast sin/cos, sincos, _x2 _x2 _x3

#ifndef XO_MATH_H
#define XO_MATH_H

#include "xo-math-config.h"

////////////////////////////////////////////////////////////////////////// Optional defines for configuration
#ifdef XO_CUSTOM_NS
#   define XOMATH_BEGIN_XO_NS()  namespace XO_CUSTOM_NS {
#   define XOMATH_END_XO_NS()    }
#elif defined(XO_SPECIFIC_NS)
#   define XOMATH_BEGIN_XO_NS()  namespace xo { namespace math {
#   define XOMATH_END_XO_NS()    } }
#elif defined(XO_SIMPLE_NS)
#   define XOMATH_BEGIN_XO_NS()  namespace math {
#   define XOMATH_END_XO_NS()    }
#elif defined(XO_NO_NS)
#   define XOMATH_BEGIN_XO_NS()
#   define XOMATH_END_XO_NS()
#else
#   define XOMATH_BEGIN_XO_NS()  namespace xo {
#   define XOMATH_END_XO_NS()    }
#endif

#if !defined(XO_ASSERT)
#   define XO_ASSERT(condition, message)
#endif

#if defined(XO_SPACE_LEFTHAND) && defined(XO_SPACE_RIGHTHAND)
static_assert(false, "xo-math found both XO_SPACE_LEFTHAND and XO_SPACE_RIGHTHAND defined. These are incompatible");
#elif !defined(XO_SPACE_LEFTHAND) && !defined(XO_SPACE_RIGHTHAND)
    //! Default to right hand space if no configuration is defined.
    //! @sa https://www.evl.uic.edu/ralph/508S98/coordinates.html
#   define XO_SPACE_RIGHTHAND 1
#endif

#if defined(XO_SPACE_YUP) && defined(XO_SPACE_ZUP)
static_assert(false, "xo-math found both XO_SPACE_YUP and XO_SPACE_ZUP defined. These are incompatible");
#elif !defined(XO_SPACE_YUP) && !defined(XO_SPACE_ZUP)
    //! Default to z up if no configuration is defined.
#   define XO_SPACE_ZUP
#endif

////////////////////////////////////////////////////////////////////////// Dependencies for xo-math headers
#if _MSC_VER 
#pragma warning(push) 
#pragma warning(disable:4265) 
#endif 

#include <math.h>
#ifndef XO_NO_OSTREAM
#   include <ostream>
#endif
#include <random>
#include <thread>
#include <limits>
#if defined(_MSC_VER)
#   include <xmmintrin.h>
#else
#   include <x86intrin.h>
#endif

#if _MSC_VER 
#pragma warning(pop) 
#endif 

// Not available in clang, so far as I can tell.
#ifndef _MM_ALIGN16
#   define _MM_ALIGN16 __attribute__((aligned(16)))
#endif

#if defined(_XOMATH_INTERNAL_MACRO_WARNING)
static_assert(false, "xo-math found an internal macro where it shouldn't have.");
#else
#   define _XOMATH_INTERNAL_MACRO_WARNING static_assert(false, "xo-math found an internal macro where it shouldn't have.");
#endif

#if defined(XOMATH_INTERNAL)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   define XOMATH_INTERNAL 1
#endif

#if !defined(_XO_MATH_STRINGIFY_HELPER)
    // Do not undef at end of file. Will break XO_MATH_VERSION... and XO_MATH_COMPILER... defines
#   define _XO_MATH_STRINGIFY_HELPER(x) #x
#endif

#if !defined(_XO_MATH_STRINGIFY)
    // Do not undef at end of file. Will break XO_MATH_VERSION... and XO_MATH_COMPILER... defines
#   define _XO_MATH_STRINGIFY(x) _XO_MATH_STRINGIFY_HELPER(x)
#endif

#include "DetectSIMD.h"

#if defined(_XOCONSTEXPR) || defined(_XONOCONSTEXPR)
_XOMATH_INTERNAL_MACRO_WARNING
#else
// todo: remove constexpr in visual studio 2013 and re-test for support
#   if defined(_MSC_VER) && _MSC_VER < 1800
#       define _XOCONSTEXPR
#       if !defined(_XONOCONSTEXPR)
#           define _XONOCONSTEXPR
#       endif
#    else
#       define _XOCONSTEXPR constexpr
#    endif
#endif

#if defined(_XOINL)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   ifdef XO_NO_INLINE
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
#   if (defined(__clang__) && defined(__APPLE__))
#       define _XOTLS __thread
#	elif (defined(_MSC_VER) && _MSC_VER < 1800)
#       define _XOTLS __declspec(thread)
#   else
#       define _XOTLS thread_local
#   endif
#endif

#if defined(_XO_TLS_ENGINE)
_XOMATH_INTERNAL_MACRO_WARNING
#else
    // apple clang doesn't give us thread_local until xcode 8.
#   if (defined(__clang__) && defined(__APPLE__)) || (defined(_MSC_VER) && _MSC_VER < 1800)
#       define _XO_TLS_ENGINE \
            static _XOTLS std::mt19937* tls_engline; \
            static _XOTLS char mem[sizeof(std::mt19937)];\
            if(!tls_engline) { \
                tls_engline = new(mem) std::mt19937((unsigned)clock()); \
            }
#   else
#       define _XO_TLS_ENGINE \
            static _XOTLS std::mt19937 tls_engline((unsigned)clock());
#   endif
#endif

#if defined(_XO_TLS_DISTRIBUTION)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   if defined(__clang__) && defined(__APPLE__) || (defined(_MSC_VER) && _MSC_VER < 1800)
#       define _XO_TLS_DISTRIBUTION dist(*tls_engline)
#   else
#       define _XO_TLS_DISTRIBUTION dist(tls_engline)
#   endif
#endif

XOMATH_BEGIN_XO_NS();

_XOCONSTEXPR const float PI = 3.141592653589793238462643383279502884197169399375105820f;
_XOCONSTEXPR const float PIx2 = 2.0f * PI;
_XOCONSTEXPR const float InversePIx2 = 1.0f / PIx2;
_XOCONSTEXPR const float TAU = PIx2;
_XOCONSTEXPR const float HalfPI = PI/2.0f;
_XOCONSTEXPR const float HalfPIx3 = HalfPI*3.0f;
_XOCONSTEXPR const float QuarterPI = PI/4.0f;

_XOCONSTEXPR const float FloatEpsilon = 0.0000001192092896f;

_XOCONSTEXPR const float Rad2Deg = 360.0f / TAU;
_XOCONSTEXPR const float Deg2Rad = TAU / 360.0f;

#if _MSC_VER 
#pragma warning(push) 
#pragma warning(disable:4311)
#pragma warning(disable:4302)
#endif 
_XOINL bool IsAligned16(const void* v) {
    return ((uintptr_t)v & 15) == 0;
}
#if _MSC_VER 
#pragma warning(pop)
#endif 


_XOINL float HexFloat(unsigned u) {
    union {
        unsigned u;
        float f;
    } Converter;
    Converter.u = u;
    return Converter.f;
}

#if defined(XO_SSE)
namespace sse {
    static const __m128 AbsMask = _mm_set1_ps(HexFloat(0x7fffffff));
    static const __m128 SignMask = _mm_set1_ps(HexFloat(0x80000000));

    _XOINL __m128 Abs(__m128 v) {
        return _mm_and_ps(AbsMask, v);
    }

    // the quoted error on _mm_rcp_ps documentation
    _XOCONSTEXPR const float SSEFloatEpsilon = 0.000366210938f;

    static const __m128 Zero = _mm_setzero_ps();
    static const __m128 One = _mm_set1_ps(1.0f);
    static const __m128 NegativeOne = _mm_set1_ps(-1.0f);
    static const __m128 Epsilon = _mm_set_ps1(SSEFloatEpsilon);
}

// We wont warn about pre-defining XO_16ALIGNED_MALLOC or XO_16ALIGNED_FREE.
// If a user wants to create their own allocator, we wont get in the way of that.
#   if !defined(XO_16ALIGNED_MALLOC)
#       define XO_16ALIGNED_MALLOC(size) _mm_malloc(size, 16)
#   endif
#   if  !defined(XO_16ALIGNED_FREE)
#       define XO_16ALIGNED_FREE(ptr) _mm_free(ptr)
#   endif

#   if defined(_XO_OVERLOAD_NEW_DELETE)
_XOMATH_INTERNAL_MACRO_WARNING
#   else
#   define _XO_OVERLOAD_NEW_DELETE() \
        _XOINL static void* operator new (std::size_t size)     { return XO_16ALIGNED_MALLOC(size); } \
        _XOINL static void* operator new[] (std::size_t size)   { return XO_16ALIGNED_MALLOC(size); } \
        _XOINL static void operator delete (void* ptr)          { XO_16ALIGNED_FREE(ptr); } \
        _XOINL static void operator delete[] (void* ptr)        { XO_16ALIGNED_FREE(ptr); }
#   endif

#else
// we don't need to overload new and delete unless memory alignment is required.
#if defined(_XO_OVERLOAD_NEW_DELETE)
_XOMATH_INTERNAL_MACRO_WARNING
#   else
#       define _XO_OVERLOAD_NEW_DELETE()
#   endif
#endif

#if defined(_XO_MIN)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   define _XO_MIN(a, b) (a < b ? a : b)
#endif

#if defined(_XO_MAX)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   define _XO_MAX(a, b) (a > b ? a : b)
#endif

// wrap for now, so we have the option to make a faster version later.
_XOINL float Min(float x, float y)      { return _XO_MIN(x, y); }
_XOINL float Max(float x, float y)      { return _XO_MAX(x, y); }
_XOINL float Abs(float f)               { return f > 0.0f ? f : -f; }
_XOINL float Sqrt(float f)              { return sqrtf(f); } 
_XOINL float Sin(float f)               { return sinf(f); } 
_XOINL float Cos(float f)               { return cosf(f); } 
_XOINL float Tan(float f)               { return tanf(f); }
_XOINL float ASin(float f)              { return asinf(f); }
_XOINL float ACos(float f)              { return acosf(f); }
_XOINL float ATan(float f)              { return atanf(f); } 
_XOINL float ATan2(float y, float x)    { return atan2f(y, x); } 

_XOINL float Difference(float x, float y) { return Abs(x-y); }

_XOINL
void Sin_x2(const float* f, float* s) {
    XO_ASSERT(IsAligned16(f) && IsAligned16(s), "xo-math Sin_x2 requires aligned params.");
    s[0] = Sin(f[0]);
    s[1] = Sin(f[1]);
}

_XOINL
void Sin_x3(const float* f, float* s) {
    XO_ASSERT(IsAligned16(f) && IsAligned16(s), "xo-math Sin_x3 requires aligned params.");
    s[0] = Sin(f[0]);
    s[1] = Sin(f[1]);
    s[2] = Sin(f[2]);
}

_XOINL
void Sin_x4(const float* f, float* s) {
    XO_ASSERT(IsAligned16(f) && IsAligned16(s), "xo-math Sin_x4 requires aligned params.");
    s[0] = Sin(f[0]);
    s[1] = Sin(f[1]);
    s[2] = Sin(f[2]);
    s[3] = Sin(f[3]);
}

_XOINL
void Cos_x2(const float* f, float* c) {
    XO_ASSERT(IsAligned16(f) && IsAligned16(c), "xo-math Cos_x2 requires aligned params.");
    c[0] = Cos(f[0]);
    c[1] = Cos(f[1]);
}

_XOINL
void Cos_x3(const float* f, float* c) {
    XO_ASSERT(IsAligned16(f) && IsAligned16(c), "xo-math Cos_x3 requires aligned params.");
    c[0] = Cos(f[0]);
    c[1] = Cos(f[1]);
    c[2] = Cos(f[2]);
}

_XOINL
void Cos_x4(const float* f, float* c) {
    XO_ASSERT(IsAligned16(f) && IsAligned16(c), "xo-math Cos_x4 requires aligned params.");
    c[0] = Cos(f[0]);
    c[1] = Cos(f[1]);
    c[2] = Cos(f[2]);
    c[3] = Cos(f[3]);
}

_XOINL
void SinCos(float f, float& s, float& c) { 
    s = Sin(f);
    c = Cos(f);
}

_XOINL
void SinCos_x2(const float* f, float* s, float* c) {
    XO_ASSERT(IsAligned16(f) && IsAligned16(s) && IsAligned16(c), "xo-math SinCos_x2 requires aligned params.");
    Sin_x2(f, s);
    Cos_x2(f, c);
}

_XOINL
void SinCos_x3(const float* f, float* s, float* c) {
    XO_ASSERT(IsAligned16(f) && IsAligned16(s) && IsAligned16(c), "xo-math SinCos_x3 requires aligned params.");
    Sin_x3(f, s);
    Cos_x3(f, c);
}

_XOINL
void SinCos_x4(const float* f, float* s, float* c) {
    XO_ASSERT(IsAligned16(f) && IsAligned16(s) && IsAligned16(c), "xo-math SinCos_x4 requires aligned params.");
    Sin_x4(f, s);
    Cos_x4(f, c);
}

_XOINL
bool CloseEnough(float x, float y, float tolerance = FloatEpsilon) {
    return Difference(x, y) * (1.0f/tolerance) <= Min(Abs(x), Abs(y));
}

_XOCONSTEXPR _XOINL float Square(float t)      { return t*t; }
_XOCONSTEXPR _XOINL double Square(double t)    { return t*t; }
_XOCONSTEXPR _XOINL int Square(int t)          { return t*t; }

_XOINL 
bool RandomBool() {
    _XO_TLS_ENGINE
    std::uniform_int_distribution<int> dist(0, 1);
    return _XO_TLS_DISTRIBUTION == 1;
}

_XOINL 
int RandomRange(int low, int high) {
    _XO_TLS_ENGINE
    std::uniform_int_distribution<int> dist(low, high);
    return _XO_TLS_DISTRIBUTION;
}

_XOINL 
float RandomRange(float low, float high) {
    _XO_TLS_ENGINE
    std::uniform_real_distribution<float> dist(low, high);
    return _XO_TLS_DISTRIBUTION;
}

XOMATH_END_XO_NS();

#if defined(_XO_ASSIGN_QUAT)
_XOMATH_INTERNAL_MACRO_WARNING
#   undef _XO_ASSIGN_QUAT
#endif
#if defined(_XO_ASSIGN_QUAT_Q)
_XOMATH_INTERNAL_MACRO_WARNING
#   undef _XO_ASSIGN_QUAT_Q
#endif

#if defined(XO_SSE)
#define _XO_ASSIGN_QUAT(W, X, Y, Z) m = _mm_set_ps(W, Z, Y, X);
#define _XO_ASSIGN_QUAT_Q(Q, W, X, Y, Z) Q.m = _mm_set_ps(W, Z, Y, X);
#else
#define _XO_ASSIGN_QUAT(W, X, Y, Z) this->w = W; this->x = X; this->y = Y; this->z = Z;
#define _XO_ASSIGN_QUAT_Q(Q, W, X, Y, Z) Q.w = W; Q.x = X; Q.y = Y; Q.z = Z;
#endif

////////////////////////////////////////////////////////////////////////// Module Includes
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

#include "Vector2Inline.h"
#include "Vector3Inline.h"
#include "Vector4Inline.h"
#include "Matrix4x4Inline.h"
#include "QuaternionInline.h"

#include "SSE.h"

////////////////////////////////////////////////////////////////////////// Remove internal macros

#if defined(XO_REDEFINABLE)
// For testing it's helpful to change settings and re-include xo-math with another XO_CUSTOM_NS defined as well.
#   undef XO_MATH_H
#endif

// don't undef the namespace macros inside xo-math cpp files.
#if !defined(_XO_MATH_OBJ)
#   undef XOMATH_BEGIN_XO_NS
#   undef XOMATH_END_XO_NS
#endif

#if !defined(XO_EXPORT_ALL)
#   undef _XOMATH_INTERNAL_MACRO_WARNING

#   undef _XOCONSTEXPR
#   undef _XOINL
#   undef _XOTLS

#   undef _XO_TLS_ENGINE
#   undef _XO_TLS_DISTRIBUTION

#   undef _XO_OVERLOAD_NEW_DELETE

#   undef _XO_MIN
#   undef _XO_MAX

#   undef _XO_ASSIGN_QUAT
#   undef _XO_ASSIGN_QUAT_Q

#   undef XOMATH_INTERNAL
#endif

////////////////////////////////////////////////////////////////////////// Add external macros

// version kinds:
// x: experimental, do not use for production.
// a: alpha, for specific feature testing. Contains untested features not in the last major revision.
// b: beta, for broad user testing.
// r: release, all features broadly tested in various applications.
// p: patch release, contains fixes for a release version.

#define XO_MATH_VERSION_DATE "Fall 2016"
#define XO_MATH_VERSION_MAJOR 0
#define XO_MATH_VERSION_KIND "x"
#define XO_MATH_VERSION_MINOR 4
#define XO_MATH_VERSION_SUB 0
#define XO_MATH_VERSION_STR _XO_MATH_STRINGIFY(XO_MATH_VERSION_MAJOR) "." _XO_MATH_STRINGIFY(XO_MATH_VERSION_MINOR) "." XO_MATH_VERSION_KIND _XO_MATH_STRINGIFY(XO_MATH_VERSION_SUB)
#define XO_MATH_VERSION (XO_MATH_VERSION_MAJOR*10000) + (XO_MATH_VERSION_MINOR*1000) + (XO_MATH_VERSION_SUB*100)

#define XO_MATH_VERSION_TXT "xo-math version " XO_MATH_VERSION_STR " " XO_MATH_VERSION_DATE "."

#if defined(_MSC_VER)
#   define XO_MATH_COMPILER_INFO "xo-math v" XO_MATH_VERSION_STR " is compiled with msvc " _XO_MATH_STRINGIFY(_MSC_VER) ", supporting simd: " XO_MATH_HIGHEST_SIMD "."
#elif defined(__clang__)
#   if defined(__APPLE__)
#       define XO_MATH_COMPILER_INFO "xo-math v" XO_MATH_VERSION_STR " is compiled with apple-clang " _XO_MATH_STRINGIFY(__clang_major__) "." _XO_MATH_STRINGIFY(__clang_minor__) "." _XO_MATH_STRINGIFY(__clang_patchlevel__) ", supporting simd: " XO_MATH_HIGHEST_SIMD "."
#   else
#       define XO_MATH_COMPILER_INFO "xo-math v" XO_MATH_VERSION_STR " is compiled with clang " _XO_MATH_STRINGIFY(__clang_major__) "." _XO_MATH_STRINGIFY(__clang_minor__) "." _XO_MATH_STRINGIFY(__clang_patchlevel__) ", supporting simd: " XO_MATH_HIGHEST_SIMD "."
#   endif
#elif defined(__GNUC__)
#   define XO_MATH_COMPILER_INFO "xo-math v" XO_MATH_VERSION_STR " is compiled with gcc " _XO_MATH_STRINGIFY(__GNUC__) "." _XO_MATH_STRINGIFY(__GNUC_MINOR__) "." _XO_MATH_STRINGIFY(__GNUC_PATCHLEVEL__) ", supporting simd: " XO_MATH_HIGHEST_SIMD "."
#else
#   define XO_MATH_COMPILER_INFO "xo-math v" XO_MATH_VERSION_STR " is compiled with an unknown compiler, supporting simd: " XO_MATH_HIGHEST_SIMD "."
#endif

#endif // XO_MATH_H
