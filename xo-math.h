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

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include DetectSIMD.h directly. Include xo-math.h.");
#else // XOMATH_INTERNAL

#if !defined(XO_SSE) && !defined(XO_SSE2) && !defined(XO_SSE3) && !defined(XO_SSSE3) && !defined(XO_SSE4_1) && !defined(XO_SSE4_2) && !defined(XO_AVX) && !defined(XO_AVX2) && !defined(XO_AVX512)
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
#           define XO_SSE4_1 1
#           define XO_SSE4_2 1
#           define XO_AVX 1
#       endif
#       if defined(__AVX2__)
#           define XO_SSE 1
#           define XO_SSE2 1
#           define XO_SSE3 1
#           define XO_SSSE3 1
#           define XO_SSE4_1 1
#           define XO_SSE4_2 1
#           define XO_AVX 1
#           define XO_AVX2 1
#       endif
#   elif defined(__clang__) || defined (__GNUC__)
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
#           define XO_SSE4_1 1
#       endif
#       if defined(__SSE4_2__)
#           define XO_SSE4_2 1
#       endif
#       if defined(__AVX__)
#           define XO_AVX 1
#       endif
#       if defined(__AVX2__)
#           define XO_AVX2 1
#       endif
#       if defined(__AVX512__) || defined(__AVX512F__)
#           define XO_AVX512 1
#       endif
#   endif
// If the user really knows what they're doing, they should be able to override seriously any of these defines they want.
// If XO_NO_VECTOR_DETECT_WARNING is not defined then we'll tell them about the lack of support on their platform 
#elif !defined(XO_NO_VECTOR_DETECT_WARNING)
#   if defined(_MSC_VER)
#       if defined(_XO_MATH_MSC_PRAGMALINK)
_XOMATH_INTERNAL_MACRO_WARNING
#       else
#           define _XO_MATH_MSC_PRAGMALINK __FILE__ "(" _XO_MATH_STRINGIFY(__LINE__) ") : "
#       endif
#       if defined(_XO_MATH_WARN)
_XOMATH_INTERNAL_MACRO_WARNING
#       else
#           define _XO_MATH_WARN(exp) (_XO_MATH_MSC_PRAGMALINK "WARNING: " exp)
#       endif

#       if !(defined(__AVX__) || defined(__AVX2__) || defined(_M_IX86_FP) || _M_IX86_FP < 1) && defined(XO_SSE)
#           undef XO_SSE
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSE is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__) || defined(_M_IX86_FP) || _M_IX86_FP < 2) && defined(XO_SSE2)
#           undef XO_SSE2
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSE2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_SSE3)
#           undef XO_SSE3
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSE3 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_SSSE3)
#           undef XO_SSSE3
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSSE3 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_SSE4_1)
#           undef XO_SSE4_1
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSE4_1 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_SSE4_2)
#           undef XO_SSE4_2
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSE4_2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_AVX)
#           undef XO_AVX
#           pragma message _XO_MATH_WARN("xo-math detected that XO_AVX is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !defined(__AVX2__) && defined(XO_AVX2)
#           undef XO_AVX2
#           pragma message _XO_MATH_WARN("xo-math detected that XO_AVX2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
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
#           pragma message _XO_MATH_WARN("xo-math detected that XO_AVX512 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif

#   undef _XO_MATH_MSC_PRAGMALINK
#   undef _XO_MATH_WARN
#   elif defined(__clang__) || defined (__GNUC__)
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
#       if !defined(__SSE4_1__) && defined(XO_SSE4_1)
#           undef XO_SSE4_1
#           warning "xo-math detected that XO_SSE4_1 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__SSE4_2__) && defined(XO_SSE4_2)
#           undef XO_SSE4_2
#           warning "xo-math detected that XO_SSE4_2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
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

#if defined(XO_AVX512)
#   define XO_MATH_HIGHEST_SIMD "avx512"
#elif defined(XO_AVX2)
#   define XO_MATH_HIGHEST_SIMD "avx2"
#elif defined(XO_AVX)
#   define XO_MATH_HIGHEST_SIMD "avx"
#elif defined(XO_SSE4_2)
#   define XO_MATH_HIGHEST_SIMD "sse4.2"
#elif defined(XO_SSE4_1)
#   define XO_MATH_HIGHEST_SIMD "sse4.1"
#elif defined(XO_SSSE3)
#   define XO_MATH_HIGHEST_SIMD "ssse3"
#elif defined(XO_SSE3)
#   define XO_MATH_HIGHEST_SIMD "sse3"
#elif defined(XO_SSE2)
#   define XO_MATH_HIGHEST_SIMD "sse2"
#elif defined(XO_SSE)
#   define XO_MATH_HIGHEST_SIMD "sse"
#else
#   define XO_MATH_HIGHEST_SIMD "none"
#endif

#endif // XOMATH_INTERNAL


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
#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector2.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

class _MM_ALIGN16 Vector2 {
public:

    ////////////////////////////////////////////////////////////////////////// Constructors
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#constructors
    Vector2(); 
    Vector2(float v); 
    Vector2(float x, float y); 
    Vector2(const Vector2& v); 
    Vector2(const class Vector3& v); 
    Vector2(const class Vector4& v); 

    ////////////////////////////////////////////////////////////////////////// Set / Get Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#set_get_methods
    const Vector2& Set(float x, float y);
    const Vector2& Set(float f);
    const Vector2& Set(const Vector2& vec);
    void Get(float& x, float& y) const;
    void Get(float* f) const;

    ////////////////////////////////////////////////////////////////////////// Special Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#special_operators
    _XOINL float& operator [](int i);
    _XOINL const float& operator [](int i) const;
    _XOINL Vector2 operator - () const;
    _XOINL Vector2 operator ~ () const;


    ////////////////////////////////////////////////////////////////////////// Math Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#math_operators
    _XOINL const Vector2& operator += (const Vector2& v);
    _XOINL const Vector2& operator += (float v);
    _XOINL const Vector2& operator += (double v);
    _XOINL const Vector2& operator += (int v);
    _XOINL const Vector2& operator += (const class Vector3& v);
    _XOINL const Vector2& operator += (const class Vector4& v);
    _XOINL const Vector2& operator -= (const Vector2& v);
    _XOINL const Vector2& operator -= (float v);
    _XOINL const Vector2& operator -= (double v);
    _XOINL const Vector2& operator -= (int v);
    _XOINL const Vector2& operator -= (const class Vector3& v);
    _XOINL const Vector2& operator -= (const class Vector4& v);
    _XOINL const Vector2& operator *= (const Vector2& v);
    _XOINL const Vector2& operator *= (float v);
    _XOINL const Vector2& operator *= (double v);
    _XOINL const Vector2& operator *= (int v);
    _XOINL const Vector2& operator *= (const class Vector3& v);
    _XOINL const Vector2& operator *= (const class Vector4& v);
    _XOINL const Vector2& operator /= (const Vector2& v);
    _XOINL const Vector2& operator /= (float v);
    _XOINL const Vector2& operator /= (double v);
    _XOINL const Vector2& operator /= (int v);
    _XOINL const Vector2& operator /= (const class Vector3& v);
    _XOINL const Vector2& operator /= (const class Vector4& v);
    _XOINL Vector2 operator + (const Vector2& v) const;
    _XOINL Vector2 operator + (float v) const;
    _XOINL Vector2 operator + (double v) const;
    _XOINL Vector2 operator + (int v) const;
    _XOINL Vector2 operator + (const class Vector3& v) const;
    _XOINL Vector2 operator + (const class Vector4& v) const;
    _XOINL Vector2 operator - (const Vector2& v) const;
    _XOINL Vector2 operator - (float v) const;
    _XOINL Vector2 operator - (double v) const;
    _XOINL Vector2 operator - (int v) const;
    _XOINL Vector2 operator - (const class Vector3& v) const;
    _XOINL Vector2 operator - (const class Vector4& v) const;
    _XOINL Vector2 operator * (const Vector2& v) const;
    _XOINL Vector2 operator * (float v) const;
    _XOINL Vector2 operator * (double v) const;
    _XOINL Vector2 operator * (int v) const;
    _XOINL Vector2 operator * (const class Vector3& v) const;
    _XOINL Vector2 operator * (const class Vector4& v) const;
    _XOINL Vector2 operator / (const Vector2& v) const;
    _XOINL Vector2 operator / (float v) const;
    _XOINL Vector2 operator / (double v) const;
    _XOINL Vector2 operator / (int v) const;
    _XOINL Vector2 operator / (const class Vector3& v) const;
    _XOINL Vector2 operator / (const class Vector4& v) const;

    ////////////////////////////////////////////////////////////////////////// Comparison Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#comparison_operators
    _XOINL bool operator < (const Vector2& v) const;
    _XOINL bool operator < (float v) const;
    _XOINL bool operator < (double v) const;
    _XOINL bool operator < (int v) const;
    _XOINL bool operator < (const class Vector3& v) const;
    _XOINL bool operator < (const class Vector4& v) const;
    _XOINL bool operator <= (const Vector2& v) const;
    _XOINL bool operator <= (float v) const;
    _XOINL bool operator <= (double v) const;
    _XOINL bool operator <= (int v) const;
    _XOINL bool operator <= (const class Vector3& v) const;
    _XOINL bool operator <= (const class Vector4& v) const;
    _XOINL bool operator > (const Vector2& v) const;
    _XOINL bool operator > (float v) const;
    _XOINL bool operator > (double v) const;
    _XOINL bool operator > (int v) const;
    _XOINL bool operator > (const class Vector3& v) const;
    _XOINL bool operator > (const class Vector4& v) const;
    _XOINL bool operator >= (const Vector2& v) const;
    _XOINL bool operator >= (float v) const;
    _XOINL bool operator >= (double v) const;
    _XOINL bool operator >= (int v) const;
    _XOINL bool operator >= (const class Vector3& v) const;
    _XOINL bool operator >= (const class Vector4& v) const;

    _XOINL bool operator == (const Vector2& v) const;
    _XOINL bool operator == (float v) const;
    _XOINL bool operator == (double v) const;
    _XOINL bool operator == (int v) const;
    _XOINL bool operator == (const class Vector3& v) const;
    _XOINL bool operator == (const class Vector4& v) const;
    _XOINL bool operator != (const Vector2& v) const;
    _XOINL bool operator != (float v) const;
    _XOINL bool operator != (double v) const;
    _XOINL bool operator != (int v) const;
    _XOINL bool operator != (const class Vector3& v) const;
    _XOINL bool operator != (const class Vector4& v) const;

