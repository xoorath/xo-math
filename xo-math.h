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
//  * Support NEON (investigate http://projectne10.github.io/Ne10/ license)
//  * Add transformation object.
//  * Move trivial methods to headers, keep only "meaningful" code in *.cpp/*inline.h files
//  * Use macros to generate variant functions for other classes, like in Vector3.h
//  * Consider other simpler documentation solution. github pages?
//  * Consider something like premake or similar for dev project files. Check out https://github.com/bkaradzic/GENie
//  * Move CI back to travis, include more compiler versions.
//  * Matrix: finish filling out stubs
//  * fast sin/cos, sincos, _x2 _x2 _x3
//  * RNG: https://blogs.unity3d.com/2015/01/07/a-primer-on-repeatable-random-numbers/
//  * Noise:
//  * Consider moving all randoms out of types themselves and into a separate file.
//  * Make a very simple graphics demo using many features of xo-math.

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
#if defined(__arm__)

#else
#   if defined(_MSC_VER)
#       include <xmmintrin.h>
#   else
#       include <x86intrin.h>
#   endif
#endif

#if _MSC_VER 
#pragma warning(pop) 
#endif 

// Not available in clang, so far as I can tell.
#ifndef _MM_ALIGN16
#   define _MM_ALIGN16 __attribute__((aligned(16)))
#endif

#define XOMATH_INTERNAL 1

#if !defined(_XO_MATH_STRINGIFY_HELPER)
    // Do not undef at end of file. Will break XO_MATH_VERSION... and XO_MATH_COMPILER... defines
#   define _XO_MATH_STRINGIFY_HELPER(x) #x
#endif

#if !defined(_XO_MATH_STRINGIFY)
    // Do not undef at end of file. Will break XO_MATH_VERSION... and XO_MATH_COMPILER... defines
#   define _XO_MATH_STRINGIFY(x) _XO_MATH_STRINGIFY_HELPER(x)
#endif




// todo: remove constexpr in visual studio 2013 and re-test for support
#if defined(_MSC_VER) && _MSC_VER < 1800
#    define _XOCONSTEXPR
#    if !defined(_XONOCONSTEXPR)
#        define _XONOCONSTEXPR
#    endif
#else
#   define _XOCONSTEXPR constexpr
#endif

#if defined(_MSC_VER)
#   define _XOINL __forceinline
#else
#   define _XOINL inline
#endif


#if (defined(__clang__) && defined(__APPLE__))
#   define _XOTLS __thread
#elif (defined(_MSC_VER) && _MSC_VER < 1800)
#   define _XOTLS __declspec(thread)
#else
#   define _XOTLS thread_local
#endif

# define _XO_NO_TLS (defined(__clang__) && defined(__APPLE__)) || (defined(_MSC_VER) && _MSC_VER < 1800)

// apple clang doesn't give us thread_local until xcode 8.
#if _XO_NO_TLS
#   define _XO_TLS_ENGINE \
        static _XOTLS std::mt19937* tls_engline; \
        static _XOTLS char mem[sizeof(std::mt19937)];\
        if(!tls_engline) { \
            tls_engline = new(mem) std::mt19937((unsigned)clock()); \
        }
#    define _XO_TLS_DISTRIBUTION dist(*tls_engline)
#else
#   define _XO_TLS_ENGINE \
        static _XOTLS std::mt19937 tls_engline((unsigned)clock());
#    define _XO_TLS_DISTRIBUTION dist(tls_engline)
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
#if !defined(XO_16ALIGNED_MALLOC)
#    define XO_16ALIGNED_MALLOC(size) _mm_malloc(size, 16)
#endif
#if  !defined(XO_16ALIGNED_FREE)
#    define XO_16ALIGNED_FREE(ptr) _mm_free(ptr)
#endif

#define _XO_OVERLOAD_NEW_DELETE() \
     _XOINL static void* operator new (std::size_t size)     { return XO_16ALIGNED_MALLOC(size); } \
     _XOINL static void* operator new[] (std::size_t size)   { return XO_16ALIGNED_MALLOC(size); } \
     _XOINL static void operator delete (void* ptr)          { XO_16ALIGNED_FREE(ptr); } \
     _XOINL static void operator delete[] (void* ptr)        { XO_16ALIGNED_FREE(ptr); }

#else
    // we don't need to overload new and delete unless memory alignment is required.
#   define _XO_OVERLOAD_NEW_DELETE()
#endif

#define _XO_MIN(a, b) (a < b ? a : b)
#define _XO_MAX(a, b) (a > b ? a : b)

// wrap for now, so we have the option to make a faster version later.
_XOINL float Min(float x, float y)      { return _XO_MIN(x, y); }
_XOINL float Max(float x, float y)      { return _XO_MAX(x, y); }
_XOINL float Abs(float f)               { return f > 0.0f ? f : -f; }
_XOINL float Sqrt(float f)              { return sqrtf(f); } 
_XOINL float Cbrt(float f)              { return cbrtf(f); }
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

#if defined(XO_SSE)
#define _XO_ASSIGN_QUAT(W, X, Y, Z) m = _mm_set_ps(W, Z, Y, X);
#define _XO_ASSIGN_QUAT_Q(Q, W, X, Y, Z) Q.m = _mm_set_ps(W, Z, Y, X);
#else
#define _XO_ASSIGN_QUAT(W, X, Y, Z) this->w = W; this->x = X; this->y = Y; this->z = Z;
#define _XO_ASSIGN_QUAT_Q(Q, W, X, Y, Z) Q.w = W; Q.x = X; Q.y = Y; Q.z = Z;
#endif

////////////////////////////////////////////////////////////////////////// Module Includes
XOMATH_BEGIN_XO_NS();

