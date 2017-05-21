//////////////////////////////////////////////////////////////////////
// Vector3.h 
//
// AUTHOR
//   Jared Thomson <twitter: @xoorath> <email:jared@xoorath.com>
//
// LICENSE
//   See end of file for license information.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "xo-math-defines.h"

XO_NAMESPACE_BEGIN

class XO_ALIGN Vector3 {
public:
  XO_OVERLOAD_NEW_DELETE(); // for alignment

  inline Vector3() {};
  inline Vector3(float x, float y, float z);
  explicit inline Vector3(float xyz);
  inline Vector3(const Vector3& other);

  inline operator float*();

  inline void Set(float x, float y, float z);
  inline void SetXY(float x, float y);
  inline void SetXZ(float x, float z);
  inline void SetYZ(float y, float z);

  inline float&       operator [](int i);
  inline const float& operator [](int i) const;

  inline Vector3 operator -() const;
  inline Vector3 operator ~() const;

  inline Vector3& operator += (const Vector3& other);
  inline Vector3  operator +  (const Vector3& other) const;
  inline Vector3& operator -= (const Vector3& other);
  inline Vector3  operator -  (const Vector3& other) const;
  inline Vector3& operator *= (const Vector3& other);
  inline Vector3  operator *  (const Vector3& other) const;
  inline Vector3& operator /= (const Vector3& other);
  inline Vector3  operator /  (const Vector3& other) const;
  inline Vector3& operator += (float f);
  inline Vector3  operator +  (float f) const;
  inline Vector3& operator -= (float f);
  inline Vector3  operator -  (float f) const;
  inline Vector3& operator *= (float f);
  inline Vector3  operator *  (float f) const;
  inline Vector3& operator /= (float f);
  inline Vector3  operator /  (float f) const;

  inline bool operator <  (const Vector3& v) const;
  inline bool operator <= (const Vector3& v) const;
  inline bool operator >  (const Vector3& v) const;
  inline bool operator >= (const Vector3& v) const;
  inline bool operator == (const Vector3& v) const;
  inline bool operator != (const Vector3& v) const;

  inline float   Magnitude() const;
  inline float   MagnitudeSquared() const;
  inline void    Normalize();
  inline Vector3 Normalized() const;
  inline float   Sum() const;
  inline Vector3 ZYX() const;

  inline static Vector3 Cross(const Vector3& a, const Vector3& b);
  inline static float   Distance(const Vector3& a, const Vector3& b);
  inline static float   DistanceSquared(const Vector3& a, const Vector3& b);
  inline static float   Dot(const Vector3& a, const Vector3& b);
  inline static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

#ifdef XO_SIMD
  inline Vector3(VectorRegister_t reg);
  inline operator VectorRegister_t&();
  inline operator const VectorRegister_t&() const;
#endif

#ifdef _MSC_VER
#   pragma warning( push )
// C4201: nonstandard extension used: nameless struct/union
#   pragma warning( disable : 4201)
#endif

  union {
#ifdef XO_SIMD
    struct { 
      float x, y, z; 
    };
    float v[4];
    VectorRegister_t reg;
#else
    struct { 
      float x, y, z; 
    };
    float v[3];
#endif
  };

#ifdef _MSC_VER
#   pragma warning( pop ) 
#endif

  static const Vector3
    UnitX, UnitY, UnitZ,
    Up, Down, Left, Right, Forward, Backward,
    One, Zero;

#if defined(XO_SSE)
  static const VectorRegister_t MASK;
#endif
};

Vector3::Vector3(float x, float y, float z) 
#if defined(XO_SSE)
  : reg(_mm_set_ps(0.0f, z, y, x))
#else
  : x(x), y(y), z(z)
#endif
{
}

// explicit inline 
Vector3::Vector3(float xyz) 
#if defined(XO_SSE)
  : reg(_mm_set1_ps(xyz))
#elif defined(XO_NEON)
  : n(vdupq_n_f32(xyz))
#else
  : x(xyz), y(xyz), z(xyz)
#endif
{
}

// inline
Vector3::Vector3(const Vector3& other)
#if defined(XO_SIMD)
  : reg(other.reg)
#else
  : x(other.x), y(other.y), z(other.z)
#endif
{
}

// inline
Vector3::operator float*() {
  return v;
}

