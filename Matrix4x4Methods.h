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

const Matrix4x4& Matrix4x4::Transform(Vector3& v) const {
    v *= *this;
    return *this;
}

const Matrix4x4& Matrix4x4::Transform(Vector4& v) const {
    v *= *this;
    return *this;
}

Matrix4x4 Matrix4x4::CreateScale(float xyz) {
    return{ {xyz,  0.0f, 0.0f, 0.0f},
            {0.0f, xyz,  0.0f, 0.0f},
            {0.0f, 0.0f, xyz,  0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};

}
 
Matrix4x4 Matrix4x4::CreateScale(float x, float y, float z) {
    return{ {x,    0.0f, 0.0f, 0.0f},
            {0.0f, y,    0.0f, 0.0f},
            {0.0f, 0.0f, z,    0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};
}

Matrix4x4 Matrix4x4::CreateScale(const Vector3& v) {
    return{ {v.x,  0.0f, 0.0f, 0.0f},
            {0.0f, v.y,  0.0f, 0.0f},
            {0.0f, 0.0f, v.z,  0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};
}
 
Matrix4x4 Matrix4x4::CreateTranslation(float x, float y, float z) {
    return{ {1.0f, 0.0f, 0.0f, x   },
            {0.0f, 1.0f, 0.0f, y   },
            {0.0f, 0.0f, 1.0f, z   },
            {0.0f, 0.0f, 0.0f, 1.0f}};
}

Matrix4x4 Matrix4x4::CreateTranslation(const Vector3& v) {
    return{ {1.0f, 0.0f, 0.0f, v.x },
            {0.0f, 1.0f, 0.0f, v.y },
            {0.0f, 0.0f, 1.0f, v.z },
            {0.0f, 0.0f, 0.0f, 1.0f} };
}

Matrix4x4 Matrix4x4::CreateRotationXRadians(float radians) {
    float cosr = Cos(radians);
    float sinr = Sin(radians);
    return {{1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, cosr,-sinr, 0.0f},
            {0.0f, sinr, cosr, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};
    
}
 
Matrix4x4 Matrix4x4::CreateRotationYRadians(float radians) {
    float cosr = Cos(radians);
    float sinr = Sin(radians);
    return {{cosr, 0.0f,-sinr, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {sinr, 0.0f, cosr, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};
}

Matrix4x4 Matrix4x4::CreateRotationZRadians(float radians) {
    float cosr = Cos(radians);
    float sinr = Sin(radians);
    return {{cosr,-sinr, 0.0f, 0.0f},
            {sinr, cosr, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};
}

Matrix4x4 Matrix4x4::CreateRotationRadians(float x, float y, float z) {
    return CreateRotationXRadians(x) * CreateRotationYRadians(y) * CreateRotationZRadians(z);
}

Matrix4x4 Matrix4x4::CreateRotationRadians(const Vector3& v) {
    return CreateRotationXRadians(v.x) * CreateRotationYRadians(v.y) * CreateRotationZRadians(v.z);
}

Matrix4x4 Matrix4x4::CreateAxisAngleRadians(const Vector3& a, float radians) {
    float c = Cos(radians);
    float s = Sin(radians);
    float t = 1.0f - c;
    const float& x = a.x;
    const float& y = a.y;
    const float& z = a.z;
    return{ { t*x*x+c  ,    t*x*y-z*s,  t*x*z+y*s,  0.0f},
            { t*x*y+z*s,    t*y*y+c  ,  t*y*z-x*s,  0.0f},
            { t*x*z-y*s,    t*y*z+x*s,  t*z*z+c  ,  0.0f},
            { 0.0f,         0.0f,       0.0f,       1.0f} };
}

Matrix4x4 Matrix4x4::CreateRotationXDegrees(float degrees) {
    return CreateRotationXRadians(degrees * Deg2Rad);
}

Matrix4x4 Matrix4x4::CreateRotationYDegrees(float degrees) {
    return CreateRotationYRadians(degrees * Deg2Rad);
}

Matrix4x4 Matrix4x4::CreateRotationZDegrees(float degrees) {
    return CreateRotationZRadians(degrees * Deg2Rad);
}

Matrix4x4 Matrix4x4::CreateRotationDegrees(float x, float y, float z) {
    return CreateRotationXDegrees(x) * CreateRotationYDegrees(y) * CreateRotationZDegrees(z);
}

Matrix4x4 Matrix4x4::CreateRotationDegrees(const Vector3& v) {
    return CreateRotationXDegrees(v.x) * CreateRotationYDegrees(v.y) * CreateRotationZDegrees(v.z);
}


Matrix4x4 Matrix4x4::CreateAxisAngleDegrees(const Vector3& a, float degrees) {
    return CreateAxisAngleDegrees(a, degrees * Deg2Rad);
}

Matrix4x4 Matrix4x4::CreateOrthographicProjection(float w, float h, float n, float f) {
    auto fmn = f - n;
    return{ {1.0f/w,    0.0f,   0.0f,           0.0f},
            {0.0f,      1.0f/h, 0.0f,           0.0f},
            {0.0f,      0.0f,   -(2.0f/fmn),    -((f+n)/fmn)},
            {0.0f,      0.0f,   0.0f,           1.0f}};
}
 
Matrix4x4 Matrix4x4::CreateProjection(float fovx, float fovy, float n, float f) {
    auto fmn = f - n;
    return{ {ATan(fovx/2.0f),   0.0f,               0.0f,               0.0f},
            {0.0f,              ATan(fovy/2.0f),    0.0f,               0.0f},
            {0.0f,              0.0f,               -((f+n)/(fmn)),     -((2.0f*(n*f))/fmn)},
            {0.0f,              0.0f,               0.0f,               1.0f}};
}


Matrix4x4 Matrix4x4::CreateLookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up) {
    Vector3 zAxis = (to - from).Normalized();
    Vector3 xAxis = Vector3::Cross(up, zAxis).Normalized();
    Vector3 yAxis = Vector3::Cross(zAxis, xAxis);
    return{ {xAxis.x,           yAxis.x,            zAxis.x,            0.0f},
            {xAxis.y,           yAxis.y,            zAxis.y,            0.0f},
            {xAxis.z,           yAxis.z,            zAxis.z,            0.0f},
            {-xAxis.Dot(from),  -yAxis.Dot(from),   -zAxis.Dot(from),   1.0f}};
}

Matrix4x4 Matrix4x4::CreateLookAtFromPosition(const Vector3& from, const Vector3& to) {
    return CreateLookAtFromPosition(from, to, Vector3::Up);
}

Matrix4x4 Matrix4x4::CreateLookAtFromDirection(const Vector3& direction, const Vector3& up) {
    Vector3 zAxis = direction.Normalized();
    Vector3 xAxis = Vector3::Cross(up, zAxis).Normalized();
    Vector3 yAxis = Vector3::Cross(zAxis, xAxis);
    return{ {xAxis.x,           yAxis.x,            zAxis.x,            0.0f},
            {xAxis.y,           yAxis.y,            zAxis.y,            0.0f},
            {xAxis.z,           yAxis.z,            zAxis.z,            0.0f},
            {0.0f,              0.0f,               0.0f,               1.0f}};
}

Matrix4x4 Matrix4x4::CreateLookAtFromDirection(const Vector3& direction) {
    return CreateLookAtFromDirection(direction, Vector3::Up);
}

XOMATH_END_XO_NS

#endif