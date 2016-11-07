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

#define _XO_MATH_OBJ
#include "xo-math.h"

XOMATH_BEGIN_XO_NS();

#if defined(_XONOCONSTEXPR)
#   if defined(XO_SSE)
const float Vector3::Epsilon = sse::SSEFloatEpsilon * 3.0f;
#   else
const float Vector3::Epsilon = FloatEpsilon * 3.0f;
#   endif
#endif

#if defined(XO_SSE2)
const __m128 Vector3::MASK = _mm_castsi128_ps(_mm_set_epi32(0, 0xffffffff, 0xffffffff, 0xffffffff));
#elif defined(XO_SSE)
const __m128 Vector3::MASK = {-1, -1, -1, 0};
#endif

const Vector3 Vector3::Origin(0.0f, 0.0f, 0.0f);

const Vector3 Vector3::UnitX(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UnitY(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UnitZ(0.0f, 0.0f, 1.0f);

const Vector3 Vector3::Left(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);

#if defined(XO_SPACE_ZUP)
const Vector3 Vector3::Up(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Down(0.0f, 0.0f, -1.0f);
#elif defined(XO_SPACE_YUP)
const Vector3 Vector3::Up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Down(0.0f, -1.0f, 0.0f);
#endif

#if defined(XO_SPACE_LEFTHAND)
#   if defined(XO_SPACE_ZUP)
const Vector3 Vector3::Forward(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::Backward(0.0f, 1.0f, 0.0f);
#   elif defined(XO_SPACE_YUP)
const Vector3 Vector3::Forward(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Backward(0.0f, 0.0f, -1.0f);
#   endif
#elif defined(XO_SPACE_RIGHTHAND)
#   if defined(XO_SPACE_ZUP)
const Vector3 Vector3::Forward(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Backward(0.0f, -1.0f, 0.0f);
#   elif defined(XO_SPACE_YUP)
const Vector3 Vector3::Forward(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::Backward(0.0f, 0.0f, 1.0f);
#   endif
#endif

const Vector3 Vector3::One(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);

#if defined(XO_SSE)

#if !defined(IDX_X)
#   define IDX_X 0
#endif

#if !defined(IDX_Y)
#   define IDX_Y 1
#endif

#if !defined(IDX_Z)
#   define IDX_Z 2
#endif

#if !defined(IDX_W)
#   define IDX_W 3
#endif

#endif

// todo: move to header
Vector3::Vector3() {
}

Vector3::Vector3(float f) :
#if defined(XO_SSE)
    m(_mm_set1_ps(f))
#else
    x(f), y(f), z(f)
#endif
{
}

Vector3::Vector3(float x, float y, float z) :
#if defined(XO_SSE)
    m(_mm_set_ps(0.0f, z, y, x))
#else
    x(x), y(y), z(z)
#endif
{
}

Vector3::Vector3(const Vector3& vec) :
#if defined(XO_SSE)
    m(vec) 
#else
    x(vec.x), y(vec.y), z(vec.z) 
#endif
{
}

#if defined(XO_SSE)
Vector3::Vector3(const __m128& vec) : 
    m(vec) 
{
}
#endif

Vector3::Vector3(const class Vector2& v) :
#if defined(XO_SSE)
    m(_mm_set_ps(0.0f, 0.0f, v.y, v.x))
#else
    x(v.x), y(v.y), z(0.0f)
#endif
{
}

Vector3::Vector3(const class Vector4& v) :
#if defined(XO_SSE)
    m(v.m)
#else
    x(v.x), y(v.y), z(v.z)
#endif
{
}

Vector3& Vector3::Set(float x, float y, float z) {
#if defined(XO_SSE)
    m = _mm_set_ps(0.0f, z, y, x);
#else
    this->x = x;
    this->y = y;
    this->z = z;
#endif
    return *this;
}

Vector3& Vector3::Set(float f) {
#if defined(XO_SSE)
    m = _mm_set1_ps(f);
#else
    this->x = f;
    this->y = f;
    this->z = f;
#endif
    return *this;
}

Vector3& Vector3::Set(const Vector3& vec) {
#if defined(XO_SSE)
    m = vec.m;
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
#endif
    return *this;
}

#if defined(XO_SSE)
Vector3& Vector3::Set(const __m128& vec) {
    m = vec;
    return *this;
}
#endif

void Vector3::Get(float& x, float& y, float &z) const {
    x = this->x;
    y = this->y;
    z = this->z;
}

void Vector3::Get(float* f) const {
#if defined(XO_SSE)
    _mm_store_ps(f, m);
#else
    f[0] = this->x;
    f[1] = this->y;
    f[2] = this->z;
#endif
}

Vector3 Vector3::ZYX() const {
#if defined(XO_SSE)
    return Vector3(_mm_shuffle_ps(m, m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Y, IDX_Z)));
#else
    return Vector3(z, y, x);
#endif
}

_XOINL float Vector3::Sum() const {
#if defined(XO_SSE3)
    __m128 x = _mm_and_ps(m, MASK);
    x = _mm_hadd_ps(x, x);
    x = _mm_hadd_ps(x, x);
    return _mm_cvtss_f32(x);
#else
    return x+y+z;
#endif
}

float Vector3::MagnitudeSquared() const {
    Vector3 x = *this;
    x *= x;
    return x.Sum();
}

// todo: move to header
float Vector3::Magnitude() const {
    return Sqrt(MagnitudeSquared());
}

Vector3& Vector3::Normalize() {
    float magnitude = MagnitudeSquared();
    if (CloseEnough(magnitude, 1.0f, Epsilon)) {
        return *this; // already normalized
    }
    else if (CloseEnough(magnitude, 0.0f, Epsilon)) {
        return *this; // zero vec
    }
    else {
        magnitude = Sqrt(magnitude);
        return (*this) /= magnitude;
    }
}

// todo: move to header
Vector3 Vector3::Normalized() const {
    return Vector3(*this).Normalize();
}

// todo: move to header
bool Vector3::IsZero() const {
    return CloseEnough(MagnitudeSquared(), 0.0f, Epsilon);
}

// todo: move to header
bool Vector3::IsNormalized() const {
    return CloseEnough(MagnitudeSquared(), 1.0f, Epsilon);
}
 
float Vector3::Dot(const Vector3& a, const Vector3& b) {
#if defined(XO_SSE4_1)
    return _mm_cvtss_f32(_mm_dp_ps(a, b, 0x7f));
#elif defined(XO_SSE3)
    auto d = _mm_and_ps(_mm_mul_ps(a.m, b.m), MASK);
    d = _mm_hadd_ps(d, d);
    d = _mm_hadd_ps(d, d);
    return _mm_cvtss_f32(d);
#elif defined(XO_SSE)
    auto d = _mm_mul_ps(a.m, b.m);
    _MM_ALIGN16 float t[4];
    _mm_store_ps(t, d);
    return t[IDX_X] + t[IDX_Y] + t[IDX_Z];
#else
    return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
#endif
}
 
void Vector3::Cross(const Vector3& a, const Vector3& b, Vector3& outVec) {
#if defined(XO_SSE)
    // Todo: There's a trick to do this with three shuffles. Look into that.
    __m128 l = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)));
    __m128 r = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)));
    outVec.m = _mm_sub_ps(l, r);
#else
    outVec.x = (a.y*b.z)-(a.z*b.y);
    outVec.y = (a.z*b.x)-(a.x*b.z);
    outVec.z = (a.x*b.y)-(a.y*b.x);
#endif
}