    ////////////////////////////////////////////////////////////////////////// Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#methods
    bool IsNormalized() const;
    bool IsZero() const;
    float Magnitude() const;
    float MagnitudeSquared() const;
    float Sum() const;

    const Vector2& Normalize();

    Vector2 Normalized() const;

    ////////////////////////////////////////////////////////////////////////// Static Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#static_methods
    static void Lerp(const Vector2& a, const Vector2& b, float t, Vector2& outVec);
    static void Max(const Vector2& a, const Vector2& b, Vector2& outVec);
    static void Midpoint(const Vector2& a, const Vector2& b, Vector2& outVec);
    static void Min(const Vector2& a, const Vector2& b, Vector2& outVec);
    static void OrthogonalCCW(const Vector2& v, Vector2& outVec);
    static void OrthogonalCW(const Vector2& v, Vector2& outVec);

    static float AngleDegrees(const Vector2& a, const Vector2& b);
    static float AngleRadians(const Vector2& a, const Vector2& b);
    static float Cross(const Vector2& a, const Vector2& b);
    static float Distance(const Vector2& a, const Vector2& b);
    static float DistanceSquared(const Vector2& a, const Vector2& b);
    static float Dot(const Vector2& a, const Vector2& b);

    ////////////////////////////////////////////////////////////////////////// Variants
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#variants
    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t);
    static Vector2 Max(const Vector2& a, const Vector2& b);
    static Vector2 Midpoint(const Vector2& a, const Vector2& b);
    static Vector2 Min(const Vector2& a, const Vector2& b);
    static Vector2 OrthogonalCCW(const Vector2& v);
    static Vector2 OrthogonalCW(const Vector2& v);

    float AngleDegrees(const Vector2& v) const;
    float AngleRadians(const Vector2& v) const;
    float Cross(const Vector2& v) const;
    float Distance(const Vector2& v) const;
    float DistanceSquared(const Vector2& v) const;
    float Dot(const Vector2& v) const;
    Vector2 Lerp(const Vector2& v, float t) const;
    Vector2 Midpoint(const Vector2& v) const;
    Vector2 OrthogonalCCW() const;
    Vector2 OrthogonalCW() const;

    ////////////////////////////////////////////////////////////////////////// Extras
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#extras
#ifndef XO_NO_OSTREAM
    friend std::ostream& operator <<(std::ostream& os, const Vector2& v) {
        os << "(x:" << v.x << ", y:" << v.y << ", mag:" << v.Magnitude() << ")";
        return os;
    }
#endif

    static const Vector2
        UnitX, 
        UnitY,
        Up,
        Down,
        Left,
        Right,
        One,
        Zero;

#if defined(_XONOCONSTEXPR)
    static const float Epsilon;
#else
    _XOCONSTEXPR static const float Epsilon = FloatEpsilon * 2.0f;
#endif

    union {
        struct { float x, y; };
        float f[2];
    };
};

XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL


#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector3.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

class _MM_ALIGN16 Vector3 {
public:
    ////////////////////////////////////////////////////////////////////////// Constructors
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#constructors
    Vector3(); 
    Vector3(float f); 
    Vector3(float x, float y, float z); 
    Vector3(const Vector3& vec); 
#if defined(XO_SSE)
    Vector3(const __m128& vec); 
#endif
    Vector3(const class Vector2& v); 
    Vector3(const class Vector4& v); 

    ////////////////////////////////////////////////////////////////////////// Set / Get Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#set_get_methods
    const Vector3& Set(float x, float y, float z);
    const Vector3& Set(float f);
    const Vector3& Set(const Vector3& vec);
#if defined(XO_SSE)
    const Vector3& Set(const __m128& vec);
#endif
    void Get(float& x, float& y, float &z) const;
    void Get(float* f) const;

    ////////////////////////////////////////////////////////////////////////// Special Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#special_operators
    _XO_OVERLOAD_NEW_DELETE();
#if defined(XO_SSE)
    _XOINL operator __m128() const;
#endif
    _XOINL float& operator [](int i);
    _XOINL const float& operator [](int i) const;
    _XOINL Vector3 operator -() const;
    _XOINL Vector3 operator ~() const;

    ////////////////////////////////////////////////////////////////////////// Math Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#math_operators
    _XOINL const Vector3& operator += (const Vector3& v);
    _XOINL const Vector3& operator += (float v);
    _XOINL const Vector3& operator += (double v);
    _XOINL const Vector3& operator += (int v);
    _XOINL const Vector3& operator += (const class Vector2& v);
    _XOINL const Vector3& operator += (const class Vector4& v);
    _XOINL const Vector3& operator -= (const Vector3& v);
    _XOINL const Vector3& operator -= (float v);
    _XOINL const Vector3& operator -= (double v);
    _XOINL const Vector3& operator -= (int v);
    _XOINL const Vector3& operator -= (const class Vector2& v);
    _XOINL const Vector3& operator -= (const class Vector4& v);
    _XOINL const Vector3& operator *= (const Vector3& v);
    _XOINL const Vector3& operator *= (float v);
    _XOINL const Vector3& operator *= (double v);
    _XOINL const Vector3& operator *= (int v);
    _XOINL const Vector3& operator *= (const class Vector2& v);
    _XOINL const Vector3& operator *= (const class Vector4& v);
    _XOINL const Vector3& operator /= (const Vector3& v);
    _XOINL const Vector3& operator /= (float v);
    _XOINL const Vector3& operator /= (double v);
    _XOINL const Vector3& operator /= (int v);
    _XOINL const Vector3& operator /= (const class Vector2& v);
    _XOINL const Vector3& operator /= (const class Vector4& v);
    _XOINL Vector3 operator + (const Vector3& v) const;
    _XOINL Vector3 operator + (float v) const;
    _XOINL Vector3 operator + (double v) const;
    _XOINL Vector3 operator + (int v) const;
    _XOINL Vector3 operator + (const class Vector2& v) const;
    _XOINL Vector3 operator + (const class Vector4& v) const;
    _XOINL Vector3 operator - (const Vector3& v) const;
    _XOINL Vector3 operator - (float v) const;
    _XOINL Vector3 operator - (double v) const;
    _XOINL Vector3 operator - (int v) const;
    _XOINL Vector3 operator - (const class Vector2& v) const;
    _XOINL Vector3 operator - (const class Vector4& v) const;
    _XOINL Vector3 operator * (const Vector3& v) const;
    _XOINL Vector3 operator * (float v) const;
    _XOINL Vector3 operator * (double v) const;
    _XOINL Vector3 operator * (int v) const;
    _XOINL Vector3 operator * (const class Vector2& v) const;
    _XOINL Vector3 operator * (const class Vector4& v) const;
    _XOINL Vector3 operator / (const Vector3& v) const;
    _XOINL Vector3 operator / (float v) const;
    _XOINL Vector3 operator / (double v) const;
    _XOINL Vector3 operator / (int v) const;
    _XOINL Vector3 operator / (const class Vector2& v) const;
    _XOINL Vector3 operator / (const class Vector4& v) const;

    ////////////////////////////////////////////////////////////////////////// Comparison Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#comparison_operators
    _XOINL bool operator < (const Vector3& v) const;
    _XOINL bool operator < (float v) const;
    _XOINL bool operator < (double v) const;
    _XOINL bool operator < (int v) const;
    _XOINL bool operator < (const class Vector2& v) const;
    _XOINL bool operator < (const class Vector4& v) const;
    _XOINL bool operator <= (const Vector3& v) const;
    _XOINL bool operator <= (float v) const;
    _XOINL bool operator <= (double v) const;
    _XOINL bool operator <= (int v) const;
    _XOINL bool operator <= (const class Vector2& v) const;
    _XOINL bool operator <= (const class Vector4& v) const;
    _XOINL bool operator > (const Vector3& v) const;
    _XOINL bool operator > (float v) const;
    _XOINL bool operator > (double v) const;
    _XOINL bool operator > (int v) const;
    _XOINL bool operator > (const class Vector2& v) const;
    _XOINL bool operator > (const class Vector4& v) const;
    _XOINL bool operator >= (const Vector3& v) const;
    _XOINL bool operator >= (float v) const;
    _XOINL bool operator >= (double v) const;
    _XOINL bool operator >= (int v) const;
    _XOINL bool operator >= (const class Vector2& v) const;
    _XOINL bool operator >= (const class Vector4& v) const;

    _XOINL bool operator == (const Vector3& v) const;
    _XOINL bool operator == (float v) const;
    _XOINL bool operator == (double v) const;
    _XOINL bool operator == (int v) const;
    _XOINL bool operator == (const class Vector2& v) const;
    _XOINL bool operator == (const class Vector4& v) const;
    _XOINL bool operator != (const Vector3& v) const;
    _XOINL bool operator != (float v) const;
    _XOINL bool operator != (double v) const;
    _XOINL bool operator != (int v) const;
    _XOINL bool operator != (const class Vector2& v) const;
    _XOINL bool operator != (const class Vector4& v) const;

    ////////////////////////////////////////////////////////////////////////// Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#methods
    bool IsNormalized() const;
    bool IsZero() const;
    float Magnitude() const;
    float MagnitudeSquared() const;
    float Sum() const;

    const Vector3& Normalize();

    Vector3 Normalized() const;
    Vector3 ZYX() const;
    

    ////////////////////////////////////////////////////////////////////////// Static Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#static_methods
    static void Cross(const Vector3& a, const Vector3& b, Vector3& outVec);
    static void Lerp(const Vector3& a, const Vector3& b, float t, Vector3& outVec);
    static void Max(const Vector3& a, const Vector3& b, Vector3& outVec);
    static void Min(const Vector3& a, const Vector3& b, Vector3& outVec);
    static void RandomAtDistance(float d, Vector3& outVec);
    static void RandomInConeDegrees(const Vector3& forward, float angle, Vector3& outVec);
    static void RandomInConeRadians(const Vector3& forward, float angle, Vector3& outVec);
    static void RandomInDistance(float d, Vector3& outVec);
    static void RandomInRange(float low, float high, Vector3& outVec);
    static void RandomInSphere(Vector3& outVec);
    static void RandomOnConeDegrees(const Vector3& forward, float angle, Vector3& outVec);
    static void RandomOnConeRadians(const Vector3& forward, float angle, Vector3& outVec);
    static void RandomOnSphere(Vector3& outVec);
    static void RotateDegrees(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec);
    static void RotateRadians(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec);
    // Calls Vector3::AngleRadians, converting the return value to degrees.
    static float AngleDegrees(const Vector3& a, const Vector3& b);
    static float AngleRadians(const Vector3& a, const Vector3& b);
    static float Distance(const Vector3&a, const Vector3&b);
    static float DistanceSquared(const Vector3& a, const Vector3& b);
    static float Dot(const Vector3& a, const Vector3& b);
    

