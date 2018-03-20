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
// See License in third-party-licenses.h for https://github.com/Microsoft/DirectXMath
// applies to the exact version checking for __vectorcall
#if defined(_MSC_VER) && !defined(_M_ARM) && \
(((_MSC_FULL_VER >= 170065501) && (_MSC_VER < 1800)) || (_MSC_FULL_VER >= 180020418))
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

#define XO_CONFIG_LEFT_HANDED 1
#define XO_CONFIG_RIGHT_HANDED 0
#define XO_CONFIG_Y_UP 1
#define XO_CONFIG_Z_UP 0
#define XO_CONFIG_DEFAULT_NEAR_PLANE 0.1f
#define XO_CONFIG_DEFAULT_FAR_PLANE 1000.f

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
namespace xo {
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
    static Vector3 XO_CC Lerp(const Vector3& left, const Vector3& right, float t);
    static float XO_CC DistanceSquared(const Vector3& left, const Vector3& right);
    static float XO_CC Distance(const Vector3& left, const Vector3& right);

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

    Vector4 XO_CC operator + (const Vector4& other) const;
    Vector4 XO_CC operator - (const Vector4& other) const;
    Vector4 XO_CC operator * (const Vector4& other) const;
    Vector4 XO_CC operator / (const Vector4& other) const;
    Vector4& XO_CC operator += (const Vector4& other);
    Vector4& XO_CC operator -= (const Vector4& other);
    Vector4& XO_CC operator *= (const Vector4& other);
    Vector4& XO_CC operator /= (const Vector4& other);

    Vector4 operator -() const;

    float operator[] (int index) const;
    float& operator[] (int index);

    float Sum() const;

    float Magnitude() const;
    float MagnitudeSquared() const;

    static bool XO_CC RoughlyEqual(const Vector4& left, const Vector4& right);
    static bool XO_CC ExactlyEqual(const Vector4& left, const Vector4& right);
    static bool XO_CC RoughlyEqual(const Vector4& left, float magnitude);
    static bool XO_CC ExactlyEqual(const Vector4& left, float magnitude);

    static Vector4 XO_CC Lerp(const Vector4& left, const Vector4& right, float t);

    static const Vector4 Zero;
    static const Vector4 One;
};

struct Matrix4x4 {
    Vector4 rows[4];
    Matrix4x4(Vector4 rows[4]) 
        : rows {
            rows[0],
            rows[1],
            rows[2],
            rows[3] }
    { }

    Matrix4x4(Vector4 const& row0, 
              Vector4 const& row1,
              Vector4 const& row2,
              Vector4 const& row3)
        : rows{
            row0,
            row1,
            row2,
            row3 }
    { }

    explicit Matrix4x4(float all)
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

    Matrix4x4 XO_CC operator * (const Matrix4x4& other) const;
    Matrix4x4& XO_CC operator *= (const Matrix4x4& other);

    Vector4 operator[] (int index) const;
    Vector4& operator[] (int index);

    float Determinant() const;

    Vector3 Up() const;
    Vector3 Down() const;
    Vector3 Left() const;
    Vector3 Right() const;
    Vector3 Forward() const;
    Vector3 Backward() const;

    static Matrix4x4 XO_CC Transpose(Matrix4x4 const& matrixIn);
    static Matrix4x4 XO_CC Invert(Matrix4x4 const& matrixIn);
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

    static const Matrix4x4 One;
    static const Matrix4x4 Zero;
    static const Matrix4x4 Identity;
};

struct Quaternion {
    float i, j, k, r;
    Quaternion(float i, float j, float k, float r) 
        : i(i)
        , j(j)
        , k(k)
        , r(r)
    { }

    Quaternion(Quaternion const& other) 
        : i(other.i)
        , j(other.j)
        , k(other.k)
        , r(other.r) 
    { }
};

// vector 3, aligned for cpu specific optimizations (where applicable) 
struct AVector3 {
    float x, y, z;
    AVector3(float x, float y, float z) 
        : x(x)
        , y(y)
        , z(z)
    { }