class _MM_ALIGN16 Vector2 {
public:

    ////////////////////////////////////////////////////////////////////////// Constructors
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#constructors
    Vector2() { } 
    Vector2(float v); 
    Vector2(float x, float y); 
    Vector2(const Vector2& v); 
    Vector2(const class Vector3& v); 
    Vector2(const class Vector4& v); 

    ////////////////////////////////////////////////////////////////////////// Set / Get Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#set_get_methods
    Vector2& Set(float x, float y);
    Vector2& Set(float f);
    Vector2& Set(const Vector2& vec);
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
    _XOINL Vector2& operator += (const Vector2& v);
    _XOINL Vector2& operator += (float v);
    _XOINL Vector2& operator += (double v);
    _XOINL Vector2& operator += (int v);
    _XOINL Vector2& operator += (const class Vector3& v);
    _XOINL Vector2& operator += (const class Vector4& v);
    _XOINL Vector2& operator -= (const Vector2& v);
    _XOINL Vector2& operator -= (float v);
    _XOINL Vector2& operator -= (double v);
    _XOINL Vector2& operator -= (int v);
    _XOINL Vector2& operator -= (const class Vector3& v);
    _XOINL Vector2& operator -= (const class Vector4& v);
    _XOINL Vector2& operator *= (const Vector2& v);
    _XOINL Vector2& operator *= (float v);
    _XOINL Vector2& operator *= (double v);
    _XOINL Vector2& operator *= (int v);
    _XOINL Vector2& operator *= (const class Vector3& v);
    _XOINL Vector2& operator *= (const class Vector4& v);
    _XOINL Vector2& operator /= (const Vector2& v);
    _XOINL Vector2& operator /= (float v);
    _XOINL Vector2& operator /= (double v);
    _XOINL Vector2& operator /= (int v);
    _XOINL Vector2& operator /= (const class Vector3& v);
    _XOINL Vector2& operator /= (const class Vector4& v);
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
    bool IsNormalized() const {
        return CloseEnough(MagnitudeSquared(), 1.0f, Epsilon);
    }
    bool IsZero() const {
        return CloseEnough(MagnitudeSquared(), 0.0f, Epsilon);
    }
    float Magnitude() const {
        return sqrtf(MagnitudeSquared());
    }
    float MagnitudeSquared() const {
        return x*x + y*y;
    }
    float Sum() const {
        return x + y;
    }

    Vector2& Normalize() {
        return (*this) /= Magnitude();
    }

    Vector2 Normalized() const {
        return Vector2(*this).Normalize();
    }

    ////////////////////////////////////////////////////////////////////////// Static Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#static_methods
    static void Lerp(const Vector2& a, const Vector2& b, float t, Vector2& outVec) {
        outVec = a + ((b - a) * t);
    }
    static void Max(const Vector2& a, const Vector2& b, Vector2& outVec) {
        outVec.Set(_XO_MAX(a.x, b.x), _XO_MAX(a.y, b.y));
    }
    static void Midpoint(const Vector2& a, const Vector2& b, Vector2& outVec) {
        Vector2::Lerp(a, b, 0.5f, outVec);
    }
    static void Min(const Vector2& a, const Vector2& b, Vector2& outVec) {
        outVec.Set(_XO_MIN(a.x, b.x), _XO_MIN(a.y, b.y));
    }
    static void OrthogonalCCW(const Vector2& v, Vector2& outVec) {
        outVec.Set(-v.y, v.x);
    }
    static void OrthogonalCW(const Vector2& v, Vector2& outVec) {
        outVec.Set(v.y, -v.x);
    }

    static float AngleDegrees(const Vector2& a, const Vector2& b) {
        return AngleRadians(a, b) * Rad2Deg;
    }
    static float AngleRadians(const Vector2& a, const Vector2& b) {
        return -ATan2(Cross(a, b), Dot(a, b));
    }
    static float Cross(const Vector2& a, const Vector2& b) {
        return (a.x * b.y) - (a.y * b.x);
    }
    static float Distance(const Vector2& a, const Vector2& b) {
        return (b - a).Magnitude();
    }
    static float DistanceSquared(const Vector2& a, const Vector2& b) {
        return (b - a).MagnitudeSquared();
    }
    static float Dot(const Vector2& a, const Vector2& b) {
        return (a * b).Sum();
    }

#define _RET_VARIANT(name) { Vector2 tempV; name(
#define _RET_VARIANT_END() tempV); return tempV; }
#define _RET_VARIANT_0(name)                                 _RET_VARIANT(name)                               _RET_VARIANT_END()
#define _RET_VARIANT_1(name, first)                          _RET_VARIANT(name) first,                        _RET_VARIANT_END()
#define _RET_VARIANT_2(name, first, second)                  _RET_VARIANT(name) first, second,                _RET_VARIANT_END()
#define _RET_VARIANT_3(name, first, second, third)           _RET_VARIANT(name) first, second, third,         _RET_VARIANT_END()
#define _RET_VARIANT_4(name, first, second, third, fourth)   _RET_VARIANT(name) first, second, third, fourth, _RET_VARIANT_END()
#define _THIS_VARIANT0(name)                                { return name(*this); }
#define _THIS_VARIANT1(name, first)                         { return name(*this, first); }
#define _THIS_VARIANT2(name, first, second)                 { return name(*this, first, second); }

