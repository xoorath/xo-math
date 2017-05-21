//////////////////////////////////////////////////////////////////////
// Matrix4x4.h 
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
#include "Vector4.h"

XO_NAMESPACE_BEGIN

class XO_ALIGN Matrix4x4 {
public:
  XO_OVERLOAD_NEW_DELETE(); // for alignment

  inline Matrix4x4();
  inline Matrix4x4(float m00, float m01, float m02, float m03,
                   float m10, float m11, float m12, float m13,
                   float m20, float m21, float m22, float m23,
                   float m30, float m31, float m32, float m33);
  inline Matrix4x4(const Matrix4x4& other);

  inline const Vector4& operator [](size_t i) const;
  inline Vector4 operator [](size_t i);
  inline const float& operator ()(size_t r, size_t c) const;
  inline float& operator ()(size_t r, size_t c);
  inline Matrix4x4 operator ~() const;

  inline Matrix4x4& operator *= (const Matrix4x4& m);
  inline Matrix4x4 operator * (const Matrix4x4& m) const;
  inline Vector4 operator * (const Vector4& v) const;

  inline void Transpose();
  inline Matrix4x4 GetTranspose() const;
  inline void MakeInverse();
  inline Matrix4x4 GetInverse() const;
  inline bool TryMakeInverse();
  inline bool TryGetInverse(Matrix4x4& outMatrix);

  static inline Matrix4x4 Scale(float x, float y, float z);
  static inline Matrix4x4 RotationX(float roation);
  static inline Matrix4x4 RotationY(float roation);
  static inline Matrix4x4 RotationZ(float roation);
  static inline Matrix4x4 Rotation(float x, float y, float z);
  static inline Matrix4x4 Translation(float x, float y, float z);

#ifdef _MSC_VER
#   pragma warning( push )
// C4201: nonstandard extension used: nameless struct/union
#   pragma warning( disable : 4201)
#endif

  union {
    float v[16];
    Vector4 r[4];
  };

#ifdef _MSC_VER
#   pragma warning( pop ) 
#endif

  static const Matrix4x4 Identity;
};

// inline
Matrix4x4::Matrix4x4() {
}

// inline
Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03,
                     float m10, float m11, float m12, float m13,
                     float m20, float m21, float m22, float m23,
                     float m30, float m31, float m32, float m33) {
  r[0].Set(m00, m01, m02, m03);
  r[1].Set(m10, m11, m12, m13);
  r[2].Set(m20, m21, m22, m23);
  r[3].Set(m30, m31, m32, m33);
}

// inline
Matrix4x4::Matrix4x4(const Matrix4x4& other) {
  r[0] = other[0];
  r[1] = other[1];
  r[2] = other[2];
  r[3] = other[3];
}

// inline
const Vector4& Matrix4x4::operator [](size_t i) const {
  return r[i];
}

// inline
Vector4 Matrix4x4::operator [](size_t i) {
  return r[i];
}

// inline
const float& Matrix4x4::operator ()(size_t _r, size_t c) const {
  return r[_r][c];
}

// inline
float& Matrix4x4::operator ()(size_t _r, size_t c) {
  return r[_r][c];
}

// inline
Matrix4x4 Matrix4x4::operator ~() const {
  return GetTranspose();
}

// inline
Matrix4x4& Matrix4x4::operator *= (const Matrix4x4& m) {
  Matrix4x4 t = m.GetTranspose();
  return (*this) = Matrix4x4(
    (r[0] * t[0]).Sum(), (r[0] * t[1]).Sum(), (r[0] * t[2]).Sum(), (r[0] * t[3]).Sum(),
    (r[1] * t[0]).Sum(), (r[1] * t[1]).Sum(), (r[1] * t[2]).Sum(), (r[1] * t[3]).Sum(),
    (r[2] * t[0]).Sum(), (r[2] * t[1]).Sum(), (r[2] * t[2]).Sum(), (r[2] * t[3]).Sum(),
    (r[3] * t[0]).Sum(), (r[3] * t[1]).Sum(), (r[3] * t[2]).Sum(), (r[3] * t[3]).Sum()
  );
}

// inline
Matrix4x4 Matrix4x4::operator * (const Matrix4x4& m) const {
  Matrix4x4 mat = *this;
  mat *= m;
  return mat;
}

