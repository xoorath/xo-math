//////////////////////////////////////////////////////////////////////
// Vector4.h 
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

class XO_ALIGN Vector4 {
public:
  XO_OVERLOAD_NEW_DELETE(); // for alignment

  inline Vector4() {};
  inline Vector4(float x, float y, float z, float w);
  explicit inline Vector4(float wxyz);
  inline Vector4(const Vector4& other);

  inline void Set(float x, float y, float z, float w);

  inline float&       operator [](int i);
  inline const float& operator [](int i) const;

  inline Vector4 operator -() const;
  inline Vector4 operator ~() const;

  inline Vector4& operator += (const Vector4& other);
  inline Vector4  operator +  (const Vector4& other) const;
  inline Vector4& operator -= (const Vector4& other);
  inline Vector4  operator -  (const Vector4& other) const;
  inline Vector4& operator *= (const Vector4& other);
  inline Vector4  operator *  (const Vector4& other) const;
  inline Vector4& operator /= (const Vector4& other);
  inline Vector4  operator /  (const Vector4& other) const;
  inline Vector4& operator += (float f);
  inline Vector4  operator +  (float f) const;
  inline Vector4& operator -= (float f);
  inline Vector4  operator -  (float f) const;
  inline Vector4& operator *= (float f);
  inline Vector4  operator *  (float f) const;
  inline Vector4& operator /= (float f);
  inline Vector4  operator /  (float f) const;

  inline bool operator <  (const Vector4& v) const;
  inline bool operator <= (const Vector4& v) const;
  inline bool operator >  (const Vector4& v) const;
  inline bool operator >= (const Vector4& v) const;
  inline bool operator == (const Vector4& v) const;
  inline bool operator != (const Vector4& v) const;

  inline float   Magnitude() const;
  inline float   MagnitudeSquared() const;
  inline void    Normalize();
  inline Vector4 Normalized() const;
  inline float   Sum() const;
  inline Vector4 WZYX() const;

#ifdef XO_SIMD
  inline Vector4(VectorRegister_t reg);
  inline operator VectorRegister_t&();
  inline operator const VectorRegister_t&() const;
#endif

#ifdef _MSC_VER
#   pragma warning( push )
// C4201: nonstandard extension used: nameless struct/union
#   pragma warning( disable : 4201)
#endif

  union {
    struct { 
      float x, y, z, w;
    };
    float v[4];
#ifdef XO_SIMD
    VectorRegister_t reg;
#endif
  };

#ifdef _MSC_VER
#   pragma warning( pop ) 
#endif

  static const Vector4
    UnitX, UnitY, UnitZ, UnitW,
    One, Zero;
};

// inline 
Vector4::Vector4(float _x, float _y, float _z, float _w)
#if defined(XO_SSE)
  : reg(_mm_set_ps(_w, _z, _y, _x))
#else
  : x(_x), y(_y), z(_z), w(_w)
#endif
{
}

// explicit inline 
Vector4::Vector4(float wxyz) 
#if defined(XO_SSE)
  : reg(_mm_set1_ps(wxyz))
#elif defined(XO_NEON)
  : n(vdupq_n_f32(wxyz))
#else
  : x(wxyz), y(wxyz), z(wxyz), w(wxyz)
#endif
{
}

// inline 
Vector4::Vector4(const Vector4& other)
#if defined(XO_SIMD)
  : reg(other.reg)
#else
  : x(other.x), y(other.y), z(other.z), w(other.w)
#endif
{
}


// inline 
void Vector4::Set(float _x, float _y, float _z, float _w) {
#if defined(XO_SSE)
  reg = _mm_set_ps(_w, _z, _y, _x);
#else
  this->x = _x;
  this->y = _y;
  this->z = _z;
  this->w = _w;
#endif
}

// inline 
float& Vector4::operator [](int i) {
  return v[i];
}

// inline 
const float& Vector4::operator [](int i) const {
  return v[i];
}

// inline 
Vector4 Vector4::operator -() const {
#if defined(XO_SSE)
  return Vector4(_mm_mul_ps(reg, simd::NegativeOne));
#elif defined(XO_NEON)
  return Vector4(vnegq_f32(reg));
#else
  return Vector4(-x, -y, -z, -w);
#endif
}

// inline 
Vector4 Vector4::operator ~() const {
  return WZYX();
}


// inline 
Vector4& Vector4::operator += (const Vector4& other) {
#if defined(XO_SIMD)
  simd::Add(*this, other, *this);
#else
  x += other.x;
  y += other.y;
  z += other.z;
  w += other.w;
#endif
  return *this;
}

// inline 
Vector4 Vector4::operator + (const Vector4& other) const {
  return Vector4(*this) += other;
}

