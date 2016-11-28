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

Vector3::Vector3(float f) :
#if defined(XO_SSE)
    xmm(_mm_set1_ps(f))
#elif defined(XO_NEON)
    n(vdupq_n_f32(f))
#else
    x(f), y(f), z(f)
#endif
{
}

Vector3::Vector3(float x, float y, float z) :
#if defined(XO_SSE)
    xmm(_mm_set_ps(0.0f, z, y, x))
#else
    x(x), y(y), z(z)
#endif
{
}

Vector3::Vector3(const Vector3& vec) :
#if defined(XO_SSE)
    xmm(vec)
#elif defined(XO_NEON)
    n(vec.n)
#else
    x(vec.x), y(vec.y), z(vec.z) 
#endif
{
}

#if defined(XO_SSE)
Vector3::Vector3(const __m128& vec) : 
    xmm(vec)
{
}

#elif defined(XO_NEON)
Vector3::Vector3(const float32x4_t& vec) :
    n(vec)
{
}
#endif

Vector3::Vector3(const class Vector2& v) :
#if defined(XO_SSE)
    xmm(_mm_set_ps(0.0f, 0.0f, v.y, v.x))
#else
    x(v.x), y(v.y), z(0.0f)
#endif
{
}

Vector3::Vector3(const class Vector4& v) :
#if defined(XO_SSE)
    xmm(v.xmm)
//#elif defined(XO_NEON)
//    n(v.n)
#else
    x(v.x), y(v.y), z(v.z)
#endif
{
}

Vector3& Vector3::Set(float x, float y, float z) {
#if defined(XO_SSE)
    xmm = _mm_set_ps(0.0f, z, y, x);
#else
    this->x = x;
    this->y = y;
    this->z = z;
#endif
    return *this;
}

Vector3& Vector3::Set(float f) {
#if defined(XO_SSE)
    xmm = _mm_set1_ps(f);
#elif defined(XO_NEON)
    n = vdupq_n_f32(f);
#else
    this->x = f;
    this->y = f;
    this->z = f;
#endif
    return *this;
}

Vector3& Vector3::Set(const Vector3& vec) {
#if defined(XO_SSE)
    xmm = vec.xmm;
#elif defined(XO_NEON)
    n = vec.n;
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
#endif
    return *this;
}

#if defined(XO_SSE)
Vector3& Vector3::Set(const __m128& vec) {
    xmm = vec;
    return *this;
}
#elif defined(XO_NEON)
Vector3& Vector3::Set(const float32x4_t& vec) {
    n = vec;
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
    _mm_store_ps(f, xmm);
#elif defined(XO_NEON)
    vst1q_f32(f, n);
#else
    f[0] = this->x;
    f[1] = this->y;
    f[2] = this->z;
#endif
}

Vector3 Vector3::ZYX() const {
#if defined(XO_SSE)
    // todo: constexpr shuffle int
    return Vector3(_mm_shuffle_ps(xmm, xmm, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Y, IDX_Z)));
#else
    return Vector3(z, y, x);
#endif
}

float Vector3::Sum() const {
#if defined(XO_SSE3)
    __m128 x = _mm_and_ps(xmm, MASK);
    x = _mm_hadd_ps(x, x);
    x = _mm_hadd_ps(x, x);
    return _mm_cvtss_f32(x);
#else
    return x+y+z;
#endif
}

Vector3& Vector3::Normalize() {
    return (*this) /= Magnitude();
}

Vector3& Vector3::NormalizeSafe() {
    float magnitude = MagnitudeSquared();
    if (magnitude == 0.0f)
        return *this;
    return *this /= Sqrt(magnitude);
}
 
float Vector3::Dot(const Vector3& a, const Vector3& b) {
#if defined(XO_SSE4_1)
    return _mm_cvtss_f32(_mm_dp_ps(a, b, 0x7f));
#else
    return (a * b).Sum();
#endif
}
 
void Vector3::Cross(const Vector3& a, const Vector3& b, Vector3& outVec) {
#if defined(XO_SSE)
    // todo: multi-compiler constexpr
    constexpr int m3021 = _MM_SHUFFLE(3, 0, 2, 1);
    __m128 result = _mm_sub_ps(_mm_mul_ps(a, _mm_shuffle_ps(b, b, m3021)), _mm_mul_ps(b, _mm_shuffle_ps(a, a, m3021)));
    outVec.xmm = _mm_shuffle_ps(result, result, m3021);
#else
    outVec.x = (a.y*b.z)-(a.z*b.y);
    outVec.y = (a.z*b.x)-(a.x*b.z);
    outVec.z = (a.x*b.y)-(a.y*b.x);
#endif
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

float Vector3::AngleRadians(const Vector3& a, const Vector3& b) {
    Vector3 cross;
    Vector3::Cross(a, b, cross);
    cross *= cross;
    return ATan2(Sqrt(cross.Sum()), Vector3::Dot(a, b));
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