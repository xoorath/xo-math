// For testing it's helpful to change settings and re-include xo-math with another XO_CUSTOM_NS defined as well.
#if !defined(XO_REDEFINABLE)
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
// If the user really knows what they're doing, they should be able to override seriously any of these defines they want.
// If XO_NO_VECTOR_DETECT_WARNING is not defined then we'll tell them about the lack of support on their platform 
#elif !defined(XO_NO_VECTOR_DETECT_WARNING)
#   if defined(_MSC_VER)
#       if !(defined(__AVX__) || defined(__AVX2__) || defined(_M_IX86_FP) || _M_IX86_FP < 1) && defined(XO_SSE)
#           undef XO_SSE
#           warning "xo-math detected that XO_SSE is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__) || defined(_M_IX86_FP) || _M_IX86_FP < 2) && defined(XO_SSE2)
#           undef XO_SSE2
#           warning "xo-math detected that XO_SSE2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_SSE3)
#           undef XO_SSE3
#           warning "xo-math detected that XO_SSE3 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_SSSE3)
#           undef XO_SSSE3
#           warning "xo-math detected that XO_SSSE3 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_SSE4)
#           undef XO_SSE4
#           warning "xo-math detected that XO_SSE4 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_AVX)
#           undef XO_AVX
#           warning "xo-math detected that XO_AVX is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__AVX2__) && defined(XO_AVX2)
#           undef XO_AVX2
#           warning "xo-math detected that XO_AVX2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
        // Hey, if you know for a fact that AVX512 is supported on your version of msvc
        // could you please find out if there's a macro to detect it? (such as __AVX512__?)
        // At the time of writing, there is no such macro documented.
        // If there is I'd appreciate a quick email or pull request. I can be reached at jared@xoorath.com
        
        // Assuming __AVX512__ is the macro for example, you can substitute:

//#     if defined(XO_AVX512)
        // for
//#     !defined(__AVX512__) && defined(XO_AVX512)

        // to get your code to stop throwing this warning.
#       if defined(XO_AVX512)
#           undef XO_AVX512
#           warning "xo-math detected that XO_AVX512 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#   elif defined(__clang__) || defined (__gcc__) // Todo: verify the gcc pre-proc, I'm just guessing here.
#       if !defined(__SSE__) && defined(XO_SSE)
#           undef XO_SSE
#           warning "xo-math detected that XO_SSE is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__SSE2__) && defined(XO_SSE2)
#           undef XO_SSE2
#           warning "xo-math detected that XO_SSE2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__SSE3__) && defined(XO_SSE3)
#           undef XO_SSE3
#           warning "xo-math detected that XO_SSE3 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__SSSE3__) && defined(XO_SSSE3)
#           undef XO_SSSE3
#           warning "xo-math detected that XO_SSSE3 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__SSE4_1__) && defined(XO_SSE4)
#           undef XO_SSE4
#           warning "xo-math detected that XO_SSE4 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__AVX__) && defined(XO_AVX)
#           undef XO_AVX
#           warning "xo-math detected that XO_AVX is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__AVX2__) && defined(XO_AVX2)
#           undef XO_AVX2
#           warning "xo-math detected that XO_AVX2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__AVX512__) && defined(XO_AVX512)
#           undef XO_AVX512
#           warning "xo-math detected that XO_AVX512 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
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

// Not available in clang, so far as I can tell.
#ifndef _MM_ALIGN16
#   define _MM_ALIGN16 __attribute__((aligned(16)))
#endif

#if defined(_XOMATH_INTERNAL_MACRO_WARNING) || defined(XOMATH_INTERNAL)
static_assert(false, "xo-math found an internal macro where it shouldn't have.");
#else
#define _XOMATH_INTERNAL_MACRO_WARNING static_assert(false, "xo-math found an internal macro where it shouldn't have.");
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
    // TODO: check if this still requires SSE2 or not. Unsure if removing the shift will do it for us.
    static const __m128 AbsMask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));

    __m128 Abs(__m128 v) {
        return _mm_and_ps(AbsMask, v);
    }
#   endif

    // the quoted error on _mm_rcp_ps documentation
    constexpr const float SSEFloatEpsilon = 0.000366210938f;

    static const __m128 Zero = _mm_setzero_ps();
    static const __m128 One = _mm_set1_ps(1.0f);
    static const __m128 NegativeOne = _mm_set1_ps(-1.0f);
    static const __m128 Epsilon = _mm_set_ps1(SSEFloatEpsilon);
}
#endif

// wrap for now, so we have the option to make a faster version later.
_XOINL float Abs(float f)               { return f > 0.0f ? f : -f; }
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

#if defined(_XO_ASSIGN_QUAT)
_XOMATH_INTERNAL_MACRO_WARNING
#   undef _XO_ASSIGN_QUAT
#endif
#if defined(_XO_ASSIGN_QUAT_Q)
_XOMATH_INTERNAL_MACRO_WARNING
#   undef _XO_ASSIGN_QUAT_Q
#endif

#if XO_SSE
#define _XO_ASSIGN_QUAT(W, X, Y, Z) m = _mm_set_ps(W, Z, Y, X);
#define _XO_ASSIGN_QUAT_Q(Q, W, X, Y, Z) Q.m = _mm_set_ps(W, Z, Y, X);
#else
#define _XO_ASSIGN_QUAT(W, X, Y, Z) this->w = W; this->x = X; this->y = Y; this->z = Z;
#define _XO_ASSIGN_QUAT_Q(Q, W, X, Y, Z) q.w = W; q.x = X; q.y = Y; q.z = Z;
#endif

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

////////////////////////////////////////////////////////////////////////// Funcs dependant on xo-math types.

XOMATH_BEGIN_XO_NS

_XOINL
Vector2 Abs(const Vector2& v) {
    return Vector2(Abs(v.x), Abs(v.y));
}

#if XO_SSE2 // TODO: check if SSE2 is required
_XOINL
Vector3 Abs(const Vector3& v) {
    return (SSE::Abs(v.m));
}

_XOINL
Vector4 Abs(const Vector4& v) {
    return Vector4(SSE::Abs(v.m));
}
#endif

XOMATH_END_XO_NS

////////////////////////////////////////////////////////////////////////// Remove internal macros

#if defined(XO_REDEFINABLE)
#   undef XO_MATH_H
#endif

#undef XOMATH_BEGIN_XO_NS
#undef XOMATH_END_XO_NS

#undef _XOMATH_INTERNAL_MACRO_WARNING

#undef _XOINL
#undef _XOTLS

#undef _XO_ASSIGN_QUAT
#undef _XO_ASSIGN_QUAT_Q

#undef XOMATH_INTERNAL


#endif // XO_MATH_H