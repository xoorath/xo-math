#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector3Operators.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

float& Vector3::operator [](int i) {
    return f[i];
}

const float& Vector3::operator [](int i) const {
    return f[i];
}

const Vector3& Vector3::operator += (const Vector3& v) {
#if XO_SSE
    m = _mm_add_ps(m, v.m);
#else
    x += v.x;
    y += v.y;
    z += v.z;
#endif
    return *this;
}

const Vector3& Vector3::operator += (float v) {
#if XO_SSE
    m = _mm_add_ps(m, _mm_set_ps1(v));
#else
    x += v;
    y += v;
    z += v;
#endif
    return *this;
}

const Vector3& Vector3::operator += (double v)  { return (*this) += float(v); }
const Vector3& Vector3::operator += (int v)     { return (*this) += float(v); }
const Vector3& Vector3::operator += (const Vector2& v) { return (*this) += Vector3(v); }
const Vector3& Vector3::operator += (const Vector4& v) { return (*this) += Vector3(v); }

const Vector3& Vector3::operator -= (const Vector3& v) {
#if XO_SSE
    m = _mm_sub_ps(m, v.m);
#else
    x -= v.x;
    y -= v.y;
    z -= v.z;
#endif
    return *this;
}

const Vector3& Vector3::operator -= (float v) {
#if XO_SSE
    m = _mm_sub_ps(m, _mm_set_ps1(v));
#else
    x -= v;
    y -= v;
    z -= v;
#endif
    return *this;
}

const Vector3& Vector3::operator -= (double v)  { return (*this) -= float(v); }
const Vector3& Vector3::operator -= (int v)     { return (*this) -= float(v); }
const Vector3& Vector3::operator -= (const Vector2& v) { return (*this) -= Vector3(v); }
const Vector3& Vector3::operator -= (const Vector4& v) { return (*this) -= Vector3(v); }

const Vector3& Vector3::operator *= (const Vector3& v) {
#if XO_SSE
    m = _mm_mul_ps(m, v.m);
#else
    x *= v.x;
    y *= v.y;
    z *= v.z;
#endif
    return *this;
}

const Vector3& Vector3::operator *= (float v) {
#if XO_SSE
    m = _mm_mul_ps(m, _mm_set_ps1(v));
#else
    x *= v;
    y *= v;
    z *= v;
#endif
    return *this;
}
const Vector3& Vector3::operator *= (double v)  { return (*this) *= float(v); }
const Vector3& Vector3::operator *= (int v)     { return (*this) *= float(v); }
const Vector3& Vector3::operator *= (const Vector2& v) { return (*this) *= Vector3(v); }
const Vector3& Vector3::operator *= (const Vector4& v) { return (*this) *= Vector3(v); }


#if XO_NO_INVERSE_DIVISION
const Vector3& Vector3::operator /= (const Vector3& v) {
#if XO_SSE
    // see: https://software.intel.com/sites/landingpage/IntrinsicsGuide
    // see: https://software.intel.com/en-us/articles/measuring-instruction-latency-and-throughput

    // _mm_div_ps
    // -----------------------------------
    // Architecture    Latency Throughput
    // Haswell         13      5
    // Ivy Bridge      13      6
    // Sandy Bridge    14      14
    // Westmere        14      12
    // Nehalem         14      12
    m = _mm_div_ps(m, v.m);
#else
    x /= v.x;
    y /= v.y;
    z /= v.z;
#endif

    return *this;
}

const Vector3& Vector3::operator /= (float v) {
#   if XO_SSE
    m = _mm_div_ps(m, _mm_set_ps1(v));
#   else
    x /= v;
    y /= v;
    z /= v;
#   endif
    return *this;
}
#else

const Vector3& Vector3::operator /= (const Vector3& v) {
#if XO_SSE
    // see: https://software.intel.com/sites/landingpage/IntrinsicsGuide
    // see: https://software.intel.com/en-us/articles/measuring-instruction-latency-and-throughput

    // _mm_rcp_ps
    // -----------------------------------
    // Architecture    Latency  Throughput
    // Haswell         5        1
    // Ivy Bridge      5        1
    // Sandy Bridge    5        1
    // Westmere        3        2
    // Nehalem         3        2

    // _mm_mul_ps
    // -----------------------------------
    // Architecture    Latency  Throughput
    // Haswell         5        0.5
    // Ivy Bridge      5        1
    // Sandy Bridge    5        1
    // Westmere        4        1
    // Nehalem         4        1
    m = _mm_mul_ps(m, _mm_rcp_ps(v.m));
#else
    x /= v.x;
    y /= v.y;
    z /= v.z;
#endif

    return *this;
}

const Vector3& Vector3::operator /= (float v) { 
#   if XO_SSE
    m = _mm_mul_ps(m, _mm_set_ps1(1.0f/v));
#   else
    v = 1.0f / v;
    x *= v;
    y *= v;
    z *= v;
#   endif
    return *this;
}
#endif
const Vector3& Vector3::operator /= (double v)  { return (*this) /= float(v); }
const Vector3& Vector3::operator /= (int v)     { return (*this) /= float(v); }
const Vector3& Vector3::operator /= (const Vector2& v) { return (*this) /= Vector3(v); }
const Vector3& Vector3::operator /= (const Vector4& v) { return (*this) /= Vector3(v); }