    explicit AVector3(float all)
        : x(all)
        , y(all)
        , z(all)
    { }

    AVector3(AVector3 const& other)
        : x(other.x)
        , y(other.y)
        , z(other.z)
    { }

    AVector3(Vector3 const& other)
        : x(other.x)
        , y(other.y)
        , z(other.z)
    { }
};

// vector 4, aligned for cpu specific optimizations (where applicable)
struct AVector4 {
    float x, y, z, w;
    AVector4(float x, float y, float z, float w)
        : x(x)
        , y(y)
        , z(z)
        , w(w)
    { }
    
    explicit AVector4(float all) 
        : x(all)
        , y(all)
        , z(all)
        , w(all)
    { }

    AVector4(AVector4 const& other)
        : x(other.x)
        , y(other.y)
        , z(other.z)
        , w(other.w)
    { }

    AVector4(Vector4 const& other)
        : x(other.x)
        , y(other.y)
        , z(other.z)
        , w(other.w)
    { }
};

// matrix4x4, aligned for cpu specific optimizations (where applicable)
struct AMatrix4x4 {
    AVector4 rows[4];
    AMatrix4x4(AVector4 rows[4])
        : rows {
            rows[0],
            rows[1],
            rows[2],
            rows[3] }
    { }

    AMatrix4x4(AMatrix4x4 const& other) 
        : rows { 
            other.rows[0],
            other.rows[1],
            other.rows[2],
            other.rows[3] }
    { }

    AMatrix4x4(Matrix4x4 const& other)
        : rows {
            AVector4(other.rows[0]),
            AVector4(other.rows[1]),
            AVector4(other.rows[2]),
            AVector4(other.rows[3]) }
    { }
};

// quaternion, aligned for cpu specific optimizations (where applicable)
struct AQuaternion {
    float i, j, k, r;
    AQuaternion(float i, float j, float k, float r) 
        : i(i)
        , j(j)
        , k(k)
        , r(r) 
    { }

    AQuaternion(AQuaternion const& other) 
        : i(other.i)
        , j(other.j)
        , k(other.k)
        , r(other.r) 
    { }

    AQuaternion(Quaternion const& other) 
        : i(other.i)
        , j(other.j)
        , k(other.k)
        , r(other.r)
    { }
};

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
Vector3 XO_CC Vector3::operator + (const Vector3& other) const { 
    return Vector3(x + other.x, y + other.y, z + other.z);
}

XO_INL
Vector3 XO_CC Vector3::operator - (const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
}

XO_INL
Vector3 XO_CC Vector3::operator * (const Vector3& other) const {
    return Vector3(x * other.x, y * other.y, z * other.z);
}

XO_INL
Vector3 XO_CC Vector3::operator / (const Vector3& other) const {
    return Vector3(x / other.x, y / other.y, z / other.z);
}

XO_INL
Vector3& XO_CC Vector3::operator += (const Vector3& other) {
    x += other.x;
    y += other.y;
    z *= other.z;
    return *this;
}