void Vector3::Max(const Vector3& a, const Vector3& b, Vector3& outVec) {
    outVec.Set(_XO_MAX(a.x, b.x), _XO_MAX(a.y, b.y), _XO_MAX(a.z, b.z));
}
 
void Vector3::Min(const Vector3& a, const Vector3& b, Vector3& outVec) {
    outVec.Set(_XO_MIN(a.x, b.x), _XO_MIN(a.y, b.y), _XO_MIN(a.z, b.z));
}

void Vector3::Lerp(const Vector3& a, const Vector3& b, float t, Vector3& outVec) {
    if(CloseEnough(t, 0.0f)) {
        outVec.Set(a);
    }
    else if(CloseEnough(t, 1.0f)) {
        outVec.Set(b);
    } 
    else {
        outVec.Set(a + ((b - a) * t));
    }
}

void Vector3::RotateRadians(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec) {
    // Rodrigues' rotation formula
    // https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
    Vector3 axv;
    float sinAng = Sin(angle);
    float cosAng = Cos(angle);
    Vector3::Cross(axis, v, axv);
    float adv = Vector3::Dot(axis, v);
    outVec.Set(v * cosAng + axv * sinAng + axis * adv * (1.0f - cosAng));
}

// todo: move to header
void Vector3::RotateDegrees(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec) {
    RotateRadians(v, axis, angle * Deg2Rad, outVec);
}

float Vector3::AngleRadians(const Vector3& a, const Vector3& b) {
    Vector3 cross;
    Vector3::Cross(a, b, cross);
    cross *= cross;
    return ATan2(Sqrt(cross.Sum()), Vector3::Dot(a, b));
}