    ////////////////////////////////////////////////////////////////////////// Variants
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#variants
    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t)    _RET_VARIANT_3(Lerp, a, b, t)
    static Vector2 Max(const Vector2& a, const Vector2& b)              _RET_VARIANT_2(Max, a, b)
    static Vector2 Midpoint(const Vector2& a, const Vector2& b)         _RET_VARIANT_2(Midpoint, a, b)
    static Vector2 Min(const Vector2& a, const Vector2& b)              _RET_VARIANT_2(Min, a, b)
    static Vector2 OrthogonalCCW(const Vector2& v)                      _RET_VARIANT_1(OrthogonalCCW, v)
    static Vector2 OrthogonalCW(const Vector2& v)                       _RET_VARIANT_1(OrthogonalCW, v)

    float AngleDegrees(const Vector2& v) const                          _THIS_VARIANT1(AngleDegrees, v)
    float AngleRadians(const Vector2& v) const                          _THIS_VARIANT1(AngleRadians, v)
    float Cross(const Vector2& v) const                                 _THIS_VARIANT1(Cross, v)
    float Distance(const Vector2& v) const                              _THIS_VARIANT1(Distance, v)
    float DistanceSquared(const Vector2& v) const                       _THIS_VARIANT1(DistanceSquared, v)
    float Dot(const Vector2& v) const                                   _THIS_VARIANT1(Dot, v)
    Vector2 Lerp(const Vector2& v, float t) const                       _THIS_VARIANT2(Lerp, v, t)
    Vector2 Midpoint(const Vector2& v) const                            _THIS_VARIANT1(Midpoint, v)
    Vector2 OrthogonalCCW() const                                       _THIS_VARIANT0(OrthogonalCCW)
    Vector2 OrthogonalCW() const                                        _THIS_VARIANT0(OrthogonalCW)

#undef _RET_VARIANT
#undef _RET_VARIANT_END
#undef _RET_VARIANT_0
#undef _RET_VARIANT_1
#undef _RET_VARIANT_2
#undef _RET_VARIANT_3
#undef _RET_VARIANT_4
#undef _THIS_VARIANT0
#undef _THIS_VARIANT1
#undef _THIS_VARIANT2

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

XOMATH_BEGIN_XO_NS();

class _MM_ALIGN16 Vector3 {
public:
    ////////////////////////////////////////////////////////////////////////// Constructors
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#constructors
    Vector3() { } 
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
    Vector3& Set(float x, float y, float z);
    Vector3& Set(float f);
    Vector3& Set(const Vector3& vec);
#if defined(XO_SSE)
    Vector3& Set(const __m128& vec);
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
    _XOINL Vector3& operator += (const Vector3& v);
    _XOINL Vector3& operator += (float v);
    _XOINL Vector3& operator += (double v);
    _XOINL Vector3& operator += (int v);
    _XOINL Vector3& operator += (const class Vector2& v);
    _XOINL Vector3& operator += (const class Vector4& v);
    _XOINL Vector3& operator -= (const Vector3& v);
    _XOINL Vector3& operator -= (float v);
    _XOINL Vector3& operator -= (double v);
    _XOINL Vector3& operator -= (int v);
    _XOINL Vector3& operator -= (const class Vector2& v);
    _XOINL Vector3& operator -= (const class Vector4& v);
    _XOINL Vector3& operator *= (const Vector3& v);
    _XOINL Vector3& operator *= (float v);
    _XOINL Vector3& operator *= (double v);
    _XOINL Vector3& operator *= (int v);
    _XOINL Vector3& operator *= (const class Vector2& v);
    _XOINL Vector3& operator *= (const class Vector4& v);
    _XOINL Vector3& operator /= (const Vector3& v);
    _XOINL Vector3& operator /= (float v);
    _XOINL Vector3& operator /= (double v);
    _XOINL Vector3& operator /= (int v);
    _XOINL Vector3& operator /= (const class Vector2& v);
    _XOINL Vector3& operator /= (const class Vector4& v);
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
    bool IsNormalized() const {
        return CloseEnough(MagnitudeSquared(), 1.0f, Epsilon);
    }
    bool IsZero() const {
        return CloseEnough(MagnitudeSquared(), 0.0f, Epsilon);
    }
    float Magnitude() const {
        return Sqrt(MagnitudeSquared());
    }
    float MagnitudeSquared() const {
        return ((*this) * (*this)).Sum();
    }
    float Sum() const;

    Vector3& Normalize();

    Vector3& NormalizeSafe();

    Vector3 Normalized() const {
        return Vector3(*this).Normalize();
    }

    Vector3 NormalizedSafe() const {
        return Vector3(*this).NormalizeSafe();
    }
    Vector3 ZYX() const;
    

    ////////////////////////////////////////////////////////////////////////// Static Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#static_methods
    static void Cross(const Vector3& a, const Vector3& b, Vector3& outVec);
    static void Lerp(const Vector3& a, const Vector3& b, float t, Vector3& outVec) {
        outVec = a + ((b - a) * t);
    }
    static void Max(const Vector3& a, const Vector3& b, Vector3& outVec) {
        outVec.Set(_XO_MAX(a.x, b.x), _XO_MAX(a.y, b.y), _XO_MAX(a.z, b.z));
    }
    static void Min(const Vector3& a, const Vector3& b, Vector3& outVec) {
        outVec.Set(_XO_MIN(a.x, b.x), _XO_MIN(a.y, b.y), _XO_MIN(a.z, b.z));
    }
    static void RotateDegrees(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec) {
        RotateRadians(v, axis, angle * Deg2Rad, outVec);
    }
    static void RotateRadians(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec);
    // Calls Vector3::AngleRadians, converting the return value to degrees.
    static float AngleDegrees(const Vector3& a, const Vector3& b) {
        return AngleRadians(a, b) * Rad2Deg;
    }
    static float AngleRadians(const Vector3& a, const Vector3& b);
    static float Distance(const Vector3&a, const Vector3&b) {
        return (b - a).Magnitude();
    }
    static float DistanceSquared(const Vector3& a, const Vector3& b) {
        return (b - a).MagnitudeSquared();
    }
    static float Dot(const Vector3& a, const Vector3& b);
    

