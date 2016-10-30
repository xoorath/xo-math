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

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector3Operators.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

#if defined(XO_SSE)
Vector3::operator __m128() const {
    return m;
}
#endif

float& Vector3::operator [](int i) {
    return f[i];
}

const float& Vector3::operator [](int i) const {
    return f[i];
}

Vector3 Vector3::operator -() const {
#if defined(XO_SSE)
    return Vector3(_mm_mul_ps(m, sse::NegativeOne));
#else
    return Vector3(-x, -y, -z);
#endif
}

Vector3 Vector3::operator ~() const {
    return ZYX();
}

const Vector3& Vector3::operator += (const Vector3& v) {
#if defined(XO_SSE)
    m = _mm_add_ps(m, v);
#else
    x += v.x;
    y += v.y;
    z += v.z;
#endif
    return *this;
}

const Vector3& Vector3::operator += (float v) {
#if defined(XO_SSE)
    m = _mm_add_ps(m, _mm_set_ps1(v));
#else
    x += v;
    y += v;
    z += v;
#endif
    return *this;
}

const Vector3& Vector3::operator += (double v)                  { return (*this) += float(v); }
const Vector3& Vector3::operator += (int v)                     { return (*this) += float(v); }
const Vector3& Vector3::operator += (const class Vector2& v)    { return (*this) += Vector3(v); }
const Vector3& Vector3::operator += (const class Vector4& v)    { return (*this) += Vector3(v); }

const Vector3& Vector3::operator -= (const Vector3& v) {
#if defined(XO_SSE)
    m = _mm_sub_ps(m, v);
#else
    x -= v.x;
    y -= v.y;
    z -= v.z;
#endif
    return *this;
}

const Vector3& Vector3::operator -= (float v) {
#if defined(XO_SSE)
    m = _mm_sub_ps(m, _mm_set_ps1(v));
#else
    x -= v;
    y -= v;
    z -= v;
#endif
    return *this;
}

const Vector3& Vector3::operator -= (double v)                  { return (*this) -= float(v); }
const Vector3& Vector3::operator -= (int v)                     { return (*this) -= float(v); }
const Vector3& Vector3::operator -= (const class Vector2& v)    { return (*this) -= Vector3(v); }
const Vector3& Vector3::operator -= (const class Vector4& v)    { return (*this) -= Vector3(v); }

const Vector3& Vector3::operator *= (const Vector3& v) {
#if defined(XO_SSE)
    m = _mm_mul_ps(m, v);
#else
    x *= v.x;
    y *= v.y;
    z *= v.z;
#endif
    return *this;
}

const Vector3& Vector3::operator *= (float v) {
#if defined(XO_SSE)
    m = _mm_mul_ps(m, _mm_set_ps1(v));
#else
    x *= v;
    y *= v;
    z *= v;
#endif
    return *this;
}

const Vector3& Vector3::operator *= (double v)                  { return (*this) *= float(v); }
const Vector3& Vector3::operator *= (int v)                     { return (*this) *= float(v); }
const Vector3& Vector3::operator *= (const class Vector2& v)    { return (*this) *= Vector3(v); }
const Vector3& Vector3::operator *= (const class Vector4& v)    { return (*this) *= Vector3(v); }

#if defined(XO_NO_INVERSE_DIVISION)
const Vector3& Vector3::operator /= (const Vector3& v) {
#if defined(XO_SSE)
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
    m = _mm_div_ps(m, v);
#else
    x /= v.x;
    y /= v.y;
    z /= v.z;
#endif

    return *this;
}

const Vector3& Vector3::operator /= (float v) {
#   if defined(XO_SSE)
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
#   if defined(XO_SSE)
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
    m = _mm_mul_ps(m, _mm_rcp_ps(v));
#   else
    x /= v.x;
    y /= v.y;
    z /= v.z;
#   endif

    return *this;
}

