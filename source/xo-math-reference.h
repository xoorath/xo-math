#pragma once
#include <inttypes.h>
#include "xo-math-macros.h"
#include "xo-math-utilities.h"
// $inline_begin
namespace xo {
struct Vector3 {
	float x, y, z;

    constexpr Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
	constexpr explicit Vector3(float all) : x(all), y(all), z(all) { }

	Vector3()                                  = default;
	~Vector3()                                 = default;
	Vector3(Vector3 const& other)              = default;
	Vector3(Vector3&& ref)                     = default;
	Vector3& operator = (Vector3 const& other) = default;
	Vector3& operator = (Vector3&& ref)        = default;

	Vector3 XO_CC operator + (const Vector3& other) const;
	Vector3 XO_CC operator - (const Vector3& other) const;
	Vector3 XO_CC operator * (const Vector3& other) const;
	Vector3 XO_CC operator / (const Vector3& other) const;
	Vector3& XO_CC operator += (const Vector3& other);
	Vector3& XO_CC operator -= (const Vector3& other);
	Vector3& XO_CC operator *= (const Vector3& other);
	Vector3& XO_CC operator /= (const Vector3& other);

	Vector3 operator -() const;

	float Sum() const;

	float Magnitude() const;
	float MagnitudeSquared() const;

	static bool XO_CC RoughlyEqual(const Vector3& left, const Vector3& right);
	static bool XO_CC ExactlyEqual(const Vector3& left, const Vector3& right);
	static bool XO_CC RoughlyEqual(const Vector3& left, float magnitude);
	static bool XO_CC ExactlyEqual(const Vector3& left, float magnitude);