    ////////////////////////////////////////////////////////////////////////// Random Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#random_methods
    static void RandomInCircle(const Vector3& up, float radius, Vector3& outVec);
    static void RandomInConeRadians(const Vector3& forward, float angle, Vector3& outVec);
    static void RandomOnCircle(const Vector3& up, float radius, Vector3& outVec);
    static void RandomOnConeRadians(const Vector3& forward, float angle, Vector3& outVec);
    static void RandomOnCube(float size, Vector3& outVec);
    static void RandomOnSphere(float radius, Vector3& outVec);

    static void RandomOnCircle(Vector3& outVec) {
        RandomOnCircle(Vector3::Up, 1.0f, outVec);
    }
    static void RandomInFanRadians(const Vector3& forward, const Vector3& up, float angle, Vector3& outVec) {
        Vector3::RotateRadians(forward, up, RandomRange(-angle*0.5f, angle*0.5f), outVec);
    }
    static void RandomInCube(float size, Vector3& outVec) {
        outVec.Set(RandomRange(-size, size), RandomRange(-size, size), RandomRange(-size, size));
    }
    static void RandomInSphere(float minRadius, float maxRadius, Vector3& outVec) {
        RandomOnSphere(Sqrt(RandomRange(minRadius, maxRadius)), outVec);
    }
    static void RandomInSphere(float radius, Vector3& outVec) {
        RandomInSphere(0.0f, radius, outVec);
    }
    static void RandomInSphere(Vector3& outVec) {
        RandomInSphere(1.0f, outVec);
    }
    static void RandomInCircle(Vector3& outVec) {
        RandomInCircle(Vector3::Up, 1.0f, outVec);
    }
    static void RandomInCube(Vector3& outVec) {
        RandomInCube(0.5f, outVec);
    }
    static void RandomInConeDegrees(const Vector3& forward, float angle, Vector3& outVec) { 
        RandomInConeRadians(forward, angle * Deg2Rad, outVec); 
    }
    static void RandomOnCube(Vector3& outVec) {
        RandomOnCube(0.5f, outVec);
    }
    static void RandomInFanDegrees(const Vector3& forward, const Vector3& up, float angle, Vector3& outVec) { 
        RandomInFanRadians(forward, up, angle * Deg2Rad, outVec); 
    }
    static void RandomOnConeDegrees(const Vector3& forward, float angle, Vector3& outVec) { 
        RandomOnConeRadians(forward, angle * Deg2Rad, outVec); 
    }
    static void RandomOnSphere(Vector3& outVec) {
        RandomOnSphere(1.0f, outVec);
    }


#define _RET_VARIANT(name) { Vector3 tempV; name(
#define _RET_VARIANT_END() tempV); return tempV; }
#define _RET_VARIANT_0(name)                                 _RET_VARIANT(name)                               _RET_VARIANT_END()
#define _RET_VARIANT_1(name, first)                          _RET_VARIANT(name) first,                        _RET_VARIANT_END()
#define _RET_VARIANT_2(name, first, second)                  _RET_VARIANT(name) first, second,                _RET_VARIANT_END()
#define _RET_VARIANT_3(name, first, second, third)           _RET_VARIANT(name) first, second, third,         _RET_VARIANT_END()
#define _RET_VARIANT_4(name, first, second, third, fourth)   _RET_VARIANT(name) first, second, third, fourth, _RET_VARIANT_END()
#define _THIS_VARIANT1(name, first)                         { return name(*this, first); }
#define _THIS_VARIANT2(name, first, second)                 { return name(*this, first, second); }

    ////////////////////////////////////////////////////////////////////////// Variants
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#variants
    float AngleDegrees(const Vector3& v) const                                                  _THIS_VARIANT1(AngleDegrees, v)
    float AngleRadians(const Vector3& v) const                                                  _THIS_VARIANT1(AngleRadians, v)
    float Distance(const Vector3& v) const                                                      _THIS_VARIANT1(Distance, v)
    float DistanceSquared(const Vector3& v) const                                               _THIS_VARIANT1(DistanceSquared, v)
    float Dot(const Vector3& v) const                                                           _THIS_VARIANT1(Dot, v)
    Vector3 Cross(const Vector3& v) const                                                       _THIS_VARIANT1(Cross, v)
    Vector3 Lerp(const Vector3& v, float t) const                                               _THIS_VARIANT2(Lerp, v, t)
    Vector3 RotateDegrees(const Vector3& axis, float angle) const                               _THIS_VARIANT2(RotateDegrees, axis, angle)
    Vector3 RotateRadians(const Vector3& axis, float angle) const                               _THIS_VARIANT2(RotateRadians, axis, angle)