const Vector3& Vector3::operator /= (float v) { 
#   if defined(XO_SSE)
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
const Vector3& Vector3::operator /= (double v)                  { return (*this) /= float(v); }
const Vector3& Vector3::operator /= (int v)                     { return (*this) /= float(v); }
const Vector3& Vector3::operator /= (const class Vector2& v)    { return (*this) /= Vector3(v); }
const Vector3& Vector3::operator /= (const class Vector4& v)    { return (*this) /= Vector3(v); }

Vector3 Vector3::operator + (const Vector3& v) const        { return Vector3(*this) += v; }
Vector3 Vector3::operator + (float v) const                 { return Vector3(*this) += v; }
Vector3 Vector3::operator + (double v) const                { return Vector3(*this) += v; }
Vector3 Vector3::operator + (int v) const                   { return Vector3(*this) += v; }
Vector3 Vector3::operator + (const class Vector2& v) const  { return Vector3(*this) += v; }
Vector3 Vector3::operator + (const class Vector4& v) const  { return Vector3(*this) += v; }

Vector3 Vector3::operator - (const Vector3& v) const        { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (float v) const                 { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (double v) const                { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (int v) const                   { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (const class Vector2& v) const  { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (const class Vector4& v) const  { return Vector3(*this) -= v; }

Vector3 Vector3::operator * (const Vector3& v) const        { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (float v) const                 { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (double v) const                { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (int v) const                   { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (const class Vector2& v) const  { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (const class Vector4& v) const  { return Vector3(*this) *= v; }

Vector3 Vector3::operator / (const Vector3& v) const        { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (float v) const                 { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (double v) const                { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (int v) const                   { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (const class Vector2& v) const  { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (const class Vector4& v) const  { return Vector3(*this) /= v; }


bool Vector3::operator < (const Vector3& v) const       { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector3::operator < (float v) const                { return MagnitudeSquared() < (v * v); }
bool Vector3::operator < (double v) const               { return MagnitudeSquared() < (float)(v * v); }
bool Vector3::operator < (int v) const                  { return MagnitudeSquared() < (float)(v * v); }
bool Vector3::operator < (const class Vector2& v) const { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector3::operator < (const class Vector4& v) const { return MagnitudeSquared() < v.MagnitudeSquared(); }

bool Vector3::operator <= (const Vector3& v) const          { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector3::operator <= (float v) const                   { return MagnitudeSquared() <= (v * v); }
bool Vector3::operator <= (double v) const                  { return MagnitudeSquared() <= (float)(v * v); }
bool Vector3::operator <= (int v) const                     { return MagnitudeSquared() <= (float)(v * v); }
bool Vector3::operator <= (const class Vector2& v) const    { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector3::operator <= (const class Vector4& v) const    { return MagnitudeSquared() <= v.MagnitudeSquared(); }

bool Vector3::operator > (const Vector3& v) const       { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector3::operator > (float v) const                { return MagnitudeSquared() > (v * v); }
bool Vector3::operator > (double v) const               { return MagnitudeSquared() > (float)(v * v); }
bool Vector3::operator > (int v) const                  { return MagnitudeSquared() > (float)(v * v); }
bool Vector3::operator > (const class Vector2& v) const { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector3::operator > (const class Vector4& v) const { return MagnitudeSquared() > v.MagnitudeSquared(); }

bool Vector3::operator >= (const Vector3& v) const          { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector3::operator >= (float v) const                   { return MagnitudeSquared() >= (v * v); }
bool Vector3::operator >= (double v) const                  { return MagnitudeSquared() >= (float)(v * v); }
bool Vector3::operator >= (int v) const                     { return MagnitudeSquared() >= (float)(v * v); }
bool Vector3::operator >= (const class Vector2& v) const    { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector3::operator >= (const class Vector4& v) const    { return MagnitudeSquared() >= v.MagnitudeSquared(); }

bool Vector3::operator == (const Vector3& v) const {
#   if defined(XO_SSE2)
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(v, m)), sse::Epsilon)) & 7) == 7;
#   elif XO_SSE
    // TODO: find a faster way with SSE to do a 'close enough' check.
    // I'm not sure if there's a way to do the sign bit masking like we have in sse::Abs to acomplish
    // what we're doing in SSE2
    return CloseEnough(x, v.x, sse::SSEFloatEpsilon) && CloseEnough(y, v.y, sse::SSEFloatEpsilon) && CloseEnough(z, v.z, sse::SSEFloatEpsilon);
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon) && CloseEnough(z, v.z, Epsilon);
#   endif
}
bool Vector3::operator == (float v) const                   { return CloseEnough(MagnitudeSquared(), v*v, Epsilon);}
bool Vector3::operator == (double v) const                  { return CloseEnough(MagnitudeSquared(), (float)(v*v), Epsilon);}
bool Vector3::operator == (int v) const                     { return CloseEnough(MagnitudeSquared(), (float)(v*v), Epsilon);}
bool Vector3::operator == (const class Vector2& v) const {
#   if defined(XO_SSE)
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(_mm_set_ps(0.0f, 0.0f, v.y, v.x), m)), sse::Epsilon)) & 3) == 3;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon);
#   endif
}
bool Vector3::operator == (const class Vector4& v) const {
#   if defined(XO_SSE)
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(v.m, m)), sse::Epsilon)) & 7) == 7;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon) && CloseEnough(z, v.z, Epsilon);
#   endif
}

bool Vector3::operator != (const Vector3& v) const          { return !((*this) == v); }
bool Vector3::operator != (float v) const                   { return !((*this) == v); }
bool Vector3::operator != (double v) const                  { return !((*this) == v); }
bool Vector3::operator != (int v) const                     { return !((*this) == v); }
bool Vector3::operator != (const class Vector2& v) const    { return !((*this) == v); }
bool Vector3::operator != (const class Vector4& v) const    { return !((*this) == v); }

XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL