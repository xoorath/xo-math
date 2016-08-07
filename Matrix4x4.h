#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Matrix4x4.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

#define INIT_MATRIX4X4(a0,b0,c0,d0, a1,b1,c1,d1, a2,b2,c2,d2, a3,b3,c3,d3) { \
    Vector4(a0, b0, c0, d0), \
    Vector4(a1, b1, c1, d1), \
    Vector4(a2, b2, c2, d2), \
    Vector4(a3, b3, c3, d3) \
}

class _MM_ALIGN16 Matrix4x4 {
public:

    Matrix4x4 Transpose() const {
        return 
            INIT_MATRIX4X4( 
                r[0][0], r[1][0], r[2][0], r[3][0],
                r[0][1], r[1][1], r[2][1], r[3][1],
                r[0][2], r[1][2], r[2][2], r[3][2],
                r[0][3], r[1][3], r[2][3], r[3][3] );
    }

    Matrix4x4 operator~() const {
        return Transpose();
    }

    Matrix4x4 operator* (const Matrix4x4& m) {
        
    }

    static Matrix4x4 RotationXRadians(float radians) {
        float cosr = Cos(radians);
        float sinr = Sin(radians);
        return
            INIT_MATRIX4X4(1.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, cosr,-sinr, 0.0f,
                           0.0f, sinr, cosr, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f);
        
    }

    static Matrix4x4 RotationYRadians(float radians) {
        float cosr = Cos(radians);
        float sinr = Sin(radians);
        return
            INIT_MATRIX4X4(cosr, 0.0f,-sinr, 0.0f,
                           0.0f, 1.0f, 0.0f, 0.0f,
                           sinr, 0.0f, cosr, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f);
    }

    static Matrix4x4 RotationZRadians(float radians) {
        float cosr = Cos(radians);
        float sinr = Sin(radians);
        return
            INIT_MATRIX4X4(cosr,-sinr, 0.0f, 0.0f,
                           sinr, cosr, 0.0f, 0.0f,
                           0.0f, 0.0f, 1.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f);
    }

    const Vector4& operator [](int i) const {
        return r[i];
    }

    Vector4& operator [](int i) {
        return r[i];
    }

    const float& operator ()(int r, int c) const {
        return this->r[r][c];
    }

    float& operator ()(int r, int c) {
        return this->r[r][c];
    }

    const Vector4& GetRow(int i) const {
        return r[i];
    }

    Vector4 GetColumn(int i) const {
        return Vector4(r[0][i], r[1][i], r[2][i], r[3][i]);
    }

    Vector4 r[4];

    static const Matrix4x4
        Identity, One, Zero;
};

const Matrix4x4 Matrix4x4::Identity = INIT_MATRIX4X4(1.0f, 0.0f, 0.0f, 0.0f,
                                                     0.0f, 1.0f, 0.0f, 0.0f,
                                                     0.0f, 0.0f, 1.0f, 0.0f,
                                                     0.0f, 0.0f, 0.0f, 1.0f);

const Matrix4x4 Matrix4x4::One = INIT_MATRIX4X4(1.0f, 1.0f, 1.0f, 1.0f,
                                                1.0f, 1.0f, 1.0f, 1.0f,
                                                1.0f, 1.0f, 1.0f, 1.0f,
                                                1.0f, 1.0f, 1.0f, 1.0f);

const Matrix4x4 Matrix4x4::Zero = INIT_MATRIX4X4(0.0f, 0.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 0.0f, 0.0f);

XOMATH_END_XO_NS

#endif // XOMATH_INTERNAL