    static Vector3 Cross(const Vector3& a, const Vector3& b)                                    _RET_VARIANT_2(Cross, a, b)
    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t)                            _RET_VARIANT_3(Lerp, a, b, t)
    static Vector3 Max(const Vector3& a, const Vector3& b)                                      _RET_VARIANT_2(Max, a, b)
    static Vector3 Min(const Vector3& a, const Vector3& b)                                      _RET_VARIANT_2(Min, a, b)
    static Vector3 RandomInCircle()                                                             _RET_VARIANT_0(RandomInCircle)
    static Vector3 RandomInCircle(const Vector3& up, float radius)                              _RET_VARIANT_2(RandomInCircle, up, radius)
    static Vector3 RandomInConeDegrees(const Vector3& forward, float angle)                     _RET_VARIANT_2(RandomInConeDegrees, forward, angle)
    static Vector3 RandomInConeRadians(const Vector3& forward, float angle)                     _RET_VARIANT_2(RandomInConeRadians, forward, angle)
    static Vector3 RandomInCube()                                                               _RET_VARIANT_0(RandomInCube)
    static Vector3 RandomInCube(float size)                                                     _RET_VARIANT_1(RandomInCube, size)
    static Vector3 RandomInFanDegrees(const Vector3& forward, const Vector3& up, float angle)   _RET_VARIANT_3(RandomInFanDegrees, forward, up, angle)
    static Vector3 RandomInFanRadians(const Vector3& forward, const Vector3& up, float angle)   _RET_VARIANT_3(RandomInFanRadians, forward, up, angle)
    static Vector3 RandomInSphere()                                                             _RET_VARIANT_0(RandomInSphere)
    static Vector3 RandomInSphere(float minRadius, float maxRadius)                             _RET_VARIANT_2(RandomInSphere, minRadius, maxRadius)
    static Vector3 RandomInSphere(float radius)                                                 _RET_VARIANT_1(RandomInSphere, radius)
    static Vector3 RandomOnCircle()                                                             _RET_VARIANT_0(RandomOnCircle)
    static Vector3 RandomOnCircle(const Vector3& up, float radius)                              _RET_VARIANT_2(RandomOnCircle, up, radius)
    static Vector3 RandomOnConeDegrees(const Vector3& forward, float angle)                     _RET_VARIANT_2(RandomOnConeDegrees, forward, angle)
    static Vector3 RandomOnConeRadians(const Vector3& forward, float angle)                     _RET_VARIANT_2(RandomOnConeRadians, forward, angle)
    static Vector3 RandomOnCube()                                                               _RET_VARIANT_0(RandomOnCube)
    static Vector3 RandomOnCube(float size)                                                     _RET_VARIANT_1(RandomOnCube, size)
    static Vector3 RandomOnSphere()                                                             _RET_VARIANT_0(RandomOnSphere)
    static Vector3 RandomOnSphere(float radius)                                                 _RET_VARIANT_1(RandomOnSphere, radius)
    static Vector3 RotateDegrees(const Vector3& v, const Vector3& axis, float angle)            _RET_VARIANT_3(RotateDegrees, v, axis, angle)
    static Vector3 RotateRadians(const Vector3& v, const Vector3& axis, float angle)            _RET_VARIANT_3(RotateRadians, v, axis, angle)

#undef _RET_VARIANT
#undef _RET_VARIANT_END
#undef _RET_VARIANT_0
#undef _RET_VARIANT_1
#undef _RET_VARIANT_2
#undef _RET_VARIANT_3
#undef _RET_VARIANT_4
#undef _THIS_VARIANT1
#undef _THIS_VARIANT2

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
    Vector4(const class Quaternion& q);

    ////////////////////////////////////////////////////////////////////////// Set / Get Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#set_get_methods
    Vector4& Set(float x, float y, float z, float w);
    Vector4& Set(float f);
    Vector4& Set(const Vector4& vec);
    Vector4& Set(const Vector2& vec);
    Vector4& Set(const Vector2& vec, float z, float w);
    Vector4& Set(const Vector3& vec);
    Vector4& Set(const Vector3& vec, float w);
#if defined(XO_SSE)
    Vector4& Set(const __m128& vec);
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
    _XOINL Vector4& operator += (const Vector4& v);
    _XOINL Vector4& operator += (float v);
    _XOINL Vector4& operator += (double v);
    _XOINL Vector4& operator += (int v);
    _XOINL Vector4& operator += (const class Vector2& v);
    _XOINL Vector4& operator += (const class Vector3& v);
    _XOINL Vector4& operator -= (const Vector4& v);
    _XOINL Vector4& operator -= (float v);
    _XOINL Vector4& operator -= (double v);
    _XOINL Vector4& operator -= (int v);
    _XOINL Vector4& operator -= (const class Vector2& v);
    _XOINL Vector4& operator -= (const class Vector3& v);
    _XOINL Vector4& operator *= (const Vector4& v);
    _XOINL Vector4& operator *= (float v);
    _XOINL Vector4& operator *= (double v);
    _XOINL Vector4& operator *= (int v);
    _XOINL Vector4& operator *= (const class Vector2& v);
    _XOINL Vector4& operator *= (const class Vector3& v);
    // See: XO_NO_INVERSE_DIVISION
    _XOINL Vector4& operator /= (const Vector4& v);
    _XOINL Vector4& operator /= (float v);
    _XOINL Vector4& operator /= (double v);
    _XOINL Vector4& operator /= (int v);
    _XOINL Vector4& operator /= (const class Vector2& v);
    _XOINL Vector4& operator /= (const class Vector3& v);
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


    bool IsNormalized() const {
        return CloseEnough(MagnitudeSquared(), 1.0f, Epsilon);
    }
    bool IsZero() const {
        return CloseEnough(MagnitudeSquared(), 0.0f, Epsilon);
    }
    float Magnitude() const {
        return Sqrt(MagnitudeSquared());
    }
    float MagnitudeSquared() const {
        return ((*this) * (*this)).Sum();
    }
    float Sum() const;

    Vector4& Normalize() {
        return (*this) /= Magnitude();
    }

    Vector4& NormalizeSafe();

    Vector4 Normalized() const {
        return Vector4(*this).Normalize();
    }

    Vector3 NormalizedSafe() const {
        return Vector3(*this).NormalizeSafe();
    }

    ////////////////////////////////////////////////////////////////////////// Static Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#static_methods
    static void Lerp(const Vector4& a, const Vector4& b, float t, Vector4& outVec) {
        outVec = a + ((b - a) * t);
    }
    static void Max(const Vector4& a, const Vector4& b, Vector4& outVec) {
        outVec.Set(_XO_MAX(a.x, b.x), _XO_MAX(a.y, b.y), _XO_MAX(a.z, b.z), _XO_MAX(a.w, b.w));
    }
    static void Min(const Vector4& a, const Vector4& b, Vector4& outVec) {
        outVec.Set(_XO_MIN(a.x, b.x), _XO_MIN(a.y, b.y), _XO_MIN(a.z, b.z), _XO_MIN(a.w, b.w));
    }
    static float Distance(const Vector4&a, const Vector4&b) {
        return (b - a).Magnitude();
    }
    static float DistanceSquared(const Vector4& a, const Vector4& b) {
        return (b - a).MagnitudeSquared();
    }
    static float Dot(const Vector4& a, const Vector4& b);
    
