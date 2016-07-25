#pragma once

XOMATH_BEGIN_XO_NS
class Vector2 {
public:
  constexpr static const float Epsilon = FloatEpsilon * 2.0f;

  Vector2() : x(0.0f), y(0.0f) {
  }
  Vector2(float v) : x(v), y(v) {
  }
  Vector2(float x, float y) : x(x), y(y) {
  }
  Vector2(const Vector2& v) : x(v[0]), y(v[1]) {
  }
  Vector2(const class Vector3& v);

  float& operator [](int i) { switch (i) { default: case 0: return x; case 1: return y; } }
  const float& operator [](int i) const { switch (i) { default: case 0: return x; case 1: return y; } }

  void Set(float x, float y) {
    this->x = x;
    this->y = y;
  }

  void Set(float v) {
    this->x = v;
    this->y = v;
  }

  void Set(const Vector2& v) {
    this->x = v[0];
    this->y = v[1];
  }

#if defined(VEC2D_SIMPLE_OP) || defined(VEC2D_SIMPLE_OP_ADD)
  static_assert(false, "Vector3 found an internal macro where it shouldn't have.");
#endif
#define VEC2D_SIMPLE_OP(op) \
  Vector2 operator op (const Vector2& v) const { return Vector2(x op v[0], y op v[1]); } \
  Vector2 operator op (float v) const { return Vector2(x op v, y op v); } \
  Vector2 operator op (double v) const { return Vector2(x op (float)v, y op (float)v); } \
  Vector2 operator op (int v) const { return Vector2(x op (float)v, y op (float)v); }
#define VEC2D_SIMPLE_OP_ADD(op) \
  const Vector2& operator op (const Vector2& v) { x op v[0]; y op v[1]; return *this; } \
  const Vector2& operator op (float v) { x op v; y op v; return *this; } \
  const Vector2& operator op (double v) { x op (float)v; y op (float)v; return *this; } \
  const Vector2& operator op (int v) { x op (float)v; y op (float)v; return *this; }

  VEC2D_SIMPLE_OP(+)
  VEC2D_SIMPLE_OP(-)
  VEC2D_SIMPLE_OP(*)
  VEC2D_SIMPLE_OP(/)
  VEC2D_SIMPLE_OP_ADD(+=)
  VEC2D_SIMPLE_OP_ADD(-=)
  VEC2D_SIMPLE_OP_ADD(*=)
  VEC2D_SIMPLE_OP_ADD(/=)

  #undef VEC2D_SIMPLE_OP
  #undef VEC2D_SIMPLE_OP_ADD

  Vector2 operator - () const { return Vector2(-x, -y); }
  Vector2 operator ~ () const { return Vector2(y, x); }

  Vector2 operator + (const class Vector3& v) const;

  float Magnitude() const {
    return sqrtf(x*x + y*y);
  }

  float MagnitudeSquared() const {
    return x*x + y*y;
  }

  void Normalize() {
    float magnitude = MagnitudeSquared();
    if (magnitude - 1.0f <= Epsilon)
      return; // already normalized
    magnitude = sqrtf(magnitude);
    if (magnitude < Epsilon)
      return; // zero vec
    magnitude = 1.0f / magnitude;
    x *= magnitude;
    y *= magnitude;
  }

  Vector2 Normalized() const {
    float magnitude = MagnitudeSquared();
    if (magnitude - 1.0f <= Epsilon)
      return *this; // already normalized
    magnitude = sqrtf(magnitude);
    if (magnitude < Epsilon)
      return *this; // zero vec
    magnitude = 1.0f / magnitude;
    return{ x * magnitude, y * magnitude };
  }

  bool IsZero() const {
    return MagnitudeSquared() <= Epsilon;
  }

  bool IsNormalized() const {
    // todo: check closeness
    return MagnitudeSquared() - 1.0f <= Epsilon;
  }

  // A note on comparing Vector3's to floats:
  // Here we compare the squared float to this vector3's squared magnitude.
  // Providing a negative float will result in the same comparison as though it were positive.
#if defined(VEC2_COMPARE_OP)
  static_assert(false, "Vector3 found an internal macro where it shouldn't have.");
#endif
#define VEC2_COMPARE_OP(op) \
  bool operator op (const Vector2& v) const { return MagnitudeSquared() op v.MagnitudeSquared(); } \
  bool operator op (float v) const { return MagnitudeSquared() op (v*v); } \
  bool operator op (double v) const { return MagnitudeSquared() op (float)(v*v); } \
  bool operator op (int v) const { return MagnitudeSquared() op (float)(v*v); }
#define VEC2_COMPARE_CLOSE_OP(op) \
  bool operator op (const Vector2& v) const { return (*this - v).MagnitudeSquared() <= Epsilon; } \
  bool operator op (float v) const { return MagnitudeSquared() - (v*v) <= Epsilon; } \
  bool operator op (double v) const { return MagnitudeSquared() - (float)(v*v) <= Epsilon; } \
  bool operator op (int v) const { return MagnitudeSquared() - (float)(v*v) <= Epsilon; }

  VEC2_COMPARE_OP(<)
  VEC2_COMPARE_OP(<= )
  VEC2_COMPARE_OP(>)
  VEC2_COMPARE_OP(>= )
  VEC2_COMPARE_CLOSE_OP(== )
  VEC2_COMPARE_CLOSE_OP(!= )

  #undef VEC2_COMPARE_OP
  #undef VEC2_COMPARE_CLOSE_OP

  static Vector2 Max(const Vector2& a, const Vector2& b) {
    if (a.MagnitudeSquared() >= b.MagnitudeSquared())
      return a;
    return b;
  }

  static Vector2 Min(const Vector2& a, const Vector2& b) {
    if (a.MagnitudeSquared() <= b.MagnitudeSquared())
      return a;
    return b;
  }

  static float AngleRadians(const Vector2& a, const Vector2& b) {
    return 0.0f; // todo
  }

  static float AngleDegrees(const Vector2& a, const Vector2& b) {
    return AngleRadians(a, b) * Rad2Deg;
  }

  static Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
    return a + ((b - a) * t);
  }

  float AngleRadians(const Vector2& v) { return AngleRadians(*this, v); }
  float AngleDegrees(const Vector2& v) { return AngleDegrees(*this, v); }

  static const Vector2
    UnitX, UnitY,
    Up, Down, Left, Right,
    One, Zero;

private:
  float x, y;
};

const Vector2 Vector2::UnitX(1.0f, 0.0f);
const Vector2 Vector2::UnitY(0.0f, 1.0f);

const Vector2 Vector2::Up(0.0f, 1.0f);
const Vector2 Vector2::Down(0.0f, -1.0f);
const Vector2 Vector2::Left(-1.0f, 0.0f);
const Vector2 Vector2::Right(1.0f, 0.0f);

const Vector2 Vector2::One(1.0f, 1.0f);
const Vector2 Vector2::Zero(0.0f, 0.0f);
XOMATH_END_XO_NS