#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Matrix4x4.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

#define INIT_MATRIX4X4(a0,b0,c0,d0, a1,b1,c1,d1, a2,b2,c2,d2, a3,b3,c3,d3) { \
    Vector4(a3, a2, a1, a0), \
    Vector4(b3, b2, b1, b0), \
    Vector4(c3, c2, c1, c0), \
    Vector4(d3, d2, d1, d0) \
}

class _MM_ALIGN16 Matrix4x4 {
public:

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

    XOMATH_INLINE Vector4& XOMATH_FAST(operator[](int i)) {
        return c[i];
    }

    XOMATH_INLINE const Vector4& XOMATH_FAST(operator[](int i) const) {
        return c[i];
    }

    // get the row i
    XOMATH_INLINE Vector4 XOMATH_FAST(operator()(int i)) {
        return Vector4(c[0][i], c[1][i], c[2][i], c[3][i]);
    }

    Vector4 c[4];

    static const Matrix4x4
        Identity, One, Zero;
};

// keep in mind when reading matrices defined this way that each vec4 is a column.
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