#define _RET_VARIANT(name) { Vector4 tempV; name(
#define _RET_VARIANT_END() tempV); return tempV; }
#define _RET_VARIANT_0(name)                                 _RET_VARIANT(name)                               _RET_VARIANT_END()
#define _RET_VARIANT_1(name, first)                          _RET_VARIANT(name) first,                        _RET_VARIANT_END()
#define _RET_VARIANT_2(name, first, second)                  _RET_VARIANT(name) first, second,                _RET_VARIANT_END()
#define _RET_VARIANT_3(name, first, second, third)           _RET_VARIANT(name) first, second, third,         _RET_VARIANT_END()
#define _RET_VARIANT_4(name, first, second, third, fourth)   _RET_VARIANT(name) first, second, third, fourth, _RET_VARIANT_END()
#define _THIS_VARIANT1(name, first)                         { return name(*this, first); }
#define _THIS_VARIANT2(name, first, second)                 { return name(*this, first, second); }

    ////////////////////////////////////////////////////////////////////////// Variants
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#variants
    static Vector4 Lerp(const Vector4& a, const Vector4& b, float t)    _RET_VARIANT_3(Lerp, a, b, t)
    static Vector4 Max(const Vector4& a, const Vector4& b)              _RET_VARIANT_2(Max, a, b)
    static Vector4 Min(const Vector4& a, const Vector4& b)              _RET_VARIANT_2(Min, a, b)

    float Distance(const Vector4& v) const                              _THIS_VARIANT1(Distance, v)
    float DistanceSquared(const Vector4& v) const                       _THIS_VARIANT1(DistanceSquared, v)
    float Dot(const Vector4& v) const                                   _THIS_VARIANT1(Dot, v)
    Vector4 Lerp(const Vector4& v, float t) const                       _THIS_VARIANT2(Lerp, v, t)

#undef _RET_VARIANT
#undef _RET_VARIANT_END
#undef _RET_VARIANT_0
#undef _RET_VARIANT_1
#undef _RET_VARIANT_2
#undef _RET_VARIANT_3
#undef _RET_VARIANT_4
#undef _THIS_VARIANT1
#undef _THIS_VARIANT2

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


    Matrix4x4& SetRow(int i, const Vector4& r);
    Matrix4x4& SetColumn(int i, const Vector4& r);
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

    _XOINL Matrix4x4& operator += (const Matrix4x4& m);
    _XOINL Matrix4x4& operator -= (const Matrix4x4& m);
    _XOINL Matrix4x4& operator *= (const Matrix4x4& m);

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


    void MakeInverse();
    void GetInverse(Matrix4x4& o) const { o = *this; o.MakeInverse(); }
    bool TryMakeInverse();
    bool TryGetInverse(Matrix4x4& o) const { o = *this; return o.TryMakeInverse(); }

    Matrix4x4& Transpose();
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

XOMATH_BEGIN_XO_NS();

class _MM_ALIGN16 Quaternion {
public:
    Quaternion();
    Quaternion(const Matrix4x4& m);
    Quaternion(float x, float y, float z, float w);

    _XO_OVERLOAD_NEW_DELETE();

    _XOINL float& operator [](int i);
    _XOINL const float& operator [](int i) const;

    _XOINL Quaternion& operator *= (const Quaternion& q);
    _XOINL Quaternion operator * (const Quaternion& q) const;

    _XOINL bool operator == (const Quaternion& q) const;
    _XOINL bool operator != (const Quaternion& q) const;

    Quaternion& MakeConjugate();
    Quaternion& MakeInverse();
    Quaternion& Normalize();
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

#define _RET_VARIANT(name) { Quaternion tempV; name(
#define _RET_VARIANT_END() tempV); return tempV; }
#define _RET_VARIANT_0(name)                                 _RET_VARIANT(name)                               _RET_VARIANT_END()
#define _RET_VARIANT_1(name, first)                          _RET_VARIANT(name) first,                        _RET_VARIANT_END()
#define _RET_VARIANT_2(name, first, second)                  _RET_VARIANT(name) first, second,                _RET_VARIANT_END()
#define _RET_VARIANT_3(name, first, second, third)           _RET_VARIANT(name) first, second, third,         _RET_VARIANT_END()
#define _RET_VARIANT_4(name, first, second, third, fourth)   _RET_VARIANT(name) first, second, third, fourth, _RET_VARIANT_END()
#define _THIS_VARIANT1(name, first)                         { return name(*this, first); }
#define _THIS_VARIANT2(name, first, second)                 { return name(*this, first, second); }

