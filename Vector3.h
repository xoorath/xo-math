#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector3.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS
class Vector3 {
public:
  constexpr static const float Epsilon = FloatEpsilon * 3.0f;

  Vector3() : x(0.0f), y(0.0f), z(0.0f) {
  }
  Vector3(float v) : x(v), y(v), z(v) {
  }
  Vector3(float x, float y, float z) : x(x), y(y), z(z) {
  }
  Vector3(const Vector3& v) : x(v[0]), y(v[1]), z(v[2]) {
  }
  Vector3(const class Vector2& v);

  float& operator [](int i) { switch (i) { default: case 0: return x; case 1: return y; case 2: return z; } }
  const float& operator [](int i) const { switch (i) { default: case 0: return x; case 1: return y; case 2: return z; } }

  void Set(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  void Set(float v) {
    this->x = v;
    this->y = v;
    this->z = v;
  }

  void Set(const Vector3& v) {
    this->x = v[0];
    this->y = v[1];
    this->z = v[2];
  }

  VEC3D_SIMPLE_OP(+)
  VEC3D_SIMPLE_OP(-)
  VEC3D_SIMPLE_OP(*)
  VEC3D_SIMPLE_OP(/ )
  VEC3D_SIMPLE_OP_ADD(+= )
  VEC3D_SIMPLE_OP_ADD(-= )
  VEC3D_SIMPLE_OP_ADD(*= )
  VEC3D_SIMPLE_OP_ADD(/= )

  Vector3 operator - () const { return Vector3(-x, -y, -z); }
  Vector3 operator ~ () const { return Vector3(z, y, x); }

  Vector3 operator + (const class Vector2& v) const;

  float Magnitude() const {
    return Sqrt(x*x + y*y + z*z);
  }

  float MagnitudeSquared() const {
    return x*x + y*y + z*z;
  }

  void Normalize() {
    float magnitude = MagnitudeSquared();
    if (magnitude - 1.0f <= Epsilon)
      return; // already normalized
    magnitude = Sqrt(magnitude);
    if (magnitude < Epsilon)
      return; // zero vec
    magnitude = 1.0f / magnitude;
    x *= magnitude;
    y *= magnitude;
    z *= magnitude;
  }

  Vector3 Normalized() const {
    float magnitude = MagnitudeSquared();
    if (magnitude - 1.0f <= Epsilon)
      return *this; // already normalized
    magnitude = Sqrt(magnitude);
    if (magnitude < Epsilon)
      return *this; // zero vec
    magnitude = 1.0f / magnitude;
    return{ x * magnitude, y * magnitude, z * magnitude };
  }

  bool IsZero() const {
    return MagnitudeSquared() <= Epsilon;
  }

  bool IsNormalized() const {
    // todo: check closeness
    return MagnitudeSquared() - 1.0f <= Epsilon;
  }

  VEC3_COMPARE_OP(<)
  VEC3_COMPARE_OP(<= )
  VEC3_COMPARE_OP(>)
  VEC3_COMPARE_OP(>= )
  VEC3_COMPARE_CLOSE_OP(==)
  VEC3_COMPARE_CLOSE_OP(!=)

  static Vector3 Max(const Vector3& a, const Vector3& b) {
    if (a.MagnitudeSquared() >= b.MagnitudeSquared())
      return a;
    return b;
  }

  static Vector3 Min(const Vector3& a, const Vector3& b) {
    if (a.MagnitudeSquared() <= b.MagnitudeSquared())
      return a;
    return b;
  }

  static float Dot(const Vector3& a, const Vector3& b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
  }

  static Vector3 Cross(const Vector3& a, const Vector3& b) {
    float x = (a[1] * b[2]) - (a[2] * b[1]);
    float y = (a[2] * b[0]) - (a[0] * b[2]);
    float z = (a[0] * b[1]) - (a[1] * b[0]);

    return{ x, y, z };
  }

  static float AngleRadians(const Vector3& a, const Vector3& b) {
    float dx = a[1] * b[2] - a[2] * b[1];
    float dy = a[2] * b[0] - a[0] * b[2];
    float dz = a[0] * b[1] - a[1] * b[0];
    return Atan2(Sqrt(dx * dx + dy * dy + dz * dz) + Epsilon, Dot(a, b));
  }

  static float AngleDegrees(const Vector3& a, const Vector3& b) {
    return AngleRadians(a, b) * Rad2Deg;
  }

  static float DistanceSquared(const Vector3& a, const Vector3& b) {
    return (a - b).MagnitudeSquared();
  }

  static float Distance(const Vector3&a, const Vector3&b) {
    return (a - b).Magnitude();
  }

  static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
    return a + ((b - a) * t);
  }

  float Dot(const Vector3& v) { return Dot(*this, v); }
  Vector3 Cross(const Vector3& v) { return Cross(*this, v); }
  float AngleRadians(const Vector3& v) { return AngleRadians(*this, v); }
  float AngleDegrees(const Vector3& v) { return AngleDegrees(*this, v); }
  float DistanceSquared(const Vector3& v) { return DistanceSquared(*this, v); }
  float Distance(const Vector3& v) { return Distance(*this, v); }

  friend std::ostream& operator <<(std::ostream& os, const Vector3& v) {
      os << '(' << v[0] << ',' << v[1] << ',' << v[2] << ')';
      return os;
  }

  static const Vector3
    UnitX, UnitY, UnitZ,
    Up, Down, Left, Right, Forward, Backward,
    One, Zero;

private:
  float x, y, z;
};

const Vector3 Vector3::UnitX(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UnitY(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UnitZ(0.0f, 0.0f, 1.0f);

const Vector3 Vector3::Up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Down(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::Left(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Forward(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Backward(0.0f, 0.0f, -1.0f);

const Vector3 Vector3::One(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);

XOMATH_END_XO_NS

#endif // XOMATH_INTERNAL