// inline
void Vector3::Set(float _x, float _y, float _z) {
#if defined(XO_SSE)
  reg = _mm_set_ps(0.0f, _z, _y, _x);
#else
  this->x = _x;
  this->y = _y;
  this->z = _z;
#endif
}

//inline
void Vector3::SetXY(float _x, float _y) {
  Set(_x, _y, z);
}

//inline
void Vector3::SetXZ(float _x, float _z) {
  Set(_x, y, _z);
}

//inline
void Vector3::SetYZ(float _y, float _z) {
  Set(x, _y, _z);
}

// inline
float& Vector3::operator [](int i) {
  return v[i];
}

// inline
const float& Vector3::operator [](int i) const {
  return v[i];
}

// inline
Vector3 Vector3::operator -() const {
#if defined(XO_SSE)
  return Vector3(_mm_mul_ps(reg, simd::NegativeOne));
#elif defined(XO_NEON)
  return Vector3(vnegq_f32(reg));
#else
  return Vector3(-x, -y, -z);
#endif
}

// inline
Vector3 Vector3::operator ~() const {
  return ZYX();
}

// inline
Vector3& Vector3::operator += (const Vector3& other) {
#if defined(XO_SIMD)
  simd::Add(*this, other, *this);
#else
  x += other.x;
  y += other.y;
  z += other.z;
#endif
  return *this;
}

// inline
Vector3 Vector3::operator + (const Vector3& other) const {
  return Vector3(*this) += other;
}

// inline
Vector3& Vector3::operator -= (const Vector3& other) {
#if defined(XO_SIMD)
  simd::Subtract(*this, other, *this);
#else
  x -= other.x;
  y -= other.y;
  z -= other.z;
#endif
  return *this;
}

// inline
Vector3 Vector3::operator - (const Vector3& other) const {
  return Vector3(*this) -= other;
}

// inline
Vector3& Vector3::operator *= (const Vector3& other) {
#if defined(XO_SIMD)
  simd::Multiply(*this, other, *this);
#else
  x *= other.x;
  y *= other.y;
  z *= other.z;
#endif
  return *this;
}

// inline
Vector3 Vector3::operator * (const Vector3& other) const {
  return Vector3(*this) *= other;
}

// inline
Vector3& Vector3::operator /= (const Vector3& other) {
#if defined(XO_SIMD)
  simd::Divide(*this, other, *this);
#else
  x /= other.x;
  y /= other.y;
  z /= other.z;
#endif
  return *this;
}

// inline
Vector3 Vector3::operator / (const Vector3& other) const {
  return Vector3(*this) /= other;
}

// inline
Vector3& Vector3::operator += (float f) {
  Vector3 other(f);
  return *this += other;
}

// inline
Vector3 Vector3::operator + (float f) const {
  Vector3 other(f);
  return *this + other;
}

// inline
Vector3& Vector3::operator -= (float f) {
  Vector3 other(f);
  return *this -= other;
}

// inline
Vector3 Vector3::operator - (float f) const {
  Vector3 other(f);
  return *this - other;
}

// inline
Vector3& Vector3::operator *= (float f) {
  Vector3 other(f);
  return *this *= other;
}

// inline
Vector3 Vector3::operator * (float f) const {
  Vector3 other(f);
  return *this * other;
}

// inline
Vector3& Vector3::operator /= (float f) {
  Vector3 other(f);
  return *this /= other;
}

// inline
Vector3 Vector3::operator / (float f) const {
  Vector3 other(f);
  return *this / other;
}

// inline
bool Vector3::operator < (const Vector3& other) const {
  return MagnitudeSquared() < other.MagnitudeSquared();
}

// inline
bool Vector3::operator <= (const Vector3& other) const {
  return MagnitudeSquared() <= other.MagnitudeSquared();
}

// inline
bool Vector3::operator > (const Vector3& other) const {
  return MagnitudeSquared() > other.MagnitudeSquared();
}

// inline
bool Vector3::operator >= (const Vector3& other) const {
  return MagnitudeSquared() >= other.MagnitudeSquared();
}