    static Quaternion AxisAngleRadians(const Vector3& axis, float radians)                          _RET_VARIANT_2(AxisAngleRadians, axis, radians)
    static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t)                       _RET_VARIANT_3(Lerp, a, b, t)
    static Quaternion LookAtFromDirection(const Vector3& direction)                                 _RET_VARIANT_1(LookAtFromDirection, direction)
    static Quaternion LookAtFromDirection(const Vector3& direction, const Vector3& up)              _RET_VARIANT_2(LookAtFromDirection, direction, up)
    static Quaternion LookAtFromPosition(const Vector3& from, const Vector3& to)                    _RET_VARIANT_2(LookAtFromPosition, from, to)
    static Quaternion LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up) _RET_VARIANT_3(LookAtFromPosition, from, to, up)
    static Quaternion RotationRadians(const Vector3& v)                                             _RET_VARIANT_1(RotationRadians, v)
    static Quaternion RotationRadians(float x, float y, float z)                                    _RET_VARIANT_3(RotationRadians, x, y, z)
    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t)                      _RET_VARIANT_3(Slerp, a, b, t)

#undef _RET_VARIANT
#undef _RET_VARIANT_END
#undef _RET_VARIANT_0
#undef _RET_VARIANT_1
#undef _RET_VARIANT_2
#undef _RET_VARIANT_3
#undef _RET_VARIANT_4
#undef _THIS_VARIANT1
#undef _THIS_VARIANT2

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


XOMATH_BEGIN_XO_NS();

float& Vector2::operator [](int i) { return f[i]; }
const float& Vector2::operator [](int i) const { return f[i]; }

Vector2 Vector2::operator - () const { return Vector2(-x, -y); }
Vector2 Vector2::operator ~ () const { return Vector2(y, x); }

Vector2& Vector2::operator += (const Vector2& v) {
    x += v.x;
    y += v.y;
    return *this; 
}
Vector2& Vector2::operator += (float v) {
    x += v;
    y += v;
    return *this; 
}
Vector2& Vector2::operator += (double v)                  { return *this += (float)v; }
Vector2& Vector2::operator += (int v)                     { return *this += (float)v; }
Vector2& Vector2::operator += (const class Vector3& v)    { return *this += Vector2(v); }
Vector2& Vector2::operator += (const class Vector4& v)    { return *this += Vector2(v); }

Vector2& Vector2::operator -= (const Vector2& v) {
    x -= v.x;
    y -= v.y;
    return *this; 
}
Vector2& Vector2::operator -= (float v) {
    x -= v;
    y -= v;
    return *this; 
}
Vector2& Vector2::operator -= (double v)                  { return *this -= (float)v; }
Vector2& Vector2::operator -= (int v)                     { return *this -= (float)v; }
Vector2& Vector2::operator -= (const class Vector3& v)    { return *this -= Vector2(v); }
Vector2& Vector2::operator -= (const class Vector4& v)    { return *this -= Vector2(v); }

Vector2& Vector2::operator *= (const Vector2& v) {
    x *= v.x;
    y *= v.y;
    return *this; 
}
Vector2& Vector2::operator *= (float v) {
    x *= v;
    y *= v;
    return *this;
}
Vector2& Vector2::operator *= (double v)                  { return *this *= (float)v; }
Vector2& Vector2::operator *= (int v)                     { return *this *= (float)v; }
Vector2& Vector2::operator *= (const class Vector3& v)    { return *this *= Vector2(v); }
Vector2& Vector2::operator *= (const class Vector4& v)    { return *this *= Vector2(v); }

Vector2& Vector2::operator /= (const Vector2& v) {
    x /= v.x;
    y /= v.y;
    return *this;
}
Vector2& Vector2::operator /= (float v) {
#if defined(XO_NO_INVERSE_DIVISION)
    x /= v;
    y /= v;
    return *this;
#else
    return (*this) *= (1.0f/v);
#endif
    
}
Vector2& Vector2::operator /= (double v)                  { return *this /= (float)v; }
Vector2& Vector2::operator /= (int v)                     { return *this /= (float)v; }
Vector2& Vector2::operator /= (const class Vector3& v)    { return *this /= Vector2(v); }
Vector2& Vector2::operator /= (const class Vector4& v)    { return *this /= Vector2(v); }

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

_XOINL 
Vector2 Abs(const Vector2& v)
{
    return Vector2(Abs(v.x), Abs(v.y));
}

XOMATH_END_XO_NS();

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

Vector3& Vector3::operator += (const Vector3& v) {
#if defined(XO_SSE)
    m = _mm_add_ps(m, v);
#else
    x += v.x;
    y += v.y;
    z += v.z;
#endif
    return *this;
}

Vector3& Vector3::operator += (float v) {
#if defined(XO_SSE)
    m = _mm_add_ps(m, _mm_set_ps1(v));
#else
    x += v;
    y += v;
    z += v;
#endif
    return *this;
}

Vector3& Vector3::operator += (double v)                  { return (*this) += float(v); }
Vector3& Vector3::operator += (int v)                     { return (*this) += float(v); }
Vector3& Vector3::operator += (const class Vector2& v)    { return (*this) += Vector3(v); }
Vector3& Vector3::operator += (const class Vector4& v)    { return (*this) += Vector3(v); }

Vector3& Vector3::operator -= (const Vector3& v) {
#if defined(XO_SSE)
    m = _mm_sub_ps(m, v);
#else
    x -= v.x;
    y -= v.y;
    z -= v.z;
#endif
    return *this;
}

Vector3& Vector3::operator -= (float v) {
#if defined(XO_SSE)
    m = _mm_sub_ps(m, _mm_set_ps1(v));
#else
    x -= v;
    y -= v;
    z -= v;
#endif
    return *this;
}

Vector3& Vector3::operator -= (double v)                  { return (*this) -= float(v); }
Vector3& Vector3::operator -= (int v)                     { return (*this) -= float(v); }
Vector3& Vector3::operator -= (const class Vector2& v)    { return (*this) -= Vector3(v); }
Vector3& Vector3::operator -= (const class Vector4& v)    { return (*this) -= Vector3(v); }

