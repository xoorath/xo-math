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

#define _XO_MATH_OBJ
#include "xo-math.h"

XOMATH_BEGIN_XO_NS();


////////////////////////////////////////////////////////////////////////// Matrix4x4.cpp

const Matrix4x4 Matrix4x4::Identity(Vector4(1.0f, 0.0f, 0.0f, 0.0f),
                                    Vector4(0.0f, 1.0f, 0.0f, 0.0f),
                                    Vector4(0.0f, 0.0f, 1.0f, 0.0f),
                                    Vector4(0.0f, 0.0f, 0.0f, 1.0f));

const Matrix4x4 Matrix4x4::One(Vector4(1.0f, 1.0f, 1.0f, 1.0f),
                               Vector4(1.0f, 1.0f, 1.0f, 1.0f),
                               Vector4(1.0f, 1.0f, 1.0f, 1.0f),
                               Vector4(1.0f, 1.0f, 1.0f, 1.0f));

const Matrix4x4 Matrix4x4::Zero(Vector4(0.0f, 0.0f, 0.0f, 0.0f),
                                Vector4(0.0f, 0.0f, 0.0f, 0.0f),
                                Vector4(0.0f, 0.0f, 0.0f, 0.0f),
                                Vector4(0.0f, 0.0f, 0.0f, 0.0f));

Matrix4x4::Matrix4x4() {
}

Matrix4x4::Matrix4x4(float m)
#if defined(XO_SSE)
{ 
    r[0].xmm = r[1].xmm = r[2].xmm = r[3].xmm = _mm_set_ps1(m);
}
#else
{
	r[0].Set(m);
	r[1].Set(m);
	r[2].Set(m);
	r[3].Set(m);
}
#endif

Matrix4x4::Matrix4x4(float a0, float b0, float c0, float d0, float a1, float b1, float c1, float d1, float a2, float b2, float c2, float d2, float a3, float b3, float c3, float d3)
{
    r[0].Set(a0, b0, c0, d0);
    r[1].Set(a1, b1, c1, d1);
    r[2].Set(a2, b2, c2, d2);
    r[3].Set(a3, b3, c3, d3);
}

// TODO: couldn't this be faster with just a single copy?
Matrix4x4::Matrix4x4(const Matrix4x4& m)
{
	r[0].Set(m.r[0]);
	r[1].Set(m.r[1]);
	r[2].Set(m.r[2]);
	r[3].Set(m.r[3]);
}

Matrix4x4::Matrix4x4(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3)
{
	r[0].Set(r0);
	r[1].Set(r1);
	r[2].Set(r2);
	r[3].Set(r3);
}