// todo: move to header
float Vector3::AngleDegrees(const Vector3& a, const Vector3& b) {
    return AngleRadians(a, b) * Rad2Deg;
}

// todo: move to header
float Vector3::DistanceSquared(const Vector3& a, const Vector3& b) {
    return (b - a).MagnitudeSquared();
}
 
// todo: move to header
float Vector3::Distance(const Vector3&a, const Vector3&b) {
    return (b - a).Magnitude();
}

void Vector3::RandomInConeRadians(const Vector3& forward, float angle, Vector3& outVec) {
    Vector3 cross;
    Vector3::Cross(forward, forward == Vector3::Up ? Vector3::Left : Vector3::Up, cross);
    Vector3::RotateRadians(forward, cross, RandomRange(0.0f, angle*0.5f), outVec);
    Vector3::RotateRadians(outVec, forward.Normalized(), RandomRange(0.0f, TAU), outVec);
}

void Vector3::RandomOnConeRadians(const Vector3& forward, float angle, Vector3& outVec) {
    Vector3 cross;
    Vector3::Cross(forward, forward == Vector3::Up ? Vector3::Left : Vector3::Up, cross);
    Vector3::RotateRadians(forward, cross, angle*0.5f, outVec);
    Vector3::RotateRadians(outVec, forward.Normalized(), RandomRange(0.0f, TAU), outVec);
}

void Vector3::RandomOnSphere(float radius, Vector3& outVec) {
    // Marsaglia's method: https://projecteuclid.org/download/pdf_1/euclid.aoms/1177692644
    float x1, x2, x12, x22;
    x1 = RandomRange(-1.0f, 1.0f);
    x2 = RandomRange(-1.0f, 1.0f);
    x12 = Square(x1);
    x22 = Square(x1);
    outVec.Set(
        2.0f * x1 * Sqrt(1.0f - x12 - x22),
        2.0f * x2 * Sqrt(1.0f - x12 - x22),
        1.0f - (2.0f * (x12 + x22))
        );
    outVec *= radius;
}

void Vector3::RandomInSphere(float minRadius, float maxRadius, Vector3& outVec) {
    RandomOnSphere(Sqrt(RandomRange(minRadius, maxRadius)), outVec);
}

void Vector3::RandomOnCube(Vector3& outVec) {
    RandomOnCube(0.5f, outVec);
}

void Vector3::RandomInCube(float size, Vector3& outVec) {
    outVec.Set(RandomRange(-size, size), RandomRange(-size, size), RandomRange(-size, size));
}

void Vector3::RandomOnCube(float size, Vector3& outVec) {
    switch (RandomRange(0, 5)) {
        case 0: outVec.Set(RandomRange(-size, size),    RandomRange(-size, size),                size);         break;
        case 1: outVec.Set(RandomRange(-size, size),    RandomRange(-size, size),               -size);         break;
        case 2: outVec.Set(RandomRange(-size, size),                 size,          RandomRange(-size, size));  break;
        case 3: outVec.Set(RandomRange(-size, size),                -size,          RandomRange(-size, size));  break;
        case 4: outVec.Set(             size,           RandomRange(-size, size),   RandomRange(-size, size));  break;
        case 5: outVec.Set(            -size,           RandomRange(-size, size),   RandomRange(-size, size));  break;
    }
}

void Vector3::RandomInFanRadians(const Vector3& forward, const Vector3& up, float angle, Vector3& outVec) {
    Vector3::RotateRadians(forward, up, RandomRange(-angle*0.5f, angle*0.5f), outVec);
}

void Vector3::RandomOnCircle(Vector3& outVec) {
    RandomOnCircle(Vector3::Up, 1.0f, outVec);
}

void Vector3::RandomInCircle(const Vector3& up, float radius, Vector3& outVec) {
    Vector3 cross;
    Vector3::Cross(up, up == Right ? Forward : Right, cross);
    Vector3::RotateRadians(cross, up.Normalized(), RandomRange(0.0f, TAU), outVec);
    outVec *= Sqrt(RandomRange(0.0f, 1.0f)) * radius;
}

void Vector3::RandomOnCircle(const Vector3& up, float radius, Vector3& outVec) {
    Vector3 cross;
    Vector3::Cross(up, up == Right ? Forward : Right, cross);
    Vector3::RotateRadians(cross, up.Normalized(), RandomRange(0.0f, TAU), outVec);
    outVec *= radius;
}



#undef IDX_X
#undef IDX_Y
#undef IDX_Z
#undef IDX_W

XOMATH_END_XO_NS();