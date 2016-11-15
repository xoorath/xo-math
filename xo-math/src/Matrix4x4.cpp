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

#define _XO_ASSERT_MSG(msg) "xo-math Matrix4x4" msg

XOMATH_BEGIN_XO_NS();

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

XOMATH_END_XO_NS();