    ////////////////////////////////////////////////////////////////////////// Variants
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#variants
    static Vector3 Cross(const Vector3& a, const Vector3& b);
    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);
    static Vector3 Max(const Vector3& a, const Vector3& b);
    static Vector3 Min(const Vector3& a, const Vector3& b);
    static Vector3 RandomAtDistance(float d);
    static Vector3 RandomInConeDegrees(const Vector3& forward, float angle);
    static Vector3 RandomInConeRadians(const Vector3& forward, float angle);
    static Vector3 RandomInDistance(float d);
    static Vector3 RandomInRange(float low, float high);
    static Vector3 RandomInSphere();
    static Vector3 RandomOnConeDegrees(const Vector3& forward, float angle);
    static Vector3 RandomOnConeRadians(const Vector3& forward, float angle);
    static Vector3 RandomOnSphere();
    static Vector3 RotateDegrees(const Vector3& v, const Vector3& axis, float angle);
    static Vector3 RotateRadians(const Vector3& v, const Vector3& axis, float angle);

    float AngleDegrees(const Vector3& v) const;
    float AngleRadians(const Vector3& v) const;
    float Distance(const Vector3& v) const;
    float DistanceSquared(const Vector3& v) const;
    float Dot(const Vector3& v) const;
    Vector3 Cross(const Vector3& v) const;
    Vector3 Lerp(const Vector3& v, float t) const;
    Vector3 RandomInConeDegrees(float angle) const;
    Vector3 RandomInConeRadians(float angle) const;
    Vector3 RandomOnConeDegrees(float angle) const;
    Vector3 RandomOnConeRadians(float angle) const;
    Vector3 RotateDegrees(const Vector3& axis, float angle) const;
    Vector3 RotateRadians(const Vector3& axis, float angle) const;

    ////////////////////////////////////////////////////////////////////////// Extras
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#extras
#ifndef XO_NO_OSTREAM
    friend std::ostream& operator <<(std::ostream& os, const Vector3& v) {
#   if defined(XO_SSE)
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", w:" << v.w << ", mag:" << v.Magnitude() << ")";
#   else
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", mag:" << v.Magnitude() << ")";
#   endif
        return os;
    }
#endif

    ////////////////////////////////////////////////////////////////////////// Static Attributes
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#public_static_attributes
    static const Vector3
        Origin, 
        UnitX, 
        UnitY, 
        UnitZ, 
        Up,
        Down,
        Left, 
        Right, 
        Forward,
        Backward,
        One, 
        Zero; 

#if defined(_XONOCONSTEXPR)
    static const float Epsilon;
#else
#   if defined(XO_SSE)
    _XOCONSTEXPR static const float Epsilon = sse::SSEFloatEpsilon * 3.0f;
#   else
    _XOCONSTEXPR static const float Epsilon = FloatEpsilon * 3.0f;
#   endif
#endif

    ////////////////////////////////////////////////////////////////////////// Members
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#public_members
#if defined(XO_SSE)
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        float f[4]; 
        __m128 m;
    };
#else
    union {
        struct {
            float x, y, z;
        };
        float f[3];
    };
#endif

private:
#if defined(XO_SSE)
    static const __m128 MASK;
#endif
};

XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector4.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

class _MM_ALIGN16 Vector4 {
public:
    ////////////////////////////////////////////////////////////////////////// Constructors
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#constructors
    Vector4(); 
    Vector4(float f); 
    Vector4(float x, float y, float z, float w); 
    Vector4(const Vector4& vec); 
#if defined(XO_SSE)
    Vector4(const __m128& vec); 
#endif
    Vector4(const class Vector2& v); 
    Vector4(const class Vector2& v, float z, float w); 
    Vector4(const class Vector3& v); 
    Vector4(const class Vector3& v, float w); 

    ////////////////////////////////////////////////////////////////////////// Set / Get Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#set_get_methods
    const Vector4& Set(float x, float y, float z, float w);
    const Vector4& Set(float f);
    const Vector4& Set(const Vector4& vec);
    const Vector4& Set(const Vector2& vec);
    const Vector4& Set(const Vector2& vec, float z, float w);
    const Vector4& Set(const Vector3& vec);
    const Vector4& Set(const Vector3& vec, float w);
#if defined(XO_SSE)
    const Vector4& Set(const __m128& vec);
#endif
    void Get(float& x, float& y, float& z, float& w) const;
    void Get(float* f) const;

    ////////////////////////////////////////////////////////////////////////// Special Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#special_operators
    _XO_OVERLOAD_NEW_DELETE();
#if defined(XO_SSE)
    _XOINL operator const __m128&() const;
#endif
    _XOINL operator float*() const { return (float*)f; }
    _XOINL float& operator [](int i);
    _XOINL const float& operator [](int i) const;
    _XOINL Vector4 operator -() const;
    _XOINL Vector4 operator ~() const;

    ////////////////////////////////////////////////////////////////////////// Math Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#math_operators
    _XOINL const Vector4& operator += (const Vector4& v);
    _XOINL const Vector4& operator += (float v);
    _XOINL const Vector4& operator += (double v);
    _XOINL const Vector4& operator += (int v);
    _XOINL const Vector4& operator += (const class Vector2& v);
    _XOINL const Vector4& operator += (const class Vector3& v);
    _XOINL const Vector4& operator -= (const Vector4& v);
    _XOINL const Vector4& operator -= (float v);
    _XOINL const Vector4& operator -= (double v);
    _XOINL const Vector4& operator -= (int v);
    _XOINL const Vector4& operator -= (const class Vector2& v);
    _XOINL const Vector4& operator -= (const class Vector3& v);
    _XOINL const Vector4& operator *= (const Vector4& v);
    _XOINL const Vector4& operator *= (float v);
    _XOINL const Vector4& operator *= (double v);
    _XOINL const Vector4& operator *= (int v);
    _XOINL const Vector4& operator *= (const class Vector2& v);
    _XOINL const Vector4& operator *= (const class Vector3& v);
    // See: XO_NO_INVERSE_DIVISION
    _XOINL const Vector4& operator /= (const Vector4& v);
    _XOINL const Vector4& operator /= (float v);
    _XOINL const Vector4& operator /= (double v);
    _XOINL const Vector4& operator /= (int v);
    _XOINL const Vector4& operator /= (const class Vector2& v);
    _XOINL const Vector4& operator /= (const class Vector3& v);
    _XOINL Vector4 operator + (const Vector4& v) const;
    _XOINL Vector4 operator + (float v) const;
    _XOINL Vector4 operator + (double v) const;
    _XOINL Vector4 operator + (int v) const;
    _XOINL Vector4 operator + (const class Vector2& v) const;
    _XOINL Vector4 operator + (const class Vector3& v) const;
    _XOINL Vector4 operator - (const Vector4& v) const;
    _XOINL Vector4 operator - (float v) const;
    _XOINL Vector4 operator - (double v) const;
    _XOINL Vector4 operator - (int v) const;
    _XOINL Vector4 operator - (const class Vector2& v) const;
    _XOINL Vector4 operator - (const class Vector3& v) const;
    _XOINL Vector4 operator * (const Vector4& v) const;
    _XOINL Vector4 operator * (float v) const;
    _XOINL Vector4 operator * (double v) const;
    _XOINL Vector4 operator * (int v) const;
    _XOINL Vector4 operator * (const class Vector2& v) const;
    _XOINL Vector4 operator * (const class Vector3& v) const;
    // See: XO_NO_INVERSE_DIVISION
    _XOINL Vector4 operator / (const Vector4& v) const;
    _XOINL Vector4 operator / (float v) const;
    _XOINL Vector4 operator / (double v) const;
    _XOINL Vector4 operator / (int v) const;
    _XOINL Vector4 operator / (const class Vector2& v) const;
    _XOINL Vector4 operator / (const class Vector3& v) const;

    ////////////////////////////////////////////////////////////////////////// Comparison Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#comparison_operators
    _XOINL bool operator < (const Vector4& v) const;
    _XOINL bool operator < (float v) const;
    _XOINL bool operator < (double v) const;
    _XOINL bool operator < (int v) const;
    _XOINL bool operator < (const class Vector2& v) const;
    _XOINL bool operator < (const class Vector3& v) const;
    _XOINL bool operator <= (const Vector4& v) const;
    _XOINL bool operator <= (float v) const;
    _XOINL bool operator <= (double v) const;
    _XOINL bool operator <= (int v) const;
    _XOINL bool operator <= (const class Vector2& v) const;
    _XOINL bool operator <= (const class Vector3& v) const;
    _XOINL bool operator > (const Vector4& v) const;
    _XOINL bool operator > (float v) const;
    _XOINL bool operator > (double v) const;
    _XOINL bool operator > (int v) const;
    _XOINL bool operator > (const class Vector2& v) const;
    _XOINL bool operator > (const class Vector3& v) const;
    _XOINL bool operator >= (const Vector4& v) const;
    _XOINL bool operator >= (float v) const;
    _XOINL bool operator >= (double v) const;
    _XOINL bool operator >= (int v) const;
    _XOINL bool operator >= (const class Vector2& v) const;
    _XOINL bool operator >= (const class Vector3& v) const;

    _XOINL bool operator == (const Vector4& v) const;
    _XOINL bool operator == (float v) const;
    _XOINL bool operator == (double v) const;
    _XOINL bool operator == (int v) const;
    _XOINL bool operator == (const class Vector2& v) const;
    _XOINL bool operator == (const class Vector3& v) const;
    _XOINL bool operator != (const Vector4& v) const;
    _XOINL bool operator != (float v) const;
    _XOINL bool operator != (double v) const;
    _XOINL bool operator != (int v) const;
    _XOINL bool operator != (const class Vector2& v) const;
    _XOINL bool operator != (const class Vector3& v) const;


    bool IsNormalized() const;
    bool IsZero() const;
    float Magnitude() const;
    float MagnitudeSquared() const;
    float Sum() const;

    const Vector4& Normalize();

    Vector4 Normalized() const;

    ////////////////////////////////////////////////////////////////////////// Static Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#static_methods
    static void Lerp(const Vector4& a, const Vector4& b, float t, Vector4& outVec);
    static void Max(const Vector4& a, const Vector4& b, Vector4& outVec);
    static void Min(const Vector4& a, const Vector4& b, Vector4& outVec);
    static float Distance(const Vector4&a, const Vector4&b);
    static float DistanceSquared(const Vector4& a, const Vector4& b);
    static float Dot(const Vector4& a, const Vector4& b);
    