// inline
bool Vector3::operator == (const Vector3& other) const {
#if defined(XO_SSE2)
  return (_mm_movemask_ps(
    _mm_cmplt_ps(
      simd::Abs(_mm_sub_ps(other, reg)),
      simd::VectorEpsilon)) & 7) == 7;
#elif XO_SSE
  return CloseEnough(x, other.x, simd::Epsilon) 
    && CloseEnough(y, other.y, simd::Epsilon) 
    && CloseEnough(z, other.z, simd::Epsilon);
#else
  return CloseEnough(x, v.x, Epsilon) 
    && CloseEnough(y, v.y, Epsilon) 
    && CloseEnough(z, v.z, Epsilon);
#endif
}

// inline
bool Vector3::operator != (const Vector3& other) const {
  return !(*this == other);
}

// inline
float Vector3::Magnitude() const {
  return Sqrt(MagnitudeSquared());
}

// inline
float Vector3::MagnitudeSquared() const {
  return ((*this) * (*this)).Sum();
}

// inline
void Vector3::Normalize() {
  *this /= Magnitude();
}

// inline
Vector3 Vector3::Normalized() const {
  Vector3 norm = *this;
  norm.Normalize();
  return norm;
}

// inline
float Vector3::Sum() const {
#if defined(XO_SSE3)
  VectorRegister_t _x = _mm_and_ps(reg, MASK);
  _x = _mm_hadd_ps(_x, _x);
  _x = _mm_hadd_ps(_x, _x);
  return _mm_cvtss_f32(_x);
#else
  return x + y + z;
#endif
}

// inline
Vector3 Vector3::ZYX() const {
#if defined(XO_SSE)
  constexpr int m3012 = _MM_SHUFFLE(3, 0, 1, 2);
  return Vector3(_mm_shuffle_ps(reg, reg, m3012));
#else
  return Vector3(z, y, x);
#endif
}

// inline static 
Vector3 Vector3::Cross(const Vector3& a, const Vector3& b) {
  Vector3 outVec;
#if defined(XO_SSE)
  constexpr int m3021 = _MM_SHUFFLE(3, 0, 2, 1);
  VectorRegister_t result = _mm_sub_ps(
    _mm_mul_ps(a, _mm_shuffle_ps(b, b, m3021)), 
    _mm_mul_ps(b, _mm_shuffle_ps(a, a, m3021)));
  outVec.reg = _mm_shuffle_ps(result, result, m3021);
#else
  outVec.x = (a.y * b.z) - (a.z * b.y);
  outVec.y = (a.z * b.x) - (a.x * b.z);
  outVec.z = (a.x * b.y) - (a.y * b.x);
#endif
  return outVec;
}

// inline static 
float Vector3::Dot(const Vector3& a, const Vector3& b) {
#if defined(XO_SSE4_1)
  return _mm_cvtss_f32(_mm_dp_ps(a, b, 0x7f));
#else
  return (a * b).Sum();
#endif
}

// inline static 
Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float t) {
  return a + ((b - a) * t);
}

// inline static 
float Vector3::Distance(const Vector3& a, const Vector3& b) {
  return (b - a).Magnitude();
}

// inline static 
float Vector3::DistanceSquared(const Vector3& a, const Vector3& b) {
  return (b - a).MagnitudeSquared();
}

#ifdef XO_SIMD
// inline
Vector3::Vector3(VectorRegister_t _reg) 
: reg(_reg) {
}

// inline
Vector3::operator VectorRegister_t&() {
  return reg;
}

// inline
Vector3::operator const VectorRegister_t&() const {
  return reg;
}
#endif

XO_NAMESPACE_END

//////////////////////////////////////////////////////////////////////
// This software is available under 2 licenses
// choose whichever you prefer.
//////////////////////////////////////////////////////////////////////

// MIT License
//////////////////////////////////////////////////////////////////////
/*
Copyright (c) 2017 Jared Thomson
Permission is hereby granted, free of charge, to any person obtaining 
a copy of this software and associated documentation files (the 
"Software"), to deal in the Software without restriction, including 
without limitation the rights to use, copy, modify, merge, publish, 
distribute, sublicense, and/or sell copies of the Software, and to 
permit persons to whom the Software is furnished to do so, subject to 
the following conditions:
The above copyright notice and this permission notice shall be 
included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////////////////////////////////

// The Unlicense
//////////////////////////////////////////////////////////////////////
/*
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.
In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
For more information, please refer to <http://unlicense.org/>
*/
//////////////////////////////////////////////////////////////////////