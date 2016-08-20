#if !defined(XO_REDEFINABLE) // For testing it's helpful to change settings and re-include xo-math.
#   pragma once
#endif
#ifndef XO_MATH_H
#define XO_MATH_H

////////////////////////////////////////////////////////////////////////// auto SSE detection based on compiler settings
// First check that the user isn't overriding our SSE settings. If they are, it's up to them to define all of it.
#if !defined(XO_SSE) && !defined(XO_SSE2) && !defined(XO_SSE3) && !defined(XO_SSSE3) && !defined(XO_SSE4) && !defined(XO_AVX) && !defined(XO_AVX2) && !defined(XO_AVX512)
#   if defined(_MSC_VER)
#       if defined(_M_IX86_FP)
#           if _M_IX86_FP == 1
#               define XO_SSE 1
#           elif _M_IX86_FP == 2
#               define XO_SSE 1
#               define XO_SSE2 1
#           endif
#       endif
#       if defined(__AVX__)
#           define XO_SSE 1
#           define XO_SSE2 1
#           define XO_SSE3 1
#           define XO_SSSE3 1
#           define XO_SSE4 1 // Todo: specify 4_1 and 4_2
#           define XO_AVX 1
#       endif
#       if defined(__AVX2__)
#           define XO_SSE 1
#           define XO_SSE2 1
#           define XO_SSE3 1
#           define XO_SSSE3 1
#           define XO_SSE4 1 // Todo: specify 4_1 and 4_2
#           define XO_AVX 1
#           define XO_AVX2 1
#       endif
// TODO: add AVX512 for msvc when it exists.
#   elif defined(__clang__) || defined (__gcc__) // Todo: verify the gcc pre-proc, I'm just guessing here.
#       if defined(__SSE__)
#           define XO_SSE 1
#       endif
#       if defined(__SSE2__)
#           define XO_SSE2 1
#       endif
#       if defined(__SSE3__)
#           define XO_SSE3 1
#       endif
#       if defined(__SSSE3__)
#           define XO_SSSE3 1
#       endif
#       if defined(__SSE4_1__)
#           define XO_SSE4 1 // Todo: specify 4_1 and 4_2 like gcc does
#       endif
#       if defined(__AVX__)
#           define XO_AVX 1
#       endif
#       if defined(__AVX2__) // Todo: verify this pre-proc
#           define XO_AVX2 1
#       endif
#       if defined(__AVX512__) // Todo: verify this pre-proc
#           define XO_AVX512 1
#       endif
#   endif
#endif

////////////////////////////////////////////////////////////////////////// XOMATH_BEGIN_XO_NS, XOMATH_END_XO_NS
#ifdef XO_CUSTOM_NS
#   define XOMATH_BEGIN_XO_NS  namespace XO_CUSTOM_NS {
#   define XOMATH_END_XO_NS    }
#elif defined(XO_SINGLE_NS)
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
#   ifdef __clang__
        // todo: find a way to get thread local storage out of clang which allows for non const-expr initialization
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

    static const _MM_ALIGN16 __m128 Zero = _mm_setzero_ps();
    static const _MM_ALIGN16 __m128 One = _mm_set1_ps(1.0f);
    static const _MM_ALIGN16 __m128 NegativeOne = _mm_set1_ps(-1.0f);
    static const _MM_ALIGN16 __m128 Epsilon = _mm_set_ps1(SSEFloatEpsilon);
}
#endif

// wrap for now, so we have the option to make a faster version later.
_XOINL float Sqrt(float f)              { return sqrtf(f); }
_XOINL float Sin(float f)               { return sinf(f); }
_XOINL float Cos(float f)               { return cosf(f); }
_XOINL float Tan(float f)               { return tanf(f); }
_XOINL float ASin(float f)              { return asinf(f); }
_XOINL float ACos(float f)              { return acosf(f); }
_XOINL float ATan(float f)              { return atanf(f); }
_XOINL float ATan2(float y, float x)    { return atan2f(y, x); }
_XOINL bool CloseEnough(float x, float y, float tolerance = FloatEpsilon) { return fabs(y - x) < tolerance; }


constexpr _XOINL float Square(float t)      { return t*t; }
constexpr _XOINL double Square(double t)    { return t*t; }
constexpr _XOINL int Square(int t)          { return t*t; }

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

////////////////////////////////////////////////////////////////////////// Module Includes

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

#include "Vector2Operators.h"
#include "Vector2Methods.h"
#include "Vector3Operators.h"
#include "Vector3Methods.h"
#include "Vector4Operators.h"
#include "Vector4Methods.h"
#include "Matrix4x4Operators.h"
#include "Matrix4x4Methods.h"
#include "QuaternionOperators.h"
#include "QuaternionMethods.h"

////////////////////////////////////////////////////////////////////////// Remove internal macros

#if defined(XO_REDEFINABLE)
#   undef XO_MATH_H
#endif

#undef XOMATH_BEGIN_XO_NS
#undef XOMATH_END_XO_NS

#undef _XOMATH_INTERNAL_MACRO_WARNING

#undef _XOINL
#undef _XOTLS

#undef XOMATH_INTERNAL


#endif // XO_MATH_H