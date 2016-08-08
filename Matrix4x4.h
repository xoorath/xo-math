#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Matrix4x4.h directly. Include GameMath.h, which fully implements this type.");
#elif defined(_XOMATH_INIT_MATRIX4X4) || defined(_XO_TRANSPOSE_SWAP)
_XOMATH_INTERNAL_MACRO_WARNING
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS


#define _XOMATH_INIT_MATRIX4X4(a0,b0,c0,d0, a1,b1,c1,d1, a2,b2,c2,d2, a3,b3,c3,d3) { \
    Vector4(a0, b0, c0, d0), \
    Vector4(a1, b1, c1, d1), \
    Vector4(a2, b2, c2, d2), \
    Vector4(a3, b3, c3, d3) \
}

class _MM_ALIGN16 Matrix4x4 {
public:

    _XOINL 
    const Matrix4x4& MakeTranspose() {
        float t;
#define _XO_TRANSPOSE_SWAP(i,j) t = r[i][j]; r[i][j] = r[j][i]; r[j][i] = t;
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
#undef _XO_TRANSPOSE_SWAP
        return *this;
    }

    _XOINL 
    Matrix4x4 Transpose() const {
        auto m = *this;
        return m.MakeTranspose();
    }

    _XOINL
    Matrix4x4 operator~() const {
        auto m = *this;
        return m.MakeTranspose();
    }

    _XOINL 
    Matrix4x4 operator+ (const Matrix4x4& m) {
        return {
            r[0] + m[0], r[1] + m[1], r[2] + m[2], r[3] + m[3]
        };
    }

    _XOINL 
    Matrix4x4 operator- (const Matrix4x4& m) {
        return{
            r[0] - m[0], r[1] - m[1], r[2] - m[2], r[3] - m[3]
        };
    }

    _XOINL 
    Matrix4x4 operator* (const Matrix4x4& m) {
        auto transposeM = ~m;
        return 
            _XOMATH_INIT_MATRIX4X4(
                (r[0] * transposeM[0]).Sum(), (r[0] * transposeM[1]).Sum(), (r[0] * transposeM[3]).Sum(), (r[0] * transposeM[4]).Sum(),
                (r[1] * transposeM[0]).Sum(), (r[1] * transposeM[1]).Sum(), (r[1] * transposeM[3]).Sum(), (r[1] * transposeM[4]).Sum(),
                (r[2] * transposeM[0]).Sum(), (r[2] * transposeM[1]).Sum(), (r[2] * transposeM[3]).Sum(), (r[2] * transposeM[4]).Sum(),
                (r[3] * transposeM[0]).Sum(), (r[3] * transposeM[1]).Sum(), (r[3] * transposeM[3]).Sum(), (r[3] * transposeM[4]).Sum()
            );
    }

    _XOINL 
    Vector4 operator* (const Vector4& m) {
        return Vector4(r[0].Sum(), r[1].Sum(), r[2].Sum(), r[3].Sum()) *= m; // *= to avoid an extra copy
    }

    _XOINL
    static Matrix4x4 RotationXRadians(float radians) {
        float cosr = Cos(radians);
        float sinr = Sin(radians);
        return
            _XOMATH_INIT_MATRIX4X4(1.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, cosr,-sinr, 0.0f,
                           0.0f, sinr, cosr, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f);
        
    }

    _XOINL
    static Matrix4x4 RotationYRadians(float radians) {
        float cosr = Cos(radians);
        float sinr = Sin(radians);
        return
            _XOMATH_INIT_MATRIX4X4(cosr, 0.0f,-sinr, 0.0f,
                           0.0f, 1.0f, 0.0f, 0.0f,
                           sinr, 0.0f, cosr, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f);
    }

    _XOINL
    static Matrix4x4 RotationZRadians(float radians) {
        float cosr = Cos(radians);
        float sinr = Sin(radians);
        return
            _XOMATH_INIT_MATRIX4X4(cosr,-sinr, 0.0f, 0.0f,
                           sinr, cosr, 0.0f, 0.0f,
                           0.0f, 0.0f, 1.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f);
    }

    _XOINL
    const Vector4& operator [](int i) const {
        return r[i];
    }

    _XOINL
    Vector4& operator [](int i) {
        return r[i];
    }

    _XOINL
    const float& operator ()(int r, int c) const {
        return this->r[r][c];
    }

    _XOINL
    float& operator ()(int r, int c) {
        return this->r[r][c];
    }

    _XOINL
    const Vector4& GetRow(int i) const {
        return r[i];
    }

    _XOINL
    Vector4 GetColumn(int i) const {
        return Vector4(r[0][i], r[1][i], r[2][i], r[3][i]);
    }

    Vector4 r[4];

    static const Matrix4x4
        Identity, One, Zero;
};

const Matrix4x4 Matrix4x4::Identity = _XOMATH_INIT_MATRIX4X4(1.0f, 0.0f, 0.0f, 0.0f,
                                                             0.0f, 1.0f, 0.0f, 0.0f,
                                                             0.0f, 0.0f, 1.0f, 0.0f,
                                                             0.0f, 0.0f, 0.0f, 1.0f);

const Matrix4x4 Matrix4x4::One = _XOMATH_INIT_MATRIX4X4(1.0f, 1.0f, 1.0f, 1.0f,
                                                        1.0f, 1.0f, 1.0f, 1.0f,
                                                        1.0f, 1.0f, 1.0f, 1.0f,
                                                        1.0f, 1.0f, 1.0f, 1.0f);

const Matrix4x4 Matrix4x4::Zero = _XOMATH_INIT_MATRIX4X4(0.0f, 0.0f, 0.0f, 0.0f,
                                                         0.0f, 0.0f, 0.0f, 0.0f,
                                                         0.0f, 0.0f, 0.0f, 0.0f,
                                                         0.0f, 0.0f, 0.0f, 0.0f);

XOMATH_END_XO_NS

#undef _XOMATH_INIT_MATRIX4X4

#endif // XOMATH_INTERNAL