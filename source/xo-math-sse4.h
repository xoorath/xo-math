#pragma once
#include <inttypes.h>
#include "xo-math-macros.h"
// $inline_begin
namespace xo {
struct Vector3 {
    float x, y, z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
    explicit Vector3(float all) : x(all), y(all), z(all) { }
    Vector3(Vector3 const& other) : x(other.x), y(other.y), z(other.z) { }
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
} // ::xo