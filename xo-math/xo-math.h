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

// About
//////////////////////////////////////////////////////////////////////////////////////////
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
// See License in third-party-licenses.h for https://github.com/Microsoft/DirectXMath
// applies to the exact version checking for __vectorcall
#include <new.h>

#if defined(_MSC_VER) && !defined(_M_ARM) && \
(((_MSC_FULL_VER >= 170065501) && (_MSC_VER < 1800)) || (_MSC_FULL_VER >= 180020418))
#   define XO_CC __vectorcall
#else
#   define XO_CC
#endif

#define XO_INL inline

#if defined(_MSC_VER)
#define XO_ALN_16               __declspec(align(16))
#define XO_ALN_16_MALLOC(size)  _aligned_malloc(size, 16)
#define XO_ALN_16_FREE(mem)     _aligned_free(mem)
#endif
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

// As an end user you can configure these values
#define XO_CONFIG_LEFT_HANDED 1
#define XO_CONFIG_Y_UP 1
#define XO_CONFIG_DEFAULT_NEAR_PLANE 0.1f
#define XO_CONFIG_DEFAULT_FAR_PLANE 1000.f

// These configs can set themselves up based on the other configs above...
#define XO_CONFIG_RIGHT_HANDED (XO_CONFIG_LEFT_HANDED == 0 ? 1 : 0)
#define XO_CONFIG_Z_UP (XO_CONFIG_Y_UP == 0 ? 1 : 0)

#if XO_SSE_CURRENT >= XO_SSE4_1
////////////////////////////////////////////////////////////////////////////////////////// xo-math-sse4.h inlined
namespace xo {

} // ::xo
////////////////////////////////////////////////////////////////////////////////////////// end xo-math-sse4.h inline
#else
////////////////////////////////////////////////////////////////////////////////////////// xo-math-reference.h inlined
#if !defined(XO_CONFIG_DEFAULT_NEAR_PLANE)
#   define XO_CONFIG_DEFAULT_NEAR_PLANE 0.1f
#endif
#if !defined(XO_CONFIG_DEFAULT_FAR_PLANE)
#   define XO_CONFIG_DEFAULT_FAR_PLANE 1000.f
#endif

#define XO_REF_ALN               XO_ALN_16
#define XO_REF_ALN_MALLOC(size)  XO_ALN_16_MALLOC(size)
#define XO_REF_ALN_FREE(mem)     XO_ALN_16_FREE(mem)

namespace xo {
//////////////////////////////////////////////////////////////////////////////////////////
struct Vector3 {
    float x, y, z;
    constexpr Vector3(float x, float y, float z) 
        : x(x)
        , y(y)
        , z(z)
    { }

    constexpr explicit Vector3(float all)
        : x(all)
        , y(all)
        , z(all) 
    { }

    Vector3() = default;
    ~Vector3() = default;
    Vector3(Vector3 const& other) = default;
    Vector3(Vector3&& ref) = default;
    Vector3& operator = (Vector3 const& other) = default;
    Vector3& operator = (Vector3&& ref) = default;

    Vector3 XO_CC operator + (Vector3 const& other) const;
    Vector3 XO_CC operator - (Vector3 const& other) const;
    Vector3 XO_CC operator * (Vector3 const& other) const;
    Vector3 XO_CC operator / (Vector3 const& other) const;
    Vector3& XO_CC operator += (Vector3 const& other);
    Vector3& XO_CC operator -= (Vector3 const& other);
    Vector3& XO_CC operator *= (Vector3 const& other);
    Vector3& XO_CC operator /= (Vector3 const& other);

    Vector3 operator -() const;

    float Sum() const;

    float Magnitude() const;
    float MagnitudeSquared() const;

    Vector3& Normalize();
    Vector3 Normalized() const;

    static bool XO_CC RoughlyEqual(Vector3 const& left, Vector3 const& right);
    static bool XO_CC ExactlyEqual(Vector3 const& left, Vector3 const& right);
    static bool XO_CC RoughlyEqual(Vector3 const& left, float magnitude);
    static bool XO_CC ExactlyEqual(Vector3 const& left, float magnitude);

    static float XO_CC DotProduct(Vector3 const& left, Vector3 const& right);
    static Vector3 XO_CC CrossProduct(Vector3 const& left, Vector3 const& right);
    static Vector3 XO_CC Lerp(Vector3 const& left, Vector3 const& right, float t);
    static float XO_CC DistanceSquared(Vector3 const& left, Vector3 const& right);
    static float XO_CC Distance(Vector3 const& left, Vector3 const& right);