// inline
Vector4 Matrix4x4::operator * (const Vector4& v) const {
  return Vector4((r[0] * v).Sum(), (r[1] * v).Sum(), (r[2] * v).Sum(), (r[3] * v).Sum());
}

namespace
{
#if defined(XO_SSE)
  inline
  void EarlyInverse(VectorRegister_t& minor0, VectorRegister_t& minor1, VectorRegister_t& minor2, VectorRegister_t& minor3,
                    VectorRegister_t& row0, VectorRegister_t& row1, VectorRegister_t& row2, VectorRegister_t& row3,
                    VectorRegister_t& det, VectorRegister_t& tmp1, float m[16]) {
    // http://download.intel.com/design/PentiumIII/sml/24504301.pdf
    // http://stackoverflow.com/questions/18743531/sse-half-loads-mm-loadh-pi-mm-loadl-pi-issue-warnings
    tmp1 = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), (__m64*)(m)), (__m64*)(m + 4));
    row1 = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), (__m64*)(m + 8)), (__m64*)(m + 12));
    row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
    row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);
    tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(m + 2)), (__m64*)(m + 6));
    row3 = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), (__m64*)(m + 10)), (__m64*)(m + 14));
    row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
    row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);
    // -----------------------------------------------
    tmp1 = _mm_mul_ps(row2, row3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
    minor0 = _mm_mul_ps(row1, tmp1);
    minor1 = _mm_mul_ps(row0, tmp1);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
    minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
    minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
    minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
    // -----------------------------------------------
    tmp1 = _mm_mul_ps(row1, row2);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
    minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
    minor3 = _mm_mul_ps(row0, tmp1);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
    minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
    minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
    minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
    // -----------------------------------------------
    tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
    row2 = _mm_shuffle_ps(row2, row2, 0x4E);
    minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
    minor2 = _mm_mul_ps(row0, tmp1);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
    minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
    minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
    minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
    // -----------------------------------------------
    tmp1 = _mm_mul_ps(row0, row1);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
    minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
    minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
    minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
    minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));
    // -----------------------------------------------
    tmp1 = _mm_mul_ps(row0, row3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
    minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
    minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
    minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
    minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));
    // -----------------------------------------------
    tmp1 = _mm_mul_ps(row0, row2);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
    minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
    minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
    minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
    minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);
    // -----------------------------------------------
    det = _mm_mul_ps(row0, minor0);
    det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
    det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
  }

  inline 
  void LateInverse(VectorRegister_t& minor0, VectorRegister_t& minor1, VectorRegister_t& minor2, VectorRegister_t& minor3,
                   VectorRegister_t& det, VectorRegister_t& tmp1, float m[16]) {
    // http://download.intel.com/design/PentiumIII/sml/24504301.pdf
    tmp1 = _mm_rcp_ss(det);
    det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
    det = _mm_shuffle_ps(det, det, 0x00);
    minor0 = _mm_mul_ps(det, minor0);
    _mm_storel_pi((__m64*)(m), minor0);
    _mm_storeh_pi((__m64*)(m + 2), minor0);
    minor1 = _mm_mul_ps(det, minor1);
    _mm_storel_pi((__m64*)(m + 4), minor1);
    _mm_storeh_pi((__m64*)(m + 6), minor1);
    minor2 = _mm_mul_ps(det, minor2);
    _mm_storel_pi((__m64*)(m + 8), minor2);
    _mm_storeh_pi((__m64*)(m + 10), minor2);
    minor3 = _mm_mul_ps(det, minor3);
    _mm_storel_pi((__m64*)(m + 12), minor3);
    _mm_storeh_pi((__m64*)(m + 14), minor3);
  }
#else
  inline 
  void EarlyInverse(float tmp[12], float src[16], float& det, float m[16]) {
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

  inline 
  void LateInverse(float &det, float m[16]) {
    // http://download.intel.com/design/PentiumIII/sml/24504301.pdf
    // calculate matrix inverse 
    det = 1.0f / det;
    for (int j = 0; j < 16; j++) {
      m[j] *= det;
    }
  }
#endif
}

