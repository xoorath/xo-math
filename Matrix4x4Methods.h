#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Matrix4x4Methods.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

Matrix4x4::Matrix4x4() {
}

// TODO: Is there a fast way to initialize just one vector4, and copy it to the other 3?
Matrix4x4::Matrix4x4(float m) :
    r{Vector4(m), Vector4(m), Vector4(m), Vector4(m)}
{
}

Matrix4x4::Matrix4x4(float a0, float b0, float c0, float d0, float a1, float b1, float c1, float d1, float a2, float b2, float c2, float d2, float a3, float b3, float c3, float d3) :
    r{Vector4(a0, b0, c0, d0), Vector4(a1, b1, c1, d1), Vector4(a2, b2, c2, d2), Vector4(a3, b3, c3, d3)}

{
}

// TODO: couldn't this be faster with just a single copy?
Matrix4x4::Matrix4x4(const Matrix4x4& m) :
    r{m.r[0], m.r[1], m.r[2], m.r[3]}
{
}

Matrix4x4::Matrix4x4(const class Vector4& r0, const class Vector4& r1, const class Vector4& r2, const class Vector4& r3)  :
    r{r0, r1, r2, r3}
{
}

Matrix4x4::Matrix4x4(const class Vector3& r0, const class Vector3& r1, const class Vector3& r2) :
    r{Vector4(r0), Vector4(r1), Vector4(r2), Vector4(0.0f, 0.0f, 0.0f, 1.0f)}
{
}

const Matrix4x4& Matrix4x4::MakeTranspose() {
#if XO_SSE
    _MM_TRANSPOSE4_PS(r[0].m, r[1].m, r[2].m, r[3].m);
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

Matrix4x4 Matrix4x4::Transpose() const {
    auto m = *this;
    return m.MakeTranspose();
}

Matrix4x4 Matrix4x4::Scale(float xyz) {
    return{ {xyz,  0.0f, 0.0f, 0.0f},
            {0.0f, xyz,  0.0f, 0.0f},
            {0.0f, 0.0f, xyz,  0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};

}
 
Matrix4x4 Matrix4x4::Scale(float x, float y, float z) {
    return{ {x,    0.0f, 0.0f, 0.0f},
            {0.0f, y,    0.0f, 0.0f},
            {0.0f, 0.0f, z,    0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};
}

Matrix4x4 Matrix4x4::Scale(const Vector3& v) {
    return{ {v.x,  0.0f, 0.0f, 0.0f},
            {0.0f, v.y,  0.0f, 0.0f},
            {0.0f, 0.0f, v.z,  0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};
}
 
Matrix4x4 Matrix4x4::Translation(float x, float y, float z) {
    return{ {1.0f, 0.0f, 0.0f, x   },
            {0.0f, 1.0f, 0.0f, y   },
            {0.0f, 0.0f, 1.0f, z   },
            {0.0f, 0.0f, 0.0f, 1.0f}};
}

Matrix4x4 Matrix4x4::Translation(const Vector3& v) {
    return{ {1.0f, 0.0f, 0.0f, v.x },
            {0.0f, 1.0f, 0.0f, v.y },
            {0.0f, 0.0f, 1.0f, v.z },
            {0.0f, 0.0f, 0.0f, 1.0f} };
}

Matrix4x4 Matrix4x4::RotationXRadians(float radians) {
    float cosr = Cos(radians);
    float sinr = Sin(radians);
    return {{1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, cosr,-sinr, 0.0f},
            {0.0f, sinr, cosr, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};
    
}
 
Matrix4x4 Matrix4x4::RotationYRadians(float radians) {
    float cosr = Cos(radians);
    float sinr = Sin(radians);
    return {{cosr, 0.0f,-sinr, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {sinr, 0.0f, cosr, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};
}

Matrix4x4 Matrix4x4::RotationZRadians(float radians) {
    float cosr = Cos(radians);
    float sinr = Sin(radians);
    return {{cosr,-sinr, 0.0f, 0.0f},
            {sinr, cosr, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};
}

Matrix4x4 Matrix4x4::RotationRadians(float x, float y, float z) {
    return RotationXRadians(x) * RotationYRadians(y) * RotationZRadians(z);
}

Matrix4x4 Matrix4x4::RotationRadians(const Vector3& v) {
    return RotationXRadians(v.x) * RotationYRadians(v.y) * RotationZRadians(v.z);
}

Matrix4x4 Matrix4x4::RotationXDegrees(float degrees) {
    return RotationXRadians(degrees * Deg2Rad);
}

Matrix4x4 Matrix4x4::RotationYDegrees(float degrees) {
    return RotationYRadians(degrees * Deg2Rad);
}

Matrix4x4 Matrix4x4::RotationZDegrees(float degrees) {
    return RotationZRadians(degrees * Deg2Rad);
}

Matrix4x4 Matrix4x4::RotationDegrees(float x, float y, float z) {
    return RotationXDegrees(x) * RotationYDegrees(y) * RotationZDegrees(z);
}

Matrix4x4 Matrix4x4::RotationDegrees(const Vector3& v) {
    return RotationXDegrees(v.x) * RotationYDegrees(v.y) * RotationZDegrees(v.z);
}

Matrix4x4 Matrix4x4::OrthographicProjection(float w, float h, float n, float f) {
    auto fmn = f - n;
    return{ {1.0f/w,    0.0f,   0.0f,           0.0f},
            {0.0f,      1.0f/h, 0.0f,           0.0f},
            {0.0f,      0.0f,   -(2.0f/ fmn),  -((f+n)/ fmn)},
            {0.0f,      0.0f,   0.0f,           1.0f}};
}
 
Matrix4x4 Matrix4x4::Projection(float fovx, float fovy, float n, float f) {
    auto fmn = f - n;
    return{ {ATan(fovx/2.0f),   0.0f,               0.0f,               0.0f},
            {0.0f,              ATan(fovy/2.0f),    0.0f,               0.0f},
            {0.0f,              0.0f,               -((f+n)/(fmn)),     -((2.0f*(n*f))/fmn)},
            {0.0f,              0.0f,               0.0f,               1.0f}};
}

const Matrix4x4& Matrix4x4::Transform(Vector3& v) const {
    v *= *this;
    return *this;
}

const Matrix4x4& Matrix4x4::Transform(Vector4& v) const {
    v *= *this;
    return *this;
}

XOMATH_END_XO_NS

#endif