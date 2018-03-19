/*
      :::    :::  ::::::::                  :::   :::       ::: ::::::::::: :::    ::: 
     :+:    :+: :+:    :+:                :+:+: :+:+:    :+: :+:   :+:     :+:    :+:  
     +:+  +:+  +:+    +:+               +:+ +:+:+ +:+  +:+   +:+  +:+     +:+    +:+   
     +#++:+   +#+    +:+ +#++:++#++:++ +#+  +:+  +#+ +#++:++#++: +#+     +#++:++#++    
   +#+  +#+  +#+    +#+               +#+       +#+ +#+     +#+ +#+     +#+    +#+     
 #+#    #+# #+#    #+#               #+#       #+# #+#     #+# #+#     #+#    #+#      
###    ###  ########                ###       ### ###     ### ###     ###    ###       

                                        v. alpha



               Fast math for game developers. No headaches, no cruft.



!! Warnings
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    - xo-math is in alpha. Contribute, test, but don't integrate yet.
    - Documentation does not exist yet.
    - Source code is untested.

// Contributing
//////////////////////////////////////////////////////////////////////////////////////////
    - There is no contributing guide yet.
    - Basically: keep it simple.
    - Repo: https://github.com/xoorath/xo-math

?? About
??????????????????????????????????????????????????????????????????????????????????????????
    xo-math is being developed by Jared Thomson (better known as Xoorath) in an effort to
    make games a little faster and a little easier to make. Twitter: @xoorath
    
    xo-math is pronounced non-phonetically "ex oh math", and spelled without 
    capitalization where fonts permit.

// License
//////////////////////////////////////////////////////////////////////////////////////////
    See also: third-party-licenses.h

    Copyright 2018 Jared Thomson

    Permission is hereby granted, free of charge, to any person obtaining a copy of this
    software and associated documentation files (the "Software"), to deal in the Software
    without restriction, including without limitation the rights to use, copy, modify,
    merge, publish, distribute, sub license, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be included in all copies
    or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
    CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once
#include <inttypes.h>
////////////////////////////////////////////////////////////////////////////////////////// xo-math-macros.h inlined
// xo-math calling convention.
#if defined(_MSC_VER) && !defined(_M_ARM) && (((_MSC_FULL_VER >= 170065501) && (_MSC_VER < 1800)) || (_MSC_FULL_VER >= 180020418)) // See Licence in third-party-licences for https://github.com/Microsoft/DirectXMath 
#   define XO_CC __vectorcall
#else
#   define XO_CC
#endif

#define XO_INL inline
////////////////////////////////////////////////////////////////////////////////////////// end xo-math-macros.h inline
////////////////////////////////////////////////////////////////////////////////////////// xo-math-detect-simd.h inlined
namespace xo { namespace simd {

#define XO_SSE_NONE 0x00
#define XO_SSE1     0x10
#define XO_SSE2     0x20
#define XO_SSE3     0x30
#define XO_SSSE3    0x31
#define XO_SSE4_1   0x41
#define XO_SSE4_2   0x42
#define XO_AVX      0x50
#define XO_AVX2     0x60
#define XO_AVX512   0x70

#if defined(_MSC_VER)
#   if defined(__AVX512__) // this define is a guess.
#       define XO_SSE_CURRENT XO_AVX512
#   elif defined(__AVX2__)
#       define XO_SSE_CURRENT XO_AVX2
#   elif defined(__AVX__)
#       define XO_SSE_CURRENT XO_AVX
#   elif defined(_M_IX86_FP)
#       if _M_IX86_FP == 1
#           define XO_SSE_CURRENT XO_SSE1
#       elif _M_IX86_FP == 2
#           define XO_SSE_CURRENT XO_SSE2
#       else
#           define XO_SSE_CURRENT XO_SSE1
#       endif
#   else
#       define XO_SSE_CURRENT XO_SSE_NONE
#       define XO_HAS_SSE 0
#   endif
#elif defined(__clang__) || defined (__GNUC__)
#   if defined(__AVX512__) || defined(__AVX512F__)
#       define XO_SSE_CURRENT XO_AVX512
#   elif defined(__AVX2__)
#       define XO_SSE_CURRENT XO_AVX2
#   elif defined(__AVX__)
#       define XO_SSE_CURRENT XO_AVX
#   elif defined(__SSE4_2__)
#       define XO_SSE_CURRENT XO_SSE4_2
#   elif defined(__SSE4_1__)
#       define XO_SSE_CURRENT XO_SSE4_1
#   elif defined(__SSSE3__)
#       define XO_SSE_CURRENT XO_SSSE3
#   elif defined(__SSE3__)
#       define XO_SSE_CURRENT XO_SSE3
#   elif defined(__SSE2__)
#       define XO_SSE_CURRENT XO_SSE2
#   elif defined(__SSE__)
#       define XO_SSE_CURRENT XO_SSE1
#else
#       define XO_SSE_CURRENT XO_SSE_NONE
#       define XO_HAS_SSE 0
#   endif
#else
#   define XO_SSE_CURRENT XO_SSE_NONE
#   define XO_HAS_SSE 0
#endif

#if !defined(XO_HAS_SSE)
#    define XO_HAS_SSE 1
#endif

enum class eXO_SSE : uint8_t
{
    eXO_SSE_NONE    = XO_SSE_NONE,
    eXO_SSE1        = XO_SSE1,
    eXO_SSE2        = XO_SSE2,
    eXO_SSE3        = XO_SSE3,
    eXO_SSSE3       = XO_SSSE3,
    eXO_SSE4_1      = XO_SSE4_1,
    eXO_SSE4_2      = XO_SSE4_2,
    eXO_AVX         = XO_AVX,
    eXO_AVX2        = XO_AVX2,
    eXO_AVX512      = XO_AVX512,
    eXO_SSE_CURRENT = XO_SSE_CURRENT
};

constexpr char const* SSEGetName(eXO_SSE version = eXO_SSE::eXO_SSE_CURRENT)
{
    switch (version)
    {
    case eXO_SSE::eXO_SSE_NONE: return "none";
    case eXO_SSE::eXO_SSE1:     return "SSE1";
    case eXO_SSE::eXO_SSE2:     return "SSE2";
    case eXO_SSE::eXO_SSE3:     return "SSE3";
    case eXO_SSE::eXO_SSSE3:    return "SSSE3";
    case eXO_SSE::eXO_SSE4_1:   return "SSE4.1";
    case eXO_SSE::eXO_SSE4_2:   return "SSE4.2";
    case eXO_SSE::eXO_AVX:      return "AVX";
    case eXO_SSE::eXO_AVX2:     return "AVX2";
    case eXO_SSE::eXO_AVX512:   return "AVX512";
    default:                    return "unknown";
    }
}

constexpr char const* SSEVersionName = SSEGetName();

#define XO_NEON_NONE 0x00
#define XO_NEON7     0x70

#if XO_HAS_SSE == 0
#   if defined(_MSC_VER)
#       if defined(_M_ARM)
#           define XO_NEON_CURRENT XO_NEON7
#       else
#           define XO_NEON_CURRENT XO_NEON_NONE
#           define XO_HAS_NEON 0
#       endif
#   elif defined(__clang__) || defined (__GNUC__)
#       if defined(__arm__) && defined(__ARM_NEON__)
#           define XO_NEON_CURRENT XO_NEON7
#       else
#           define XO_NEON_CURRENT XO_NEON_NONE
#           define XO_HAS_NEON 0
#       endif
#   else
#       define XO_NEON_CURRENT XO_NEON_NONE
#       define XO_HAS_NEON 0
#   endif
#else
#   define XO_NEON_CURRENT XO_NEON_NONE
#   define XO_HAS_NEON 0
#endif

#if !defined(XO_HAS_NEON)
#    define XO_HAS_NEON 1
#endif

enum class eXO_NEON : uint8_t
{
    eXO_NEON_NONE    = XO_NEON_NONE,
    eXO_NEON7        = XO_NEON7,
    eXO_NEON_CURRENT = XO_NEON_CURRENT
};

constexpr char const* NEONGetName(eXO_NEON version = eXO_NEON::eXO_NEON_CURRENT)
{
    switch (version)
    {
    case eXO_NEON::eXO_NEON_NONE: return "none";
    case eXO_NEON::eXO_NEON7:     return "v7";
    default:                      return "unknown";
    }
}

constexpr char const* NEONVersionName = NEONGetName();

} } // ::xo::simd
////////////////////////////////////////////////////////////////////////////////////////// end xo-math-detect-simd.h inline

#if XO_SSE_CURRENT >= XO_SSE4_1
////////////////////////////////////////////////////////////////////////////////////////// xo-math-sse4.h inlined
namespace xo {
struct Vector3 {
    float x, y, z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
    explicit Vector3(float all) : x(all), y(all), z(all) { }
    Vector3(Vector3 const& other) : x(other.x), y(other.y), z(other.z) { }
};

struct Vector4 {
    float x, y, z, w;
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
    explicit Vector4(float all) : x(all), y(all), z(all), w(all) { }
    Vector4(Vector4 const& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }
};

struct Matrix4x4 {
    Vector4 rows[4];
    Matrix4x4(Vector4 rows[4]) : rows{ rows[0], rows[1], rows[2], rows[3]} { }
    Matrix4x4(Matrix4x4 const& other) : rows{ other.rows[0], other.rows[1], other.rows[2], other.rows[3] } { }
};

struct Quaternion
{
    float i, j, k, r;
    Quaternion(float i, float j, float k, float r) : i(i), j(j), k(k), r(r) { }
    Quaternion(Quaternion const& other) : i(other.i), j(other.j), k(other.k), r(other.r) { }
};

// vector 3, aligned for cpu specific optimizations (where applicable) 
struct AVector3 {
    float x, y, z;
    AVector3(float x, float y, float z) : x(x), y(y), z(z) { }
    explicit AVector3(float all) : x(all), y(all), z(all) { }
    AVector3(AVector3 const& other) : x(other.x), y(other.y), z(other.z) { }
    AVector3(Vector3 const& other) : x(other.x), y(other.y), z(other.z) { }
};

// vector 4, aligned for cpu specific optimizations (where applicable)
struct AVector4 {
    float x, y, z, w;
    AVector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
    explicit AVector4(float all) : x(all), y(all), z(all), w(all) { }
    AVector4(AVector4 const& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }
    AVector4(Vector4 const& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }
};

// matrix4x4, aligned for cpu specific optimizations (where applicable)
struct AMatrix4x4 {
    AVector4 rows[4];
    AMatrix4x4(AVector4 rows[4]) : rows{ rows[0], rows[1], rows[2], rows[3]} { }
    AMatrix4x4(AMatrix4x4 const& other) : rows{ other.rows[0], other.rows[1], other.rows[2], other.rows[3] } { }
    AMatrix4x4(Matrix4x4 const& other) : rows{ AVector4(other.rows[0]), AVector4(other.rows[1]), AVector4(other.rows[2]), AVector4(other.rows[3]) } { }
};

// quaternion, aligned for cpu specific optimizations (where applicable)
struct AQuaternion {
    float i, j, k, r;
    AQuaternion(float i, float j, float k, float r) : i(i), j(j), k(k), r(r) {}
    AQuaternion(AQuaternion const& other) : i(other.i), j(other.j), k(other.k), r(other.r) {}
    AQuaternion(Quaternion const& other) : i(other.i), j(other.j), k(other.k), r(other.r) {}
};
} // ::xo
////////////////////////////////////////////////////////////////////////////////////////// end xo-math-sse4.h inline
#else
////////////////////////////////////////////////////////////////////////////////////////// xo-math-reference.h inlined
namespace xo {
struct Vector3 {
    float x, y, z;

    constexpr Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
    constexpr explicit Vector3(float all) : x(all), y(all), z(all) { }

    Vector3()                                  = default;
    ~Vector3()                                 = default;
    Vector3(Vector3 const& other)              = default;
    Vector3(Vector3&& ref)                     = default;
    Vector3& operator = (Vector3 const& other) = default;
    Vector3& operator = (Vector3&& ref)        = default;

    Vector3 XO_CC operator + (const Vector3& other) const;
    Vector3 XO_CC operator - (const Vector3& other) const;
    Vector3 XO_CC operator * (const Vector3& other) const;
    Vector3 XO_CC operator / (const Vector3& other) const;
    Vector3& XO_CC operator += (const Vector3& other);
    Vector3& XO_CC operator -= (const Vector3& other);
    Vector3& XO_CC operator *= (const Vector3& other);
    Vector3& XO_CC operator /= (const Vector3& other);

    Vector3 operator -() const;

    float Sum() const;

    float Magnitude() const;
    float MagnitudeSquared() const;

    static bool XO_CC RoughlyEqual(const Vector3& left, const Vector3& right);
    static bool XO_CC ExactlyEqual(const Vector3& left, const Vector3& right);
    static bool XO_CC RoughlyEqual(const Vector3& left, float magnitude);
    static bool XO_CC ExactlyEqual(const Vector3& left, float magnitude);

    static float XO_CC DotProduct(const Vector3& left, const Vector3& right);
    static Vector3 XO_CC CrossProduct(const Vector3& left, const Vector3& right);

    static const Vector3 Zero;
    static const Vector3 One;
    static const Vector3 Up;
    static const Vector3 Down;
    static const Vector3 Left;
    static const Vector3 Right;
    static const Vector3 Forward;
    static const Vector3 Backward;
};

struct Vector4 {
    float x, y, z, w;
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
    explicit Vector4(float all) : x(all), y(all), z(all), w(all) { }
    Vector4(Vector4 const& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }
};

struct Matrix4x4 {
    Vector4 rows[4];
    Matrix4x4(Vector4 rows[4]) : rows{ rows[0], rows[1], rows[2], rows[3]} { }
    Matrix4x4(Matrix4x4 const& other) : rows{ other.rows[0], other.rows[1], other.rows[2], other.rows[3] } { }
};

struct Quaternion
{
    float i, j, k, r;
    Quaternion(float i, float j, float k, float r) : i(i), j(j), k(k), r(r) { }
    Quaternion(Quaternion const& other) : i(other.i), j(other.j), k(other.k), r(other.r) { }
};

// vector 3, aligned for cpu specific optimizations (where applicable) 
struct AVector3 {
    float x, y, z;
    AVector3(float x, float y, float z) : x(x), y(y), z(z) { }
    explicit AVector3(float all) : x(all), y(all), z(all) { }
    AVector3(AVector3 const& other) : x(other.x), y(other.y), z(other.z) { }
    AVector3(Vector3 const& other) : x(other.x), y(other.y), z(other.z) { }
};

// vector 4, aligned for cpu specific optimizations (where applicable)
struct AVector4 {
    float x, y, z, w;
    AVector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
    explicit AVector4(float all) : x(all), y(all), z(all), w(all) { }
    AVector4(AVector4 const& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }
    AVector4(Vector4 const& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }
};

// matrix4x4, aligned for cpu specific optimizations (where applicable)
struct AMatrix4x4 {
    AVector4 rows[4];
    AMatrix4x4(AVector4 rows[4]) : rows{ rows[0], rows[1], rows[2], rows[3]} { }
    AMatrix4x4(AMatrix4x4 const& other) : rows{ other.rows[0], other.rows[1], other.rows[2], other.rows[3] } { }
    AMatrix4x4(Matrix4x4 const& other) : rows{ AVector4(other.rows[0]), AVector4(other.rows[1]), AVector4(other.rows[2]), AVector4(other.rows[3]) } { }
};

// quaternion, aligned for cpu specific optimizations (where applicable)
struct AQuaternion {
    float i, j, k, r;
    AQuaternion(float i, float j, float k, float r) : i(i), j(j), k(k), r(r) {}
    AQuaternion(AQuaternion const& other) : i(other.i), j(other.j), k(other.k), r(other.r) {}
    AQuaternion(Quaternion const& other) : i(other.i), j(other.j), k(other.k), r(other.r) {}
};

////////////////////////////////////////////////////////////////////////////////////////// Vector 3
#if defined(XO_MATH_IMPL)
/*static*/ const Vector3 Vector3::Zero(0.f);
/*static*/ const Vector3 Vector3::One(1.f);
/*static*/ const Vector3 Vector3::Up(0.f, 1.f, 0.f);
/*static*/ const Vector3 Vector3::Down(0.f, -1.f, 0.f);
/*static*/ const Vector3 Vector3::Left(-1.f, 0.f, 0.f);
/*static*/ const Vector3 Vector3::Right(1.f, 0.f, 0.f);
/*static*/ const Vector3 Vector3::Forward(0.f, 0.f, 1.f);
/*static*/ const Vector3 Vector3::Backward(0.f, 0.f, -1.f);
#endif