// inline 
Vector4& Vector4::operator -= (const Vector4& other) {
#if defined(XO_SIMD)
  simd::Subtract(*this, other, *this);
#else
  x -= other.x;
  y -= other.y;
  z -= other.z;
  w -= other.w;
#endif
  return *this;
}

// inline 
Vector4 Vector4::operator - (const Vector4& other) const {
  return Vector4(*this) -= other;
}

// inline 
Vector4& Vector4::operator *= (const Vector4& other) {
#if defined(XO_SIMD)
  simd::Multiply(*this, other, *this);
#else
  x *= other.x;
  y *= other.y;
  z *= other.z;
  w *= other.w;
#endif
  return *this;
}

// inline 
Vector4 Vector4::operator * (const Vector4& other) const {
  return Vector4(*this) *= other;
}

// inline 
Vector4& Vector4::operator /= (const Vector4& other) {
#if defined(XO_SIMD)
  simd::Divide(*this, other, *this);
#else
  x /= other.x;
  y /= other.y;
  z /= other.z;
  w /= other.w;
#endif
  return *this;
}

// inline 
Vector4 Vector4::operator / (const Vector4& other) const {
  return Vector4(*this) /= other;
}

// inline 
Vector4& Vector4::operator += (float f) {
  Vector4 other(f);
  return *this += other;
}

// inline 
Vector4  Vector4::operator + (float f) const {
  Vector4 other(f);
  return *this + other;
}

// inline 
Vector4& Vector4::operator -= (float f) {
  Vector4 other(f);
  return *this -= other;
}

// inline 
Vector4  Vector4::operator - (float f) const {
  Vector4 other(f);
  return *this - other;
}

// inline 
Vector4& Vector4::operator *= (float f) {
  Vector4 other(f);
  return *this *= other;
}

// inline 
Vector4  Vector4::operator * (float f) const {
  Vector4 other(f);
  return *this * other;
}

// inline 
Vector4& Vector4::operator /= (float f) {
  Vector4 other(f);
  return *this /= other;
}

// inline 
Vector4  Vector4::operator / (float f) const {
  Vector4 other(f);
  return *this / other;
}

// inline 
bool Vector4::operator < (const Vector4& v) const {
  return MagnitudeSquared() < v.MagnitudeSquared();
}

// inline 
bool Vector4::operator <= (const Vector4& v) const {
  return MagnitudeSquared() <= v.MagnitudeSquared();
}

// inline 
bool Vector4::operator > (const Vector4& v) const {
  return MagnitudeSquared() > v.MagnitudeSquared();
}

// inline 
bool Vector4::operator >= (const Vector4& v) const {
  return MagnitudeSquared() >= v.MagnitudeSquared();
}

// inline 
bool Vector4::operator == (const Vector4& other) const {
#if defined(XO_SSE2)
  return (_mm_movemask_ps(
    _mm_cmplt_ps(
      simd::Abs(_mm_sub_ps(other, reg)),
      simd::VectorEpsilon)) & 15) == 15;
#elif XO_SSE
  return CloseEnough(x, other.x, simd::Epsilon) 
    && CloseEnough(y, other.y, simd::Epsilon) 
    && CloseEnough(z, other.z, simd::Epsilon)
    && CloseEnough(w, other.w, simd::Epsilon);
#else
  return CloseEnough(x, v.x, Epsilon) 
    && CloseEnough(y, v.y, Epsilon)
    && CloseEnough(z, v.z, Epsilon)
    && CloseEnough(w, v.w, Epsilon);
#endif
}

// inline 
bool Vector4::operator != (const Vector4& other) const {
  return !(*this == other);
}


// inline 
float Vector4::Magnitude() const {
  return Sqrt(MagnitudeSquared());
}

// inline 
float Vector4::MagnitudeSquared() const {
  return ((*this) * (*this)).Sum();
}

// inline 
void Vector4::Normalize() {
  *this /= Magnitude();
}

// inline 
Vector4 Vector4::Normalized() const {
  Vector4 norm = *this;
  norm.Normalize();
  return norm;
}

// inline 
float Vector4::Sum() const {
#if defined(XO_SSE3)
  VectorRegister_t _x = _mm_hadd_ps(reg, reg);
  _x = _mm_hadd_ps(_x, _x);
  return _mm_cvtss_f32(_x);
#else
  return x + y + z + w;
#endif
}

// inline 
Vector4 Vector4::WZYX() const {
#if defined(XO_SSE)
  return Vector4(_mm_shuffle_ps(reg, reg, _MM_SHUFFLE(0, 1, 2, 3)));
#else
  return Vector4(w, z, y, x);
#endif
}

#ifdef XO_SIMD
// inline 
Vector4::Vector4(VectorRegister_t _reg)
: reg(_reg) {

}

// inline 
Vector4::operator VectorRegister_t&() {
  return reg;
}

// inline 
Vector4::operator const VectorRegister_t&() const {
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