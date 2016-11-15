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
const float Vector4::Epsilon = sse::SSEFloatEpsilon * 4.0f;
#   else
const float Vector4::Epsilon = FloatEpsilon * 4.0f;
#   endif
#endif

const Vector4 Vector4::One(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 Vector4::Zero(0.0f, 0.0f, 0.0f, 0.0f);

const Vector4 Vector4::UnitX(1.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::UnitY(0.0f, 1.0f, 0.0f, 0.0f);
const Vector4 Vector4::UnitZ(0.0f, 0.0f, 1.0f, 0.0f);
const Vector4 Vector4::UnitW(0.0f, 0.0f, 0.0f, 1.0f);

#define IDX_X 0
#define IDX_Y 1
#define IDX_Z 2
#define IDX_W 3

Vector4::Vector4() {
}

Vector4::Vector4(float f) :
#if defined(XO_SSE)
    xmm(_mm_set1_ps(f))
{
}
#else
    x(f), y(f), z(f), w(f)
{
}
#endif

Vector4::Vector4(float x, float y, float z, float w) :
#if defined(XO_SSE)
    xmm(_mm_set_ps(w, z, y, x))
{
}
#else
    x(x), y(y), z(z), w(w)
{
}
#endif
Vector4::Vector4(const Vector4& vec) :
#if defined(XO_SSE)
    xmm(vec.xmm)
{
}
#else
    x(vec.x), y(vec.y), z(vec.z), w(vec.w)
{
}
#endif


#if defined(XO_SSE)
Vector4::Vector4(const __m128& vec) : 
    xmm(vec)
{
}
#endif

Vector4::Vector4(const class Vector2& vec) :
#if defined(XO_SSE)
    xmm(_mm_set_ps(0.0f, 0.0f, vec.y, vec.x)) 
{
}
#else
    x(vec.x), y(vec.y), z(0.0f), w(0.0f)
{
}
#endif

Vector4::Vector4(const class Vector2& vec, float z, float w) :
#if defined(XO_SSE)
    xmm(_mm_set_ps(w, z, vec.y, vec.x))
{
}
#else
    x(vec.x), y(vec.y), z(z), w(w)
{
}
#endif

Vector4::Vector4(const class Vector3& vec) :
#if defined(XO_SSE)
    xmm(vec.xmm)
{
}
#else
    x(vec.x), y(vec.y), z(vec.z), w(0.0f)
{
}
#endif

Vector4::Vector4(const class Vector3& vec, float w) :
#if defined(XO_SSE)
    xmm(vec.xmm)
{
    //! @todo there's likely an sse way to do this.
    w = w;
}
#else
    x(vec.x), y(vec.y), z(vec.z), w(w)
{
}
#endif

Vector4::Vector4(const class Quaternion& q) :
#if defined(XO_SSE)
    xmm(q.xmm)
#else
    x(q.x), y(q.y), z(q.z), w(q.w)
#endif
{
}

Vector4& Vector4::Set(float x, float y, float z, float w) {
#if defined(XO_SSE)
    xmm = _mm_set_ps(w, z, y, x);
#else
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
#endif
    return *this;
}

Vector4& Vector4::Set(float f) {
#if defined(XO_SSE)
    xmm = _mm_set1_ps(f);
#else
    this->x = f;
    this->y = f;
    this->z = f;
    this->w = w;
#endif
    return *this;
}

Vector4& Vector4::Set(const Vector4& vec) {
#if defined(XO_SSE)
    xmm = vec.xmm;
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = vec.w;
#endif
    return *this;
}

Vector4& Vector4::Set(const Vector2& vec) {
#if defined(XO_SSE)
    xmm = _mm_set_ps(0.0f, 0.0f, vec.y, vec.x);
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = 0.0f;
    this->w = 0.0f;
#endif
    return *this;
}

Vector4& Vector4::Set(const Vector2& vec, float z, float w) {
#if defined(XO_SSE)
    xmm = _mm_set_ps(w, z, vec.y, vec.x);
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = z;
    this->w = w;
#endif
    return *this;
}

Vector4& Vector4::Set(const Vector3& vec) {
#if defined(XO_SSE)
    // TODO: mask with xmm, don't just break out w
    this->xmm = vec.xmm;
    this->w = 0.0f;
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = 0.0f;
#endif
    return *this;
}

Vector4& Vector4::Set(const Vector3& vec, float w) {
#if defined(XO_SSE)
    // todo: consider masking here.
    this->xmm = vec.xmm;
    this->w = w;
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = w;
#endif
    return *this;
}

#if defined(XO_SSE)
Vector4& Vector4::Set(const __m128& vec) {
    xmm = vec;
    return *this;
}
#endif

void Vector4::Get(float& x, float& y, float& z, float& w) const {
    x = this->x;
    y = this->y;
    z = this->z;
    w = this->w;
}

void Vector4::Get(float* f) const {
#if defined(XO_SSE)
    _mm_store_ps(f, xmm);
#else
    f[0] = this->x;
    f[1] = this->y;
    f[2] = this->z;
    f[3] = this->w;
#endif
}
 
float Vector4::Sum() const {
#if defined(XO_SSE)
    __m128 s = _mm_hadd_ps(xmm, xmm);
    return _mm_cvtss_f32(_mm_hadd_ps(s, s));
#else
    return x+y+z+w;
#endif
}

Vector4& Vector4::NormalizeSafe() {
    float magnitude = MagnitudeSquared();
    if (magnitude == 0.0f)
        return *this;
    return *this /= Sqrt(magnitude);
}

float Vector4::Dot(const Vector4& a, const Vector4& b) {
#if defined(XO_SSE4)
    return _mm_cvtss_f32(_mm_dp_ps(a.xmm, b.xmm, 0xff));
#else
    return (a * b).Sum();
#endif
}

XOMATH_END_XO_NS();