	static float XO_CC DotProduct(const Vector3& left, const Vector3& right);
	static Vector3 XO_CC CrossProduct(const Vector3& left, const Vector3& right);

	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 Up;
	static const Vector3 Down;
	static const Vector3 Left;
	static const Vector3 Right;
	static const Vector3 Forward;
	static const Vector3 Backward;
};

struct Vector4 {
    float x, y, z, w;
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
    explicit Vector4(float all) : x(all), y(all), z(all), w(all) { }
    Vector4(Vector4 const& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }
};

struct Matrix4x4 {
    Vector4 rows[4];
    Matrix4x4(Vector4 rows[4]) : rows{ rows[0], rows[1], rows[2], rows[3]} { }
	Matrix4x4(Matrix4x4 const& other) : rows{ other.rows[0], other.rows[1], other.rows[2], other.rows[3] } { }
};

struct Quaternion
{
	float i, j, k, r;
	Quaternion(float i, float j, float k, float r) : i(i), j(j), k(k), r(r) { }
	Quaternion(Quaternion const& other) : i(other.i), j(other.j), k(other.k), r(other.r) { }
};

// vector 3, aligned for cpu specific optimizations (where applicable) 
struct AVector3 {
    float x, y, z;
    AVector3(float x, float y, float z) : x(x), y(y), z(z) { }
    explicit AVector3(float all) : x(all), y(all), z(all) { }
    AVector3(AVector3 const& other) : x(other.x), y(other.y), z(other.z) { }
    AVector3(Vector3 const& other) : x(other.x), y(other.y), z(other.z) { }
};

// vector 4, aligned for cpu specific optimizations (where applicable)
struct AVector4 {
    float x, y, z, w;
    AVector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
    explicit AVector4(float all) : x(all), y(all), z(all), w(all) { }
    AVector4(AVector4 const& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }
    AVector4(Vector4 const& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }
};

// matrix4x4, aligned for cpu specific optimizations (where applicable)
struct AMatrix4x4 {
    AVector4 rows[4];
    AMatrix4x4(AVector4 rows[4]) : rows{ rows[0], rows[1], rows[2], rows[3]} { }
    AMatrix4x4(AMatrix4x4 const& other) : rows{ other.rows[0], other.rows[1], other.rows[2], other.rows[3] } { }
    AMatrix4x4(Matrix4x4 const& other) : rows{ AVector4(other.rows[0]), AVector4(other.rows[1]), AVector4(other.rows[2]), AVector4(other.rows[3]) } { }
};

// quaternion, aligned for cpu specific optimizations (where applicable)
struct AQuaternion {
    float i, j, k, r;
    AQuaternion(float i, float j, float k, float r) : i(i), j(j), k(k), r(r) {}
    AQuaternion(AQuaternion const& other) : i(other.i), j(other.j), k(other.k), r(other.r) {}
    AQuaternion(Quaternion const& other) : i(other.i), j(other.j), k(other.k), r(other.r) {}
};

////////////////////////////////////////////////////////////////////////////////////////// Vector 3
#if defined(XO_MATH_IMPL)
/*static*/ const Vector3 Vector3::Zero(0.f);
/*static*/ const Vector3 Vector3::One(1.f);
/*static*/ const Vector3 Vector3::Up(0.f, 1.f, 0.f);
/*static*/ const Vector3 Vector3::Down(0.f, -1.f, 0.f);
/*static*/ const Vector3 Vector3::Left(-1.f, 0.f, 0.f);
/*static*/ const Vector3 Vector3::Right(1.f, 0.f, 0.f);
/*static*/ const Vector3 Vector3::Forward(0.f, 0.f, 1.f);
/*static*/ const Vector3 Vector3::Backward(0.f, 0.f, -1.f);
#endif

XO_INL Vector3 XO_CC Vector3::operator + (const Vector3& other) const {  return Vector3(x + other.x, y + other.y, z + other.z); }
XO_INL Vector3 XO_CC Vector3::operator - (const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
XO_INL Vector3 XO_CC Vector3::operator * (const Vector3& other) const { return Vector3(x * other.x, y * other.y, z * other.z); }
XO_INL Vector3 XO_CC Vector3::operator / (const Vector3& other) const { return Vector3(x / other.x, y / other.y, z / other.z); }
XO_INL Vector3& XO_CC Vector3::operator += (const Vector3& other)     { x += other.x; y += other.y; z *= other.z; return *this; }
XO_INL Vector3& XO_CC Vector3::operator -= (const Vector3& other)     { x -= other.x; y -= other.y; z -= other.z; return *this; }
XO_INL Vector3& XO_CC Vector3::operator *= (const Vector3& other)     { x *= other.x; y *= other.y; z *= other.z; return *this; }
XO_INL Vector3& XO_CC Vector3::operator /= (const Vector3& other)     { x /= other.x; y /= other.y; z /= other.z; return *this; }

XO_INL Vector3 Vector3::operator -() const { return Vector3(-x, -y, -z); };

XO_INL float Vector3::Sum() const { return x + y + z; }

XO_INL float Vector3::MagnitudeSquared() const { return ((*this) * (*this)).Sum(); }
XO_INL float Vector3::Magnitude() const { return Sqrt(MagnitudeSquared()); }

/*static*/ XO_INL 
bool XO_CC Vector3::RoughlyEqual(const Vector3& left, const Vector3& right) { 
    return CloseEnough(left.x, right.x) 
        && CloseEnough(left.y, right.y) 
        && CloseEnough(left.z, right.z);
}

/*static*/ XO_INL
bool XO_CC Vector3::ExactlyEqual(const Vector3& left, const Vector3& right) { 
    return left.x == right.x
        && left.y == right.y
        && left.z == right.z;
}
/*static*/ XO_INL 
bool XO_CC Vector3::RoughlyEqual(const Vector3& left, float magnitude) {
    return CloseEnough(left.MagnitudeSquared(), Pow(magnitude, 2));
}

/*static*/ XO_INL 
bool XO_CC Vector3::ExactlyEqual(const Vector3& left, float magnitude) {
    return left.MagnitudeSquared() == Pow(magnitude, 2);
}

/*static*/ XO_INL
float XO_CC Vector3::DotProduct(const Vector3& left, const Vector3& right) {
	return (left * right).Sum();
}

/*static*/ XO_INL 
Vector3 XO_CC Vector3::CrossProduct(const Vector3& left, const Vector3& right) {
	return Vector3(
		left.y * right.z - left.z * right.y,
		left.z * right.x - left.x * right.z,
		left.x * right.y - left.y * right.x);
}

} // ::xo