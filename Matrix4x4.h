#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Matrix4x4.h directly. Include GameMath.h, which fully implements this type.");
#elif defined(_XOMATH_INIT_MATRIX4X4) || defined(_XO_TRANSPOSE_SWAP)
_XOMATH_INTERNAL_MACRO_WARNING
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

class _MM_ALIGN16 Matrix4x4 {
public:

    _XOINL Matrix4x4();

    // initialize all elements to m
    _XOINL Matrix4x4(float m);

    // specify each element in row major form.
    // [a0, b0, c0, d0]
    // [a1, b1, c1, d1]
    // [a2, b2, c2, d2]
    // [a3, b3, c3, d3]
    _XOINL Matrix4x4(float a0, float b0, float c0, float d0, float a1, float b1, float c1, float d1, float a2, float b2, float c2, float d2, float a3, float b3, float c3, float d3);

    // specify this matrix as a copy of m
    _XOINL Matrix4x4(const Matrix4x4& m);

    // initialize each row as the Vector4's 
    // [r0.x, r0.y, r0.z, r0.w]
    // [r1.x, r1.y, r1.z, r1.w]
    // [r2.x, r2.y, r2.z, r2.w]
    // [r3.x, r3.y, r3.z, r3.w]
    _XOINL Matrix4x4(const class Vector4& r0, const class Vector4& r1, const class Vector4& r2, const class Vector4& r3);

    // Specify the upper left of the matrix as one Vector3 per row, leaving the rightmost column as 0, except it's bottommost element.
    // [r0.x, r0.y, r0.z, 0.0]
    // [r1.x, r1.y, r1.z, 0.0]
    // [r2.x, r2.y, r2.z, 0.0]
    // [0.0,  0.0,  0.0,  1.0]
    _XOINL Matrix4x4(const class Vector3& r0, const class Vector3& r1, const class Vector3& r2);

    // Set this matrix as a transpose of itself, then return a ref to itself.
    // See: https://en.wikipedia.org/wiki/Transpose
    _XOINL const Matrix4x4& MakeTranspose();

    // Return a transpose of this matrix
    // See: https://en.wikipedia.org/wiki/Transpose
    _XOINL Matrix4x4 Transpose() const;

    _XOINL
    Matrix4x4 operator ~() const {
        auto m = *this;
        return m.MakeTranspose();
    }

    
    _XOINL Matrix4x4 operator* (const Matrix4x4& m);

    _XOINL 
    Vector4 operator* (const Vector4& m) {
        return Vector4(r[0].Sum(), r[1].Sum(), r[2].Sum(), r[3].Sum()) *= m; // *= to avoid an extra copy
    }

    _XOINL
    static Matrix4x4 RotationXRadians(float radians) {
        float cosr = Cos(radians);
        float sinr = Sin(radians);
        return {1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, cosr,-sinr, 0.0f,
                0.0f, sinr, cosr, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f};
        
    }

    _XOINL
    static Matrix4x4 RotationYRadians(float radians) {
        float cosr = Cos(radians);
        float sinr = Sin(radians);
        return {cosr, 0.0f,-sinr, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                sinr, 0.0f, cosr, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f};
    }

    _XOINL
    static Matrix4x4 RotationZRadians(float radians) {
        float cosr = Cos(radians);
        float sinr = Sin(radians);
        return {cosr,-sinr, 0.0f, 0.0f,
                sinr, cosr, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f};
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

const Matrix4x4 Matrix4x4::Identity =   {1.0f, 0.0f, 0.0f, 0.0f,
                                         0.0f, 1.0f, 0.0f, 0.0f,
                                         0.0f, 0.0f, 1.0f, 0.0f,
                                         0.0f, 0.0f, 0.0f, 1.0f};

const Matrix4x4 Matrix4x4::One =    {1.0f, 1.0f, 1.0f, 1.0f,
                                     1.0f, 1.0f, 1.0f, 1.0f,
                                     1.0f, 1.0f, 1.0f, 1.0f,
                                     1.0f, 1.0f, 1.0f, 1.0f};

const Matrix4x4 Matrix4x4::Zero =   {0.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 0.0f};

XOMATH_END_XO_NS

#undef _XOMATH_INIT_MATRIX4X4

#endif // XOMATH_INTERNAL