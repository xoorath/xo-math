#pragma once

#include "xo-math-defines.h"

XO_NAMESPACE_BEGIN

class XO_ALIGN Vector3 {
public:
  XO_OVERLOAD_NEW_DELETE(); // for alignment

  inline Vector3(float x, float y, float z);
  inline Vector3(const Vector3& other);

  inline void Set(float x, float y, float z);

  inline float& operator [](int i);
  inline const float& operator [](int i) const;

  inline Vector3 operator -() const;
  inline Vector3 operator ~() const;

  inline Vector3& operator += (const Vector3& other);
  inline Vector3 operator + (const Vector3& other) const;
  inline Vector3& operator -= (const Vector3& other);
  inline Vector3 operator - (const Vector3& other) const;
  inline Vector3& operator *= (const Vector3& other);
  inline Vector3 operator * (const Vector3& other) const;
  inline Vector3& operator /= (const Vector3& other);
  inline Vector3 operator / (const Vector3& other) const;

  inline bool operator < (const Vector3& v) const;
  inline bool operator <= (const Vector3& v) const;
  inline bool operator > (const Vector3& v) const;
  inline bool operator >= (const Vector3& v) const;
  inline bool operator == (const Vector3& v) const;
  inline bool operator != (const Vector3& v) const;

  inline float Sum() const;
  inline Vector3 ZYX() const;
  inline float Magnitude() const;
  inline float MagnitudeSquared() const;
  inline void Normalize();
  inline Vector3 Normalized() const;

  inline static Vector3 Cross(const Vector3& a, const Vector3& b);
  inline static float Dot(const Vector3& a, const Vector3& b);
  inline static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);
  inline static float Distance(const Vector3& a, const Vector3& b);

#ifdef XO_SIMD
  inline Vector3(VectorRegister_t reg);
  inline operator VectorRegister_t&();
  inline operator const VectorRegister_t&() const;
#endif

#pragma warning( push )
// C4201: nonstandard extension used: nameless struct/union
#pragma warning( disable : 4201)
  union {
#ifdef XO_SIMD
    struct { 
      float x, y, z; 
    };
    float v[4];
    VectorRegister_t reg;
#else
    struct { 
      float x, y, z; 
    };
    float v[3];
#endif
  };
#pragma warning( pop ) 

  static const Vector3
    UnitX, UnitY, UnitZ,
    Up, Down, Left, Right, Forward, Backward,
    One, Zero;

#if defined(XO_SSE)
  static const VectorRegister_t MASK;
#endif
};

Vector3::Vector3(float x, float y, float z) 
#if defined XO_SSE
  : reg(_mm_set_ps(0.0f, z, y, x))
#else
  : x(x), y(y), z(z)
#endif
{
}

// inline
Vector3::Vector3(const Vector3& other)
#if defined XO_SSE
  : reg(other.reg)
#else
  : x(other.x), y(other.y), z(other.z)
#endif
{
}

// inline
void Vector3::Set(float _x, float _y, float _z) {
#if defined XO_SSE
  reg = _mm_set_ps(0.0f, _z, _y, _x);
#else
  this->x = _x;
  this->y = _y;
  this->z = _z;
#endif
}

// inline
float& Vector3::operator [](int i) {
  return v[i];
}

// inline
const float& Vector3::operator [](int i) const {
  return v[i];
}

// inline
Vector3 Vector3::operator -() const {
#if defined(XO_SSE)
  return Vector3(_mm_mul_ps(reg, simd::NegativeOne));
#elif defined(XO_NEON)
  return Vector3(vnegq_f32(reg));
#else
  return Vector3(-x, -y, -z);
#endif
}

// inline
Vector3 Vector3::operator ~() const {
  return ZYX();
}

// inline
Vector3& Vector3::operator += (const Vector3& other) {
#if defined(XO_SIMD)
  simd::Add(*this, other, *this);
#else
  x += other.x;
  y += other.y;
  z += other.z;
#endif
  return *this;
}

// inline
Vector3 Vector3::operator + (const Vector3& other) const {
  return Vector3(*this) += other;
}

// inline
Vector3& Vector3::operator -= (const Vector3& other) {
#if defined(XO_SIMD)
  simd::Subtract(*this, other, *this);
#else
  x -= other.x;
  y -= other.y;
  z -= other.z;
#endif
  return *this;
}

// inline
Vector3 Vector3::operator - (const Vector3& other) const {
  return Vector3(*this) -= other;
}

// inline
Vector3& Vector3::operator *= (const Vector3& other) {
#if defined(XO_SIMD)
  simd::Multiply(*this, other, *this);
#else
  x *= other.x;
  y *= other.y;
  z *= other.z;
#endif
  return *this;
}

// inline
Vector3 Vector3::operator * (const Vector3& other) const {
  return Vector3(*this) *= other;
}

// inline
Vector3& Vector3::operator /= (const Vector3& other) {
#if defined(XO_SIMD)
  simd::Divide(*this, other, *this);
#else
  x /= other.x;
  y /= other.y;
  z /= other.z;
#endif
  return *this;
}

// inline
Vector3 Vector3::operator / (const Vector3& other) const {
  return Vector3(*this) /= other;
}

// inline
bool Vector3::operator < (const Vector3& other) const {
  return MagnitudeSquared() < other.MagnitudeSquared();
}

// inline
bool Vector3::operator <= (const Vector3& other) const {
  return MagnitudeSquared() <= other.MagnitudeSquared();
}

// inline
bool Vector3::operator > (const Vector3& other) const {
  return MagnitudeSquared() > other.MagnitudeSquared();
}

// inline
bool Vector3::operator >= (const Vector3& other) const {
  return MagnitudeSquared() >= other.MagnitudeSquared();
}

// inline
bool Vector3::operator == (const Vector3&) const {
  return false; // todo
}

// inline
bool Vector3::operator != (const Vector3&) const {
  return false; // todo
}

// inline
float Vector3::Sum() const {
#if defined(XO_SSE3)
    VectorRegister_t x = _mm_and_ps(reg, MASK);
    x = _mm_hadd_ps(x, x);
    x = _mm_hadd_ps(x, x);
    return _mm_cvtss_f32(x);
#else
    return x + y + z;
#endif
}

// inline
Vector3 Vector3::ZYX() const {
#if defined(XO_SSE)
    // todo: constexpr shuffle int
    return Vector3(_mm_shuffle_ps(reg, reg, _MM_SHUFFLE(3, 0, 1, 2)));
#else
    return Vector3(z, y, x);
#endif
}

// inline
float Vector3::Magnitude() const {
  return Sqrt(MagnitudeSquared());
}

// inline
float Vector3::MagnitudeSquared() const {
  return ((*this) * (*this)).Sum();
}

// inline
void Vector3::Normalize() {

}

// inline
Vector3 Vector3::Normalized() const {

}

// inline static 
Vector3 Vector3::Cross(const Vector3&, const Vector3&) {

}

// inline static 
float Vector3::Dot(const Vector3&, const Vector3&) {

}

// inline static 
Vector3 Vector3::Lerp(const Vector3&, const Vector3&, float) {

}

// inline static 
float Vector3::Distance(const Vector3&, const Vector3&) {

}

#ifdef XO_SIMD
// inline
Vector3::Vector3(VectorRegister_t _reg) 
: reg(_reg) 
{
}

// inline
Vector3::operator VectorRegister_t&() {
  return reg;
}

// inline
Vector3::operator const VectorRegister_t&() const {
  return reg;
}
#endif

XO_NAMESPACE_END