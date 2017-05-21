//////////////////////////////////////////////////////////////////////
// Quaternion.h 
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
#include "Vector3.h"
#include "Vector4.h"

XO_NAMESPACE_BEGIN

class XO_ALIGN Quaternion {
public:
  XO_OVERLOAD_NEW_DELETE(); // for alignment

  inline Quaternion() { }
  inline Quaternion(float i, float j, float k, float r);
  inline Quaternion(const Quaternion& other);

  inline void       Normalize();
  inline Quaternion Normalized() const;

  inline void DecomposeRotation(Vector3& axis, float& angle) const;

  static inline Quaternion RotationAxisAngle(const Vector3& axis, float angle);
  static inline Quaternion RotationEuler(const Vector3& euler);

#ifdef XO_SIMD
  inline Quaternion(VectorRegister_t reg);
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
      float i, j, k, r;
    };
    float v[4];
#ifdef XO_SIMD
    VectorRegister_t reg;
#endif
  };

#ifdef _MSC_VER
#   pragma warning( pop ) 
#endif

  static const Quaternion Identity;
};

// inline
Quaternion::Quaternion(float _i, float _j, float _k, float _r)
#if defined(XO_SSE)
: reg(_mm_set_ps(_r, _k, _j, _i))
#else
: i(_i), j(_j), k(_k), r(_r)
#endif
{
}

// inline
Quaternion::Quaternion(const Quaternion& other) 
#if defined(XO_SIMD)
: reg(other.reg)
#else
: i(other.i), j(other.j), k(other.k), r(other.r)
#endif
{
}

// inline
void Quaternion::Normalize() {
#if defined(XO_SIMD)
  VectorRegister_t temp;
  simd::Square(*this, temp);
  float magnitude = simd::SumXYZW(temp);
#else
  float magnitude = i*i + j*j + k*k + r*r;
#endif
  magnitude = Sqrt(magnitude);
  (*(Vector4*)this) /= magnitude;
}

// inline
Quaternion Quaternion::Normalized() const {
  Quaternion quat = *this;
  quat.Normalize();
  return quat;
}

// inline
void Quaternion::DecomposeRotation(Vector3& axis, float& angle) const {
  Quaternion q = Normalized();
#if defined(XO_SIMD)
  // todo: don't we need to normalize axis in sse too?
  axis.reg = q.reg;
#else
  axis.Set(q.i, w.j, q.k);
  axis.Normalize();
#endif
  angle = (2.0f * ACos(q.r));
}

// static inline
Quaternion Quaternion::RotationAxisAngle(const Vector3& axis, float angle) {
  Quaternion rot;
  float hr = angle * 0.5f;
  float sr = Sin(hr);

  Vector3 n = axis.Normalized();
  n *= sr;
#ifdef XO_SIMD
  rot.reg = n.reg;
#else
  rot.i = n.x;
  rot.j = n.y;
  rot.k = n.z;
#endif
  rot.r =  Cos(angle);
  return rot;
}

// static inline
Quaternion Quaternion::RotationEuler(const Vector3& euler) {
  Vector3 hv = euler * 0.5f;
  Vector3 s;
  Vector3 c;
  SinCos3(hv, s, c);
  return Quaternion(
    c[0] * c[1] * c[2] + s[0] * s[1] * s[2],
    s[0] * c[1] * c[2] - c[0] * s[1] * s[2],
    c[0] * s[1] * c[2] + s[0] * c[1] * s[2],
    c[0] * c[1] * s[2] - s[0] * s[1] * c[2]);
}

#ifdef XO_SIMD
// inline
Quaternion::Quaternion(VectorRegister_t _reg) 
: reg(_reg)
{
}

// inline
Quaternion::operator VectorRegister_t&() {
  return reg;
}

// inline
Quaternion::operator const VectorRegister_t&() const {
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