XO_INL Vector3 XO_CC Vector3::operator + (const Vector3& other) const {  return Vector3(x + other.x, y + other.y, z + other.z); }
XO_INL Vector3 XO_CC Vector3::operator - (const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
XO_INL Vector3 XO_CC Vector3::operator * (const Vector3& other) const { return Vector3(x * other.x, y * other.y, z * other.z); }
XO_INL Vector3 XO_CC Vector3::operator / (const Vector3& other) const { return Vector3(x / other.x, y / other.y, z / other.z); }
XO_INL Vector3& XO_CC Vector3::operator += (const Vector3& other)     { x += other.x; y += other.y; z *= other.z; return *this; }
XO_INL Vector3& XO_CC Vector3::operator -= (const Vector3& other)     { x -= other.x; y -= other.y; z -= other.z; return *this; }
XO_INL Vector3& XO_CC Vector3::operator *= (const Vector3& other)     { x *= other.x; y *= other.y; z *= other.z; return *this; }
XO_INL Vector3& XO_CC Vector3::operator /= (const Vector3& other)     { x /= other.x; y /= other.y; z /= other.z; return *this; }

XO_INL Vector3 Vector3::operator -() const { return Vector3(-x, -y, -z); };

XO_INL float Vector3::Sum() const { return x + y + z; }

XO_INL float Vector3::MagnitudeSquared() const { return ((*this) * (*this)).Sum(); }
XO_INL float Vector3::Magnitude() const { return Sqrt(MagnitudeSquared()); }

/*static*/ XO_INL 
bool XO_CC Vector3::RoughlyEqual(const Vector3& left, const Vector3& right) { 
    return CloseEnough(left.x, right.x) 
        && CloseEnough(left.y, right.y) 
        && CloseEnough(left.z, right.z);
}

/*static*/ XO_INL
bool XO_CC Vector3::ExactlyEqual(const Vector3& left, const Vector3& right) { 
    return left.x == right.x
        && left.y == right.y
        && left.z == right.z;
}
/*static*/ XO_INL 
bool XO_CC Vector3::RoughlyEqual(const Vector3& left, float magnitude) {
    return CloseEnough(left.MagnitudeSquared(), Pow(magnitude, 2));
}

/*static*/ XO_INL 
bool XO_CC Vector3::ExactlyEqual(const Vector3& left, float magnitude) {
    return left.MagnitudeSquared() == Pow(magnitude, 2);
}

/*static*/ XO_INL
float XO_CC Vector3::DotProduct(const Vector3& left, const Vector3& right) {
    return (left * right).Sum();
}

/*static*/ XO_INL 
Vector3 XO_CC Vector3::CrossProduct(const Vector3& left, const Vector3& right) {
    return Vector3(
        left.y * right.z - left.z * right.y,
        left.z * right.x - left.x * right.z,
        left.x * right.y - left.y * right.x);
}

} // ::xo
////////////////////////////////////////////////////////////////////////////////////////// end xo-math-reference.h inline
#endif

////////////////////////////////////////////////////////////////////////////////////////// third-party-licenses.h inlined
/*****************************************************************************************
Licence for: https://github.com/Microsoft/DirectXMath
This licence only applies to parts of xo-math where explicitly indicated

                               The MIT License (MIT)

Copyright (c) 2018 Microsoft Corp

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, 
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
permit persons to whom the Software is furnished to do so, subject to the following 
conditions: 

The above copyright notice and this permission notice shall be included in all copies 
or substantial portions of the Software.  

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*****************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////// end third-party-licenses.h inline