    ////////////////////////////////////////////////////////////////////////// Variants
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#variants
    static Vector4 Lerp(const Vector4& a, const Vector4& b, float t);
    static Vector4 Max(const Vector4& a, const Vector4& b);
    static Vector4 Min(const Vector4& a, const Vector4& b);

    float Distance(const Vector4& v) const;
    float DistanceSquared(const Vector4& v) const;
    float Dot(const Vector4& v) const;
    Vector4 Lerp(const Vector4& v, float t) const;

    ////////////////////////////////////////////////////////////////////////// Extras
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#extras
#ifndef XO_NO_OSTREAM
    friend std::ostream& operator <<(std::ostream& os, const Vector4& v) {
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", w:" << v.w << ", mag:" << v.Magnitude() << ")";
        return os;
    }
#endif

    ////////////////////////////////////////////////////////////////////////// Static Attributes
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#public_static_attributes
    static const Vector4
        One, 
        Zero, 
        UnitX, 
        UnitY, 
        UnitZ, 
        UnitW; 

#if defined(_XONOCONSTEXPR)
    static const float Epsilon;
#else
#   if defined(XO_SSE)
    _XOCONSTEXPR static const float Epsilon = sse::SSEFloatEpsilon * 4.0f;
#   else
    _XOCONSTEXPR static const float Epsilon = FloatEpsilon * 4.0f;
#   endif
#endif

    ////////////////////////////////////////////////////////////////////////// Members
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#public_members
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        float f[4]; 
#if defined(XO_SSE)
        __m128 m;
#endif
    };
};

XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Matrix4x4.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

class _MM_ALIGN16 Matrix4x4 {
public:
    //> See
    Matrix4x4(); 
    explicit Matrix4x4(float m); 
    Matrix4x4(float m00, float m01, float m02, float m03,
                     float m10, float m11, float m12, float m13,
                     float m20, float m21, float m22, float m23,
                     float m30, float m31, float m32, float m33);
    Matrix4x4(const Matrix4x4& m);
    Matrix4x4(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3);
    Matrix4x4(const Vector3& r0, const Vector3& r1, const Vector3& r2);
    Matrix4x4(const class Quaternion& q);


    const Matrix4x4& SetRow(int i, const Vector4& r);
    const Matrix4x4& SetColumn(int i, const Vector4& r);
    const Vector4& GetRow(int i) const;
    Vector4 GetColumn(int i) const;

    ////////////////////////////////////////////////////////////////////////// Special Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/matrix4x4.html#special_operators
    _XO_OVERLOAD_NEW_DELETE();
    _XOINL operator float*() const { return (float*)this; }
    _XOINL const Vector4& operator [](int i) const;
    _XOINL Vector4& operator [](int i);
    _XOINL const float& operator ()(int r, int c) const;
    _XOINL float& operator ()(int r, int c);
    _XOINL Matrix4x4 operator ~() const;

    _XOINL const Matrix4x4& operator += (const Matrix4x4& m);
    _XOINL const Matrix4x4& operator -= (const Matrix4x4& m);
    _XOINL const Matrix4x4& operator *= (const Matrix4x4& m);

    _XOINL Matrix4x4 operator + (const Matrix4x4& m) const;
    _XOINL Matrix4x4 operator - (const Matrix4x4& m) const;
    _XOINL Matrix4x4 operator * (const Matrix4x4& m) const;

    _XOINL Vector4 operator * (const Vector4& v) const;

    _XOINL Vector3 operator * (const Vector3& v) const;


    ////////////////////////////////////////////////////////////////////////// Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/matrix4x4.html#methods
    bool HasOrthonormalBasis() const;
    bool IsUnitary() const;
    float Determinant() const;
    bool HasInverse() const;

    bool TryGetInverse(Matrix4x4& m) const;
    bool TryGetInverse(Matrix4x4& m, float& outDeterminant) const;
    void GetInverse(Matrix4x4& m) const;
    void GetInverse(Matrix4x4& m, float& outDeterminant) const;
    Matrix4x4 GetInverse() const;
    Matrix4x4 GetInverse(float& outDeterminant) const;

    bool TryMakeInverse();
    bool TryMakeInverse(float& outDeterminant);
    // Same as TryMakeInverse but fails silently for determinant values of 0.
    const Matrix4x4& MakeInverse();
    // Same as TryMakeInverse with an out param, but fails silently for determinant values of 0.
    const Matrix4x4& MakeInverse(float& outDeterminant);
    const Matrix4x4& Transpose();
    const Matrix4x4& Transform(Vector3& v) const;
    const Matrix4x4& Transform(Vector4& v) const;

    Matrix4x4 Transposed() const;

    ////////////////////////////////////////////////////////////////////////// Static Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/matrix4x4.html#static_methods
    static void Scale(float xyz, Matrix4x4& outMatrix);
    static void Scale(float x, float y, float z, Matrix4x4& outMatrix);
    static void Scale(const Vector3& v, Matrix4x4& outMatrix);
    static void Translation(float x, float y, float z, Matrix4x4& outMatrix);
    static void Translation(const Vector3& v, Matrix4x4& outMatrix);
    static void RotationXRadians(float radians, Matrix4x4& outMatrix);
    static void RotationYRadians(float radians, Matrix4x4& outMatrix);
    static void RotationZRadians(float radians, Matrix4x4& outMatrix);
    static void RotationRadians(float x, float y, float z, Matrix4x4& outMatrix);
    static void RotationRadians(const Vector3& v, Matrix4x4& outMatrix);
    static void AxisAngleRadians(const Vector3& axis, float radians, Matrix4x4& outMatrix);
    static void RotationXDegrees(float degrees, Matrix4x4& outMatrix);
    static void RotationYDegrees(float degrees, Matrix4x4& outMatrix);
    static void RotationZDegrees(float degrees, Matrix4x4& outMatrix);
    static void RotationDegrees(float x, float y, float z, Matrix4x4& outMatrix);
    static void RotationDegrees(const Vector3& v, Matrix4x4& outMatrix);
    static void AxisAngleDegrees(const Vector3& axis, float degrees, Matrix4x4& outMatrix);
    static void OrthographicProjection(float width, float height, float near, float far, Matrix4x4& outMatrix);
    static void PerspectiveProjectionRadians(float fovx, float fovy, float near, float far, Matrix4x4& outMatrix);
    static void PerspectiveProjectionDegrees(float fovx, float fovy, float near, float far, Matrix4x4& outMatrix);
    static void LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up, Matrix4x4& outMatrix);
    static void LookAtFromPosition(const Vector3& from, const Vector3& to, Matrix4x4& outMatrix);
    static void LookAtFromDirection(const Vector3& direction, const Vector3& up, Matrix4x4& outMatrix);
    static void LookAtFromDirection(const Vector3& direction, Matrix4x4& outMatrix);

    static Matrix4x4 Scale(float xyz);
    static Matrix4x4 Scale(float x, float y, float z);
    static Matrix4x4 Scale(const Vector3& v);

    static Matrix4x4 Translation(float x, float y, float z);
    static Matrix4x4 Translation(const Vector3& v);
    
    static Matrix4x4 RotationXRadians(float radians);
    static Matrix4x4 RotationYRadians(float radians);
    static Matrix4x4 RotationZRadians(float radians);
    static Matrix4x4 RotationRadians(float x, float y, float z);
    static Matrix4x4 RotationRadians(const Vector3& v);
    static Matrix4x4 AxisAngleRadians(const Vector3& axis, float radians);

    static Matrix4x4 RotationXDegrees(float degrees);
    static Matrix4x4 RotationYDegrees(float degrees);
    static Matrix4x4 RotationZDegrees(float degrees);
    static Matrix4x4 RotationDegrees(float x, float y, float z);
    static Matrix4x4 RotationDegrees(const Vector3& v);
    static Matrix4x4 AxisAngleDegrees(const Vector3& axis, float degrees);

    static Matrix4x4 OrthographicProjection(float width, float height, float near, float far);
    static Matrix4x4 PerspectiveProjectionRadians(float fovx, float fovy, float near, float far);
    static Matrix4x4 PerspectiveProjectionDegrees(float fovx, float fovy, float near, float far);

    static Matrix4x4 LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up);
    static Matrix4x4 LookAtFromPosition(const Vector3& from, const Vector3& to);
    static Matrix4x4 LookAtFromDirection(const Vector3& direction, const Vector3& up);
    static Matrix4x4 LookAtFromDirection(const Vector3& direction);

    ////////////////////////////////////////////////////////////////////////// Extras
    // See: http://xo-math.rtfd.io/en/latest/classes/matrix4x4.html#extras
#ifndef XO_NO_OSTREAM
    friend std::ostream& operator <<(std::ostream& os, const Matrix4x4& m) {
        os << "\nrow 0: " << m.r[0] << "\nrow 1: " << m.r[1] << "\nrow 2: " << m.r[2] << "\nrow 3: " << m.r[3] << "\n";
        return os;
    }
#endif

    union {
        Vector4 r[4];
        float m[16];
        struct {
            float   m00, m01, m02, m03,
                    m10, m11, m12, m13,
                    m20, m21, m22, m23,
                    m30, m31, m32, m33;
        };
    };

    static const Matrix4x4
        Identity,
        One,
        Zero;
};


XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Quaternion.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

class _MM_ALIGN16 Quaternion {
public:
    Quaternion();
    Quaternion(const Matrix4x4& m);
    Quaternion(float x, float y, float z, float w);

    _XO_OVERLOAD_NEW_DELETE();

    _XOINL float& operator [](int i);
    _XOINL const float& operator [](int i) const;

    _XOINL const Quaternion& operator *= (const Quaternion& q);
    _XOINL Quaternion operator * (const Quaternion& q) const;

    _XOINL bool operator == (const Quaternion& q) const;
    _XOINL bool operator != (const Quaternion& q) const;

    const Quaternion& MakeConjugate();
    const Quaternion& MakeInverse();
    const Quaternion& Normalize();
    Quaternion Conjugate() const;
    Quaternion Inverse() const;
    Quaternion Normalized() const;
    void GetAxisAngleRadians(Vector3& axis, float& radians) const;

