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
static_assert(false, "Don't include Vector2Methods.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

Vector2::Vector2() {
}

Vector2::Vector2(float v) : x(v), y(v) {
}

Vector2::Vector2(float x, float y) : x(x), y(y) {
}

Vector2::Vector2(const Vector2& v) : x(v.x), y(v.y) {
}

Vector2::Vector2(const class Vector3& v) : x(v.x), y(v.y) {
}

Vector2::Vector2(const class Vector4& v) : x(v.x), y(v.y) {
}

const Vector2& Vector2::Set(float x, float y) {
    this->x = x;
    this->y = y;
    return *this;
}

const Vector2& Vector2::Set(float v) {
    x = v;
    y = v;
    return *this;
}

const Vector2& Vector2::Set(const Vector2& v) {
    x = v.x;
    y = v.y;
    return *this;
}

void Vector2::Get(float& x, float& y) const {
    x = this->x;
    y = this->y;
}
void Vector2::Get(float* f) const {
    f[0] = this->x;
    f[1] = this->y;
}

float Vector2::Sum() const {
    return x+y;
}

float Vector2::Magnitude() const {
    return sqrtf(x*x + y*y);
}

float Vector2::MagnitudeSquared() const {
    return x*x + y*y;
}

const Vector2& Vector2::Normalize() {
    float magnitude = MagnitudeSquared();
    if (CloseEnough(magnitude, 1.0f, Epsilon))
        return *this; // already normalized
    if (CloseEnough(magnitude, 0.0f, Epsilon))
        return *this; // zero vec
    magnitude = sqrtf(magnitude);
    (*this) /= magnitude;
    return *this;
}

Vector2 Vector2::Normalized() const {
    return Vector2(*this).Normalize();
}

bool Vector2::IsZero() const {
    return CloseEnough(MagnitudeSquared(), 0.0f, Epsilon);
}

bool Vector2::IsNormalized() const {
  // todo: check closeness
    return CloseEnough(MagnitudeSquared(), 1.0f, Epsilon);
}


float Vector2::Dot(const Vector2& a, const Vector2& b) {
    return (a.x * b.x) + (a.y * b.y);
}

float Vector2::Cross(const Vector2& a, const Vector2& b) {
    return (a.x * b.y) - (a.y * b.x);
}


float Vector2::AngleRadians(const Vector2& a, const Vector2& b) {
    return -ATan2(Cross(a, b), Dot(a, b));
}

float Vector2::AngleDegrees(const Vector2& a, const Vector2& b) {
    return AngleRadians(a, b) * Rad2Deg;
}

float Vector2::Distance(const Vector2& a, const Vector2& b) {
    return (b-a).Magnitude();
}

float Vector2::DistanceSquared(const Vector2& a, const Vector2& b) {
    return (b-a).MagnitudeSquared();
}

void Vector2::Max(const Vector2& a, const Vector2& b, Vector2& outVec) {
    outVec.Set(_XO_MAX(a.x, b.x), _XO_MAX(a.y, b.y));
}

void Vector2::Min(const Vector2& a, const Vector2& b, Vector2& outVec) {
    outVec.Set(_XO_MIN(a.x, b.x), _XO_MIN(a.y, b.y));
}

void Vector2::OrthogonalCCW(const Vector2& v, Vector2& outVec) {
    outVec.Set(-v.y, v.x);
}

void Vector2::OrthogonalCW(const Vector2& v, Vector2& outVec) {
    outVec.Set(v.y, -v.x);
}

void Vector2::Lerp(const Vector2& a, const Vector2& b, float t, Vector2& outVec) {
    if(CloseEnough(t, 0.0f, Epsilon)) {
        outVec.Set(a);
    }
    else if(CloseEnough(t, 1.0f, Epsilon)) {
        outVec.Set(b);
    }
    else {
        outVec.Set(a + ((b - a) * t));
    }
}

void Vector2::Midpoint(const Vector2& a, const Vector2& b, Vector2& outVec) {
    Vector2::Lerp(a, b, 0.5f, outVec);
}

Vector2 Vector2::Max(const Vector2& a, const Vector2& b) {
    Vector2 tempVec;
    Max(a, b, tempVec);
    return tempVec;
}

Vector2 Vector2::Min(const Vector2& a, const Vector2& b) {
    Vector2 tempVec;
    Min(a, b, tempVec);
    return tempVec;
}

Vector2 Vector2::OrthogonalCCW(const Vector2& v) {
    Vector2 tempVec;
    OrthogonalCCW(v, tempVec);
    return tempVec;
}

Vector2 Vector2::OrthogonalCW(const Vector2& v) {
    Vector2 tempVec;
    OrthogonalCW(v, tempVec);
    return tempVec;
}

Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float t) {
    Vector2 tempVec;
    Lerp(a, b, t, tempVec);
    return tempVec;
}

Vector2 Vector2::Midpoint(const Vector2& a, const Vector2& b) {
    Vector2 tempVec;
    Midpoint(a, b, tempVec);
    return tempVec;
}

float Vector2::Dot(const Vector2& v) const                  { return Dot(*this, v); }
float Vector2::Cross(const Vector2& v) const                { return Cross(*this, v); }
Vector2 Vector2::OrthogonalCCW() const                      { return OrthogonalCCW(*this); }
Vector2 Vector2::OrthogonalCW() const                       { return OrthogonalCW(*this); }
float Vector2::AngleRadians(const Vector2& v) const         { return AngleRadians(*this, v); }
float Vector2::AngleDegrees(const Vector2& v) const         { return AngleDegrees(*this, v); }
float Vector2::Distance(const Vector2& v) const             { return Distance(*this, v); }
float Vector2::DistanceSquared(const Vector2& v) const      { return DistanceSquared(*this, v); }
Vector2 Vector2::Lerp(const Vector2& v, float t) const      { return Lerp(*this, v, t); }
Vector2 Vector2::Midpoint(const Vector2& v) const           { return Midpoint(*this, v); }

XOMATH_END_XO_NS();

#endif