XO_INL
Vector3& XO_CC Vector3::operator -= (const Vector3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

XO_INL
Vector3& XO_CC Vector3::operator *= (const Vector3& other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
}

XO_INL
Vector3& XO_CC Vector3::operator /= (const Vector3& other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
}

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

/*static*/ XO_INL
Vector3 XO_CC Vector3::Lerp(const Vector3& left, const Vector3& right, float t) {
    return left + Vector3(t) * (right - left);
}

/*static*/ XO_INL
float XO_CC Vector3::DistanceSquared(const Vector3& left, const Vector3& right) {
    return (right - left).MagnitudeSquared();
}

/*static*/ XO_INL
float XO_CC Vector3::Distance(const Vector3& left, const Vector3& right) {
    return (right - left).Magnitude();
}

////////////////////////////////////////////////////////////////////////////////////////// Vector 4

XO_INL
Vector4 XO_CC Vector4::operator + (const Vector4& other) const {
    return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

XO_INL
Vector4 XO_CC Vector4::operator - (const Vector4& other) const {
    return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

XO_INL
Vector4 XO_CC Vector4::operator * (const Vector4& other) const {
    return Vector4(x*other.x, y*other.y, z*other.z, w*other.w);
}

XO_INL
Vector4 XO_CC Vector4::operator / (const Vector4& other) const {
    return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
}

XO_INL 
Vector4& XO_CC Vector4::operator += (const Vector4& other) {
    x += other.x; 
    y += other.y; 
    z += other.z; 
    w += other.w; 
    return *this;
};

XO_INL 
Vector4& XO_CC Vector4::operator -= (const Vector4& other) {
    x -=other.x;
    y -=other.y;
    z -= other.z;
    w -= other.w;
    return *this; };

XO_INL 
Vector4& XO_CC Vector4::operator *= (const Vector4& other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    w *= other.w;
    return *this;
};

XO_INL 
Vector4& XO_CC Vector4::operator /= (const Vector4& other) {
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

/*static*/ XO_INL 
bool XO_CC Vector4::RoughlyEqual(const Vector4& left, const Vector4& right) {
    return CloseEnough(left.x, right.x)
        && CloseEnough(left.y, right.y)
        && CloseEnough(left.z, right.z)
        && CloseEnough(left.w, right.w);
}

/*static*/ XO_INL 
bool XO_CC Vector4::ExactlyEqual(const Vector4& left, const Vector4& right) {
    return left.x == right.x
        && left.y == right.y
        && left.z == right.z
        && left.w == right.w;
}

/*static*/ XO_INL 
bool XO_CC Vector4::RoughlyEqual(const Vector4& left, float magnitude) {
    return CloseEnough(left.MagnitudeSquared(), Pow(magnitude, 2));
}

/*static*/ XO_INL
bool XO_CC Vector4::ExactlyEqual(const Vector4& left, float magnitude) {
    return left.MagnitudeSquared() == Pow(magnitude, 2);
}

/*static*/ XO_INL 
Vector4 XO_CC Vector4::Lerp(const Vector4& left, const Vector4& right, float t) {
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
Matrix4x4 XO_CC Matrix4x4::operator * (const Matrix4x4& other) const {
    return Matrix4x4(*this) *= other;
}

XO_INL
Matrix4x4& XO_CC Matrix4x4::operator *= (const Matrix4x4& other) {
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
float Matrix4x4::Determinant() const {
    return 0.f;
}

XO_INL
Vector3 Matrix4x4::Up() const {
    return Vector3::Zero;
}

XO_INL
Vector3 Matrix4x4::Down() const {
    return Vector3::Zero;
}

XO_INL
Vector3 Matrix4x4::Left() const {
    return Vector3::Zero;
}
XO_INL
Vector3 Matrix4x4::Right() const {
    return Vector3::Zero;
}

XO_INL
Vector3 Matrix4x4::Forward() const {
    return Vector3::Zero;
}

XO_INL
Vector3 Matrix4x4::Backward() const {
    return Vector3::Zero;
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

/*static*/ XO_INL 
Matrix4x4 XO_CC Matrix4x4::Invert(Matrix4x4 const& matrixIn) {
    return Matrix4x4();
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
    return Matrix4x4();
}

/*static*/ XO_INL 
Matrix4x4 XO_CC Matrix4x4::PerspectiveFOV(float fov, 
                                          float aspect, 
                                          float nearPlane, 
                                          float farPlane) {
    return Matrix4x4();
}

/*static*/ XO_INL 
Matrix4x4 XO_CC Matrix4x4::Perspective(float width,
                                       float height, 
                                       float aspect, 
                                       float nearPlane, 
                                       float farPlane) {
    return Matrix4x4();
}

/*static*/ XO_INL
Matrix4x4 XO_CC Matrix4x4::Orthographic(float width,
                                        float height,
                                        float nearPlane,
                                        float farPlane) {
    return Matrix4x4();
}

/*static*/ XO_INL
Matrix4x4 XO_CC Matrix4x4::LookAt(Vector3 const& from,
                                  Vector3 const& to, 
                                  Vector3 const& up) {
    return Matrix4x4();
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