#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector2Operators.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

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
const Vector2& Vector2::operator += (double v)          { return *this += (float)v; }
const Vector2& Vector2::operator += (int v)             { return *this += (float)v; }
const Vector2& Vector2::operator += (const Vector3& v)  { return *this += Vector2(v); }
const Vector2& Vector2::operator += (const Vector4& v)  { return *this += Vector2(v); }

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
const Vector2& Vector2::operator -= (double v)          { return *this -= (float)v; }
const Vector2& Vector2::operator -= (int v)             { return *this -= (float)v; }
const Vector2& Vector2::operator -= (const Vector3& v)  { return *this -= Vector2(v); }
const Vector2& Vector2::operator -= (const Vector4& v)  { return *this -= Vector2(v); }

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
const Vector2& Vector2::operator *= (double v)          { return *this *= (float)v; }
const Vector2& Vector2::operator *= (int v)             { return *this *= (float)v; }
const Vector2& Vector2::operator *= (const Vector3& v)  { return *this; }
const Vector2& Vector2::operator *= (const Vector4& v)  { return *this; }

const Vector2& Vector2::operator /= (const Vector2& v) {
    x /= v.x;
    y /= v.y;
    return *this;
}
const Vector2& Vector2::operator /= (float v) {
#if XO_NO_INVERSE_DIVISION
    x /= v;
    y /= v;
    return *this;
#else
    return (*this) *= (1.0f/v);
#endif
    
}
const Vector2& Vector2::operator /= (double v)          { return *this /= (float)v; }
const Vector2& Vector2::operator /= (int v)             { return *this /= (float)v; }
const Vector2& Vector2::operator /= (const Vector3& v)  { return *this /= Vector2(v); }
const Vector2& Vector2::operator /= (const Vector4& v)  { return *this /= Vector2(v); }

Vector2 Vector2::operator + (const Vector2& v) const    { return Vector2(*this) += v; }
Vector2 Vector2::operator + (float v) const             { return Vector2(*this) += v; }
Vector2 Vector2::operator + (double v) const            { return Vector2(*this) += v; }
Vector2 Vector2::operator + (int v) const               { return Vector2(*this) += v; }
Vector2 Vector2::operator + (const Vector3& v) const    { return Vector2(*this) += v; }
Vector2 Vector2::operator + (const Vector4& v) const    { return Vector2(*this) += v; }

Vector2 Vector2::operator - (const Vector2& v) const    { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (float v) const             { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (double v) const            { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (int v) const               { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (const Vector3& v) const    { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (const Vector4& v) const    { return Vector2(*this) -= v; }

Vector2 Vector2::operator * (const Vector2& v) const    { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (float v) const             { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (double v) const            { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (int v) const               { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (const Vector3& v) const    { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (const Vector4& v) const    { return Vector2(*this) *= v; }

Vector2 Vector2::operator / (const Vector2& v) const    { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (float v) const             { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (double v) const            { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (int v) const               { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (const Vector3& v) const    { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (const Vector4& v) const    { return Vector2(*this) /= v; }

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
#   if XO_SSE2
    // Todo: check that this is actually faster.
    return (_mm_movemask_ps(_mm_cmplt_ps(SSE::Abs(_mm_sub_ps(_mm_set_ps(0.0f, 0.0f, y, x), v.m)), SSE::Epsilon)) & 0b0011) == 0b0011;
#   elif XO_SSE
    // TODO: find a faster way with SSE to do a 'close enough' check.
    // I'm not sure if there's a way to do the sign bit masking like we have in SSE::Abs to acomplish
    // what we're doing in SSE2
    return CloseEnough(x, v.x, SSE::SSEFloatEpsilon) && CloseEnough(y, v.y, SSE::SSEFloatEpsilon);
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon);
#   endif
}
bool Vector2::operator == (const class Vector4& v) const {
#   if XO_SSE2
    // Todo: check that this is actually faster.
    return (_mm_movemask_ps(_mm_cmplt_ps(SSE::Abs(_mm_sub_ps(_mm_set_ps(0.0f, 0.0f, y, x), v.m)), SSE::Epsilon)) & 0b0011) == 0b0011;
#   elif XO_SSE
    // TODO: find a faster way with SSE to do a 'close enough' check.
    // I'm not sure if there's a way to do the sign bit masking like we have in SSE::Abs to acomplish
    // what we're doing in SSE2
    return CloseEnough(x, v.x, SSE::SSEFloatEpsilon) && CloseEnough(y, v.y, SSE::SSEFloatEpsilon);
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

XOMATH_END_XO_NS

#endif