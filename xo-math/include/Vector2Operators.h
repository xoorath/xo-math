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
static_assert(false, "Don't include Vector2Operators.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

float& Vector2::operator [](int i) { return f[i]; }
const float& Vector2::operator [](int i) const { return f[i]; }

Vector2 Vector2::operator - () const { return Vector2(-x, -y); }
Vector2 Vector2::operator ~ () const { return Vector2(y, x); }

const Vector2& Vector2::operator += (const Vector2& v) {
    x += v.x;
    y += v.y;
    return *this; 
}
const Vector2& Vector2::operator += (float v) {
    x += v;
    y += v;
    return *this; 
}
const Vector2& Vector2::operator += (double v)                  { return *this += (float)v; }
const Vector2& Vector2::operator += (int v)                     { return *this += (float)v; }
const Vector2& Vector2::operator += (const class Vector3& v)    { return *this += Vector2(v); }
const Vector2& Vector2::operator += (const class Vector4& v)    { return *this += Vector2(v); }

const Vector2& Vector2::operator -= (const Vector2& v) {
    x -= v.x;
    y -= v.y;
    return *this; 
}
const Vector2& Vector2::operator -= (float v) {
    x -= v;
    y -= v;
    return *this; 
}
const Vector2& Vector2::operator -= (double v)                  { return *this -= (float)v; }
const Vector2& Vector2::operator -= (int v)                     { return *this -= (float)v; }
const Vector2& Vector2::operator -= (const class Vector3& v)    { return *this -= Vector2(v); }
const Vector2& Vector2::operator -= (const class Vector4& v)    { return *this -= Vector2(v); }

const Vector2& Vector2::operator *= (const Vector2& v) {
    x *= v.x;
    y *= v.y;
    return *this; 
}
const Vector2& Vector2::operator *= (float v) {
    x *= v;
    y *= v;
    return *this;
}
const Vector2& Vector2::operator *= (double v)                  { return *this *= (float)v; }
const Vector2& Vector2::operator *= (int v)                     { return *this *= (float)v; }
const Vector2& Vector2::operator *= (const class Vector3& v)    { return *this *= Vector2(v); }
const Vector2& Vector2::operator *= (const class Vector4& v)    { return *this *= Vector2(v); }

const Vector2& Vector2::operator /= (const Vector2& v) {
    x /= v.x;
    y /= v.y;
    return *this;
}
const Vector2& Vector2::operator /= (float v) {
#if defined(XO_NO_INVERSE_DIVISION)
    x /= v;
    y /= v;
    return *this;
#else
    return (*this) *= (1.0f/v);
#endif
    
}
const Vector2& Vector2::operator /= (double v)                  { return *this /= (float)v; }
const Vector2& Vector2::operator /= (int v)                     { return *this /= (float)v; }
const Vector2& Vector2::operator /= (const class Vector3& v)    { return *this /= Vector2(v); }
const Vector2& Vector2::operator /= (const class Vector4& v)    { return *this /= Vector2(v); }

Vector2 Vector2::operator + (const Vector2& v) const        { return Vector2(*this) += v; }
Vector2 Vector2::operator + (float v) const                 { return Vector2(*this) += v; }
Vector2 Vector2::operator + (double v) const                { return Vector2(*this) += v; }
Vector2 Vector2::operator + (int v) const                   { return Vector2(*this) += v; }
Vector2 Vector2::operator + (const class Vector3& v) const  { return Vector2(*this) += v; }
Vector2 Vector2::operator + (const class Vector4& v) const  { return Vector2(*this) += v; }

Vector2 Vector2::operator - (const Vector2& v) const        { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (float v) const                 { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (double v) const                { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (int v) const                   { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (const class Vector3& v) const  { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (const class Vector4& v) const  { return Vector2(*this) -= v; }

Vector2 Vector2::operator * (const Vector2& v) const        { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (float v) const                 { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (double v) const                { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (int v) const                   { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (const class Vector3& v) const  { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (const class Vector4& v) const  { return Vector2(*this) *= v; }

Vector2 Vector2::operator / (const Vector2& v) const        { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (float v) const                 { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (double v) const                { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (int v) const                   { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (const class Vector3& v) const  { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (const class Vector4& v) const  { return Vector2(*this) /= v; }

bool Vector2::operator < (const Vector2& v) const           { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector2::operator < (float v) const                    { return MagnitudeSquared() < (v * v); }
bool Vector2::operator < (double v) const                   { return MagnitudeSquared() < (float)(v * v); }
bool Vector2::operator < (int v) const                      { return MagnitudeSquared() < (float)(v * v); }
bool Vector2::operator < (const class Vector3& v) const     { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector2::operator < (const class Vector4& v) const     { return MagnitudeSquared() < v.MagnitudeSquared(); }

bool Vector2::operator <= (const Vector2& v) const          { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector2::operator <= (float v) const                   { return MagnitudeSquared() <= (v * v); }
bool Vector2::operator <= (double v) const                  { return MagnitudeSquared() <= (float)(v * v); }
bool Vector2::operator <= (int v) const                     { return MagnitudeSquared() <= (float)(v * v); }
bool Vector2::operator <= (const class Vector3& v) const    { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector2::operator <= (const class Vector4& v) const    { return MagnitudeSquared() <= v.MagnitudeSquared(); }

bool Vector2::operator > (const Vector2& v) const           { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector2::operator > (float v) const                    { return MagnitudeSquared() > (v * v); }
bool Vector2::operator > (double v) const                   { return MagnitudeSquared() > (float)(v * v); }
bool Vector2::operator > (int v) const                      { return MagnitudeSquared() > (float)(v * v); }
bool Vector2::operator > (const class Vector3& v) const     { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector2::operator > (const class Vector4& v) const     { return MagnitudeSquared() > v.MagnitudeSquared(); }

bool Vector2::operator >= (const Vector2& v) const          { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector2::operator >= (float v) const                   { return MagnitudeSquared() >= (v * v); }
bool Vector2::operator >= (double v) const                  { return MagnitudeSquared() >= (float)(v * v); }
bool Vector2::operator >= (int v) const                     { return MagnitudeSquared() >= (float)(v * v); }
bool Vector2::operator >= (const class Vector3& v) const    { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector2::operator >= (const class Vector4& v) const    { return MagnitudeSquared() >= v.MagnitudeSquared(); }

bool Vector2::operator == (const Vector2& v) const {
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(x, v.x, Epsilon);
}
bool Vector2::operator == (float v) const                   { return CloseEnough(MagnitudeSquared(), v*v, Epsilon); }
bool Vector2::operator == (double v) const                  { return *this == (float)(v*v); }
bool Vector2::operator == (int v) const                     { return *this == (float)(v*v); }
bool Vector2::operator == (const class Vector3& v) const {
#   if defined(XO_SSE2)
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(_mm_set_ps(0.0f, 0.0f, y, x), v.m)), sse::Epsilon)) & 3) == 3;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon);
#   endif
}
bool Vector2::operator == (const class Vector4& v) const {
#   if defined(XO_SSE)
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(_mm_set_ps(0.0f, 0.0f, y, x), v.m)), sse::Epsilon)) & 3) == 3;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon);
#   endif
}

bool Vector2::operator != (const Vector2& v) const          { return !((*this) == v); }
bool Vector2::operator != (float v) const                   { return !((*this) == v); }
bool Vector2::operator != (double v) const                  { return !((*this) == v); }
bool Vector2::operator != (int v) const                     { return !((*this) == v); }
bool Vector2::operator != (const class Vector3& v) const    { return !((*this) == v); }
bool Vector2::operator != (const class Vector4& v) const    { return !((*this) == v); }

XOMATH_END_XO_NS();

#endif