    static void AxisAngleRadians(const Vector3& axis, float radians, Quaternion& outQuat);
    static void Lerp(const Quaternion& a, const Quaternion& b, float t, Quaternion& outQuat);
    static void LookAtFromDirection(const Vector3& direction, const Vector3& up, Quaternion& outQuat);
    static void LookAtFromDirection(const Vector3& direction, Quaternion& outQuat);
    static void LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up, Quaternion& outQuat);
    static void LookAtFromPosition(const Vector3& from, const Vector3& to, Quaternion& outQuat);
    static void RotationRadians(const Vector3& v, Quaternion& outQuat);
    static void RotationRadians(float x, float y, float z, Quaternion& outQuat);
    static void Slerp(const Quaternion& a, const Quaternion& b, float t, Quaternion& outQuat);

    static Quaternion AxisAngleRadians(const Vector3& axis, float radians);
    static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t);
    static Quaternion LookAtFromDirection(const Vector3& direction);
    static Quaternion LookAtFromDirection(const Vector3& direction, const Vector3& up);
    static Quaternion LookAtFromPosition(const Vector3& from, const Vector3& to);
    static Quaternion LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up);
    static Quaternion RotationRadians(const Vector3& v);
    static Quaternion RotationRadians(float x, float y, float z);
    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);

    static const Quaternion
        Identity,
        Zero;

#if defined(_XONOCONSTEXPR)
    static const float Epsilon;
#else
#   if defined(XO_SSE)
    _XOCONSTEXPR static const float Epsilon = sse::SSEFloatEpsilon * 4.0f;
#   else
    _XOCONSTEXPR static const float Epsilon = FloatEpsilon * 4.0f;
#   endif
#endif

    union {
        struct {
            float x, y, z, w;
        };
        float f[4];
#if defined(XO_SSE)
        __m128 m;
#endif
    };

private:
#if defined(XO_SSE)
    static const unsigned IDX_X = 0;
    static const unsigned IDX_Y = 1;
    static const unsigned IDX_Z = 2;
    static const unsigned IDX_W = 3;
#endif
};

XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL


#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector2Operators.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

float& Vector2::operator [](int i) { return f[i]; }
const float& Vector2::operator [](int i) const { return f[i]; }

Vector2 Vector2::operator - () const { return Vector2(-x, -y); }
Vector2 Vector2::operator ~ () const { return Vector2(y, x); }

const Vector2& Vector2::operator += (const Vector2& v) {
    x += v.x;
    y += v.y;
    return *this; 
}
const Vector2& Vector2::operator += (float v) {
    x += v;
    y += v;
    return *this; 
}
const Vector2& Vector2::operator += (double v)                  { return *this += (float)v; }
const Vector2& Vector2::operator += (int v)                     { return *this += (float)v; }
const Vector2& Vector2::operator += (const class Vector3& v)    { return *this += Vector2(v); }
const Vector2& Vector2::operator += (const class Vector4& v)    { return *this += Vector2(v); }

const Vector2& Vector2::operator -= (const Vector2& v) {
    x -= v.x;
    y -= v.y;
    return *this; 
}
const Vector2& Vector2::operator -= (float v) {
    x -= v;
    y -= v;
    return *this; 
}
const Vector2& Vector2::operator -= (double v)                  { return *this -= (float)v; }
const Vector2& Vector2::operator -= (int v)                     { return *this -= (float)v; }
const Vector2& Vector2::operator -= (const class Vector3& v)    { return *this -= Vector2(v); }
const Vector2& Vector2::operator -= (const class Vector4& v)    { return *this -= Vector2(v); }

const Vector2& Vector2::operator *= (const Vector2& v) {
    x *= v.x;
    y *= v.y;
    return *this; 
}
const Vector2& Vector2::operator *= (float v) {
    x *= v;
    y *= v;
    return *this;
}
const Vector2& Vector2::operator *= (double v)                  { return *this *= (float)v; }
const Vector2& Vector2::operator *= (int v)                     { return *this *= (float)v; }
const Vector2& Vector2::operator *= (const class Vector3& v)    { return *this *= Vector2(v); }
const Vector2& Vector2::operator *= (const class Vector4& v)    { return *this *= Vector2(v); }

const Vector2& Vector2::operator /= (const Vector2& v) {
    x /= v.x;
    y /= v.y;
    return *this;
}
const Vector2& Vector2::operator /= (float v) {
#if defined(XO_NO_INVERSE_DIVISION)
    x /= v;
    y /= v;
    return *this;
#else
    return (*this) *= (1.0f/v);
#endif
    
}
const Vector2& Vector2::operator /= (double v)                  { return *this /= (float)v; }
const Vector2& Vector2::operator /= (int v)                     { return *this /= (float)v; }
const Vector2& Vector2::operator /= (const class Vector3& v)    { return *this /= Vector2(v); }
const Vector2& Vector2::operator /= (const class Vector4& v)    { return *this /= Vector2(v); }

Vector2 Vector2::operator + (const Vector2& v) const        { return Vector2(*this) += v; }
Vector2 Vector2::operator + (float v) const                 { return Vector2(*this) += v; }
Vector2 Vector2::operator + (double v) const                { return Vector2(*this) += v; }
Vector2 Vector2::operator + (int v) const                   { return Vector2(*this) += v; }
Vector2 Vector2::operator + (const class Vector3& v) const  { return Vector2(*this) += v; }
Vector2 Vector2::operator + (const class Vector4& v) const  { return Vector2(*this) += v; }