Matrix4x4::Matrix4x4(const Vector3& r0, const Vector3& r1, const Vector3& r2)
{
	r[0].Set(r0);
	r[1].Set(r1);
	r[2].Set(r2);
	r[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4::Matrix4x4(const class Quaternion& q) {
    Vector4* v4 = (Vector4*)&q;
    Vector4 q2 = *v4 + *v4;

    Vector4 qq2 = (*v4) * q2;
    Vector4 wq2 = q2 * q.w;

    float xy2 = q.x * q2.y;
    float xz2 = q.x * q2.z;
    float yz2 = q.y * q2.z;

    r[0].Set( 1.0f - qq2.y - qq2.z,  xy2 + wq2.z,            xz2 - wq2.y,          0.0f);
    r[1].Set( xy2 - wq2.z,           1.0f - qq2.x - qq2.z,   yz2 + wq2.x,          0.0f);
    r[2].Set( xz2 + wq2.y,           yz2 - wq2.x,            1.0f - qq2.x - qq2.y, 0.0f);
    r[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
}

const Vector4& Matrix4x4::GetRow(int i) const {
    return r[i];
}

Vector4 Matrix4x4::GetColumn(int i) const {
    return Vector4(r[0][i], r[1][i], r[2][i], r[3][i]);
}

namespace
{
#if defined(XO_SSE)
    _XOINL void EarlyInverse(__m128& minor0, __m128& minor1, __m128& minor2, __m128& minor3,
                             __m128& row0, __m128& row1, __m128& row2, __m128& row3,
                             __m128& det, __m128& tmp1, float m[16])
    {
        // http://download.intel.com/design/PentiumIII/sml/24504301.pdf
        // http://stackoverflow.com/questions/18743531/sse-half-loads-mm-loadh-pi-mm-loadl-pi-issue-warnings
#   define _fake_initialization() _mm_setzero_ps()

        tmp1 = _mm_loadh_pi(_mm_loadl_pi(_fake_initialization(), (__m64*)(m)), (__m64*)(m + 4));
        row1 = _mm_loadh_pi(_mm_loadl_pi(_fake_initialization(), (__m64*)(m + 8)), (__m64*)(m + 12));
        row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
        row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);
        tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(m + 2)), (__m64*)(m + 6));
        row3 = _mm_loadh_pi(_mm_loadl_pi(_fake_initialization(), (__m64*)(m + 10)), (__m64*)(m + 14));
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
#   undef _fake_initialization
    }

    _XOINL void LateInverse(__m128& minor0, __m128& minor1, __m128& minor2, __m128& minor3,
                            __m128& det, __m128& tmp1, float m[16])
    {
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
    _XOINL void EarlyInverse(float tmp[12], float src[16], float& det, float m[16])
    {
        // http://download.intel.com/design/PentiumIII/sml/24504301.pdf
        // transpose matrix
        for (int i = 0; i < 4; i++)
        {
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

    _XOINL void LateInverse(float &det, float m[16])
    {
        // http://download.intel.com/design/PentiumIII/sml/24504301.pdf
        // calculate matrix inverse 
        det = 1 / det;
        for (int j = 0; j < 16; j++)
            m[j] *= det;
    }
#endif
}

void Matrix4x4::MakeInverse() {
#if defined(XO_SSE)
    __m128 minor0, minor1, minor2, minor3;
    __m128 row0, row1, row2, row3;
    __m128 det, tmp1;
    EarlyInverse(minor0, minor1, minor2, minor3, row0, row1, row2, row3, det, tmp1, m);
    LateInverse(minor0, minor1, minor2, minor3, det, tmp1, m);
#else
    float tmp[12]; // temp array for pairs
    float src[16]; // array of transpose source matrix
    float det; // determinant
    EarlyInverse(tmp, src, det, m);
    LateInverse(det, m);
#endif
}

bool Matrix4x4::TryMakeInverse()
{
#if defined(XO_SSE)
    __m128 minor0, minor1, minor2, minor3;
    __m128 row0, row1, row2, row3;
    __m128 det, tmp1;
    EarlyInverse(minor0, minor1, minor2, minor3, row0, row1, row2, row3, det, tmp1, m);
    if (_mm_cvtss_f32(det) == 0.0f)
        return false;
    LateInverse(minor0, minor1, minor2, minor3, det, tmp1, m);
    return true;
#else
    float tmp[12]; // temp array for pairs
    float src[16]; // array of transpose source matrix
    float det; // determinant
    EarlyInverse(tmp, src, det, m);
    if (det == 0.0f)
        return false;
    LateInverse(det, m);
    return true;
#endif
}

Matrix4x4& Matrix4x4::Transpose() {
#if defined(XO_SSE)
    _MM_TRANSPOSE4_PS(r[0].xmm, r[1].xmm, r[2].xmm, r[3].xmm);
#else
    float t;
#   define _XO_TRANSPOSE_SWAP(i,j) t = r[i][j]; r[i][j] = r[j][i]; r[j][i] = t;
    // The number of swaps should be:
    //      (((n*n)-n)/2) = 6
    //      (((4*4)-4)/2) = 6
    //      (((all elements)-identity)/(two elements processed per swap)) = 6
    _XO_TRANSPOSE_SWAP(0, 1);
    _XO_TRANSPOSE_SWAP(0, 2);
    _XO_TRANSPOSE_SWAP(0, 3);
    _XO_TRANSPOSE_SWAP(1, 2);
    _XO_TRANSPOSE_SWAP(1, 3);
    _XO_TRANSPOSE_SWAP(2, 3);
#   undef _XO_TRANSPOSE_SWAP
#endif
    return *this;
}

Matrix4x4 Matrix4x4::Transposed() const {
    Matrix4x4 m(*this);
    return m.Transpose();
}

const Matrix4x4& Matrix4x4::Transform(Vector3& v) const {
    v = (*this) * Vector4(v);
    return *this;
}

const Matrix4x4& Matrix4x4::Transform(Vector4& v) const {
    v = (*this) * v;
    return *this;
}

void Matrix4x4::Scale(float xyz, Matrix4x4& m) {
    m[0].Set(xyz,  0.0f, 0.0f, 0.0f);
    m[1].Set(0.0f, xyz,  0.0f, 0.0f);
    m[2].Set(0.0f, 0.0f, xyz,  0.0f);
    m[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
}
 
void Matrix4x4::Scale(float x, float y, float z, Matrix4x4& m) {
    m[0].Set(x,    0.0f, 0.0f, 0.0f);
    m[1].Set(0.0f, y,    0.0f, 0.0f);
    m[2].Set(0.0f, 0.0f, z,    0.0f);
    m[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
}

void Matrix4x4::Scale(const Vector3& v, Matrix4x4& m) {
    m[0].Set(v.x,  0.0f, 0.0f, 0.0f);
    m[1].Set(0.0f, v.y,  0.0f, 0.0f);
    m[2].Set(0.0f, 0.0f, v.z,  0.0f);
    m[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
}
 
void Matrix4x4::Translation(float x, float y, float z, Matrix4x4& m) {
    m[0].Set(1.0f, 0.0f, 0.0f, 0.0f);
    m[1].Set(0.0f, 1.0f, 0.0f, 0.0f);
    m[2].Set(0.0f, 0.0f, 1.0f, 0.0f);
    m[3].Set(x,    y,    z,    1.0f);
}

void Matrix4x4::Translation(const Vector3& v, Matrix4x4& m) {
    m[0].Set(1.0f, 0.0f, 0.0f, 0.0f);
    m[1].Set(0.0f, 1.0f, 0.0f, 0.0f);
    m[2].Set(0.0f, 0.0f, 1.0f, 0.0f);
    m[3].Set(v,                1.0f);
}

void Matrix4x4::RotationXRadians(float radians, Matrix4x4& m) {
    float sinr, cosr;

    SinCos(radians, sinr, cosr);
    m[0].Set(1.0f, 0.0f, 0.0f, 0.0f);
    m[1].Set(0.0f, cosr,-sinr, 0.0f);
    m[2].Set(0.0f, sinr, cosr, 0.0f);
    m[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
}
 
void Matrix4x4::RotationYRadians(float radians, Matrix4x4& m) {
    float sinr, cosr;
    SinCos(radians, sinr, cosr);
    m[0].Set(cosr, 0.0f,-sinr, 0.0f);
    m[1].Set(0.0f, 1.0f, 0.0f, 0.0f);
    m[2].Set(sinr, 0.0f, cosr, 0.0f);
    m[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
}

void Matrix4x4::RotationZRadians(float radians, Matrix4x4& m) {
    float sinr, cosr;
    SinCos(radians, sinr, cosr);
    m[0].Set(cosr,-sinr, 0.0f, 0.0f);
    m[1].Set(sinr, cosr, 0.0f, 0.0f);
    m[2].Set(0.0f, 0.0f, 1.0f, 0.0f);
    m[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
}

void Matrix4x4::RotationRadians(float x, float y, float z, Matrix4x4& m) {
    _XOSIMDALIGN float c[4];
    _XOSIMDALIGN float s[4];
    Vector4 v(x, y, z, 0.0f);
    SinCos_x4(v.f, s, c);

    m[0].Set(c[1]*c[2],                     -c[1]*s[2],                 s[1],           0.0f);
    m[1].Set(c[2]*s[0]*s[1]+c[0]*s[2],      c[0]*c[2]-s[0]*s[1]*s[2],   -c[1]*s[0],     0.0f);
    m[2].Set(-c[0]*c[2]*s[1]+s[0]*s[2],     c[2]*s[0]+c[0]*s[1]*s[2],   c[0]*c[1],      0.0f);
    m[3].Set(0.0f,                          0.0f,                       0.0f,           1.0f);
}

void Matrix4x4::RotationRadians(const Vector3& v, Matrix4x4& m) {
    _XOSIMDALIGN float c[4];
    _XOSIMDALIGN float s[4];
    SinCos_x4(v.f, s, c);

    m[0].Set(c[1]*c[2],                     -c[1]*s[2],                 s[1],           0.0f);
    m[1].Set(c[2]*s[0]*s[1]+c[0]*s[2],      c[0]*c[2]-s[0]*s[1]*s[2],   -c[1]*s[0],     0.0f);
    m[2].Set(-c[0]*c[2]*s[1]+s[0]*s[2],     c[2]*s[0]+c[0]*s[1]*s[2],   c[0]*c[1],      0.0f);
    m[3].Set(0.0f,                          0.0f,                       0.0f,           1.0f);
}

void Matrix4x4::AxisAngleRadians(const Vector3& a, float radians, Matrix4x4& m) {
    float s, c;
    SinCos(radians, s, c);
    float t = 1.0f - c;
    const float& x = a.x;
    const float& y = a.y;
    const float& z = a.z;
    m = Matrix4x4(
             t*x*x+c  ,    t*x*y-z*s,  t*x*z+y*s,  0.0f,
             t*x*y+z*s,    t*y*y+c  ,  t*y*z-x*s,  0.0f,
             t*x*z-y*s,    t*y*z+x*s,  t*z*z+c  ,  0.0f,
             0.0f,         0.0f,       0.0f,       1.0f
        );
}

void Matrix4x4::RotationXDegrees(float degrees, Matrix4x4& m) {
    RotationXRadians(degrees * Deg2Rad, m);
}

void Matrix4x4::RotationYDegrees(float degrees, Matrix4x4& m) {
    RotationYRadians(degrees * Deg2Rad, m);
}

void Matrix4x4::RotationZDegrees(float degrees, Matrix4x4& m) {
    RotationZRadians(degrees * Deg2Rad, m);
}

void Matrix4x4::RotationDegrees(float x, float y, float z, Matrix4x4& m) {
    Matrix4x4 m0, m1, m2;
    RotationXDegrees(x, m0);
    RotationYDegrees(y, m1);
    RotationZDegrees(z, m2);
    m = m0 * m1 * m2;
}

void Matrix4x4::RotationDegrees(const Vector3& v, Matrix4x4& m) {
    Matrix4x4 m0, m1, m2;
    RotationXDegrees(v.x, m0);
    RotationYDegrees(v.y, m1);
    RotationZDegrees(v.z, m2);
    m = m0 * m1 * m2;
}


void Matrix4x4::AxisAngleDegrees(const Vector3& a, float degrees, Matrix4x4& m) {
    AxisAngleRadians(a, degrees * Deg2Rad, m);
}

void Matrix4x4::OrthographicProjection(float w, float h, float n, float f, Matrix4x4& m) {
    XO_ASSERT(w != 0.0f, _XO_ASSERT_MSG("::OrthographicProjection Width (w) should not be zero."));
    XO_ASSERT(h != 0.0f, _XO_ASSERT_MSG("::OrthographicProjection Height (h) should not be zero."));
    m = Matrix4x4(
            1.0f/w,    0.0f,       0.0f,    0.0f,
            0.0f,      1.0f/h,     0.0f,    0.0f,
            0.0f,      0.0f,       f-n,     0.0f,
            0.0f,      0.0f,       n*(f-n), 1.0f
        );
}
 
void Matrix4x4::PerspectiveProjectionRadians(float fovx, float fovy, float n, float f, Matrix4x4& m) {
    XO_ASSERT(n != f, _XO_ASSERT_MSG("::PerspectiveProjectionRadians Near (n) and far (f) values should not be equal."));
    m = Matrix4x4(
            ATan(fovx/2.0f),   0.0f,               0.0f,               0.0f,
            0.0f,              ATan(fovy/2.0f),    0.0f,               0.0f,
            0.0f,              0.0f,               f/(f-n),            1.0f,
            0.0f,              0.0f,               -n*(f/-n),          1.0f
        );
}

void Matrix4x4::PerspectiveProjectionDegrees(float fovx, float fovy, float n, float f, Matrix4x4& outMatrix) {
    Matrix4x4::PerspectiveProjectionRadians(fovx * Deg2Rad, fovy * Deg2Rad, n, f, outMatrix);
}

void Matrix4x4::LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up, Matrix4x4& m) {
    Vector3 zAxis = (to - from).Normalized();
    Vector3 xAxis = Vector3::Cross(up, zAxis).Normalized();
    Vector3 yAxis = Vector3::Cross(zAxis, xAxis);
    m = Matrix4x4(
            xAxis.x,           yAxis.x,            zAxis.x,            0.0f,
            xAxis.y,           yAxis.y,            zAxis.y,            0.0f,
            xAxis.z,           yAxis.z,            zAxis.z,            0.0f,
            -xAxis.Dot(from),  -yAxis.Dot(from),   -zAxis.Dot(from),   1.0f
        );
}

void Matrix4x4::LookAtFromPosition(const Vector3& from, const Vector3& to, Matrix4x4& m) {
    LookAtFromPosition(from, to, Vector3::Up, m);
}

void Matrix4x4::LookAtFromDirection(const Vector3& direction, const Vector3& up, Matrix4x4& m) {
    Vector3 zAxis = direction.Normalized();
    Vector3 xAxis = Vector3::Cross(up, zAxis).Normalized();
    Vector3 yAxis = Vector3::Cross(zAxis, xAxis);
    m = Matrix4x4(
            xAxis.x,           yAxis.x,            zAxis.x,            0.0f,
            xAxis.y,           yAxis.y,            zAxis.y,            0.0f,
            xAxis.z,           yAxis.z,            zAxis.z,            0.0f,
            0.0f,              0.0f,               0.0f,               1.0f
        );
}

void Matrix4x4::LookAtFromDirection(const Vector3& direction, Matrix4x4& m) {
    LookAtFromDirection(direction, Vector3::Up, m);
}

Matrix4x4 Matrix4x4::Scale(float xyz) {
    Matrix4x4 m;
    Scale(xyz, m);
    return m;
}

Matrix4x4 Matrix4x4::Scale(float x, float y, float z) {
    Matrix4x4 m;
    Scale(x, y, z, m);
    return m;
}

Matrix4x4 Matrix4x4::Scale(const Vector3& v) {
    Matrix4x4 m;
    Scale(v, m);
    return m;
}

Matrix4x4 Matrix4x4::Translation(float x, float y, float z) {
    Matrix4x4 m;
    Translation(x, y, z, m);
    return m;
}

Matrix4x4 Matrix4x4::Translation(const Vector3& v) {
    Matrix4x4 m;
    Translation(v, m);
    return m;
}

Matrix4x4 Matrix4x4::RotationXRadians(float radians) {
    Matrix4x4 m;
    RotationXRadians(radians, m);
    return m;
}

Matrix4x4 Matrix4x4::RotationYRadians(float radians) {
    Matrix4x4 m;
    RotationYRadians(radians, m);
    return m;
}

Matrix4x4 Matrix4x4::RotationZRadians(float radians) {
    Matrix4x4 m;
    RotationZRadians(radians, m);
    return m;
}

Matrix4x4 Matrix4x4::RotationRadians(float x, float y, float z) {
    Matrix4x4 m;
    RotationRadians(x, y, z, m);
    return m;
}

Matrix4x4 Matrix4x4::RotationRadians(const Vector3& v) {
    Matrix4x4 m;
    RotationRadians(v, m);
    return m;
}

Matrix4x4 Matrix4x4::AxisAngleRadians(const Vector3& axis, float radians) {
    Matrix4x4 m;
    AxisAngleRadians(axis, radians, m);
    return m;
}

Matrix4x4 Matrix4x4::RotationXDegrees(float degrees) {
    Matrix4x4 m;
    RotationXDegrees(degrees, m);
    return m;
}

Matrix4x4 Matrix4x4::RotationYDegrees(float degrees) {
    Matrix4x4 m;
    RotationYDegrees(degrees, m);
    return m;
}

Matrix4x4 Matrix4x4::RotationZDegrees(float degrees) {
    Matrix4x4 m;
    RotationZDegrees(degrees, m);
    return m;
}

Matrix4x4 Matrix4x4::RotationDegrees(float x, float y, float z) {
    Matrix4x4 m;
    RotationDegrees(x, y, z, m);
    return m;
}

Matrix4x4 Matrix4x4::RotationDegrees(const Vector3& v) {
    Matrix4x4 m;
    RotationDegrees(v, m);
    return m;
}

Matrix4x4 Matrix4x4::AxisAngleDegrees(const Vector3& axis, float degrees) {
    Matrix4x4 m;
    AxisAngleDegrees(axis, degrees, m);
    return m;
}

Matrix4x4 Matrix4x4::OrthographicProjection(float w, float h, float n, float f) {
    Matrix4x4 m;
    OrthographicProjection(w, h, n, f, m);
    return m;
}

Matrix4x4 Matrix4x4::PerspectiveProjectionRadians(float fovx, float fovy, float n, float f) {
    Matrix4x4 m;
    PerspectiveProjectionRadians(fovx, fovy, n, f, m);
    return m;
}

Matrix4x4 Matrix4x4::PerspectiveProjectionDegrees(float fovx, float fovy, float n, float f) {
    Matrix4x4 m;
    PerspectiveProjectionDegrees(fovx, fovy, n, f, m);
    return m;
}

Matrix4x4 Matrix4x4::LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up) {
    Matrix4x4 m;
    LookAtFromPosition(from, to, up, m);
    return m;
}
Matrix4x4 Matrix4x4::LookAtFromPosition(const Vector3& from, const Vector3& to) {
    Matrix4x4 m;
    LookAtFromPosition(from, to, m);
    return m;
}
Matrix4x4 Matrix4x4::LookAtFromDirection(const Vector3& direction, const Vector3& up) {
    Matrix4x4 m;
    LookAtFromDirection(direction, up, m);
    return m;
}
Matrix4x4 Matrix4x4::LookAtFromDirection(const Vector3& direction) {
    Matrix4x4 m;
    LookAtFromDirection(direction, m);
    return m;
}


////////////////////////////////////////////////////////////////////////// Quaternion.cpp

#if defined(_XONOCONSTEXPR)
#   if defined(XO_SSE)
const float Quaternion::Epsilon = sse::SSEFloatEpsilon * 4.0f;
#   else
const float Quaternion::Epsilon = FloatEpsilon * 4.0f;
#   endif
#endif

const Quaternion Quaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);
const Quaternion Quaternion::Zero(0.0f, 0.0f, 0.0f, 0.0f);

namespace xo_internal
{
    _XOINL float QuaternionSquareSum(const Quaternion& q)
    {
#if defined(XO_SSE)
        __m128 square = _mm_mul_ps(q.xmm, q.xmm);
        square = _mm_hadd_ps(square, square);
        square = _mm_hadd_ps(square, square);
        return _mm_cvtss_f32(square);
#else
        return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
#endif
    }
}

Quaternion::Quaternion()
{
}

Quaternion::Quaternion(const Matrix4x4& mat)
{
    Vector3 xAxis(mat[0]);
    Vector3 yAxis(mat[1]);
    Vector3 zAxis(mat[2]);

    Vector3 scale(xAxis.Magnitude(), yAxis.Magnitude(), zAxis.Magnitude());

    // don't use close enough, skip the abs since we're all positive value.
    // todo: do we actually care about near-zero?
    if (scale.x <= FloatEpsilon || scale.y <= FloatEpsilon || scale.z <= FloatEpsilon)
    {
#if defined(XO_SSE)

#else
        w = 1.0f;
        x = y = z = 0.0f;
#endif
        return; // too close.
    }

#if defined(XO_SSE)
#   if defined(XO_NO_INVERSE_DIVISION)
    Vector3 recipScale = Vector3(_mm_div_ps(Vector4::One.xmm, scale.xmm));
#   else
    Vector3 recipScale = Vector3(_mm_rcp_ps(scale.xmm));
#   endif
#else
    Vector3 recipScale = Vector3(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
#endif
    xAxis *= recipScale.x;
    yAxis *= recipScale.y;
    zAxis *= recipScale.z;

    // Now calculate the rotation from the resulting matrix (axes).
    float trace = xAxis.x + yAxis.y + zAxis.z + 1.0f;

    if (trace > 1.0f)
    {
        float s = 0.5f / Sqrt(trace);
        _XO_ASSIGN_QUAT(
            0.25f / s,
            (yAxis.z - zAxis.y) * s,
            (xAxis.y - yAxis.x) * s,
            (zAxis.x - xAxis.z) * s);
    }
    else
    {
        if (xAxis.x > yAxis.y && xAxis.x > zAxis.z)
        {
            float s = 0.5f / Sqrt(1.0f + xAxis.x - yAxis.y - zAxis.z);
            _XO_ASSIGN_QUAT(
                (yAxis.z - zAxis.y) * s,
                0.25f / s,
                (zAxis.x + xAxis.z) * s,
                (yAxis.x + xAxis.y) * s);
        }
        else if (yAxis.y > zAxis.z)
        {
            float s = 0.5f / Sqrt(1.0f + yAxis.y - xAxis.x - zAxis.z);
            _XO_ASSIGN_QUAT(
                (zAxis.x - xAxis.z) * s,
                (yAxis.x + xAxis.y) * s,
                (zAxis.y + yAxis.z) * s,
                0.25f / s);
        }
        else
        {
            float s = 0.5f / Sqrt(1.0f + zAxis.z - xAxis.x - yAxis.y);
            _XO_ASSIGN_QUAT(
                (xAxis.y - yAxis.x) * s,
                (zAxis.x + xAxis.z) * s,
                0.25f / s,
                (zAxis.y + yAxis.z) * s);
        }
    }
}

Quaternion::Quaternion(float x, float y, float z, float w) :
#if defined(XO_SSE)
    xmm(_mm_set_ps(w, z, y, x))
#else
    x(x), y(y), z(z), w(w)
#endif
{
}

Quaternion Quaternion::Inverse() const
{
    return Quaternion(*this).MakeInverse();
}

Quaternion& Quaternion::MakeInverse()
{
    float magnitude = xo_internal::QuaternionSquareSum(*this);

    if (CloseEnough(magnitude, 1.0f, Epsilon))
    {
        return MakeConjugate();
    }
    if (CloseEnough(magnitude, 0.0f, Epsilon))
    {
        return *this;
    }

    MakeConjugate();
    (*(Vector4*)this) /= magnitude;
    return *this;
}

Quaternion Quaternion::Normalized() const
{
    return Quaternion(*this).Normalize();
}

Quaternion& Quaternion::Normalize()
{
    float magnitude = xo_internal::QuaternionSquareSum(*this);
    if (CloseEnough(magnitude, 1.0f, Epsilon))
    {
        return *this;
    }

    magnitude = Sqrt(magnitude);
    if (CloseEnough(magnitude, 0.0f, Epsilon))
    {
        return *this;
    }

    (*(Vector4*)this) /= magnitude;
    return *this;
}

Quaternion Quaternion::Conjugate() const
{
    return Quaternion(*this).MakeConjugate();
}

Quaternion& Quaternion::MakeConjugate()
{
    _XO_ASSIGN_QUAT(w, -x, -y, -z);
    return *this;
}

void Quaternion::GetAxisAngleRadians(Vector3& axis, float& radians) const
{
    Quaternion q = Normalized();

#if defined(XO_SSE)
    // todo: don't we need to normalize axis in sse too?
    axis.xmm = q.xmm;
#else
    axis.x = q.x;
    axis.y = q.y;
    axis.z = q.z;
    axis.Normalize();
#endif
    radians = (2.0f * ACos(q.w));
}

void Quaternion::RotationRadians(float x, float y, float z, Quaternion& outQuat)
{
    RotationRadians(Vector3(x, y, z), outQuat);
}

void Quaternion::RotationRadians(const Vector3& v, Quaternion& outQuat)
{
    Vector3 hv = v * 0.5f;
    _XOSIMDALIGN float s[3];
    _XOSIMDALIGN float c[3];
    SinCos_x3(hv.f, s, c);

    _XO_ASSIGN_QUAT_Q(outQuat,
                      c[0] * c[1] * c[2] + s[0] * s[1] * s[2],
                      s[0] * c[1] * c[2] - c[0] * s[1] * s[2],
                      c[0] * s[1] * c[2] + s[0] * c[1] * s[2],
                      c[0] * c[1] * s[2] - s[0] * s[1] * c[2]);
}

void Quaternion::AxisAngleRadians(const Vector3& axis, float radians, Quaternion& outQuat)
{
    float hr = radians * 0.5f;
    float sr = Sin(hr);

    Vector3 n = axis.Normalized();
    n *= sr;
    _XO_ASSIGN_QUAT_Q(outQuat, Cos(radians), n.x, n.y, n.z);
}

void Quaternion::LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up, Quaternion& outQuat)
{
    LookAtFromDirection(to - from, up, outQuat);
}

void Quaternion::LookAtFromPosition(const Vector3& from, const Vector3& to, Quaternion& outQuat)
{
    LookAtFromPosition(from, to, Vector3::Up, outQuat);
}

void Quaternion::LookAtFromDirection(const Vector3&, const Vector3&, Quaternion&)
{
    // Todo
}

void Quaternion::LookAtFromDirection(const Vector3& direction, Quaternion& outQuat)
{
    LookAtFromDirection(direction, Vector3::Up, outQuat);
}

void Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t, Quaternion& outQuat)
{
    //      The folowing copyright and licence applies to the contents of this Quaternion::Slerp method

    //      Copyright 2013 BlackBerry Inc.
    //      Licensed under the Apache License, Version 2.0 (the "License");
    //      you may not use this file except in compliance with the License.
    //      You may obtain a copy of the License at
    //      http://www.apache.org/licenses/LICENSE-2.0
    //      Unless required by applicable law or agreed to in writing, software
    //      distributed under the License is distributed on an "AS IS" BASIS,
    //      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    //      See the License for the specific language governing permissions and
    //      limitations under the License.

    //      Original file from GamePlay3D: http://gameplay3d.org

    //      Modified by Jared Thomson in 2016:
    //        - removed assertions
    //        - made compatable with xo-math

    // Fast slerp implementation by kwhatmough:
    // It contains no division operations, no trig, no inverse trig
    // and no sqrt. Not only does this code tolerate small constraint
    // errors in the input quaternions, it actually corrects for them.
    if (CloseEnough(t, 0.0f))
    {
        outQuat = a;
        return;
    }
    else if (CloseEnough(t, 1.0f))
    {
        outQuat = b;
        return;
    }
    else if (a == b)
    {
        outQuat = a;
    }
    else
    {
        float halfY, alpha, beta;
        float u, f1, f2a, f2b;
        float ratio1, ratio2;
        float halfSecHalfTheta, versHalfTheta;
        float sqNotU, sqU;

        Vector4 * va = (Vector4*)&a;
        Vector4 * vb = (Vector4*)&b;

        float cosTheta = ((*va) * (*vb)).Sum();

        // As usual in all slerp implementations, we fold theta.
        alpha = cosTheta >= 0 ? 1.0f : -1.0f;
        halfY = 1.0f + alpha * cosTheta;

        // Here we bisect the interval, so we need to fold t as well.
        f2b = t - 0.5f;
        u = f2b >= 0 ? f2b : -f2b;
        f2a = u - f2b;
        f2b += u;
        u += u;
        f1 = 1.0f - u;

        // One iteration of Newton to get 1-cos(theta / 2) to good accuracy.
        halfSecHalfTheta = 1.09f - (0.476537f - 0.0903321f * halfY) * halfY;
        halfSecHalfTheta *= 1.5f - halfY * halfSecHalfTheta * halfSecHalfTheta;
        versHalfTheta = 1.0f - halfY * halfSecHalfTheta;

        // Evaluate series expansions of the coefficients.
        sqNotU = f1 * f1;
        ratio2 = 0.0000440917108f * versHalfTheta;
        ratio1 = -0.00158730159f + (sqNotU - 16.0f) * ratio2;
        ratio1 = 0.0333333333f + ratio1 * (sqNotU - 9.0f) * versHalfTheta;
        ratio1 = -0.333333333f + ratio1 * (sqNotU - 4.0f) * versHalfTheta;
        ratio1 = 1.0f + ratio1 * (sqNotU - 1.0f) * versHalfTheta;

        sqU = u * u;
        ratio2 = -0.00158730159f + (sqU - 16.0f) * ratio2;
        ratio2 = 0.0333333333f + ratio2 * (sqU - 9.0f) * versHalfTheta;
        ratio2 = -0.333333333f + ratio2 * (sqU - 4.0f) * versHalfTheta;
        ratio2 = 1.0f + ratio2 * (sqU - 1.0f) * versHalfTheta;

        // Perform the bisection and resolve the folding done earlier.
        f1 *= ratio1 * halfSecHalfTheta;
        f2a *= ratio2;
        f2b *= ratio2;
        alpha *= f1 + f2a;
        beta = f1 + f2b;

        // Apply final coefficients to a and b as usual.
        float w = alpha * a.w + beta * b.w;
        float x = alpha * a.x + beta * b.x;
        float y = alpha * a.y + beta * b.y;
        float z = alpha * a.z + beta * b.z;

        // This final adjustment to the quaternion's length corrects for
        // any small constraint error in the inputs q1 and q2 But as you
        // can see, it comes at the cost of 9 additional multiplication
        // operations. If this error-correcting feature is not required,
        // the following code may be removed.
        f1 = 1.5f - 0.5f * (w * w + x * x + y * y + z * z);
        _XO_ASSIGN_QUAT_Q(outQuat, w * f1, x * f1, y * f1, z * f1);
    }
}

void Quaternion::Lerp(const Quaternion& a, const Quaternion& b, float t, Quaternion& outQuat)
{
    Vector4& vq = (Vector4&)outQuat;
    const Vector4& va = a;
    const Vector4& vb = b;
    // Todo: give the vectors the same static 'outQuat' type interface, then just use the Vector4::Lerp for this lerp
    vq = va + (vb - va) * t;
}


////////////////////////////////////////////////////////////////////////// SSE.cpp

#if defined(XO_SSE)
namespace sse {

    volatile _XOTLS unsigned LastKnownControlWord = 0;

    void UpdateControlWord() {
        LastKnownControlWord = _mm_getcsr();
    }

    void SetControlWord(unsigned control) {
        _mm_setcsr(LastKnownControlWord = control);
    }

    void SetControlWordAddative(unsigned control) {
        _mm_setcsr(LastKnownControlWord |= control);
    }

    void RemoveControlWord(unsigned control) {
        _mm_setcsr(LastKnownControlWord &= ~control);
    }

    bool HasControlFlagBeenSet(unsigned flags, bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        if((LastKnownControlWord & flags) == flags) {
            if(thenFlush) {
                RemoveControlWord(flags);
            }
            return true;
        }
        return false;
    }

    bool HasControlFlagBeenSet(mxcsr::Flags flags, bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        return HasControlFlagBeenSet((unsigned)flags, withUpdate, thenFlush);
    }

    bool HasInvalidOperationExceptionOccured(bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        return HasControlFlagBeenSet(mxcsr::Flags::InvalidOperation, withUpdate, thenFlush);
    }

    bool HasDenormalExceptionOccured(bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        return HasControlFlagBeenSet(mxcsr::Flags::Denormal, withUpdate, thenFlush);
    }

    bool HasDivideByZeroExceptionOccured(bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        return HasControlFlagBeenSet(mxcsr::Flags::DivideByZero, withUpdate, thenFlush);
    }

    bool HasOverflowExceptionOccured(bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        return HasControlFlagBeenSet(mxcsr::Flags::Overflow, withUpdate, thenFlush);
    }

    bool HasUnderflowExceptionOccured(bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        return HasControlFlagBeenSet(mxcsr::Flags::Underflow, withUpdate, thenFlush);
    }

    bool HasPrecisionExceptionOccured(bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        return HasControlFlagBeenSet(mxcsr::Flags::Precision, withUpdate, thenFlush);
    }

    void SetControlMask(unsigned mask, bool value, bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        if(value) {
            SetControlWordAddative(mask);
        }
        else {
            RemoveControlWord(mask);
        }
    }

    void SetControlMask(mxcsr::Masks mask, bool value, bool withUpdate/*= false*/) {
        SetControlMask((unsigned)mask, value, withUpdate);
    }

    bool GetControlMask(unsigned mask, bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        return (LastKnownControlWord & mask) == mask;
    }

    bool GetControlMask(mxcsr::Masks mask, bool withUpdate/*= false*/) {
        return GetControlMask((unsigned)mask, withUpdate);
    }

    void SetInvalidOperationExceptionMask(bool value, bool withUpdate/*= false*/) {
        SetControlMask(mxcsr::Masks::InvalidOperation, value, withUpdate);
    }

    void SetDenormalExceptionMask(bool value, bool withUpdate/*= false*/) {
        SetControlMask(mxcsr::Masks::Denormal, value, withUpdate);
    }

    void SetDivideByZeroExceptionMask(bool value, bool withUpdate/*= false*/) {
        SetControlMask(mxcsr::Masks::DivideByZero, value, withUpdate);
    }

    void SetOverflowExceptionMask(bool value, bool withUpdate/*= false*/) {
        SetControlMask(mxcsr::Masks::Overflow, value, withUpdate);
    }

    void SetUnderflowExceptionMask(bool value, bool withUpdate/*= false*/) {
        SetControlMask(mxcsr::Masks::Underflow, value, withUpdate);
    }

    void SetPrecisionExceptionMask(bool value, bool withUpdate/*= false*/) {
        SetControlMask(mxcsr::Masks::Precision, value, withUpdate);
    }

    void ThrowAllExceptions(bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        SetInvalidOperationExceptionMask(false);
        SetDenormalExceptionMask(false);
        SetDivideByZeroExceptionMask(false);
        SetOverflowExceptionMask(false);
        SetUnderflowExceptionMask(false);
        SetPrecisionExceptionMask(false);
    }

    void ThrowNoExceptions(bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        SetInvalidOperationExceptionMask(true);
        SetDenormalExceptionMask(true);
        SetDivideByZeroExceptionMask(true);
        SetOverflowExceptionMask(true);
        SetUnderflowExceptionMask(true);
        SetPrecisionExceptionMask(true);
    }

    bool GetInvalidOperationExceptionMask(bool withUpdate/*= false*/) {
        return GetControlMask(mxcsr::Masks::InvalidOperation, withUpdate);
    }

    bool GetDenormalExceptionMask(bool withUpdate/*= false*/) {
        return GetControlMask(mxcsr::Masks::Denormal, withUpdate);
    }

    bool GetDivideByZeroExceptionMask(bool withUpdate/*= false*/) {
        return GetControlMask(mxcsr::Masks::DivideByZero, withUpdate);
    }

    bool GetOverflowExceptionMask(bool withUpdate/*= false*/) {
        return GetControlMask(mxcsr::Masks::Overflow, withUpdate);
    }

    bool GetUnderflowExceptionMask(bool withUpdate/*= false*/) {
        return GetControlMask(mxcsr::Masks::Underflow, withUpdate);
    }

    bool GetPrecisionExceptionMask(bool withUpdate/*= false*/) {
        return GetControlMask(mxcsr::Masks::Precision, withUpdate);
    }

    mxcsr::Rounding GetRoundingMode(bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        return (mxcsr::Rounding)(LastKnownControlWord & (unsigned)mxcsr::Rounding::Bits);
    }

    void SetRoundingMode(unsigned mode, bool withUpdate/*= false*/) {
        mode &= (unsigned)mxcsr::Rounding::Bits;
        if(withUpdate) {
            UpdateControlWord();
        }
        SetControlWordAddative(mode);
    }

    void SetRoundingMode(mxcsr::Rounding mode, bool withUpdate/*= false*/) {
        SetRoundingMode((unsigned)mode, withUpdate);
    }

    bool HasDenormalsAreZeroSet(bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        return (LastKnownControlWord & (unsigned)mxcsr::DAZ::DenormalsAreZero) == (unsigned)mxcsr::DAZ::DenormalsAreZero;
    }

    void SetDenormalsAreZero(bool value, bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        if(value) {
            SetControlWordAddative((unsigned)mxcsr::DAZ::DenormalsAreZero);
        }
        else {
            RemoveControlWord((unsigned)mxcsr::DAZ::DenormalsAreZero);
        }
    }

    bool HasFlushToZeroSet(bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        return (LastKnownControlWord & (unsigned)mxcsr::FZ::FlushToZero) == (unsigned)mxcsr::FZ::FlushToZero;
    }

    void SetFlushToZero(bool value, bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        if(value) {
            SetControlWordAddative((unsigned)mxcsr::FZ::FlushToZero);
        }
        else {
            RemoveControlWord((unsigned)mxcsr::FZ::FlushToZero);
        }
    }

    void GetAllMXCSRInfo(std::ostream& os, bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        os << "MXCSR rounding:\n";
        os << "\t";
        switch(GetRoundingMode()) {
            case mxcsr::Rounding::Nearest:
                os << "Nearest";
                break;
            case mxcsr::Rounding::Positive:
                os << "Positive";
                break;
            case mxcsr::Rounding::Negative:
                os << "Negative";
                break;
            case mxcsr::Rounding::Zero:
                os << "Zero";
                break;
        }
        os << "\n";
        os << "MXCSR masks:\n";
        os << "\t" << "InvalidOperationException: " << GetInvalidOperationExceptionMask() << "\n"; 
        os << "\t" << "DenormalException: " << GetDenormalExceptionMask() << "\n"; 
        os << "\t" << "DivideByZeroException: " << GetDivideByZeroExceptionMask() << "\n"; 
        os << "\t" << "OverflowException: " << GetOverflowExceptionMask() << "\n"; 
        os << "\t" << "UnderflowException: " << GetUnderflowExceptionMask() << "\n"; 
        os << "\t" << "PrecisionException: " << GetPrecisionExceptionMask() << "\n"; 
        os << "\t" << "DenormalsAreZero: " << HasDenormalsAreZeroSet() << "\n";
        os << "\t" << "FlushToZero: " << HasFlushToZeroSet() << "\n";
        os << "MXCSR flags:\n";
        os << "\t" << "InvalidOperationException: " << HasInvalidOperationExceptionOccured() << "\n"; 
        os << "\t" << "DenormalException: " << HasDenormalExceptionOccured() << "\n"; 
        os << "\t" << "DivideByZeroException: " << HasDivideByZeroExceptionOccured() << "\n"; 
        os << "\t" << "OverflowException: " << HasOverflowExceptionOccured() << "\n"; 
        os << "\t" << "UnderflowException: " << HasUnderflowExceptionOccured() << "\n"; 
        os << "\t" << "PrecisionException: " << HasPrecisionExceptionOccured() << "\n";
    }
}
#endif


////////////////////////////////////////////////////////////////////////// Vector2.cpp

#if defined(_XONOCONSTEXPR)
const float Vector2::Epsilon = FloatEpsilon * 2.0f;
#endif

const Vector2 Vector2::UnitX(1.0f, 0.0f);
const Vector2 Vector2::UnitY(0.0f, 1.0f);

const Vector2 Vector2::Up(0.0f, 1.0f);
const Vector2 Vector2::Down(0.0f, -1.0f);
const Vector2 Vector2::Left(-1.0f, 0.0f);
const Vector2 Vector2::Right(1.0f, 0.0f);

const Vector2 Vector2::One(1.0f, 1.0f);
const Vector2 Vector2::Zero(0.0f, 0.0f);

Vector2::Vector2(float v) : x(v), y(v) {
}

Vector2::Vector2(float x, float y) : x(x), y(y) {
}

Vector2::Vector2(const Vector2& v) : x(v.x), y(v.y) {
}

Vector2::Vector2(const class Vector3& v) : x(v.x), y(v.y) {
}

Vector2::Vector2(const class Vector4& v) : x(v.x), y(v.y) {
}

Vector2& Vector2::Set(float x, float y) {
    this->x = x;
    this->y = y;
    return *this;
}

Vector2& Vector2::Set(float v) {
    x = v;
    y = v;
    return *this;
}

Vector2& Vector2::Set(const Vector2& v) {
    x = v.x;
    y = v.y;
    return *this;
}

void Vector2::Get(float& x, float& y) const {
    x = this->x;
    y = this->y;
}
void Vector2::Get(float* f) const {
    f[0] = this->x;
    f[1] = this->y;
}




////////////////////////////////////////////////////////////////////////// Vector3.cpp

#if defined(_XONOCONSTEXPR)
#   if defined(XO_SSE)
const float Vector3::Epsilon = sse::SSEFloatEpsilon * 3.0f;
#   else
const float Vector3::Epsilon = FloatEpsilon * 3.0f;
#   endif
#endif

#if defined(XO_SSE2)
const __m128 Vector3::MASK = _mm_castsi128_ps(_mm_set_epi32(0, 0xffffffff, 0xffffffff, 0xffffffff));
#elif defined(XO_SSE)
const __m128 Vector3::MASK = {-1, -1, -1, 0};
#endif

const Vector3 Vector3::Origin(0.0f, 0.0f, 0.0f);

const Vector3 Vector3::UnitX(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UnitY(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UnitZ(0.0f, 0.0f, 1.0f);

const Vector3 Vector3::Left(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);

#if defined(XO_SPACE_ZUP)
const Vector3 Vector3::Up(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Down(0.0f, 0.0f, -1.0f);
#elif defined(XO_SPACE_YUP)
const Vector3 Vector3::Up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Down(0.0f, -1.0f, 0.0f);
#endif

#if defined(XO_SPACE_LEFTHAND)
#   if defined(XO_SPACE_ZUP)
const Vector3 Vector3::Forward(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::Backward(0.0f, 1.0f, 0.0f);
#   elif defined(XO_SPACE_YUP)
const Vector3 Vector3::Forward(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Backward(0.0f, 0.0f, -1.0f);
#   endif
#elif defined(XO_SPACE_RIGHTHAND)
#   if defined(XO_SPACE_ZUP)
const Vector3 Vector3::Forward(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Backward(0.0f, -1.0f, 0.0f);
#   elif defined(XO_SPACE_YUP)
const Vector3 Vector3::Forward(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::Backward(0.0f, 0.0f, 1.0f);
#   endif
#endif

const Vector3 Vector3::One(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);

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

Vector3::Vector3(float f) :
#if defined(XO_SSE)
    xmm(_mm_set1_ps(f))
#elif defined(XO_NEON)
    n(vdupq_n_f32(f))
#else
    x(f), y(f), z(f)
#endif
{
}

Vector3::Vector3(float x, float y, float z) :
#if defined(XO_SSE)
    xmm(_mm_set_ps(0.0f, z, y, x))
#else
    x(x), y(y), z(z)
#endif
{
}

Vector3::Vector3(const Vector3& vec) :
#if defined(XO_SSE)
    xmm(vec)
#elif defined(XO_NEON)
    n(vec.n)
#else
    x(vec.x), y(vec.y), z(vec.z) 
#endif
{
}

#if defined(XO_SSE)
Vector3::Vector3(const __m128& vec) : 
    xmm(vec)
{
}

#elif defined(XO_NEON)
Vector3::Vector3(const float32x4_t& vec) :
    n(vec)
{
}
#endif

Vector3::Vector3(const class Vector2& v) :
#if defined(XO_SSE)
    xmm(_mm_set_ps(0.0f, 0.0f, v.y, v.x))
#else
    x(v.x), y(v.y), z(0.0f)
#endif
{
}

Vector3::Vector3(const class Vector4& v) :
#if defined(XO_SSE)
    xmm(v.xmm)
//#elif defined(XO_NEON)
//    n(v.n)
#else
    x(v.x), y(v.y), z(v.z)
#endif
{
}

Vector3& Vector3::Set(float x, float y, float z) {
#if defined(XO_SSE)
    xmm = _mm_set_ps(0.0f, z, y, x);
#else
    this->x = x;
    this->y = y;
    this->z = z;
#endif
    return *this;
}

Vector3& Vector3::Set(float f) {
#if defined(XO_SSE)
    xmm = _mm_set1_ps(f);
#elif defined(XO_NEON)
    n = vdupq_n_f32(f);
#else
    this->x = f;
    this->y = f;
    this->z = f;
#endif
    return *this;
}

Vector3& Vector3::Set(const Vector3& vec) {
#if defined(XO_SSE)
    xmm = vec.xmm;
#elif defined(XO_NEON)
    n = vec.n;
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
#endif
    return *this;
}

#if defined(XO_SSE)
Vector3& Vector3::Set(const __m128& vec) {
    xmm = vec;
    return *this;
}
#elif defined(XO_NEON)
Vector3& Vector3::Set(const float32x4_t& vec) {
    n = vec;
    return *this;
}
#endif

void Vector3::Get(float& x, float& y, float &z) const {
    x = this->x;
    y = this->y;
    z = this->z;
}

void Vector3::Get(float* f) const {
#if defined(XO_SSE)
    _mm_store_ps(f, xmm);
#elif defined(XO_NEON)
    vst1q_f32(f, n);
#else
    f[0] = this->x;
    f[1] = this->y;
    f[2] = this->z;
#endif
}

Vector3 Vector3::ZYX() const {
#if defined(XO_SSE)
    // todo: constexpr shuffle int
    return Vector3(_mm_shuffle_ps(xmm, xmm, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Y, IDX_Z)));
#else
    return Vector3(z, y, x);
#endif
}

float Vector3::Sum() const {
#if defined(XO_SSE3)
    __m128 x = _mm_and_ps(xmm, MASK);
    x = _mm_hadd_ps(x, x);
    x = _mm_hadd_ps(x, x);
    return _mm_cvtss_f32(x);
#else
    return x+y+z;
#endif
}

Vector3& Vector3::Normalize() {
    return (*this) /= Magnitude();
}

Vector3& Vector3::NormalizeSafe() {
    float magnitude = MagnitudeSquared();
    if (magnitude == 0.0f)
        return *this;
    return *this /= Sqrt(magnitude);
}
 
float Vector3::Dot(const Vector3& a, const Vector3& b) {
#if defined(XO_SSE4_1)
    return _mm_cvtss_f32(_mm_dp_ps(a, b, 0x7f));
#else
    return (a * b).Sum();
#endif
}
 
void Vector3::Cross(const Vector3& a, const Vector3& b, Vector3& outVec) {
#if defined(XO_SSE)
    // todo: multi-compiler constexpr
    constexpr int m3021 = _MM_SHUFFLE(3, 0, 2, 1);
    __m128 result = _mm_sub_ps(_mm_mul_ps(a, _mm_shuffle_ps(b, b, m3021)), _mm_mul_ps(b, _mm_shuffle_ps(a, a, m3021)));
    outVec.xmm = _mm_shuffle_ps(result, result, m3021);
#else
    outVec.x = (a.y*b.z)-(a.z*b.y);
    outVec.y = (a.z*b.x)-(a.x*b.z);
    outVec.z = (a.x*b.y)-(a.y*b.x);
#endif
}

void Vector3::RotateRadians(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec) {
    // Rodrigues' rotation formula
    // https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
    Vector3 axv;
    float sinAng = Sin(angle);
    float cosAng = Cos(angle);
    Vector3::Cross(axis, v, axv);
    float adv = Vector3::Dot(axis, v);
    outVec.Set(v * cosAng + axv * sinAng + axis * adv * (1.0f - cosAng));
}

float Vector3::AngleRadians(const Vector3& a, const Vector3& b) {
    Vector3 cross;
    Vector3::Cross(a, b, cross);
    cross *= cross;
    return ATan2(Sqrt(cross.Sum()), Vector3::Dot(a, b));
}

void Vector3::RandomInConeRadians(const Vector3& forward, float angle, Vector3& outVec) {
    Vector3 cross;
    Vector3::Cross(forward, forward == Vector3::Up ? Vector3::Left : Vector3::Up, cross);
    Vector3::RotateRadians(forward, cross, RandomRange(0.0f, angle*0.5f), outVec);
    Vector3::RotateRadians(outVec, forward.Normalized(), RandomRange(0.0f, TAU), outVec);
}

void Vector3::RandomOnConeRadians(const Vector3& forward, float angle, Vector3& outVec) {
    Vector3 cross;
    Vector3::Cross(forward, forward == Vector3::Up ? Vector3::Left : Vector3::Up, cross);
    Vector3::RotateRadians(forward, cross, angle*0.5f, outVec);
    Vector3::RotateRadians(outVec, forward.Normalized(), RandomRange(0.0f, TAU), outVec);
}

void Vector3::RandomOnSphere(float radius, Vector3& outVec) {
    // Marsaglia's method: https://projecteuclid.org/download/pdf_1/euclid.aoms/1177692644
    float x1, x2, x12, x22;
    x1 = RandomRange(-1.0f, 1.0f);
    x2 = RandomRange(-1.0f, 1.0f);
    x12 = Square(x1);
    x22 = Square(x1);
    outVec.Set(
        2.0f * x1 * Sqrt(1.0f - x12 - x22),
        2.0f * x2 * Sqrt(1.0f - x12 - x22),
        1.0f - (2.0f * (x12 + x22))
        );
    outVec *= radius;
}

void Vector3::RandomOnCube(float size, Vector3& outVec) {
    switch (RandomRange(0, 5)) {
        case 0: outVec.Set(RandomRange(-size, size),    RandomRange(-size, size),                size);         break;
        case 1: outVec.Set(RandomRange(-size, size),    RandomRange(-size, size),               -size);         break;
        case 2: outVec.Set(RandomRange(-size, size),                 size,          RandomRange(-size, size));  break;
        case 3: outVec.Set(RandomRange(-size, size),                -size,          RandomRange(-size, size));  break;
        case 4: outVec.Set(             size,           RandomRange(-size, size),   RandomRange(-size, size));  break;
        case 5: outVec.Set(            -size,           RandomRange(-size, size),   RandomRange(-size, size));  break;
    }
}

void Vector3::RandomInCircle(const Vector3& up, float radius, Vector3& outVec) {
    Vector3 cross;
    Vector3::Cross(up, up == Right ? Forward : Right, cross);
    Vector3::RotateRadians(cross, up.Normalized(), RandomRange(0.0f, TAU), outVec);
    outVec *= Sqrt(RandomRange(0.0f, 1.0f)) * radius;
}

void Vector3::RandomOnCircle(const Vector3& up, float radius, Vector3& outVec) {
    Vector3 cross;
    Vector3::Cross(up, up == Right ? Forward : Right, cross);
    Vector3::RotateRadians(cross, up.Normalized(), RandomRange(0.0f, TAU), outVec);
    outVec *= radius;
}



#undef IDX_X
#undef IDX_Y
#undef IDX_Z
#undef IDX_W


////////////////////////////////////////////////////////////////////////// Vector4.cpp

#if defined(_XONOCONSTEXPR)
#   if defined(XO_SSE)
const float Vector4::Epsilon = sse::SSEFloatEpsilon * 4.0f;
#   else
const float Vector4::Epsilon = FloatEpsilon * 4.0f;
#   endif
#endif

const Vector4 Vector4::One(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 Vector4::Zero(0.0f, 0.0f, 0.0f, 0.0f);

const Vector4 Vector4::UnitX(1.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::UnitY(0.0f, 1.0f, 0.0f, 0.0f);
const Vector4 Vector4::UnitZ(0.0f, 0.0f, 1.0f, 0.0f);
const Vector4 Vector4::UnitW(0.0f, 0.0f, 0.0f, 1.0f);

#define IDX_X 0
#define IDX_Y 1
#define IDX_Z 2
#define IDX_W 3

Vector4::Vector4() {
}

Vector4::Vector4(float f) :
#if defined(XO_SSE)
    xmm(_mm_set1_ps(f))
{
}
#else
    x(f), y(f), z(f), w(f)
{
}
#endif

Vector4::Vector4(float x, float y, float z, float w) :
#if defined(XO_SSE)
    xmm(_mm_set_ps(w, z, y, x))
{
}
#else
    x(x), y(y), z(z), w(w)
{
}
#endif
Vector4::Vector4(const Vector4& vec) :
#if defined(XO_SSE)
    xmm(vec.xmm)
{
}
#else
    x(vec.x), y(vec.y), z(vec.z), w(vec.w)
{
}
#endif


#if defined(XO_SSE)
Vector4::Vector4(const __m128& vec) : 
    xmm(vec)
{
}
#endif

Vector4::Vector4(const class Vector2& vec) :
#if defined(XO_SSE)
    xmm(_mm_set_ps(0.0f, 0.0f, vec.y, vec.x)) 
{
}
#else
    x(vec.x), y(vec.y), z(0.0f), w(0.0f)
{
}
#endif

Vector4::Vector4(const class Vector2& vec, float z, float w) :
#if defined(XO_SSE)
    xmm(_mm_set_ps(w, z, vec.y, vec.x))
{
}
#else
    x(vec.x), y(vec.y), z(z), w(w)
{
}
#endif

Vector4::Vector4(const class Vector3& vec) :
#if defined(XO_SSE)
    xmm(vec.xmm)
{
}
#else
    x(vec.x), y(vec.y), z(vec.z), w(0.0f)
{
}
#endif

Vector4::Vector4(const class Vector3& vec, float w) :
#if defined(XO_SSE)
    xmm(vec.xmm)
{
    //! @todo there's likely an sse way to do this.
    w = w;
}
#else
    x(vec.x), y(vec.y), z(vec.z), w(w)
{
}
#endif

Vector4::Vector4(const class Quaternion& q) :
#if defined(XO_SSE)
    xmm(q.xmm)
#else
    x(q.x), y(q.y), z(q.z), w(q.w)
#endif
{
}

Vector4& Vector4::Set(float x, float y, float z, float w) {
#if defined(XO_SSE)
    xmm = _mm_set_ps(w, z, y, x);
#else
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
#endif
    return *this;
}

Vector4& Vector4::Set(float f) {
#if defined(XO_SSE)
    xmm = _mm_set1_ps(f);
#else
    this->x = f;
    this->y = f;
    this->z = f;
    this->w = w;
#endif
    return *this;
}

Vector4& Vector4::Set(const Vector4& vec) {
#if defined(XO_SSE)
    xmm = vec.xmm;
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = vec.w;
#endif
    return *this;
}

Vector4& Vector4::Set(const Vector2& vec) {
#if defined(XO_SSE)
    xmm = _mm_set_ps(0.0f, 0.0f, vec.y, vec.x);
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = 0.0f;
    this->w = 0.0f;
#endif
    return *this;
}

Vector4& Vector4::Set(const Vector2& vec, float z, float w) {
#if defined(XO_SSE)
    xmm = _mm_set_ps(w, z, vec.y, vec.x);
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = z;
    this->w = w;
#endif
    return *this;
}

Vector4& Vector4::Set(const Vector3& vec) {
#if defined(XO_SSE)
    // TODO: mask with xmm, don't just break out w
    this->xmm = vec.xmm;
    this->w = 0.0f;
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = 0.0f;
#endif
    return *this;
}

Vector4& Vector4::Set(const Vector3& vec, float w) {
#if defined(XO_SSE)
    // todo: consider masking here.
    this->xmm = vec.xmm;
    this->w = w;
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = w;
#endif
    return *this;
}

#if defined(XO_SSE)
Vector4& Vector4::Set(const __m128& vec) {
    xmm = vec;
    return *this;
}
#endif

void Vector4::Get(float& x, float& y, float& z, float& w) const {
    x = this->x;
    y = this->y;
    z = this->z;
    w = this->w;
}

void Vector4::Get(float* f) const {
#if defined(XO_SSE)
    _mm_store_ps(f, xmm);
#else
    f[0] = this->x;
    f[1] = this->y;
    f[2] = this->z;
    f[3] = this->w;
#endif
}
 
float Vector4::Sum() const {
#if defined(XO_SSE)
    __m128 s = _mm_hadd_ps(xmm, xmm);
    return _mm_cvtss_f32(_mm_hadd_ps(s, s));
#else
    return x+y+z+w;
#endif
}

Vector4& Vector4::NormalizeSafe() {
    float magnitude = MagnitudeSquared();
    if (magnitude == 0.0f)
        return *this;
    return *this /= Sqrt(magnitude);
}

float Vector4::Dot(const Vector4& a, const Vector4& b) {
#if defined(XO_SSE4)
    return _mm_cvtss_f32(_mm_dp_ps(a.xmm, b.xmm, 0xff));
#else
    return (a * b).Sum();
#endif
}


XOMATH_END_XO_NS();
