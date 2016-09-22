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

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Matrix4x4Operators.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

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

const Vector4& Matrix4x4::GetRow(int i) const {
    return r[i];
}

Vector4 Matrix4x4::GetColumn(int i) const {
    return Vector4(r[0][i], r[1][i], r[2][i], r[3][i]);
}

Matrix4x4 Matrix4x4::operator ~() const {
    auto m = *this;
    return m.MakeTranspose();
}

const Matrix4x4& Matrix4x4::operator += (const Matrix4x4& m) {
    r[0] += m[0];
    r[1] += m[1];
    r[2] += m[2];
    r[3] += m[3];
    return *this;
}

const Matrix4x4& Matrix4x4::operator -= (const Matrix4x4& m) {
    r[0] -= m[0];
    r[1] -= m[1];
    r[2] -= m[2];
    r[3] -= m[3];
    return *this;
}

const Matrix4x4& Matrix4x4::operator *= (const Matrix4x4& m) {
    auto t = m.Transpose();
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

#endif