Vector2 Vector2::operator - (const Vector2& v) const        { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (float v) const                 { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (double v) const                { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (int v) const                   { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (const class Vector3& v) const  { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (const class Vector4& v) const  { return Vector2(*this) -= v; }

Vector2 Vector2::operator * (const Vector2& v) const        { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (float v) const                 { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (double v) const                { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (int v) const                   { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (const class Vector3& v) const  { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (const class Vector4& v) const  { return Vector2(*this) *= v; }

Vector2 Vector2::operator / (const Vector2& v) const        { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (float v) const                 { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (double v) const                { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (int v) const                   { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (const class Vector3& v) const  { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (const class Vector4& v) const  { return Vector2(*this) /= v; }

bool Vector2::operator < (const Vector2& v) const           { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector2::operator < (float v) const                    { return MagnitudeSquared() < (v * v); }
bool Vector2::operator < (double v) const                   { return MagnitudeSquared() < (float)(v * v); }
bool Vector2::operator < (int v) const                      { return MagnitudeSquared() < (float)(v * v); }
bool Vector2::operator < (const class Vector3& v) const     { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector2::operator < (const class Vector4& v) const     { return MagnitudeSquared() < v.MagnitudeSquared(); }

bool Vector2::operator <= (const Vector2& v) const          { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector2::operator <= (float v) const                   { return MagnitudeSquared() <= (v * v); }
bool Vector2::operator <= (double v) const                  { return MagnitudeSquared() <= (float)(v * v); }
bool Vector2::operator <= (int v) const                     { return MagnitudeSquared() <= (float)(v * v); }
bool Vector2::operator <= (const class Vector3& v) const    { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector2::operator <= (const class Vector4& v) const    { return MagnitudeSquared() <= v.MagnitudeSquared(); }

bool Vector2::operator > (const Vector2& v) const           { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector2::operator > (float v) const                    { return MagnitudeSquared() > (v * v); }
bool Vector2::operator > (double v) const                   { return MagnitudeSquared() > (float)(v * v); }
bool Vector2::operator > (int v) const                      { return MagnitudeSquared() > (float)(v * v); }
bool Vector2::operator > (const class Vector3& v) const     { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector2::operator > (const class Vector4& v) const     { return MagnitudeSquared() > v.MagnitudeSquared(); }

bool Vector2::operator >= (const Vector2& v) const          { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector2::operator >= (float v) const                   { return MagnitudeSquared() >= (v * v); }
bool Vector2::operator >= (double v) const                  { return MagnitudeSquared() >= (float)(v * v); }
bool Vector2::operator >= (int v) const                     { return MagnitudeSquared() >= (float)(v * v); }
bool Vector2::operator >= (const class Vector3& v) const    { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector2::operator >= (const class Vector4& v) const    { return MagnitudeSquared() >= v.MagnitudeSquared(); }

bool Vector2::operator == (const Vector2& v) const {
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(x, v.x, Epsilon);
}
bool Vector2::operator == (float v) const                   { return CloseEnough(MagnitudeSquared(), v*v, Epsilon); }
bool Vector2::operator == (double v) const                  { return *this == (float)(v*v); }
bool Vector2::operator == (int v) const                     { return *this == (float)(v*v); }
bool Vector2::operator == (const class Vector3& v) const {
#   if defined(XO_SSE2)
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(_mm_set_ps(0.0f, 0.0f, y, x), v.m)), sse::Epsilon)) & 3) == 3;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon);
#   endif
}
bool Vector2::operator == (const class Vector4& v) const {
#   if defined(XO_SSE)
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(_mm_set_ps(0.0f, 0.0f, y, x), v.m)), sse::Epsilon)) & 3) == 3;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon);
#   endif
}

bool Vector2::operator != (const Vector2& v) const          { return !((*this) == v); }
bool Vector2::operator != (float v) const                   { return !((*this) == v); }
bool Vector2::operator != (double v) const                  { return !((*this) == v); }
bool Vector2::operator != (int v) const                     { return !((*this) == v); }
bool Vector2::operator != (const class Vector3& v) const    { return !((*this) == v); }
bool Vector2::operator != (const class Vector4& v) const    { return !((*this) == v); }

XOMATH_END_XO_NS();

#endif

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector3Operators.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

#if defined(XO_SSE)
Vector3::operator __m128() const {
    return m;
}
#endif

float& Vector3::operator [](int i) {
    return f[i];
}

const float& Vector3::operator [](int i) const {
    return f[i];
}

Vector3 Vector3::operator -() const {
#if defined(XO_SSE)
    return Vector3(_mm_mul_ps(m, sse::NegativeOne));
#else
    return Vector3(-x, -y, -z);
#endif
}

Vector3 Vector3::operator ~() const {
    return ZYX();
}

const Vector3& Vector3::operator += (const Vector3& v) {
#if defined(XO_SSE)
    m = _mm_add_ps(m, v);
#else
    x += v.x;
    y += v.y;
    z += v.z;
#endif
    return *this;
}

const Vector3& Vector3::operator += (float v) {
#if defined(XO_SSE)
    m = _mm_add_ps(m, _mm_set_ps1(v));
#else
    x += v;
    y += v;
    z += v;
#endif
    return *this;
}

const Vector3& Vector3::operator += (double v)                  { return (*this) += float(v); }
const Vector3& Vector3::operator += (int v)                     { return (*this) += float(v); }
const Vector3& Vector3::operator += (const class Vector2& v)    { return (*this) += Vector3(v); }
const Vector3& Vector3::operator += (const class Vector4& v)    { return (*this) += Vector3(v); }

const Vector3& Vector3::operator -= (const Vector3& v) {
#if defined(XO_SSE)
    m = _mm_sub_ps(m, v);
#else
    x -= v.x;
    y -= v.y;
    z -= v.z;
#endif
    return *this;
}

const Vector3& Vector3::operator -= (float v) {
#if defined(XO_SSE)
    m = _mm_sub_ps(m, _mm_set_ps1(v));
#else
    x -= v;
    y -= v;
    z -= v;
#endif
    return *this;
}

const Vector3& Vector3::operator -= (double v)                  { return (*this) -= float(v); }
const Vector3& Vector3::operator -= (int v)                     { return (*this) -= float(v); }
const Vector3& Vector3::operator -= (const class Vector2& v)    { return (*this) -= Vector3(v); }
const Vector3& Vector3::operator -= (const class Vector4& v)    { return (*this) -= Vector3(v); }

const Vector3& Vector3::operator *= (const Vector3& v) {
#if defined(XO_SSE)
    m = _mm_mul_ps(m, v);
#else
    x *= v.x;
    y *= v.y;
    z *= v.z;
#endif
    return *this;
}

const Vector3& Vector3::operator *= (float v) {
#if defined(XO_SSE)
    m = _mm_mul_ps(m, _mm_set_ps1(v));
#else
    x *= v;
    y *= v;
    z *= v;
#endif
    return *this;
}

const Vector3& Vector3::operator *= (double v)                  { return (*this) *= float(v); }
const Vector3& Vector3::operator *= (int v)                     { return (*this) *= float(v); }
const Vector3& Vector3::operator *= (const class Vector2& v)    { return (*this) *= Vector3(v); }
const Vector3& Vector3::operator *= (const class Vector4& v)    { return (*this) *= Vector3(v); }

#if defined(XO_NO_INVERSE_DIVISION)
const Vector3& Vector3::operator /= (const Vector3& v) {
#if defined(XO_SSE)
    // see: https://software.intel.com/sites/landingpage/IntrinsicsGuide
    // see: https://software.intel.com/en-us/articles/measuring-instruction-latency-and-throughput

    // _mm_div_ps
    // -----------------------------------
    // Architecture    Latency Throughput
    // Haswell         13      5
    // Ivy Bridge      13      6
    // Sandy Bridge    14      14
    // Westmere        14      12
    // Nehalem         14      12
    m = _mm_div_ps(m, v);
#else
    x /= v.x;
    y /= v.y;
    z /= v.z;
#endif

    return *this;
}

const Vector3& Vector3::operator /= (float v) {
#   if defined(XO_SSE)
    m = _mm_div_ps(m, _mm_set_ps1(v));
#   else
    x /= v;
    y /= v;
    z /= v;
#   endif
    return *this;
}
#else

const Vector3& Vector3::operator /= (const Vector3& v) {
#   if defined(XO_SSE)
    // see: https://software.intel.com/sites/landingpage/IntrinsicsGuide
    // see: https://software.intel.com/en-us/articles/measuring-instruction-latency-and-throughput

    // _mm_rcp_ps
    // -----------------------------------
    // Architecture    Latency  Throughput
    // Haswell         5        1
    // Ivy Bridge      5        1
    // Sandy Bridge    5        1
    // Westmere        3        2
    // Nehalem         3        2

    // _mm_mul_ps
    // -----------------------------------
    // Architecture    Latency  Throughput
    // Haswell         5        0.5
    // Ivy Bridge      5        1
    // Sandy Bridge    5        1
    // Westmere        4        1
    // Nehalem         4        1
    m = _mm_mul_ps(m, _mm_rcp_ps(v));
#   else
    x /= v.x;
    y /= v.y;
    z /= v.z;
#   endif

    return *this;
}

const Vector3& Vector3::operator /= (float v) { 
#   if defined(XO_SSE)
    m = _mm_mul_ps(m, _mm_set_ps1(1.0f/v));
#   else
    v = 1.0f / v;
    x *= v;
    y *= v;
    z *= v;
#   endif
    return *this;
}
#endif
const Vector3& Vector3::operator /= (double v)                  { return (*this) /= float(v); }
const Vector3& Vector3::operator /= (int v)                     { return (*this) /= float(v); }
const Vector3& Vector3::operator /= (const class Vector2& v)    { return (*this) /= Vector3(v); }
const Vector3& Vector3::operator /= (const class Vector4& v)    { return (*this) /= Vector3(v); }

Vector3 Vector3::operator + (const Vector3& v) const        { return Vector3(*this) += v; }
Vector3 Vector3::operator + (float v) const                 { return Vector3(*this) += v; }
Vector3 Vector3::operator + (double v) const                { return Vector3(*this) += v; }
Vector3 Vector3::operator + (int v) const                   { return Vector3(*this) += v; }
Vector3 Vector3::operator + (const class Vector2& v) const  { return Vector3(*this) += v; }
Vector3 Vector3::operator + (const class Vector4& v) const  { return Vector3(*this) += v; }

Vector3 Vector3::operator - (const Vector3& v) const        { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (float v) const                 { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (double v) const                { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (int v) const                   { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (const class Vector2& v) const  { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (const class Vector4& v) const  { return Vector3(*this) -= v; }

Vector3 Vector3::operator * (const Vector3& v) const        { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (float v) const                 { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (double v) const                { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (int v) const                   { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (const class Vector2& v) const  { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (const class Vector4& v) const  { return Vector3(*this) *= v; }

Vector3 Vector3::operator / (const Vector3& v) const        { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (float v) const                 { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (double v) const                { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (int v) const                   { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (const class Vector2& v) const  { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (const class Vector4& v) const  { return Vector3(*this) /= v; }


bool Vector3::operator < (const Vector3& v) const       { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector3::operator < (float v) const                { return MagnitudeSquared() < (v * v); }
bool Vector3::operator < (double v) const               { return MagnitudeSquared() < (float)(v * v); }
bool Vector3::operator < (int v) const                  { return MagnitudeSquared() < (float)(v * v); }
bool Vector3::operator < (const class Vector2& v) const { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector3::operator < (const class Vector4& v) const { return MagnitudeSquared() < v.MagnitudeSquared(); }

bool Vector3::operator <= (const Vector3& v) const          { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector3::operator <= (float v) const                   { return MagnitudeSquared() <= (v * v); }
bool Vector3::operator <= (double v) const                  { return MagnitudeSquared() <= (float)(v * v); }
bool Vector3::operator <= (int v) const                     { return MagnitudeSquared() <= (float)(v * v); }
bool Vector3::operator <= (const class Vector2& v) const    { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector3::operator <= (const class Vector4& v) const    { return MagnitudeSquared() <= v.MagnitudeSquared(); }

bool Vector3::operator > (const Vector3& v) const       { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector3::operator > (float v) const                { return MagnitudeSquared() > (v * v); }
bool Vector3::operator > (double v) const               { return MagnitudeSquared() > (float)(v * v); }
bool Vector3::operator > (int v) const                  { return MagnitudeSquared() > (float)(v * v); }
bool Vector3::operator > (const class Vector2& v) const { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector3::operator > (const class Vector4& v) const { return MagnitudeSquared() > v.MagnitudeSquared(); }

bool Vector3::operator >= (const Vector3& v) const          { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector3::operator >= (float v) const                   { return MagnitudeSquared() >= (v * v); }
bool Vector3::operator >= (double v) const                  { return MagnitudeSquared() >= (float)(v * v); }
bool Vector3::operator >= (int v) const                     { return MagnitudeSquared() >= (float)(v * v); }
bool Vector3::operator >= (const class Vector2& v) const    { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector3::operator >= (const class Vector4& v) const    { return MagnitudeSquared() >= v.MagnitudeSquared(); }

bool Vector3::operator == (const Vector3& v) const {
#   if defined(XO_SSE2)
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(v, m)), sse::Epsilon)) & 7) == 7;
#   elif XO_SSE
    // TODO: find a faster way with SSE to do a 'close enough' check.
    // I'm not sure if there's a way to do the sign bit masking like we have in sse::Abs to acomplish
    // what we're doing in SSE2
    return CloseEnough(x, v.x, sse::SSEFloatEpsilon) && CloseEnough(y, v.y, sse::SSEFloatEpsilon) && CloseEnough(z, v.z, sse::SSEFloatEpsilon);
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon) && CloseEnough(z, v.z, Epsilon);
#   endif
}
bool Vector3::operator == (float v) const                   { return CloseEnough(MagnitudeSquared(), v*v, Epsilon);}
bool Vector3::operator == (double v) const                  { return CloseEnough(MagnitudeSquared(), (float)(v*v), Epsilon);}
bool Vector3::operator == (int v) const                     { return CloseEnough(MagnitudeSquared(), (float)(v*v), Epsilon);}
bool Vector3::operator == (const class Vector2& v) const {
#   if defined(XO_SSE)
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(_mm_set_ps(0.0f, 0.0f, v.y, v.x), m)), sse::Epsilon)) & 3) == 3;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon);
#   endif
}
bool Vector3::operator == (const class Vector4& v) const {
#   if defined(XO_SSE)
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(v.m, m)), sse::Epsilon)) & 7) == 7;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon) && CloseEnough(z, v.z, Epsilon);
#   endif
}

bool Vector3::operator != (const Vector3& v) const          { return !((*this) == v); }
bool Vector3::operator != (float v) const                   { return !((*this) == v); }
bool Vector3::operator != (double v) const                  { return !((*this) == v); }
bool Vector3::operator != (int v) const                     { return !((*this) == v); }
bool Vector3::operator != (const class Vector2& v) const    { return !((*this) == v); }
bool Vector3::operator != (const class Vector4& v) const    { return !((*this) == v); }

XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector4Operators.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

#if defined(XO_SSE)

#if defined IDX_X
_XOMATH_INTERNAL_MACRO_WARNING
#   else
#       define IDX_X 0
#   endif
#if defined IDX_Y
_XOMATH_INTERNAL_MACRO_WARNING
#   else
#       define IDX_Y 1
#   endif
#if defined IDX_Z
_XOMATH_INTERNAL_MACRO_WARNING
#   else
#       define IDX_Z 2
#   endif
#if defined IDX_W
_XOMATH_INTERNAL_MACRO_WARNING
#   else
#       define IDX_W 3
#   endif

#endif

#if defined(XO_SSE)
Vector4::operator const __m128&() const {
    return m;
}
#endif

float& Vector4::operator [](int i) {
    return f[i];
}

const float& Vector4::operator [](int i) const {
    return f[i];
}

Vector4 Vector4::operator -() const {
#if defined(XO_SSE)
    return Vector4(_mm_mul_ps(m, sse::NegativeOne));
#else
    return Vector4(-x, -y, -z, -w);
#endif
}

Vector4 Vector4::operator ~() const {
#if defined(XO_SSE)
    return Vector3(_mm_shuffle_ps(m, m, _MM_SHUFFLE(IDX_X, IDX_Y, IDX_Z, IDX_W)));
#else
    return Vector4(w, z, y, x);
#endif
}

const Vector4& Vector4::operator += (const Vector4& v) {
#if defined(XO_SSE)
    m = _mm_add_ps(m, v.m);
#else
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
#endif
    return *this;
}

const Vector4& Vector4::operator += (float v) {
#if defined(XO_SSE)
    m = _mm_add_ps(m, _mm_set_ps1(v));
#else
    x += v;
    y += v;
    z += v;
    w += v;
#endif
    return *this;
}

const Vector4& Vector4::operator += (double v)          { return (*this) += (float)v; }
const Vector4& Vector4::operator += (int v)             { return (*this) += (float)v; }
const Vector4& Vector4::operator += (const class Vector2& v)  { return (*this) += Vector4(v); }
const Vector4& Vector4::operator += (const class Vector3& v)  { return (*this) += Vector4(v); }

const Vector4& Vector4::operator -= (const Vector4& v) {
#if defined(XO_SSE)
    m = _mm_sub_ps(m, v.m);
#else
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
#endif
    return *this;
}

const Vector4& Vector4::operator -= (float v) {
#if defined(XO_SSE)
    m = _mm_sub_ps(m, _mm_set_ps1(v));
#else
    x -= v;
    y -= v;
    z -= v;
    w -= v;
#endif
    return *this;
}

const Vector4& Vector4::operator -= (double v)          { return (*this) -= (float)v; }
const Vector4& Vector4::operator -= (int v)             { return (*this) -= (float)v; }
const Vector4& Vector4::operator -= (const class Vector2& v)  { return (*this) -= Vector4(v); }
const Vector4& Vector4::operator -= (const class Vector3& v)  { return (*this) -= Vector4(v); }

const Vector4& Vector4::operator *= (const Vector4& v) {
#if defined(XO_SSE)
    m = _mm_mul_ps(m, v.m);
#else
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
#endif
    return *this;
}

const Vector4& Vector4::operator *= (float v) {
#if defined(XO_SSE)
    m = _mm_mul_ps(m, _mm_set_ps1(v));
#else
    x *= v;
    y *= v;
    z *= v;
    w *= v;
#endif
    return *this;
}

const Vector4& Vector4::operator *= (double v)          { return (*this) *= (float)v; }
const Vector4& Vector4::operator *= (int v)             { return (*this) *= (float)v; }
const Vector4& Vector4::operator *= (const class Vector2& v)  { return (*this) *= Vector4(v); }
const Vector4& Vector4::operator *= (const class Vector3& v)  { return (*this) *= Vector4(v); }

#if defined(XO_NO_INVERSE_DIVISION)
const Vector4& Vector4::operator /= (const Vector4& v) {
#   if defined(XO_SSE)
    // see: https://software.intel.com/sites/landingpage/IntrinsicsGuide
    // see: https://software.intel.com/en-us/articles/measuring-instruction-latency-and-throughput

    // _mm_div_ps
    // -----------------------------------
    // Architecture    Latency Throughput
    // Haswell         13      5
    // Ivy Bridge      13      6
    // Sandy Bridge    14      14
    // Westmere        14      12
    // Nehalem         14      12
    m = _mm_div_ps(m, v.m);
#   else
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
#   endif
    return *this;
}

const Vector4& Vector4::operator /= (float v) {
#   if defined(XO_SSE)
    m = _mm_div_ps(m, _mm_set_ps1(v));
#   else
    x /= v;
    y /= v;
    z /= v;
    w /= v;
#   endif
    return *this;
}
#else

const Vector4& Vector4::operator /= (const Vector4& v) {
#   if defined(XO_SSE)
    // see: https://software.intel.com/sites/landingpage/IntrinsicsGuide
    // see: https://software.intel.com/en-us/articles/measuring-instruction-latency-and-throughput

    // _mm_rcp_ps
    // -----------------------------------
    // Architecture    Latency  Throughput
    // Haswell         5        1
    // Ivy Bridge      5        1
    // Sandy Bridge    5        1
    // Westmere        3        2
    // Nehalem         3        2

    // _mm_mul_ps
    // -----------------------------------
    // Architecture    Latency  Throughput
    // Haswell         5        0.5
    // Ivy Bridge      5        1
    // Sandy Bridge    5        1
    // Westmere        4        1
    // Nehalem         4        1
    m = _mm_mul_ps(m, _mm_rcp_ps(v.m));
#   else
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
#endif
    return *this;
}

const Vector4& Vector4::operator /= (float v) { 
#   if defined(XO_SSE)
    m = _mm_mul_ps(m, _mm_set_ps1(1.0f/v));
#   else
    v = 1.0f / v;
    x *= v;
    y *= v;
    z *= v;
    w *= v;
#   endif
    return *this;
}
#endif
const Vector4& Vector4::operator /= (double v)          { return (*this) /= float(v); }
const Vector4& Vector4::operator /= (int v)             { return (*this) /= float(v); }
const Vector4& Vector4::operator /= (const class Vector2& v)  { return (*this) /= Vector4(v); }
const Vector4& Vector4::operator /= (const class Vector3& v)  { return (*this) /= Vector4(v); }

Vector4 Vector4::operator + (const Vector4& v) const    { return Vector4(*this) += v; }
Vector4 Vector4::operator + (float v) const             { return Vector4(*this) += v; }
Vector4 Vector4::operator + (double v) const            { return Vector4(*this) += v; }
Vector4 Vector4::operator + (int v) const               { return Vector4(*this) += v; }
Vector4 Vector4::operator + (const class Vector2& v) const    { return Vector4(*this) += v; }
Vector4 Vector4::operator + (const class Vector3& v) const    { return Vector4(*this) += v; }

Vector4 Vector4::operator - (const Vector4& v) const    { return Vector4(*this) -= v; }
Vector4 Vector4::operator - (float v) const             { return Vector4(*this) -= v; }
Vector4 Vector4::operator - (double v) const            { return Vector4(*this) -= v; }
Vector4 Vector4::operator - (int v) const               { return Vector4(*this) -= v; }
Vector4 Vector4::operator - (const class Vector2& v) const    { return Vector4(*this) -= v; }
Vector4 Vector4::operator - (const class Vector3& v) const    { return Vector4(*this) -= v; }

Vector4 Vector4::operator * (const Vector4& v) const    { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (float v) const             { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (double v) const            { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (int v) const               { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (const class Vector2& v) const    { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (const class Vector3& v) const    { return Vector4(*this) *= v; }

Vector4 Vector4::operator / (const Vector4& v) const    { return Vector4(*this) /= v; }
Vector4 Vector4::operator / (float v) const             { return Vector4(*this) /= v; }
Vector4 Vector4::operator / (double v) const            { return Vector4(*this) /= v; }
Vector4 Vector4::operator / (int v) const               { return Vector4(*this) /= v; }
Vector4 Vector4::operator / (const class Vector2& v) const    { return Vector4(*this) /= v; }
Vector4 Vector4::operator / (const class Vector3& v) const    { return Vector4(*this) /= v; }

bool Vector4::operator < (const Vector4& v) const       { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector4::operator < (float v) const                { return MagnitudeSquared() < (v * v); }
bool Vector4::operator < (double v) const               { return MagnitudeSquared() < (float)(v * v); }
bool Vector4::operator < (int v) const                  { return MagnitudeSquared() < (float)(v * v); }
bool Vector4::operator < (const class Vector2& v) const       { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector4::operator < (const class Vector3& v) const       { return MagnitudeSquared() < v.MagnitudeSquared(); }

bool Vector4::operator <= (const Vector4& v) const      { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector4::operator <= (float v) const               { return MagnitudeSquared() <= (v * v); }
bool Vector4::operator <= (double v) const              { return MagnitudeSquared() <= (float)(v * v); }
bool Vector4::operator <= (int v) const                 { return MagnitudeSquared() <= (float)(v * v); }
bool Vector4::operator <= (const class Vector2& v) const      { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector4::operator <= (const class Vector3& v) const      { return MagnitudeSquared() <= v.MagnitudeSquared(); }

bool Vector4::operator > (const Vector4& v) const       { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector4::operator > (float v) const                { return MagnitudeSquared() > (v * v); }
bool Vector4::operator > (double v) const               { return MagnitudeSquared() > (float)(v * v); }
bool Vector4::operator > (int v) const                  { return MagnitudeSquared() > (float)(v * v); }
bool Vector4::operator > (const class Vector2& v) const       { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector4::operator > (const class Vector3& v) const       { return MagnitudeSquared() > v.MagnitudeSquared(); }

bool Vector4::operator >= (const Vector4& v) const      { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector4::operator >= (float v) const               { return MagnitudeSquared() >= (v * v); }
bool Vector4::operator >= (double v) const              { return MagnitudeSquared() >= (float)(v * v); }
bool Vector4::operator >= (int v) const                 { return MagnitudeSquared() >= (float)(v * v); }
bool Vector4::operator >= (const class Vector2& v) const      { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector4::operator >= (const class Vector3& v) const      { return MagnitudeSquared() >= v.MagnitudeSquared(); }

bool Vector4::operator == (const Vector4& v) const {
#   if defined(XO_SSE2)
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(v.m, m)), sse::Epsilon)) & 15) == 15;
#   elif XO_SSE
    // TODO: find a faster way with SSE to do a 'close enough' check.
    // I'm not sure if there's a way to do the sign bit masking like we have in sse::Abs to acomplish
    // what we're doing in SSE2
    return 
        CloseEnough(x, v.x, sse::SSEFloatEpsilon) && 
        CloseEnough(y, v.y, sse::SSEFloatEpsilon) && 
        CloseEnough(z, v.z, sse::SSEFloatEpsilon) &&
        CloseEnough(w, v.w, sse::SSEFloatEpsilon);
#   else
    return 
        CloseEnough(x, v.x, Epsilon) && 
        CloseEnough(y, v.y, Epsilon) && 
        CloseEnough(z, v.z, Epsilon) &&
        CloseEnough(w, v.w, Epsilon);
#   endif
}
bool Vector4::operator == (float v) const           { return CloseEnough(MagnitudeSquared(), v * v, Epsilon); }
bool Vector4::operator == (double v) const          { return CloseEnough(MagnitudeSquared(), (float)(v * v), Epsilon); }
bool Vector4::operator == (int v) const             { return CloseEnough(MagnitudeSquared(), (float)(v * v), Epsilon); }

bool Vector4::operator == (const class Vector2& v) const {
#   if defined(XO_SSE)
    // Todo: check that this is actually faster.
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(m, _mm_set_ps(0.0f, 0.0f, v.y, v.x))), sse::Epsilon)) & 3) == 3;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon);
#   endif
}

bool Vector4::operator == (const class Vector3& v) const {
#   if defined(XO_SSE)
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(v.m, m)), sse::Epsilon)) & 7) == 7;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon) && CloseEnough(z, v.z, Epsilon);
#   endif
}

bool Vector4::operator != (const Vector4& v) const  { return !((*this) == v); }
bool Vector4::operator != (float v) const           { return !((*this) == v); }
bool Vector4::operator != (double v) const          { return !((*this) == v); }
bool Vector4::operator != (int v) const             { return !((*this) == v); }
bool Vector4::operator != (const class Vector2& v) const  { return !((*this) == v); }
bool Vector4::operator != (const class Vector3& v) const  { return !((*this) == v); }

#undef IDX_X
#undef IDX_Y
#undef IDX_Z
#undef IDX_W

XOMATH_END_XO_NS();

#endif

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Matrix4x4Operators.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

const Vector4& Matrix4x4::operator [](int i) const {
    return r[i];
}

Vector4& Matrix4x4::operator [](int i) {
    return r[i];
}

const float& Matrix4x4::operator ()(int r, int c) const {
    return this->r[r][c];
}

float& Matrix4x4::operator ()(int r, int c) {
    return this->r[r][c];
}

Matrix4x4 Matrix4x4::operator ~() const {
    auto m = *this;
    return m.Transpose();
}

const Matrix4x4& Matrix4x4::operator += (const Matrix4x4& m) {
    r[0] += m[0];
    r[1] += m[1];
    r[2] += m[2];
    r[3] += m[3];
    return *this;
}

const Matrix4x4& Matrix4x4::operator -= (const Matrix4x4& m) {
    r[0] -= m[0];
    r[1] -= m[1];
    r[2] -= m[2];
    r[3] -= m[3];
    return *this;
}

const Matrix4x4& Matrix4x4::operator *= (const Matrix4x4& m) {
    auto t = m.Transposed();
    return (*this) = Matrix4x4(
        (r[0] * t[0]).Sum(), (r[0] * t[1]).Sum(), (r[0] * t[2]).Sum(), (r[0] * t[3]).Sum(),
        (r[1] * t[0]).Sum(), (r[1] * t[1]).Sum(), (r[1] * t[2]).Sum(), (r[1] * t[3]).Sum(),
        (r[2] * t[0]).Sum(), (r[2] * t[1]).Sum(), (r[2] * t[2]).Sum(), (r[2] * t[3]).Sum(),
        (r[3] * t[0]).Sum(), (r[3] * t[1]).Sum(), (r[3] * t[2]).Sum(), (r[3] * t[3]).Sum()
    );
}

Matrix4x4 Matrix4x4::operator + (const Matrix4x4& m) const { return Matrix4x4(*this) += m; }
Matrix4x4 Matrix4x4::operator - (const Matrix4x4& m) const { return Matrix4x4(*this) -= m; }
Matrix4x4 Matrix4x4::operator * (const Matrix4x4& m) const { return Matrix4x4(*this) *= m; }

Vector4 Matrix4x4::operator * (const Vector4& v) const {
    return Vector4((r[0] * v).Sum(), (r[1] * v).Sum(), (r[2] * v).Sum(), (r[3] * v).Sum());
}

Vector3 Matrix4x4::operator * (const Vector3& v) const {
    return Vector3((r[0] * v).Sum(), (r[1] * v).Sum(), (r[2] * v).Sum());
}

XOMATH_END_XO_NS();

#endif

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include QuaternionOperators.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

float& Quaternion::operator [](int i) { 
  return f[i]; 
}

const float& Quaternion::operator [](int i) const { 
  return f[i]; 
}

const Quaternion& Quaternion::operator *= (const Quaternion& q) {
    // TODO: see if there's a cute intrinsic way to do this.
    _XO_ASSIGN_QUAT(
      w * q.x + x * q.w + y * q.z - z * q.y,
      w * q.y - x * q.z + y * q.w + z * q.x,
      w * q.z + x * q.y - y * q.x + z * q.w,
      w * q.w - x * q.x - y * q.y - z * q.z);
  return *this;
}

Quaternion Quaternion::operator * (const Quaternion& q) const {
  return Quaternion(*this) *= q;
}

bool Quaternion::operator == (const Quaternion& q) const {
#   if defined(XO_SSE)
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(q.m, m)), sse::Epsilon)) & 15) == 15;
#   else
    return 
        CloseEnough(x, q.x, Epsilon) && 
        CloseEnough(y, q.y, Epsilon) && 
        CloseEnough(z, q.z, Epsilon) &&
        CloseEnough(w, q.w, Epsilon);
#   endif
}

bool Quaternion::operator != (const Quaternion& q) const {
  return !((*this) == q);
}

XOMATH_END_XO_NS();

#endif


#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include SSE.h directly. Include xo-math.h.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

_XOINL Vector2 Abs(const Vector2& v) {
    return Vector2(Abs(v.x), Abs(v.y));
}

_XOINL Vector3 Abs(const Vector3& v) {
    return (sse::Abs(v.m));
}

_XOINL Vector4 Abs(const Vector4& v) {
    return Vector4(sse::Abs(v.m));
}

#if defined(XO_SSE)

namespace sse {

    // The control of MXCSR usage is inspired by Agner Fog's use of them in vectorclasses.
    // vectorclasses uses them to optionally speed up subnormal operations.
    // To achieve this in xomath, call the following once per thread where xo-math is used:
    //      sse::UpdateControlWord();       // updates the thread-local state.
    //      sse::SetDenormalsAreZero(true); // force all denormal values to 0
    //      sse::SetFlushToZero(true);      // underflowing operations produce 0
    // Note: this will only produce speed gains where subnormal values are likely to occur.
    // See http://wm.ite.pl/articles/sse-penalties-of-errors.html for more details.
    namespace mxcsr {
        // Flags that are set on the CPU if an exception had occured.
        // They will remain set until manually unset.
        enum class Flags {
            InvalidOperation                = (1 << 0),
            Denormal                        = (1 << 1),
            DivideByZero                    = (1 << 2),
            Overflow                        = (1 << 3),
            Underflow                       = (1 << 4),
            Precision                       = (1 << 5),
        };

        enum class DAZ {
            DenormalsAreZero                = (1 << 6),
        };

        enum class Masks {
            InvalidOperation                = (1 << 7),
            Denormal                        = (1 << 8),
            DivideByZero                    = (1 << 9),
            Overflow                        = (1 << 10),
            Underflow                       = (1 << 11),
            Precision                       = (1 << 12)
        };

        enum class Rounding {
            Nearest                         = 0,
            Negative                        = (1 << 13),
            Positive                        = (1 << 14),
            Zero                            = (1 << 13) | (1 << 14),
            
            Bits                            = (1 << 13) | (1 << 14)
        };

        enum class FZ {
            FlushToZero                     = (1 << 15)
        };
    }

    bool GetControlMask(mxcsr::Masks mask, bool withUpdate = false);
    bool GetControlMask(unsigned mask, bool withUpdate = false);
    bool GetDenormalExceptionMask(bool withUpdate = false);
    bool GetDivideByZeroExceptionMask(bool withUpdate = false);
    bool GetInvalidOperationExceptionMask(bool withUpdate = false);
    bool GetOverflowExceptionMask(bool withUpdate = false);
    bool GetPrecisionExceptionMask(bool withUpdate = false);
    bool GetUnderflowExceptionMask(bool withUpdate = false);

    bool HasControlFlagBeenSet(mxcsr::Flags flags, bool withUpdate = false, bool thenFlush = false);
    bool HasControlFlagBeenSet(unsigned flags, bool withUpdate = false, bool thenFlush = false);
    bool HasDenormalExceptionOccured(bool withUpdate = false, bool thenFlush = false);
    bool HasDenormalsAreZeroSet(bool withUpdate = false);
    bool HasDivideByZeroExceptionOccured(bool withUpdate = false, bool thenFlush = false);
    bool HasFlushToZeroSet(bool withUpdate = false);
    bool HasInvalidOperationExceptionOccured(bool withUpdate = false, bool thenFlush = false);
    bool HasOverflowExceptionOccured(bool withUpdate = false, bool thenFlush = false);
    bool HasPrecisionExceptionOccured(bool withUpdate = false, bool thenFlush = false);
    bool HasUnderflowExceptionOccured(bool withUpdate = false, bool thenFlush = false);

    mxcsr::Rounding GetRoundingMode(bool withUpdate = false);
    void GetAllMXCSRInfo(std::ostream& os, bool withUpdate = false);

    void RemoveControlWord(unsigned control);

    void SetControlMask(mxcsr::Masks mask, bool value, bool withUpdate = false);
    void SetControlMask(unsigned mask, bool value, bool withUpdate = false);
    void SetControlWord(unsigned control);
    void SetControlWordAddative(unsigned control);
    void SetDenormalExceptionMask(bool value, bool withUpdate = false);
    void SetDenormalsAreZero(bool value, bool withUpdate = false);
    void SetDivideByZeroExceptionMask(bool value, bool withUpdate = false);
    void SetFlushToZero(bool value, bool withUpdate = false);
    void SetInvalidOperationExceptionMask(bool value, bool withUpdate = false);
    void SetOverflowExceptionMask(bool value, bool withUpdate = false);
    void SetPrecisionExceptionMask(bool value, bool withUpdate = false);
    void SetRoundingMode(mxcsr::Rounding mode, bool withUpdate = false);
    void SetRoundingMode(unsigned mode, bool withUpdate = false);
    void SetUnderflowExceptionMask(bool value, bool withUpdate = false);

    void ThrowAllExceptions(bool withUpdate = false);
    void ThrowNoExceptions(bool withUpdate = false);
    void UpdateControlWord();
}
#endif

XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL


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