Vector3& Vector3::operator *= (const Vector3& v) {
#if defined(XO_SSE)
    m = _mm_mul_ps(m, v);
#else
    x *= v.x;
    y *= v.y;
    z *= v.z;
#endif
    return *this;
}

Vector3& Vector3::operator *= (float v) {
#if defined(XO_SSE)
    m = _mm_mul_ps(m, _mm_set_ps1(v));
#else
    x *= v;
    y *= v;
    z *= v;
#endif
    return *this;
}

Vector3& Vector3::operator *= (double v)                  { return (*this) *= float(v); }
Vector3& Vector3::operator *= (int v)                     { return (*this) *= float(v); }
Vector3& Vector3::operator *= (const class Vector2& v)    { return (*this) *= Vector3(v); }
Vector3& Vector3::operator *= (const class Vector4& v)    { return (*this) *= Vector3(v); }

#if defined(XO_NO_INVERSE_DIVISION)
Vector3& Vector3::operator /= (const Vector3& v) {
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

Vector3& Vector3::operator /= (float v) {
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

Vector3& Vector3::operator /= (const Vector3& v) {
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

Vector3& Vector3::operator /= (float v) { 
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
Vector3& Vector3::operator /= (double v)                  { return (*this) /= float(v); }
Vector3& Vector3::operator /= (int v)                     { return (*this) /= float(v); }
Vector3& Vector3::operator /= (const class Vector2& v)    { return (*this) /= Vector3(v); }
Vector3& Vector3::operator /= (const class Vector4& v)    { return (*this) /= Vector3(v); }

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

_XOINL
Vector3 Abs(const Vector3& v)
{
#if defined(XO_SSE)
    return (sse::Abs(v.m));
#else
    return Vector3(Abs(v.x), Abs(v.y), Abs(v.z));
#endif
}

XOMATH_END_XO_NS();

XOMATH_BEGIN_XO_NS();

#if defined(XO_SSE)

#if !defined(IDX_X)
#   define IDX_X 0
#endif

#if !defined(IDX_Y)
#   define IDX_Y 1
#endif

#if !defined(IDX_Z)
#   define IDX_Z 2
#endif

#if !defined(IDX_W)
#   define IDX_W 3
#endif

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

Vector4& Vector4::operator += (const Vector4& v) {
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

Vector4& Vector4::operator += (float v) {
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

Vector4& Vector4::operator += (double v)          { return (*this) += (float)v; }
Vector4& Vector4::operator += (int v)             { return (*this) += (float)v; }
Vector4& Vector4::operator += (const class Vector2& v)  { return (*this) += Vector4(v); }
Vector4& Vector4::operator += (const class Vector3& v)  { return (*this) += Vector4(v); }

Vector4& Vector4::operator -= (const Vector4& v) {
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

Vector4& Vector4::operator -= (float v) {
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

Vector4& Vector4::operator -= (double v)          { return (*this) -= (float)v; }
Vector4& Vector4::operator -= (int v)             { return (*this) -= (float)v; }
Vector4& Vector4::operator -= (const class Vector2& v)  { return (*this) -= Vector4(v); }
Vector4& Vector4::operator -= (const class Vector3& v)  { return (*this) -= Vector4(v); }

Vector4& Vector4::operator *= (const Vector4& v) {
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

Vector4& Vector4::operator *= (float v) {
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

Vector4& Vector4::operator *= (double v)          { return (*this) *= (float)v; }
Vector4& Vector4::operator *= (int v)             { return (*this) *= (float)v; }
Vector4& Vector4::operator *= (const class Vector2& v)  { return (*this) *= Vector4(v); }
Vector4& Vector4::operator *= (const class Vector3& v)  { return (*this) *= Vector4(v); }

#if defined(XO_NO_INVERSE_DIVISION)
Vector4& Vector4::operator /= (const Vector4& v) {
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

Vector4& Vector4::operator /= (float v) {
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

Vector4& Vector4::operator /= (const Vector4& v) {
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

Vector4& Vector4::operator /= (float v) { 
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
Vector4& Vector4::operator /= (double v)          { return (*this) /= float(v); }
Vector4& Vector4::operator /= (int v)             { return (*this) /= float(v); }
Vector4& Vector4::operator /= (const class Vector2& v)  { return (*this) /= Vector4(v); }
Vector4& Vector4::operator /= (const class Vector3& v)  { return (*this) /= Vector4(v); }

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

_XOINL 
Vector4 Abs(const Vector4& v)
{
#if defined(XO_SSE)
    return (sse::Abs(v.m));
#else
    return Vector4(Abs(v.x), Abs(v.y), Abs(v.z));
#endif
}

#undef IDX_X
#undef IDX_Y
#undef IDX_Z
#undef IDX_W

XOMATH_END_XO_NS();

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

Matrix4x4& Matrix4x4::operator += (const Matrix4x4& m) {
    r[0] += m[0];
    r[1] += m[1];
    r[2] += m[2];
    r[3] += m[3];
    return *this;
}

Matrix4x4& Matrix4x4::operator -= (const Matrix4x4& m) {
    r[0] -= m[0];
    r[1] -= m[1];
    r[2] -= m[2];
    r[3] -= m[3];
    return *this;
}

Matrix4x4& Matrix4x4::operator *= (const Matrix4x4& m) {
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

XOMATH_BEGIN_XO_NS();

float& Quaternion::operator [](int i) { 
  return f[i]; 
}

const float& Quaternion::operator [](int i) const { 
  return f[i]; 
}

Quaternion& Quaternion::operator *= (const Quaternion& q) {
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


XOMATH_BEGIN_XO_NS();

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

