#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Matrix4x4Methods.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

Matrix4x4::Matrix4x4() {
}

// TODO: Is there a fast way to initialize just one vector4, and copy it to the other 3?
Matrix4x4::Matrix4x4(float m) :
    r {
        Vector4(m),
        Vector4(m),
        Vector4(m),
        Vector4(m)
    }
{
}

Matrix4x4::Matrix4x4(float a0, float b0, float c0, float d0, float a1, float b1, float c1, float d1, float a2, float b2, float c2, float d2, float a3, float b3, float c3, float d3) :
    r {
        Vector4(a0, b0, c0, d0),
        Vector4(a1, b1, c1, d1),
        Vector4(a2, b2, c2, d2),
        Vector4(a3, b3, c3, d3)
    }

{
}

// TODO: couldn't this be faster with just a single copy?
Matrix4x4::Matrix4x4(const Matrix4x4& m) :
    r {
        m.r[0], 
        m.r[1], 
        m.r[2],
        m.r[3]
    }
{
}

Matrix4x4::Matrix4x4(const class Vector4& r0, const class Vector4& r1, const class Vector4& r2, const class Vector4& r3)  :
    r {
        r0,
        r1,
        r2,
        r3
    }
{
}

Matrix4x4::Matrix4x4(const class Vector3& r0, const class Vector3& r1, const class Vector3& r2) :
    r {
        Vector4(r0), 
        Vector4(r1), 
        Vector4(r2), 
        Vector4(0.0f, 0.0f, 0.0f, 1.0f)
    }
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

void Matrix4x4::Scale(float xyz, Matrix4x4& m) {
    m = {
            {xyz,  0.0f, 0.0f, 0.0f},
            {0.0f, xyz,  0.0f, 0.0f},
            {0.0f, 0.0f, xyz,  0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
}
 
void Matrix4x4::Scale(float x, float y, float z, Matrix4x4& m) {
    m = {
            {x,    0.0f, 0.0f, 0.0f},
            {0.0f, y,    0.0f, 0.0f},
            {0.0f, 0.0f, z,    0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
}

void Matrix4x4::Scale(const Vector3& v, Matrix4x4& m) {
    m = {
            {v.x,  0.0f, 0.0f, 0.0f},
            {0.0f, v.y,  0.0f, 0.0f},
            {0.0f, 0.0f, v.z,  0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
}
 
void Matrix4x4::Translation(float x, float y, float z, Matrix4x4& m) {
    m = {
            {1.0f, 0.0f, 0.0f, x   },
            {0.0f, 1.0f, 0.0f, y   },
            {0.0f, 0.0f, 1.0f, z   },
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
}

void Matrix4x4::Translation(const Vector3& v, Matrix4x4& m) {
    m = {
            {1.0f, 0.0f, 0.0f, v.x },
            {0.0f, 1.0f, 0.0f, v.y },
            {0.0f, 0.0f, 1.0f, v.z },
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
}

void Matrix4x4::RotationXRadians(float radians, Matrix4x4& m) {
    float cosr = Cos(radians);
    float sinr = Sin(radians);
    m = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, cosr,-sinr, 0.0f},
            {0.0f, sinr, cosr, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
    
}
 
void Matrix4x4::RotationYRadians(float radians, Matrix4x4& m) {
    float cosr = Cos(radians);
    float sinr = Sin(radians);
    m = {
            {cosr, 0.0f,-sinr, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {sinr, 0.0f, cosr, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
}

void Matrix4x4::RotationZRadians(float radians, Matrix4x4& m) {
    float cosr = Cos(radians);
    float sinr = Sin(radians);
    m = {
            {cosr,-sinr, 0.0f, 0.0f},
            {sinr, cosr, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
}

void Matrix4x4::RotationRadians(float x, float y, float z, Matrix4x4& m) {
    Matrix4x4 m0, m1, m2;
    RotationXRadians(x, m0);
    RotationYRadians(y, m1);
    RotationZRadians(z, m2);
    m = m0 * m1 * m2;
}

void Matrix4x4::RotationRadians(const Vector3& v, Matrix4x4& m) {
    Matrix4x4 m0, m1, m2;
    RotationXRadians(v.x, m0);
    RotationYRadians(v.y, m1);
    RotationZRadians(v.z, m2);
    m = m0 * m1 * m2;
}

void Matrix4x4::AxisAngleRadians(const Vector3& a, float radians, Matrix4x4& m) {
    float c = Cos(radians);
    float s = Sin(radians);
    float t = 1.0f - c;
    const float& x = a.x;
    const float& y = a.y;
    const float& z = a.z;
    m = {
            { t*x*x+c  ,    t*x*y-z*s,  t*x*z+y*s,  0.0f},
            { t*x*y+z*s,    t*y*y+c  ,  t*y*z-x*s,  0.0f},
            { t*x*z-y*s,    t*y*z+x*s,  t*z*z+c  ,  0.0f},
            { 0.0f,         0.0f,       0.0f,       1.0f}
        };
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
    auto fmn = f - n;
    m = {
            {1.0f/w,    0.0f,   0.0f,           0.0f},
            {0.0f,      1.0f/h, 0.0f,           0.0f},
            {0.0f,      0.0f,   -(2.0f/fmn),    -((f+n)/fmn)},
            {0.0f,      0.0f,   0.0f,           1.0f}
        };
}
 
void Matrix4x4::Projection(float fovx, float fovy, float n, float f, Matrix4x4& m) {
    auto fmn = f - n;
    m = {
            {ATan(fovx/2.0f),   0.0f,               0.0f,               0.0f},
            {0.0f,              ATan(fovy/2.0f),    0.0f,               0.0f},
            {0.0f,              0.0f,               -((f+n)/(fmn)),     -((2.0f*(n*f))/fmn)},
            {0.0f,              0.0f,               0.0f,               1.0f}
        };
}


void Matrix4x4::LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up, Matrix4x4& m) {
    Vector3 zAxis = (to - from).Normalized();
    Vector3 xAxis = Vector3::Cross(up, zAxis).Normalized();
    Vector3 yAxis = Vector3::Cross(zAxis, xAxis);
    m = {
            {xAxis.x,           yAxis.x,            zAxis.x,            0.0f},
            {xAxis.y,           yAxis.y,            zAxis.y,            0.0f},
            {xAxis.z,           yAxis.z,            zAxis.z,            0.0f},
            {-xAxis.Dot(from),  -yAxis.Dot(from),   -zAxis.Dot(from),   1.0f}
        };
}

void Matrix4x4::LookAtFromPosition(const Vector3& from, const Vector3& to, Matrix4x4& m) {
    LookAtFromPosition(from, to, Vector3::Up, m);
}

void Matrix4x4::LookAtFromDirection(const Vector3& direction, const Vector3& up, Matrix4x4& m) {
    Vector3 zAxis = direction.Normalized();
    Vector3 xAxis = Vector3::Cross(up, zAxis).Normalized();
    Vector3 yAxis = Vector3::Cross(zAxis, xAxis);
    m = {
            {xAxis.x,           yAxis.x,            zAxis.x,            0.0f},
            {xAxis.y,           yAxis.y,            zAxis.y,            0.0f},
            {xAxis.z,           yAxis.z,            zAxis.z,            0.0f},
            {0.0f,              0.0f,               0.0f,               1.0f}
        };
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
Matrix4x4 Matrix4x4::Projection(float fovx, float fovy, float n, float f) {
    Matrix4x4 m;
    Projection(fovx, fovy, n, f, m);
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

XOMATH_END_XO_NS

#endif