    static const Vector3 Zero;
    static const Vector3 One;
    static const Vector3 Up;
    static const Vector3 Down;
    static const Vector3 Left;
    static const Vector3 Right;
    static const Vector3 Forward;
    static const Vector3 Backward;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct Vector4 {
    union {
        struct { float x, y, z, w; };
        float v[4];
    };
    constexpr Vector4(float x, float y, float z, float w)
        : x(x)
        , y(y)
        , z(z)
        , w(w)
    { }

    constexpr explicit Vector4(float all)
        : x(all)
        , y(all)
        , z(all)
        , w(all)
    { }

    Vector4() = default;
    ~Vector4() = default;
    Vector4(Vector4 const& other) = default;
    Vector4(Vector4&& ref) = default;
    Vector4& operator = (Vector4 const& other) = default;
    Vector4& operator = (Vector4&& ref) = default;

    Vector4 XO_CC operator + (Vector4 const& other) const;
    Vector4 XO_CC operator - (Vector4 const& other) const;
    Vector4 XO_CC operator * (Vector4 const& other) const;
    Vector4 XO_CC operator / (Vector4 const& other) const;
    Vector4& XO_CC operator += (Vector4 const& other);
    Vector4& XO_CC operator -= (Vector4 const& other);
    Vector4& XO_CC operator *= (Vector4 const& other);
    Vector4& XO_CC operator /= (Vector4 const& other);

    Vector4 operator -() const;

    float operator[] (int index) const;
    float& operator[] (int index);

    float Sum() const;

    float Magnitude() const;
    float MagnitudeSquared() const;
    Vector4 Normalized() const;
    Vector4& Normalize();

    static bool XO_CC RoughlyEqual(Vector4 const& left, Vector4 const& right);
    static bool XO_CC ExactlyEqual(Vector4 const& left, Vector4 const& right);
    static bool XO_CC RoughlyEqual(Vector4 const& left, float magnitude);
    static bool XO_CC ExactlyEqual(Vector4 const& left, float magnitude);

    static float XO_CC DotProduct(Vector4 const& left, Vector4 const& right);
    static Vector4 XO_CC Lerp(Vector4 const& left, Vector4 const& right, float t);

    static const Vector4 Zero;
    static const Vector4 One;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct Matrix4x4 {
    union {
        Vector4 rows[4];
        float v[16];
    };
    constexpr Matrix4x4(Vector4 const rows[4])
        : rows {
            rows[0],
            rows[1],
            rows[2],
            rows[3] }
    { }

    constexpr Matrix4x4(Vector4 const& row0, 
                        Vector4 const& row1,
                        Vector4 const& row2,
                        Vector4 const& row3)
        : rows{
            row0,
            row1,
            row2,
            row3 }
    { }

    constexpr explicit Matrix4x4(float all)
        : rows{
            Vector4(all),
            Vector4(all),
            Vector4(all),
            Vector4(all) }
    { }

    Matrix4x4() = default;
    ~Matrix4x4() = default;
    Matrix4x4(Matrix4x4 const& other) = default;
    Matrix4x4(Matrix4x4&& ref) = default;
    Matrix4x4& operator = (Matrix4x4 const& other) = default;
    Matrix4x4& operator = (Matrix4x4&& ref) = default;

    Matrix4x4 XO_CC operator * (Matrix4x4 const& other) const;
    Matrix4x4& XO_CC operator *= (Matrix4x4 const& other);

    Vector4 operator[] (int index) const;
    Vector4& operator[] (int index);
    
    Vector3 Up() const;
    Vector3 Down() const;
    Vector3 Left() const;
    Vector3 Right() const;
    Vector3 Forward() const;
    Vector3 Backward() const;

    static Matrix4x4 XO_CC Transpose(Matrix4x4 const& matrixIn);
    static Matrix4x4 XO_CC Invert(Matrix4x4 const& matrixIn);
    static bool XO_CC InvertSafe(Matrix4x4 const& matrixIn, Matrix4x4& matrixOut);
    static Matrix4x4 XO_CC Translation(Vector3 const& pos);
    static Matrix4x4 XO_CC Scale(Vector3 const& scale);
    static Matrix4x4 XO_CC RotationYaw(float yaw);
    static Matrix4x4 XO_CC RotationPitch(float pitch);
    static Matrix4x4 XO_CC RotationRoll(float roll);
    static Matrix4x4 XO_CC RotationYawPitchRoll(float yaw, float pitch, float roll);
    static Matrix4x4 XO_CC RotationAxisAngle(Vector3 const& axis, float angle);
    static Matrix4x4 XO_CC PerspectiveFOV(float fov, 
                                          float aspect, 
                                          float nearPlane = XO_CONFIG_DEFAULT_NEAR_PLANE, 
                                          float farPlane = XO_CONFIG_DEFAULT_FAR_PLANE);
    static Matrix4x4 XO_CC Perspective(float width, 
                                       float height, 
                                       float aspect, 
                                       float nearPlane = XO_CONFIG_DEFAULT_NEAR_PLANE, 
                                       float farPlane = XO_CONFIG_DEFAULT_FAR_PLANE);
    static Matrix4x4 XO_CC Orthographic(float width,
                                        float height,
                                        float nearPlane,
                                        float farPlane);
    static Matrix4x4 XO_CC LookAt(Vector3 const& from, 
                                  Vector3 const& to, 
                                  Vector3 const& up = Vector3::Up);

    static bool XO_CC RoughlyEqual(Matrix4x4 const& left, Matrix4x4 const& right);
    static bool XO_CC ExactlyEqual(Matrix4x4 const& left, Matrix4x4 const& right);

    static const Matrix4x4 One;
    static const Matrix4x4 Zero;
    static const Matrix4x4 Identity;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct Quaternion {
    union {
        struct { float i, j, k, r; };
        Vector4 vec4;
    };

    constexpr Quaternion(float i, float j, float k, float r)
        : i(i)
        , j(j)
        , k(k)
        , r(r)
    { }

    constexpr explicit Quaternion(float all)
        : i(all)
        , j(all)
        , k(all)
        , r(all)
    { }

    constexpr explicit Quaternion(Vector4 const& v4)
        : vec4(v4)
    { }

    Quaternion() = default;
    ~Quaternion() = default;
    Quaternion(Quaternion const& other) = default;
    Quaternion(Quaternion&& ref) = default;
    Quaternion& operator = (Quaternion const& other) = default;
    Quaternion& operator = (Quaternion&& ref) = default;

    Quaternion operator + (Quaternion other) const;
    Quaternion operator * (float scalar) const;
    Quaternion operator -() const;

    float Magnitude() const;
    float MagnitudeSquared() const;
    Quaternion Normalized() const;
    Quaternion& Normalize();

    Matrix4x4 ToMatrix() const;

    static Quaternion XO_CC Invert(Quaternion const& quat);
    static Quaternion XO_CC RotationAxisAngle(Vector3 const& axis, float angle);
    static Quaternion XO_CC RotationYawPitchRoll(float yaw, float pitch, float roll);
    static float XO_CC DotProduct(Quaternion const& left, Quaternion const& right);
    static Quaternion XO_CC Lerp(Quaternion const& start, Quaternion const& end, float t);
    static Quaternion XO_CC Slerp(Quaternion const& start, 
                                  Quaternion const& end, 
                                  float t);

    static bool XO_CC RoughlyEqual(Quaternion const& left, Quaternion const& right);
    static bool XO_CC ExactlyEqual(Quaternion const& left, Quaternion const& right);

    static const Quaternion Zero;
    static const Quaternion Identity;
};

//////////////////////////////////////////////////////////////////////////////////////////
// vector 3, aligned for cpu specific optimizations (where applicable) 
using AVector3 = XO_REF_ALN Vector3;

//////////////////////////////////////////////////////////////////////////////////////////
// vector 4, aligned for cpu specific optimizations (where applicable)
using AVector4 = XO_REF_ALN Vector4;

//////////////////////////////////////////////////////////////////////////////////////////
// matrix4x4, aligned for cpu specific optimizations (where applicable)
using AMatrix4x4 = XO_REF_ALN Matrix4x4;

//////////////////////////////////////////////////////////////////////////////////////////
// quaternion, aligned for cpu specific optimizations (where applicable)
using AQuaternion = XO_REF_ALN Quaternion;

////////////////////////////////////////////////////////////////////////////////////////// Vector 3
#if defined(XO_MATH_IMPL)
/*static*/ const Vector3 Vector3::Zero(0.f);
/*static*/ const Vector3 Vector3::One(1.f);
/*static*/ const Vector3 Vector3::Left(-1.f, 0.f, 0.f);
/*static*/ const Vector3 Vector3::Right(1.f, 0.f, 0.f);

#   if !defined(XO_CONFIG_Y_UP) || !defined(XO_CONFIG_Z_UP)
    static_assert(false, 
        "define both XO_CONFIG_Y_UP and XO_CONFIG_Z_UP. One should have a value of 1, and\
 the other should have a value of 0");
#   endif

#   if !defined(XO_CONFIG_LEFT_HANDED) || !defined(XO_CONFIG_RIGHT_HANDED)
    static_assert(false, 
        "define both XO_CONFIG_LEFT_HANDED and XO_CONFIG_RIGHT_HANDED. One should have a \
value of 1, and the other should have a value of 0");
#   endif

#   if XO_CONFIG_Y_UP
    static_assert(XO_CONFIG_Z_UP == 0, 
        "XO_CONFIG_Z_UP should be 0 if XO_CONFIG_Y_UP is 1");
/*static*/ const Vector3 Vector3::Up(0.f, 1.f, 0.f);
/*static*/ const Vector3 Vector3::Down(0.f, -1.f, 0.f);
#       if XO_CONFIG_LEFT_HANDED
        static_assert(XO_CONFIG_RIGHT_HANDED == 0, 
            "XO_CONFIG_RIGHT_HANDED should be 0 if XO_CONFIG_LEFT_HANDED is 1");
/*static*/ const Vector3 Vector3::Forward(0.f, 0.f, 1.f);
/*static*/ const Vector3 Vector3::Backward(0.f, 0.f, -1.f);
#       elif XO_CONFIG_RIGHT_HANDED
        static_assert(XO_CONFIG_LEFT_HANDED == 0, 
            "XO_CONFIG_LEFT_HANDED should be 0 if XO_CONFIG_RIGHT_HANDED is 1");
/*static*/ const Vector3 Vector3::Forward(0.f, 0.f, -1.f);
/*static*/ const Vector3 Vector3::Backward(0.f, 0.f, 1.f);
#       else
        static_assert(false, 
            "XO_CONFIG_LEFT_HANDED or XO_CONFIG_RIGHT_HANDED should have a non zero \
value...");
#       endif
#   elif XO_CONFIG_Z_UP
// no static assert here about XO_CONFIG_Y_UP, because it's been checked.
/*static*/ const Vector3 Vector3::Up(0.f, 0.f, 1.f);
/*static*/ const Vector3 Vector3::Down(0.f, 0.f, -1.f);
#       if XO_CONFIG_LEFT_HANDED
        static_assert(XO_CONFIG_RIGHT_HANDED == 0, 
            "XO_CONFIG_RIGHT_HANDED should be 0 if XO_CONFIG_LEFT_HANDED is 1");
/*static*/ const Vector3 Vector3::Forward(0.f, -1.f, 0.f);
/*static*/ const Vector3 Vector3::Backward(0.f, 1.f, 0.f);
#       elif XO_CONFIG_RIGHT_HANDED
        static_assert(XO_CONFIG_LEFT_HANDED == 0, 
            "XO_CONFIG_LEFT_HANDED should be 0 if XO_CONFIG_RIGHT_HANDED is 1");
/*static*/ const Vector3 Vector3::Forward(0.f, 1.f, 0.f);
/*static*/ const Vector3 Vector3::Backward(0.f, -1.f, 0.f);
#       else
        static_assert(false,
            "XO_CONFIG_LEFT_HANDED or XO_CONFIG_RIGHT_HANDED should have a non zero \
value...");
#       endif
#   else
    static_assert(false,
        "XO_CONFIG_Y_UP or XO_CONFIG_Z_UP should have a non zero value...");
#   endif
#endif

XO_INL 
Vector3 XO_CC Vector3::operator + (Vector3 const& other) const { 
    return Vector3(x + other.x, y + other.y, z + other.z);
}

XO_INL
Vector3 XO_CC Vector3::operator - (Vector3 const& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
}

XO_INL
Vector3 XO_CC Vector3::operator * (Vector3 const& other) const {
    return Vector3(x * other.x, y * other.y, z * other.z);
}

XO_INL
Vector3 XO_CC Vector3::operator / (Vector3 const& other) const {
    return Vector3(x / other.x, y / other.y, z / other.z);
}

XO_INL
Vector3& XO_CC Vector3::operator += (Vector3 const& other) {
    x += other.x;
    y += other.y;
    z *= other.z;
    return *this;
}

XO_INL
Vector3& XO_CC Vector3::operator -= (Vector3 const& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

XO_INL
Vector3& XO_CC Vector3::operator *= (Vector3 const& other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
}

XO_INL
Vector3& XO_CC Vector3::operator /= (Vector3 const& other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
}

XO_INL Vector3 Vector3::operator -() const { return Vector3(-x, -y, -z); };

XO_INL float Vector3::Sum() const { return x + y + z; }

XO_INL float Vector3::MagnitudeSquared() const { return ((*this) * (*this)).Sum(); }
XO_INL float Vector3::Magnitude() const { return Sqrt(MagnitudeSquared()); }

XO_INL Vector3& Vector3::Normalize() { return (*this) /= Vector3(Magnitude()); }
XO_INL Vector3 Vector3::Normalized() const { return Vector3(*this).Normalize(); }

/*static*/ XO_INL
bool XO_CC Vector3::RoughlyEqual(Vector3 const& left, Vector3 const& right) {
    return CloseEnough(left.x, right.x)
        && CloseEnough(left.y, right.y)
        && CloseEnough(left.z, right.z);
}

/*static*/ XO_INL
bool XO_CC Vector3::ExactlyEqual(Vector3 const& left, Vector3 const& right) {
    return left.x == right.x
        && left.y == right.y
        && left.z == right.z;
}
/*static*/ XO_INL
bool XO_CC Vector3::RoughlyEqual(Vector3 const& left, float magnitude) {
    return CloseEnough(left.MagnitudeSquared(), Pow<2>(magnitude));
}

/*static*/ XO_INL
bool XO_CC Vector3::ExactlyEqual(Vector3 const& left, float magnitude) {
    return left.MagnitudeSquared() == Pow<2>(magnitude);
}

/*static*/ XO_INL
float XO_CC Vector3::DotProduct(Vector3 const& left, Vector3 const& right) {
    return (left * right).Sum();
}

/*static*/ XO_INL
Vector3 XO_CC Vector3::CrossProduct(Vector3 const& left, Vector3 const& right) {
    return Vector3(
        left.y * right.z - left.z * right.y,
        left.z * right.x - left.x * right.z,
        left.x * right.y - left.y * right.x);
}

/*static*/ XO_INL
Vector3 XO_CC Vector3::Lerp(Vector3 const& left, Vector3 const& right, float t) {
    return left + Vector3(t) * (right - left);
}

/*static*/ XO_INL
float XO_CC Vector3::DistanceSquared(Vector3 const& left, Vector3 const& right) {
    return (right - left).MagnitudeSquared();
}

/*static*/ XO_INL
float XO_CC Vector3::Distance(Vector3 const& left, Vector3 const& right) {
    return (right - left).Magnitude();
}

////////////////////////////////////////////////////////////////////////////////////////// Vector 4

XO_INL
Vector4 XO_CC Vector4::operator + (Vector4 const& other) const {
    return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

XO_INL
Vector4 XO_CC Vector4::operator - (Vector4 const& other) const {
    return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

XO_INL
Vector4 XO_CC Vector4::operator * (Vector4 const& other) const {
    return Vector4(x*other.x, y*other.y, z*other.z, w*other.w);
}

XO_INL
Vector4 XO_CC Vector4::operator / (Vector4 const& other) const {
    return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
}

XO_INL 
Vector4& XO_CC Vector4::operator += (Vector4 const& other) {
    x += other.x; 
    y += other.y; 
    z += other.z; 
    w += other.w; 
    return *this;
};

XO_INL 
Vector4& XO_CC Vector4::operator -= (Vector4 const& other) {
    x -=other.x;
    y -=other.y;
    z -= other.z;
    w -= other.w;
    return *this; };

XO_INL 
Vector4& XO_CC Vector4::operator *= (Vector4 const& other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    w *= other.w;
    return *this;
};

XO_INL 
Vector4& XO_CC Vector4::operator /= (Vector4 const& other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    w /= other.w;
    return *this;
};

XO_INL Vector4 Vector4::operator -() const { return Vector4(-x, -y, -z, -w); }

XO_INL float Vector4::operator[] (int index) const { return v[index]; }
XO_INL float& Vector4::operator[] (int index) { return v[index]; }

XO_INL float Vector4::Sum() const { return x + y + z + w; }

XO_INL float Vector4::MagnitudeSquared() const { return x * x + y * y + z * z + w * w; }
XO_INL float Vector4::Magnitude() const { return Sqrt(MagnitudeSquared()); }
XO_INL Vector4 Vector4::Normalized() const { return Vector4(*this).Normalize(); }
XO_INL Vector4& Vector4::Normalize() { return (*this) /= Vector4(Magnitude()); };

/*static*/ XO_INL 
bool XO_CC Vector4::RoughlyEqual(Vector4 const& left, Vector4 const& right) {
    return CloseEnough(left.x, right.x)
        && CloseEnough(left.y, right.y)
        && CloseEnough(left.z, right.z)
        && CloseEnough(left.w, right.w);
}

/*static*/ XO_INL 
bool XO_CC Vector4::ExactlyEqual(Vector4 const& left, Vector4 const& right) {
    return left.x == right.x
        && left.y == right.y
        && left.z == right.z
        && left.w == right.w;
}

/*static*/ XO_INL 
bool XO_CC Vector4::RoughlyEqual(Vector4 const& left, float magnitude) {
    return CloseEnough(left.MagnitudeSquared(), Pow<2>(magnitude));
}

/*static*/ XO_INL
bool XO_CC Vector4::ExactlyEqual(Vector4 const& left, float magnitude) {
    return left.MagnitudeSquared() == Pow<2>(magnitude);
}

/*static*/ XO_INL
float XO_CC Vector4::DotProduct(Vector4 const& left, Vector4 const& right) {
    return (left * right).Sum();
}

/*static*/ XO_INL 
Vector4 XO_CC Vector4::Lerp(Vector4 const& left, Vector4 const& right, float t) {
    return left + Vector4(t) * (right - left);
}

#if defined(XO_MATH_IMPL)
/*static*/ const Vector4 Vector4::Zero(0.f);
/*static*/ const Vector4 Vector4::One(1.f);
#endif

////////////////////////////////////////////////////////////////////////////////////////// Matrix4x4

#if defined(XO_MATH_IMPL)
/*static*/ const Matrix4x4 Matrix4x4::One(1.f);
/*static*/ const Matrix4x4 Matrix4x4::Zero(0.f);
/*static*/ const Matrix4x4 Matrix4x4::Identity(
    Vector4(1.f, 0.f, 0.f, 0.f),
    Vector4(0.f, 1.f, 0.f, 0.f),
    Vector4(0.f, 0.f, 1.f, 0.f),
    Vector4(0.f, 0.f, 0.f, 1.f));
#endif

XO_INL 
Matrix4x4 XO_CC Matrix4x4::operator * (Matrix4x4 const& other) const {
    return Matrix4x4(*this) *= other;
}

XO_INL
Matrix4x4& XO_CC Matrix4x4::operator *= (Matrix4x4 const& other) {
    Matrix4x4 transposed = Transpose(*this);
    return (*this) = Matrix4x4(
        Vector4((rows[0] * transposed[0]).Sum(),
                (rows[0] * transposed[1]).Sum(),
                (rows[0] * transposed[2]).Sum(),
                (rows[0] * transposed[3]).Sum()),
        Vector4((rows[1] * transposed[0]).Sum(),
                (rows[1] * transposed[1]).Sum(),
                (rows[1] * transposed[2]).Sum(),
                (rows[1] * transposed[3]).Sum()),
        Vector4((rows[2] * transposed[0]).Sum(),
                (rows[2] * transposed[1]).Sum(),
                (rows[2] * transposed[2]).Sum(),
                (rows[2] * transposed[3]).Sum()),
        Vector4((rows[3] * transposed[0]).Sum(),
                (rows[3] * transposed[1]).Sum(),
                (rows[3] * transposed[2]).Sum(),
                (rows[3] * transposed[3]).Sum()));
}

XO_INL Vector4 Matrix4x4::operator[] (int index) const { return rows[index]; }
XO_INL Vector4& Matrix4x4::operator[] (int index) { return rows[index]; }

XO_INL
Vector3 Matrix4x4::Up() const {
#if defined(XO_CONFIG_Y_UP) && XO_CONFIG_Y_UP
    return Vector3(rows[1][0], rows[1][1], rows[1][2]);
#elif defined(XO_CONFIG_Z_UP) && XO_CONFIG_Z_UP
    return Vector3(rows[2][0], rows[2][1], rows[2][2]);
#else
    static_assert(false, "Define XO_CONFIG_Y_UP and XO_CONFIG_Z_UP. One should have a \
value of 1, the other should have a value of 0.");
#endif
}

XO_INL
Vector3 Matrix4x4::Down() const {
    return -Up();
}

XO_INL
Vector3 Matrix4x4::Left() const {
    return -Right();
}
XO_INL
Vector3 Matrix4x4::Right() const {
    return Vector3(rows[0][0], rows[0][1], rows[0][2]);
}

XO_INL
Vector3 Matrix4x4::Forward() const {
#if defined(XO_CONFIG_Y_UP) && XO_CONFIG_Y_UP
#   if defined(XO_CONFIG_LEFT_HANDED) && XO_CONFIG_LEFT_HANDED
    return Vector3(rows[2][0], rows[2][1], rows[2][2]);
#   elif defined(XO_CONFIG_RIGHT_HANDED) && XO_CONFIG_RIGHT_HANDED
    return Vector3(-rows[2][0], -rows[2][1], -rows[2][2]);
#   else
    static_assert(false, "Define XO_CONFIG_LEFT_HANDED and XO_CONFIG_RIGHT_HANDED. One \
should have a value of 1, the other should have a value of 0.");
#   endif
#elif defined(XO_CONFIG_Z_UP) && XO_CONFIG_Z_UP
#   if defined(XO_CONFIG_LEFT_HANDED) && XO_CONFIG_LEFT_HANDED
    return Vector3(-rows[1][0], -rows[1][1], -rows[1][2]);
#   elif defined(XO_CONFIG_RIGHT_HANDED) && XO_CONFIG_RIGHT_HANDED
    return Vector3(rows[1][0], rows[1][1], rows[1][2]);
#   else
    static_assert(false, "Define XO_CONFIG_LEFT_HANDED and XO_CONFIG_RIGHT_HANDED. One \
should have a value of 1, the other should have a value of 0.");
#   endif
#else
    static_assert(false, "Define XO_CONFIG_Y_UP and XO_CONFIG_Z_UP. One should have a \
value of 1, the other should have a value of 0.");
#endif
}

XO_INL
Vector3 Matrix4x4::Backward() const {
    return -Forward();
}

/*static*/ XO_INL 
Matrix4x4 XO_CC Matrix4x4::Transpose(Matrix4x4 const& matrixIn) {
    Matrix4x4 transposed = Matrix4x4(matrixIn);
    float t;
#define XO_TRANSPOSE_SWAP(i,j) \
    t = transposed.rows[i][j];\
    transposed.rows[i][j] = transposed.rows[j][i]; \
    transposed.rows[j][i] = t;

    XO_TRANSPOSE_SWAP(0, 1);
    XO_TRANSPOSE_SWAP(0, 2);
    XO_TRANSPOSE_SWAP(0, 3);
    XO_TRANSPOSE_SWAP(1, 2);
    XO_TRANSPOSE_SWAP(1, 3);
    XO_TRANSPOSE_SWAP(2, 3);
#undef XO_TRANSPOSE_SWAP
    return transposed;
}

namespace {
XO_INL
void XO_CC EarlyInverse(float tmp[12], float src[16], float& det, float m[16]) {
    // http://download.intel.com/design/PentiumIII/sml/24504301.pdf
    // transpose matrix
    for (int i = 0; i < 4; i++) {
        src[i] = m[i * 4];
        src[i + 4] = m[i * 4 + 1];
        src[i + 8] = m[i * 4 + 2];
        src[i + 12] = m[i * 4 + 3];
    }
    // calculate pairs for first 8 elements (cofactors)
    tmp[0] = src[10] * src[15];
    tmp[1] = src[11] * src[14];
    tmp[2] = src[9] * src[15];
    tmp[3] = src[11] * src[13];
    tmp[4] = src[9] * src[14];
    tmp[5] = src[10] * src[13];
    tmp[6] = src[8] * src[15];
    tmp[7] = src[11] * src[12];
    tmp[8] = src[8] * src[14];
    tmp[9] = src[10] * src[12];
    tmp[10] = src[8] * src[13];
    tmp[11] = src[9] * src[12];
    // calculate first 8 elements (cofactors)
    m[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
    m[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
    m[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
    m[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
    m[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
    m[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
    m[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
    m[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
    m[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
    m[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
    m[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
    m[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
    m[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
    m[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
    m[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
    m[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
    // calculate pairs for second 8 elements (cofactors)
    tmp[0] = src[2] * src[7];
    tmp[1] = src[3] * src[6];
    tmp[2] = src[1] * src[7];
    tmp[3] = src[3] * src[5];
    tmp[4] = src[1] * src[6];
    tmp[5] = src[2] * src[5];
    tmp[6] = src[0] * src[7];
    tmp[7] = src[3] * src[4];
    tmp[8] = src[0] * src[6];
    tmp[9] = src[2] * src[4];
    tmp[10] = src[0] * src[5];
    tmp[11] = src[1] * src[4];
    // calculate second 8 elements (cofactors)
    m[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
    m[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
    m[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
    m[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
    m[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
    m[10] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
    m[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
    m[11] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
    m[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
    m[12] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
    m[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
    m[13] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
    m[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
    m[14] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
    m[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
    m[15] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];
    // calculate determinant
    det = src[0] * m[0] + src[1] * m[1] + src[2] * m[2] + src[3] * m[3];
}

XO_INL
void XO_CC LateInverse(float &det, float m[16]) {
    // http://download.intel.com/design/PentiumIII/sml/24504301.pdf
    // calculate matrix inverse 
    det = 1.0f / det;
    for (int j = 0; j < 16; j++) {
        m[j] *= det;
    }
}
}

/*static*/ XO_INL 
Matrix4x4 XO_CC Matrix4x4::Invert(Matrix4x4 const& matrixIn) {
    float tmp[12]; // temp array for pairs
    float src[16]; // array of transpose source matrix
    float det; // determinant
    Matrix4x4 inverted(matrixIn);
    EarlyInverse(tmp, src, det, inverted.v);
    LateInverse(det, inverted.v);
    return inverted;
}

/*static*/ XO_INL
bool XO_CC Matrix4x4::InvertSafe(Matrix4x4 const& matrixIn, Matrix4x4& matrixOut) {
    float tmp[12]; // temp array for pairs
    float src[16]; // array of transpose source matrix
    float det; // determinant
    Matrix4x4 inverted(matrixIn);
    EarlyInverse(tmp, src, det, inverted.v);
    if (CloseEnough(det, 0.f))
        return false;
    LateInverse(det, inverted.v);
    matrixOut = inverted;
    return true;
}

/*static*/ XO_INL 
Matrix4x4 XO_CC Matrix4x4::Translation(Vector3 const& pos) {
    return Matrix4x4(
        Vector4(1.f,   0.f,   0.f,   0.f),
        Vector4(0.f,   1.f,   0.f,   0.f),
        Vector4(0.f,   0.f,   1.f,   0.f),
        Vector4(pos.x, pos.y, pos.z, 1.f));
}

/*static*/ XO_INL 
Matrix4x4 XO_CC Matrix4x4::Scale(Vector3 const& scale) {
    return Matrix4x4(
        Vector4(scale.x, 0.f,     0.f,     0.f),
        Vector4(0.f,     scale.y, 0.f,     0.f),
        Vector4(0.f,     0.f,     scale.z, 0.f),
        Vector4(0.f,     0.f,     0.f,     1.f));
}

/*static*/ XO_INL 
Matrix4x4 XO_CC Matrix4x4::RotationYaw(float yaw) {
    float s, c;
    SinCos(yaw, s, c);
    return Matrix4x4(
        Vector4(c,   0.f, -s,  0.f),
        Vector4(0.f, 1.f, 0.f, 0.f),
        Vector4(s,   0.f, c,   0.f),
        Vector4(0.f, 0.f, 0.f, 1.f));
}

/*static*/ XO_INL
Matrix4x4 XO_CC Matrix4x4::RotationPitch(float pitch) {
    float s, c;
    SinCos(pitch, s, c);
    return Matrix4x4(
        Vector4(1.f, 0.f, 0.f, 0.f),
        Vector4(0.f, c,   -s,  0.f),
        Vector4(0.f, s,   c,   0.f),
        Vector4(0.f, 0.f, 0.f, 1.f));
}

/*static*/ XO_INL 
Matrix4x4 XO_CC Matrix4x4::RotationRoll(float roll) {
    float s, c;
    SinCos(roll, s, c);
    return Matrix4x4(
        Vector4(c,   -s,  0.f, 0.f),
        Vector4(s,   c,   0.f, 0.f),
        Vector4(0.f, 0.f, 1.f, 0.f),
        Vector4(0.f, 0.f, 0.f, 1.f));
}

/*static*/ XO_INL 
Matrix4x4 XO_CC Matrix4x4::RotationYawPitchRoll(float yaw, float pitch, float roll) {
    return RotationYaw(yaw) * RotationPitch(pitch) * RotationRoll(roll);
}

/*static*/ XO_INL
Matrix4x4 XO_CC Matrix4x4::RotationAxisAngle(Vector3 const& axis, float angle) {
    float s, c;
    SinCos(angle, s, c);
    float t = 1.f - c;
    Vector3 a = axis.Normalized();
    Matrix4x4 rotation(Matrix4x4::Identity);

    rotation[0][0] = c + a.x*a.x*t;
    rotation[1][1] = c + a.y*a.y*t;
    rotation[2][2] = c + a.z*a.z*t;


    float tmp1 = a.x*a.y*t;
    float tmp2 = a.z*s;
    rotation[1][0] = tmp1 + tmp2;
    rotation[0][1] = tmp1 - tmp2;
    
    tmp1 = a.x*a.z*t;
    tmp2 = a.y*s;
    rotation[2][0] = tmp1 - tmp2;
    rotation[0][2] = tmp1 + tmp2;    tmp1 = a.y*a.z*t;

    tmp2 = a.x*s;
    rotation[2][1] = tmp1 + tmp2;
    rotation[1][2] = tmp1 - tmp2;

    return rotation;
}

/*static*/ XO_INL 
Matrix4x4 XO_CC Matrix4x4::PerspectiveFOV(float fov, 
                                          float aspect, 
                                          float nearPlane, 
                                          float farPlane) {
    float s, c;
    SinCos(fov*0.5f, s, c);
    float h = c / s;                             // height
    float w = h / aspect;                        // width
    float r = farPlane / (nearPlane - farPlane); // range
    float rn = r * nearPlane;                    // range*near
    return Matrix4x4(
        Vector4(w,   0.f, 0.f, 0.f),
        Vector4(0.f, h,   0.f, 0.f),
        Vector4(0.f, 0.f, r,   0.f),
        Vector4(0.f, 0.f, rn,  0.f));
}

/*static*/ XO_INL 
Matrix4x4 XO_CC Matrix4x4::Perspective(float width,
                                       float height, 
                                       float aspect, 
                                       float nearPlane, 
                                       float farPlane) {
    float n2 = Pow<2>(nearPlane);
    float r = farPlane / (nearPlane - farPlane);
    float w = n2 / width;
    float h = n2 / height;
    float rn = r * nearPlane;
    return Matrix4x4(
        Vector4(w,   0.f, 0.f, 0.f),
        Vector4(0.f, h,   0.f, 0.f),
        Vector4(0.f, 0.f, r,   0.f),
        Vector4(0.f, 0.f, rn,  0.f));
}

/*static*/ XO_INL
Matrix4x4 XO_CC Matrix4x4::Orthographic(float width,
                                        float height,
                                        float nearPlane,
                                        float farPlane) {
    float r = 1.f / (nearPlane - farPlane);
    float w = 2.f / width;
    float h = 2.f / height;
    float rn = r * nearPlane;
    return Matrix4x4(
        Vector4(w, 0.f, 0.f, 0.f),
        Vector4(0.f, h, 0.f, 0.f),
        Vector4(0.f, 0.f, r, 0.f),
        Vector4(0.f, 0.f, rn, 0.f));
}

/*static*/ XO_INL
Matrix4x4 XO_CC Matrix4x4::LookAt(Vector3 const& from,
                                  Vector3 const& to, 
                                  Vector3 const& up) {
    Vector3 dir = from - to;
    Vector3 r2 = dir.Normalized();
    Vector3 r0 = Vector3::CrossProduct(up, r2).Normalized();
    Vector3 r1 = Vector3::CrossProduct(r2, r0);
    Vector3 nfrom = -from;

    float d0 = Vector3::DotProduct(r0, nfrom);
    float d1 = Vector3::DotProduct(r1, nfrom);
    float d2 = Vector3::DotProduct(r2, nfrom);
    return Matrix4x4(
        Vector4(r0.x, r0.y, r0.z, 0.f),
        Vector4(r0.x, r0.y, r0.z, 0.f),
        Vector4(r0.x, r0.y, r0.z, 0.f),
        Vector4(d0,   d1,   d2,   1.f));
}

/*static*/ XO_INL 
bool XO_CC Matrix4x4::RoughlyEqual(Matrix4x4 const& left, Matrix4x4 const& right) {
    return Vector4::RoughlyEqual(left[0], right[0])
        && Vector4::RoughlyEqual(left[1], right[1])
        && Vector4::RoughlyEqual(left[2], right[2])
        && Vector4::RoughlyEqual(left[3], right[3]);
}

/*static*/ XO_INL 
bool XO_CC Matrix4x4::ExactlyEqual(Matrix4x4 const& left, Matrix4x4 const& right) {
    return Vector4::ExactlyEqual(left[0], right[0])
        && Vector4::ExactlyEqual(left[1], right[1])
        && Vector4::ExactlyEqual(left[2], right[2])
        && Vector4::ExactlyEqual(left[3], right[3]);
}

////////////////////////////////////////////////////////////////////////////////////////// Quaternion
#if defined(XO_MATH_IMPL)
/*static*/ const Quaternion Quaternion::Zero(0.f);
/*static*/ const Quaternion Quaternion::Identity(0.f, 0.f, 0.f, 1.f);
#endif

XO_INL
Quaternion Quaternion::operator + (Quaternion other) const {
    return Quaternion(i + other.i, j + other.j, k + other.k, r + other.r);
}

XO_INL
Quaternion Quaternion::operator *(float s) const { 
    return Quaternion(i*s, j*s, k*s, r*s); 
}

XO_INL
Quaternion Quaternion::operator -() const {
    return Quaternion(-i, -j, -k, -r);
}

XO_INL
float Quaternion::Magnitude() const {
    return vec4.Magnitude();
}

XO_INL 
float Quaternion::MagnitudeSquared() const {
    return vec4.MagnitudeSquared();
};

XO_INL
Quaternion Quaternion::Normalized() const {
    return Quaternion(vec4.Normalized());
};

XO_INL
Quaternion& Quaternion::Normalize() {
    vec4.Normalize(); return *this;
};

XO_INL
Matrix4x4 Quaternion::ToMatrix() const {
    // See: https://www.flipcode.com/documents/matrfaq.html#Q54
    float ii = i * i;
    float ij = i * j;
    float ik = i * k;
    float ir = i * r;
    float jj = j * j;
    float jk = j * k;
    float jr = j * r;
    float kk = k * k;
    float kr = k * r;
    return Matrix4x4(
        Vector4(1.f - 2.f * (jj + kk), 2.f * (ij - kr), 2.f * (ik + jr), 0.f),
        Vector4(2.f * (ij + kr), 1.f - 2.f * (ii + kk), 2.f * (jk - ir), 0.f),
        Vector4(2.f * (ik - jr), 2.f * (jk + ir), 1.f - 2.f * (ii + jj), 0.f),
        Vector4(0.f, 0.f, 0.f, 1.f));
}

/*static*/ XO_INL
Quaternion XO_CC Quaternion::Invert(Quaternion const& quat) {
    return Quaternion(-quat.i, -quat.j, -quat.k, quat.r);
}

/*static*/ XO_INL
Quaternion XO_CC Quaternion::RotationAxisAngle(Vector3 const& axis, float angle) {
    float s, c;
    SinCos(angle*0.5f, s, c);
    return Quaternion(axis.x*s, axis.y*s, axis.z*s, c);
}

/*static*/ XO_INL
Quaternion XO_CC Quaternion::RotationYawPitchRoll(float yaw, float pitch, float roll) {
    float sr, cp, sp, cy, sy, cr;
    SinCos(yaw * 0.5f, sy, cy);
    SinCos(pitch * 0.5f, sp, cp);
    SinCos(roll * 0.5f, sr, cr);
    return Quaternion(cy * cr * cp + sy * sr * sp,
                      cy * sr * cp - sy * cr * sp,
                      cy * cr * sp + sy * sr * cp,
                      sy * cr * cp - cy * sr * sp);
}

/*static*/ XO_INL
float XO_CC Quaternion::DotProduct(Quaternion const& left, Quaternion const& right) {
    return Vector4::DotProduct(left.vec4, right.vec4);
}

/*static*/ XO_INL
Quaternion XO_CC Quaternion::Lerp(Quaternion const& start,
                                  Quaternion const& end,
                                  float t) {
    return Quaternion(Vector4::Lerp(start.vec4, end.vec4, t));
}

/*static*/ XO_INL
Quaternion XO_CC Quaternion::Slerp(Quaternion const& start, 
                                   Quaternion const& end, 
                                   float t) {
    Quaternion s = start.Normalized();
    Quaternion e = end.Normalized();
    float d = Quaternion::DotProduct(s, e);
    if (d < 0.f) {
        e = -e;
        d = -d;
    }

    if (CloseEnough(d, 1.f)) {
        return Lerp(s, e, t).Normalize();
    }

    float th0 = ACos(d);
    float th = th0 * t;

    float st, ct, sth0;
    SinCos(th, st, ct);
    sth0 = Sin(th0);
    float s0 = ct - d * st / sth0;
    float s1 = st / sth0;
    return (s * s0) + (e * s1);
}

/*static*/ XO_INL 
bool XO_CC Quaternion::RoughlyEqual(Quaternion const& left, Quaternion const& right) {
    return CloseEnough(left.i, right.i)
        && CloseEnough(left.j, right.j)
        && CloseEnough(left.k, right.k)
        && CloseEnough(left.r, right.r);
}

/*static*/ XO_INL 
bool XO_CC Quaternion::ExactlyEqual(Quaternion const& left, Quaternion const& right) {
    return left.i == right.i
        && left.j == right.j
        && left.k == right.k
        && left.r == right.r;
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