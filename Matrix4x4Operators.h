#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Matrix4x4Operators.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

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
    return (*this) = {
        (r[0] * t[0]).Sum(), (r[0] * t[1]).Sum(), (r[0] * t[2]).Sum(), (r[0] * t[3]).Sum(),
        (r[1] * t[0]).Sum(), (r[1] * t[1]).Sum(), (r[1] * t[2]).Sum(), (r[1] * t[3]).Sum(),
        (r[2] * t[0]).Sum(), (r[2] * t[1]).Sum(), (r[2] * t[2]).Sum(), (r[2] * t[3]).Sum(),
        (r[3] * t[0]).Sum(), (r[3] * t[1]).Sum(), (r[3] * t[2]).Sum(), (r[3] * t[3]).Sum()
    };
}

const Matrix4x4& Matrix4x4::operator *= (float scale) {
    r[0][0] *= scale;
    r[1][1] *= scale;
    r[2][2] *= scale;
    return *this;
}

Matrix4x4 Matrix4x4::operator + (const Matrix4x4& m) const { return Matrix4x4(*this) += m; }
Matrix4x4 Matrix4x4::operator - (const Matrix4x4& m) const { return Matrix4x4(*this) -= m; }
Matrix4x4 Matrix4x4::operator * (const Matrix4x4& m) const { return Matrix4x4(*this) *= m; }
Matrix4x4 Matrix4x4::operator * (float scale) const { return Matrix4x4(*this) *= scale; }

XOMATH_END_XO_NS

#endif