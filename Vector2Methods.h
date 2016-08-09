#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector2Methods.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

Vector2::Vector2() : x(0.0f), y(0.0f) {
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

void Vector2::Set(float x, float y) {
    this->x = x;
    this->y = y;
}

void Vector2::Set(float v) {
    this->x = v;
    this->y = v;
}

void Vector2::Set(const Vector2& v) {
    this->x = v[0];
    this->y = v[1];
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

Vector2 Vector2::Max(const Vector2& a, const Vector2& b) {
    if (a.MagnitudeSquared() >= b.MagnitudeSquared())
        return a;
    return b;
}

Vector2 Vector2::Min(const Vector2& a, const Vector2& b) {
    if (a.MagnitudeSquared() <= b.MagnitudeSquared())
        return a;
    return b;
}

float Vector2::Dot(const Vector2& a, const Vector2& b) {
    return (a.x * b.x) + (a.y * b.y);
}

float Vector2::Cross(const Vector2& a, const Vector2& b) {
    return (a.x * b.y) - (a.y * b.x);
}

// input vector rotated 90 degrees
Vector2 Vector2::OrthogonalCCW(const Vector2& v) {
    return Vector2(-v.y, v.x);
}

// input vector rotated -90 degrees
Vector2 Vector2::OrthogonalCW(const Vector2& v) {
    return Vector2(v.y, -v.x);
}

float Vector2::AngleRadians(const Vector2& a, const Vector2& b) {
#ifdef XOMATH_VEC2_CW
    return -atan2(Cross(a, b), Dot(a, b));
#else
    return atan2(Cross(a, b), Dot(a, b));
#endif
}

float Vector2::AngleDegrees(const Vector2& a, const Vector2& b) {
    return AngleRadians(a, b) * Rad2Deg;
}

Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float t) {
    return a + ((b - a) * t);
}


float Vector2::Dot(const Vector2& v) const              { return Dot(*this, v); }
Vector2 Vector2::Cross(const Vector2& v) const          { return Cross(*this, v); }
Vector2 Vector2::OrthogonalCCW() const                  { return OrthogonalCCW(*this); }
Vector2 Vector2::OrthogonalCW() const                   { return OrthogonalCW(*this); }
float Vector2::AngleRadians(const Vector2& v) const     { return AngleRadians(*this, v); }
float Vector2::AngleDegrees(const Vector2& v) const     { return AngleDegrees(*this, v); }
Vector2 Vector2::Lerp(const Vector2& v, float t) const  { return Lerp(*this, v, t); }

XOMATH_END_XO_NS

#endif