Vector3 Vector3::operator + (const Vector3& v) const        { return Vector3(*this) += v; }
Vector3 Vector3::operator + (float v) const                 { return Vector3(*this) += v; }
Vector3 Vector3::operator + (double v) const                { return Vector3(*this) += v; }
Vector3 Vector3::operator + (int v) const                   { return Vector3(*this) += v; }
Vector3 Vector3::operator + (const Vector2& v) const        { return Vector3(*this) += v; }
Vector3 Vector3::operator + (const Vector4& v) const        { return Vector3(*this) += v; }

Vector3 Vector3::operator - (const Vector3& v) const        { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (float v) const                 { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (double v) const                { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (int v) const                   { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (const Vector2& v) const        { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (const Vector4& v) const        { return Vector3(*this) -= v; }

Vector3 Vector3::operator * (const Vector3& v) const        { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (float v) const                 { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (double v) const                { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (int v) const                   { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (const Vector2& v) const        { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (const Vector4& v) const        { return Vector3(*this) *= v; }

Vector3 Vector3::operator / (const Vector3& v) const        { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (float v) const                 { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (double v) const                { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (int v) const                   { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (const Vector2& v) const        { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (const Vector4& v) const        { return Vector3(*this) /= v; }


bool Vector3::operator < (const Vector3& v) const       { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector3::operator < (float v) const                { return MagnitudeSquared() < (v * v); }
bool Vector3::operator < (double v) const               { return MagnitudeSquared() < (float)(v * v); }
bool Vector3::operator < (int v) const                  { return MagnitudeSquared() < (float)(v * v); }
bool Vector3::operator < (const Vector2& v) const { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector3::operator < (const Vector4& v) const { return MagnitudeSquared() < v.MagnitudeSquared(); }

bool Vector3::operator <= (const Vector3& v) const          { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector3::operator <= (float v) const                   { return MagnitudeSquared() <= (v * v); }
bool Vector3::operator <= (double v) const                  { return MagnitudeSquared() <= (float)(v * v); }
bool Vector3::operator <= (int v) const                     { return MagnitudeSquared() <= (float)(v * v); }
bool Vector3::operator <= (const Vector2& v) const    { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector3::operator <= (const Vector4& v) const    { return MagnitudeSquared() <= v.MagnitudeSquared(); }

bool Vector3::operator > (const Vector3& v) const       { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector3::operator > (float v) const                { return MagnitudeSquared() > (v * v); }
bool Vector3::operator > (double v) const               { return MagnitudeSquared() > (float)(v * v); }
bool Vector3::operator > (int v) const                  { return MagnitudeSquared() > (float)(v * v); }
bool Vector3::operator > (const Vector2& v) const { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector3::operator > (const Vector4& v) const { return MagnitudeSquared() > v.MagnitudeSquared(); }

bool Vector3::operator >= (const Vector3& v) const          { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector3::operator >= (float v) const                   { return MagnitudeSquared() >= (v * v); }
bool Vector3::operator >= (double v) const                  { return MagnitudeSquared() >= (float)(v * v); }
bool Vector3::operator >= (int v) const                     { return MagnitudeSquared() >= (float)(v * v); }
bool Vector3::operator >= (const Vector2& v) const    { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector3::operator >= (const Vector4& v) const    { return MagnitudeSquared() >= v.MagnitudeSquared(); }

bool Vector3::operator == (const Vector3& v) const {
#if XO_SSE
    // todo: keep a constant mask of [0x00000000, 0xffffffff, 0xffffffff, 0xffffffff]
    // compare that to the result of _mm_cmpeq_ps instead.
    return (_mm_movemask_ps(_mm_cmpeq_ps(m, v.m)) & 0b1110) == 0b1110;
#else
    return v.x == x && v.y == y; && v.z == z;
#endif
}
bool Vector3::operator == (float v) const                   { return CloseEnough(MagnitudeSquared(), v*v);}
bool Vector3::operator == (double v) const                  { return CloseEnough(MagnitudeSquared(), (float)(v*v));}
bool Vector3::operator == (int v) const                     { return CloseEnough(MagnitudeSquared(), (float)(v*v));}
bool Vector3::operator == (const Vector2& v) const {
#if XO_SSE
    // todo: keep a constant mask of [0x00000000, 0x00000000, 0xffffffff, 0xffffffff]
    // compare that to the result of _mm_cmpeq_ps instead.
    return (_mm_movemask_ps(_mm_cmpeq_ps(m, _mm_load_ps(v.f))) & 0b1100) == 0b1100;
#else
    return v.x == x && v.y == y;
#endif
}
bool Vector3::operator == (const Vector4& v) const {
#if XO_SSE
    // todo: keep a constant mask of [0x00000000, 0xffffffff, 0xffffffff, 0xffffffff]
    // compare that to the result of _mm_cmpeq_ps instead.
    return (_mm_movemask_ps(_mm_cmpeq_ps(m, v.m)) & 0b1110) == 0b1110;
#else
    return v.x == x && v.y == y; && v.z == z;
#endif
}

bool Vector3::operator != (const Vector3& v) const          { return !((*this) == v); }
bool Vector3::operator != (float v) const                   { return !((*this) == v); }
bool Vector3::operator != (double v) const                  { return !((*this) == v); }
bool Vector3::operator != (int v) const                     { return !((*this) == v); }
bool Vector3::operator != (const Vector2& v) const    { return !((*this) == v); }
bool Vector3::operator != (const Vector4& v) const    { return !((*this) == v); }
XOMATH_END_XO_NS

#endif // XOMATH_INTERNAL