// inline
void Matrix4x4::Transpose() {
#if defined(XO_SSE)
  _MM_TRANSPOSE4_PS(r[0], r[1], r[2], r[3]);
#else
  float t;
#   define XO_TRANSPOSE_SWAP(i,j) t = r[i][j]; r[i][j] = r[j][i]; r[j][i] = t;
  // The number of swaps should be:
  //      (((n*n)-n)/2) = 6
  //      (((4*4)-4)/2) = 6
  //      (((all elements)-identity)/(two elements processed per swap)) = 6
  XO_TRANSPOSE_SWAP(0, 1);
  XO_TRANSPOSE_SWAP(0, 2);
  XO_TRANSPOSE_SWAP(0, 3);
  XO_TRANSPOSE_SWAP(1, 2);
  XO_TRANSPOSE_SWAP(1, 3);
  XO_TRANSPOSE_SWAP(2, 3);
#   undef XO_TRANSPOSE_SWAP
#endif
}

// inline
Matrix4x4 Matrix4x4::GetTranspose() const {
  Matrix4x4 mat = *this;
  mat.Transpose();
  return mat;
}

// inline
void Matrix4x4::MakeInverse() {
#if defined(XO_SSE)
  VectorRegister_t minor0, minor1, minor2, minor3;
  VectorRegister_t row0, row1, row2, row3;
  VectorRegister_t det, tmp1;
  EarlyInverse(minor0, minor1, minor2, minor3, row0, row1, row2, row3, det, tmp1, v);
  LateInverse(minor0, minor1, minor2, minor3, det, tmp1, v);
#else
  float tmp[12]; // temp array for pairs
  float src[16]; // array of transpose source matrix
  float det; // determinant
  EarlyInverse(tmp, src, det, v);
  LateInverse(det, v);
#endif
}

// inline
Matrix4x4 Matrix4x4::GetInverse() const {
  Matrix4x4 mat = *this;
  mat.MakeInverse();
  return mat;
}

// inline
bool Matrix4x4::TryMakeInverse() {
#if defined(XO_SSE)
  VectorRegister_t minor0, minor1, minor2, minor3;
  VectorRegister_t row0, row1, row2, row3;
  VectorRegister_t det, tmp1;
  EarlyInverse(minor0, minor1, minor2, minor3, row0, row1, row2, row3, det, tmp1, v);
  if (_mm_cvtss_f32(det) == 0.0f) {
    return false;
  }
  LateInverse(minor0, minor1, minor2, minor3, det, tmp1, v);
#else
  float tmp[12]; // temp array for pairs
  float src[16]; // array of transpose source matrix
  float det; // determinant
  EarlyInverse(tmp, src, det, v);
  if(det == 0.0f) {
    return false;
  }
  LateInverse(det, v);
#endif
  return true;
}

// inline
bool Matrix4x4::TryGetInverse(Matrix4x4& outMatrix) {
  outMatrix = *this;
  return outMatrix.TryMakeInverse();
}

// static inline
Matrix4x4 Matrix4x4::Scale(float x, float y, float z) {
  return Matrix4x4(x,    0.0f, 0.0f, 0.0f,
                   0.0f, y,    0.0f, 0.0f,
                   0.0f, 0.0f, z,    0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

// static inline
Matrix4x4 Matrix4x4::RotationX(float rotation) {
  float s, c;
  SinCos(rotation, s, c);
  return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, c,    -s,   0.0f,
                   0.0f, s,    c,    0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

// static inline
Matrix4x4 Matrix4x4::RotationY(float rotation) {
  float s, c;
  SinCos(rotation, s, c);
  return Matrix4x4(c,    0.0f, -s,   0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   s,    0.0f, c,    0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

// static inline
Matrix4x4 Matrix4x4::RotationZ(float rotation) {
  float s, c;
  SinCos(rotation, s, c);
  return Matrix4x4(c,    -s,   0.0f, 0.0f,
                   s,    c,    0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

// static inline
Matrix4x4 Matrix4x4::Rotation(float x, float y, float z) {
  Vector4 r(x, y, z, 0.0f);
  Vector4 s, c;
  SinCos4(r, s, c);
  return Matrix4x4(c.y*c.z,              -c.y*s.z,            s.y,      0.0f,
                   c.z*s.x*s.y+c.x*s.z,  c.x*c.z-s.x*s.y*s.z, -c.y*s.x, 0.0f,
                   -c.x*c.z*s.y+s.x*s.z, c.z*s.x+c.x*s.y*s.z, c.x*c.y,  0.0f,
                   0.0f,                 0.0f,                0.0f,     1.0f);
}

// static inline
Matrix4x4 Matrix4x4::Translation(float x, float y, float z) {
  return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   x,    y,    z,    1.0f);
}

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