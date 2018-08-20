// warning C4530: C++ exception handler used, but unwind semantics are not enabled.
// warning C4577: 'noexcept' used with no exception handling mode specified; termination on exception is not guaranteed.
#pragma warning(disable : 4530 4577)
/*
    features.cpp

    The purpose of this file is to touch all features of xo-math, to ensure it compiles 
    under the current configuration. It's not intended to be a unit test, or produce any
    meaningful code. The purpose is simply for compilation testing.
*/
#include "xo-math.h"

// anonymous namespace forces these functions to be unique between features.cpp and 
// features2.cpp
namespace {

void AllHelperFeatures() {
    {float a = xo::Abs(1.f); XO_UNUSED(a); }
    {float a = xo::Max(1.f, 2.f); XO_UNUSED(a); }
    {float a = xo::Max(1.f, 2.f, 3.f); XO_UNUSED(a); }
    {float a = xo::Min(1.f, 2.f); XO_UNUSED(a); }
    {float a = xo::Min(1.f, 2.f, 3.f); XO_UNUSED(a); }
    {float a = xo::Clamp(1.f, 2.f, 3.f); XO_UNUSED(a); }
    {float a = xo::Lerp(1.f, 2.f, 0.5f); XO_UNUSED(a); }
    {float a = xo::RelativeEpsilon(10.f); XO_UNUSED(a); }
    {float a = xo::RelativeEpsilon(10.f, 20.f); XO_UNUSED(a); }
    {float a = xo::CloseEnough(1.f, 1.000005f); XO_UNUSED(a); }
    {float a = xo::Sqrt(1.f); XO_UNUSED(a); }
    {float a = xo::Pow<1>(1.f); XO_UNUSED(a); }
    {float a = xo::Pow<2>(1.f); XO_UNUSED(a); }
    {float a = xo::Pow<3>(1.f); XO_UNUSED(a); }
    {float a = xo::Pow<4>(1.f); XO_UNUSED(a); }
    {float a = xo::Sin(1.f); XO_UNUSED(a); }
    {float a = xo::Cos(1.f); XO_UNUSED(a); }
    {float a = xo::ASin(1.f); XO_UNUSED(a); }
    {float a = xo::ACos(1.f); XO_UNUSED(a); }
    {float a, b; xo::SinCos(1.f, a, b); XO_UNUSED(a); XO_UNUSED(b); }
    {float a, b; xo::ASinACos(1.f, a, b); XO_UNUSED(a); XO_UNUSED(b); }
}

void AllVector3Features() {
    xo::Vector3 const _a(xo::Vector3::Up);
    xo::Vector3 const _b(xo::Vector3::Down);

    { xo::Vector3 a(1.f, 2.f, 3.f); XO_UNUSED(a); }
    { xo::Vector3 a(1.f); XO_UNUSED(a); }
    { xo::Vector3 a = xo::Vector3(); XO_UNUSED(a); }
    { xo::Vector3 a(xo::Vector3::Up); XO_UNUSED(a); }
    { xo::Vector3 a; XO_UNUSED(a); }
    { xo::Vector3 a = xo::Vector3::Up; XO_UNUSED(a); }
    { xo::Vector3 a = xo::Vector3(); XO_UNUSED(a); }
    { xo::Vector3 a = _a + _b; XO_UNUSED(a); }
    { xo::Vector3 a = _a - _b; XO_UNUSED(a); }
    { xo::Vector3 a = _a * _b; XO_UNUSED(a); }
    { xo::Vector3 a = _a / _b; XO_UNUSED(a); }
    { xo::Vector3 a = _a; a += _b; XO_UNUSED(a); }
    { xo::Vector3 a = _a; a -= _b; XO_UNUSED(a); }
    { xo::Vector3 a = _a; a *= _b; XO_UNUSED(a); }
    { xo::Vector3 a = _a; a /= _b; XO_UNUSED(a); }
    { xo::Vector3 a = -_a; XO_UNUSED(a); }
    { float a = _a.Sum(); XO_UNUSED(a); }
    { float a = _a.Magnitude(); XO_UNUSED(a); }
    { float a = _a.MagnitudeSquared(); XO_UNUSED(a); }
    { xo::Vector3 a = _a; a.Normalize(); XO_UNUSED(a); }
    { xo::Vector3 a = _a.Normalized(); XO_UNUSED(a); }
    { bool a = xo::Vector3::RoughlyEqual(_a, _b); XO_UNUSED(a); }
    { bool a = xo::Vector3::ExactlyEqual(_a, _b); XO_UNUSED(a); }
    { bool a = xo::Vector3::RoughlyEqual(_a, 1.f); XO_UNUSED(a); }
    { bool a = xo::Vector3::ExactlyEqual(_a, 1.f); XO_UNUSED(a); }
    { float a = xo::Vector3::DotProduct(_a, _b); XO_UNUSED(a); }
    { xo::Vector3 a = xo::Vector3::CrossProduct(_a, _b); XO_UNUSED(a); }
    { xo::Vector3 a = xo::Vector3::Lerp(_a, _b, 0.5f); XO_UNUSED(a); }
    { float a = xo::Vector3::DistanceSquared(_a, _b); XO_UNUSED(a); }
    { float a = xo::Vector3::Distance(_a, _b); XO_UNUSED(a); }
    { xo::Vector3 const& a = xo::Vector3::Zero; XO_UNUSED(a); }
    { xo::Vector3 const& a = xo::Vector3::One; XO_UNUSED(a); }
    { xo::Vector3 const& a = xo::Vector3::Up; XO_UNUSED(a); }
    { xo::Vector3 const& a = xo::Vector3::Down; XO_UNUSED(a); }
    { xo::Vector3 const& a = xo::Vector3::Left; XO_UNUSED(a); }
    { xo::Vector3 const& a = xo::Vector3::Right; XO_UNUSED(a); }
    { xo::Vector3 const& a = xo::Vector3::Forward; XO_UNUSED(a); }
    { xo::Vector3 const& a = xo::Vector3::Backward; XO_UNUSED(a); }
}

void AllVector4Features() {
    xo::Vector4 const _a(xo::Vector4::One);
    xo::Vector4 const _b(-xo::Vector4::One);
    xo::Vector3 const v3(1.f, 1.f, 1.f);

    { xo::Vector4 a(1.f, 2.f, 3.f, 4.f); XO_UNUSED(a); }
    { xo::Vector4 a(1.f); XO_UNUSED(a); }
    { xo::Vector4 a; XO_UNUSED(a); }
    { xo::Vector4 a(_a); XO_UNUSED(a); }
    { xo::Vector4 a(xo::Vector4(1.f)); XO_UNUSED(a); }
    { xo::Vector4 a(v3); XO_UNUSED(a); }
    { xo::Vector4 a(v3, 1.f); XO_UNUSED(a); }
    { xo::Vector4 a = _a; XO_UNUSED(a); }
    { xo::Vector4 a = xo::Vector4(_a); XO_UNUSED(a); }
    { xo::Vector4 a = _a + _b; XO_UNUSED(a); }
    { xo::Vector4 a = _a - _b; XO_UNUSED(a); }
    { xo::Vector4 a = _a * _b; XO_UNUSED(a); }
    { xo::Vector4 a = _a / _b; XO_UNUSED(a); }
    { xo::Vector4 a = _a; a += _b; XO_UNUSED(a); }
    { xo::Vector4 a = _a; a -= _b; XO_UNUSED(a); }
    { xo::Vector4 a = _a; a *= _b; XO_UNUSED(a); }
    { xo::Vector4 a = _a; a /= _b; XO_UNUSED(a); }
    { xo::Vector4 a = -_a; XO_UNUSED(a); }
    { float a = _a[0]; XO_UNUSED(a); }
    { xo::Vector4 a = _a; a[0] = 1.f; XO_UNUSED(a); }
    { float a = _a.Sum(); XO_UNUSED(a); }
    { float a = _a.Magnitude(); XO_UNUSED(a); }
    { float a = _a.MagnitudeSquared(); XO_UNUSED(a); }
    { xo::Vector4 a = _a.Normalized(); XO_UNUSED(a); }
    { xo::Vector4 a = _a; a.Normalize(); XO_UNUSED(a); }
    { bool a = xo::Vector4::RoughlyEqual(_a, _b); XO_UNUSED(a); }
    { bool a = xo::Vector4::ExactlyEqual(_a, _b); XO_UNUSED(a); }
    { bool a = xo::Vector4::RoughlyEqual(_a, 1.f); XO_UNUSED(a); }
    { bool a = xo::Vector4::ExactlyEqual(_a, 1.f); XO_UNUSED(a); }
    { float a = xo::Vector4::DotProduct(_a, _b); XO_UNUSED(a); }
    { xo::Vector4 a = xo::Vector4::Lerp(_a, _b, 0.5f); XO_UNUSED(a); }
}

void AllMatrix4x4Features() {
    xo::Matrix4x4 const _a(xo::Matrix4x4::Identity);
    xo::Matrix4x4 const _b(xo::Matrix4x4::Identity);
    xo::Vector4 const _v[4] = {
        xo::Vector4(1.f, 0.f, 0.f, 0.f),
        xo::Vector4(0.f, 1.f, 0.f, 0.f),
        xo::Vector4(0.f, 0.f, 1.f, 0.f),
        xo::Vector4(0.f, 0.f, 0.f, 1.f)
    };

    { xo::Matrix4x4 a(_v); XO_UNUSED(a); }
    { xo::Matrix4x4 a(_v[0], _v[1], _v[2], _v[3]); XO_UNUSED(a); }
    { xo::Matrix4x4 a(0.f); XO_UNUSED(a); }
    { xo::Matrix4x4 a; XO_UNUSED(a); }
    { xo::Matrix4x4 a(_a); XO_UNUSED(a); }
    { xo::Matrix4x4 a(xo::Matrix4x4(0.f)); XO_UNUSED(a); }
    { xo::Matrix4x4 a = _a; XO_UNUSED(a); }
    { xo::Matrix4x4 a = xo::Matrix4x4(0.f); XO_UNUSED(a); }
    { xo::Matrix4x4 a = _a * _b; XO_UNUSED(a); }
    { xo::Matrix4x4 a = _a; a *= _b; XO_UNUSED(a); }
    { xo::Vector4 a = _a[0]; XO_UNUSED(a); }
    { xo::Matrix4x4 a = _a; a[0] = xo::Vector4::One; XO_UNUSED(a); }
    { xo::Vector3 a = _a.Up(); XO_UNUSED(a); }
    { xo::Vector3 a = _a.Down(); XO_UNUSED(a); }
    { xo::Vector3 a = _a.Left(); XO_UNUSED(a); }
    { xo::Vector3 a = _a.Right(); XO_UNUSED(a); }
    { xo::Vector3 a = _a.Forward(); XO_UNUSED(a); }
    { xo::Vector3 a = _a.Backward(); XO_UNUSED(a); }
    { xo::Matrix4x4 a = xo::Matrix4x4::Transpose(_a); XO_UNUSED(a); }
    { xo::Matrix4x4 a = xo::Matrix4x4::Invert(_a); XO_UNUSED(a); }
    { xo::Matrix4x4 a;  bool b = xo::Matrix4x4::InvertSafe(_a, a); XO_UNUSED(a); XO_UNUSED(b); }
    { xo::Matrix4x4 a = xo::Matrix4x4::Translation(xo::Vector3::Up); XO_UNUSED(a); }
    { xo::Matrix4x4 a = xo::Matrix4x4::Scale(xo::Vector3::Up); XO_UNUSED(a); }
    { xo::Matrix4x4 a = xo::Matrix4x4::RotationYaw(0.f); XO_UNUSED(a); }
    { xo::Matrix4x4 a = xo::Matrix4x4::RotationPitch(0.f); XO_UNUSED(a); }
    { xo::Matrix4x4 a = xo::Matrix4x4::RotationRoll(0.f); XO_UNUSED(a); }
    { xo::Matrix4x4 a = xo::Matrix4x4::RotationYawPitchRoll(0.f, 0.f, 0.f); XO_UNUSED(a); }
    { xo::Matrix4x4 a = xo::Matrix4x4::RotationAxisAngle(xo::Vector3::Up, 0.f); XO_UNUSED(a); }
    { xo::Matrix4x4 a = xo::Matrix4x4::PerspectiveFOV(0.f, 0.f); XO_UNUSED(a); }
    { xo::Matrix4x4 a = xo::Matrix4x4::PerspectiveFOV(0.f, 0.f, 0.f); XO_UNUSED(a); }
    { xo::Matrix4x4 a = xo::Matrix4x4::PerspectiveFOV(0.f, 0.f, 0.f, 0.f); XO_UNUSED(a); }
    { xo::Matrix4x4 a = xo::Matrix4x4::Perspective(0.f, 0.f, 0.f); XO_UNUSED(a); }
    { xo::Matrix4x4 a = xo::Matrix4x4::Perspective(0.f, 0.f, 0.f, 0.f); XO_UNUSED(a); }
    { xo::Matrix4x4 a = xo::Matrix4x4::Perspective(0.f, 0.f, 0.f, 0.f, 0.f); XO_UNUSED(a); }
    { bool a = xo::Matrix4x4::RoughlyEqual(_a, _b); XO_UNUSED(a); }
    { bool a = xo::Matrix4x4::ExactlyEqual(_a, _b); XO_UNUSED(a); }
    { xo::Matrix4x4 const& a = xo::Matrix4x4::One; XO_UNUSED(a); }
    { xo::Matrix4x4 const& a = xo::Matrix4x4::Zero; XO_UNUSED(a); }
    { xo::Matrix4x4 const& a = xo::Matrix4x4::Identity; XO_UNUSED(a); }
}

void AllQuaternionFeatures() {
    xo::Quaternion const _a(xo::Quaternion::Identity);
    xo::Quaternion const _b(xo::Quaternion::Identity);

    { xo::Quaternion a(0.f, 0.f, 0.f, 1.f); XO_UNUSED(a); }
    { xo::Quaternion a(0.f); XO_UNUSED(a); }
    { xo::Quaternion a(xo::Vector4::One); XO_UNUSED(a); }
    { xo::Quaternion a; XO_UNUSED(a); }
    { xo::Quaternion a(xo::Quaternion::Identity); XO_UNUSED(a); }
    { xo::Quaternion a(xo::Quaternion(1.f)); XO_UNUSED(a); }
    { xo::Quaternion a = xo::Quaternion::Identity; XO_UNUSED(a); }
    { xo::Quaternion a = xo::Quaternion(0.f); XO_UNUSED(a); }
    { xo::Quaternion a = _a + _b; XO_UNUSED(a); }
    { xo::Quaternion a = _a * 2.f; XO_UNUSED(a); }
    { xo::Quaternion a = -_a; XO_UNUSED(a); }
    { float a = _a.Magnitude(); XO_UNUSED(a); }
    { float a = _a.MagnitudeSquared(); XO_UNUSED(a); }
    { xo::Quaternion a = _a.Normalized(); XO_UNUSED(a); }
    { xo::Quaternion a = _a; a.Normalize(); XO_UNUSED(a); }
    { xo::Matrix4x4 a = _a.ToMatrix(); XO_UNUSED(a); }
    { xo::Quaternion a = xo::Quaternion::Invert(_a); XO_UNUSED(a); }
    { xo::Quaternion a = xo::Quaternion::RotationAxisAngle(xo::Vector3::Up, 0.f); XO_UNUSED(a); }
    { xo::Quaternion a = xo::Quaternion::RotationEuler({ 0.f, 0.f, 0.f }); XO_UNUSED(a); }
    { float a = xo::Quaternion::DotProduct(_a, _b); XO_UNUSED(a); }
    { xo::Quaternion a = xo::Quaternion::Lerp(_a, _b, 0.f); XO_UNUSED(a); }
    { xo::Quaternion a = xo::Quaternion::Slerp(_a, _b, 0.f); XO_UNUSED(a); }
    { bool a = xo::Quaternion::RoughlyEqual(_a, _b); XO_UNUSED(a); }
    { bool a = xo::Quaternion::ExactlyEqual(_a, _b); XO_UNUSED(a); }
    { xo::Quaternion a = xo::Quaternion::Zero; XO_UNUSED(a); }
    { xo::Quaternion a = xo::Quaternion::Identity; XO_UNUSED(a); }
}

void AllAVector3Features() {
    xo::AVector3 const _a(xo::AVector3::Up);
    xo::AVector3 const _b(xo::AVector3::Down);

    { xo::AVector3 a(1.f, 2.f, 3.f); XO_UNUSED(a); }
    { xo::AVector3 a(1.f); XO_UNUSED(a); }
    { xo::AVector3 a = xo::AVector3(); XO_UNUSED(a); }
    { xo::AVector3 a(xo::AVector3::Up); XO_UNUSED(a); }
    { xo::AVector3 a; XO_UNUSED(a); }
    { xo::AVector3 a = xo::AVector3::Up; XO_UNUSED(a); }
    { xo::AVector3 a = xo::AVector3(); XO_UNUSED(a); }
    { xo::AVector3 a = _a + _b; XO_UNUSED(a); }
    { xo::AVector3 a = _a - _b; XO_UNUSED(a); }
    { xo::AVector3 a = _a * _b; XO_UNUSED(a); }
    { xo::AVector3 a = _a / _b; XO_UNUSED(a); }
    { xo::AVector3 a = _a; a += _b; XO_UNUSED(a); }
    { xo::AVector3 a = _a; a -= _b; XO_UNUSED(a); }
    { xo::AVector3 a = _a; a *= _b; XO_UNUSED(a); }
    { xo::AVector3 a = _a; a /= _b; XO_UNUSED(a); }
    { xo::AVector3 a = -_a; XO_UNUSED(a); }
    { float a = _a.Sum(); XO_UNUSED(a); }
    { float a = _a.Magnitude(); XO_UNUSED(a); }
    { float a = _a.MagnitudeSquared(); XO_UNUSED(a); }
    { xo::AVector3 a = _a; a.Normalize(); XO_UNUSED(a); }
    { xo::AVector3 a = _a.Normalized(); XO_UNUSED(a); }
    { bool a = xo::AVector3::RoughlyEqual(_a, _b); XO_UNUSED(a); }
    { bool a = xo::AVector3::ExactlyEqual(_a, _b); XO_UNUSED(a); }
    { bool a = xo::AVector3::RoughlyEqual(_a, 1.f); XO_UNUSED(a); }
    { bool a = xo::AVector3::ExactlyEqual(_a, 1.f); XO_UNUSED(a); }
    { float a = xo::AVector3::DotProduct(_a, _b); XO_UNUSED(a); }
    { xo::AVector3 a = xo::AVector3::CrossProduct(_a, _b); XO_UNUSED(a); }
    { xo::AVector3 a = xo::AVector3::Lerp(_a, _b, 0.5f); XO_UNUSED(a); }
    { float a = xo::AVector3::DistanceSquared(_a, _b); XO_UNUSED(a); }
    { float a = xo::AVector3::Distance(_a, _b); XO_UNUSED(a); }
    { xo::AVector3 const& a = xo::AVector3::Zero; XO_UNUSED(a); }
    { xo::AVector3 const& a = xo::AVector3::One; XO_UNUSED(a); }
    { xo::AVector3 const& a = xo::AVector3::Up; XO_UNUSED(a); }
    { xo::AVector3 const& a = xo::AVector3::Down; XO_UNUSED(a); }
    { xo::AVector3 const& a = xo::AVector3::Left; XO_UNUSED(a); }
    { xo::AVector3 const& a = xo::AVector3::Right; XO_UNUSED(a); }
    { xo::AVector3 const& a = xo::AVector3::Forward; XO_UNUSED(a); }
    { xo::AVector3 const& a = xo::AVector3::Backward; XO_UNUSED(a); }
}

void AllAVector4Features() {
    xo::AVector4 const _a(xo::AVector4::One);
    xo::AVector4 const _b(-xo::AVector4::One);
    xo::AVector3 const v3(1.f, 1.f, 1.f);

    { xo::AVector4 a(1.f, 2.f, 3.f, 4.f); XO_UNUSED(a); }
    { xo::AVector4 a(1.f); XO_UNUSED(a); }
    { xo::AVector4 a; XO_UNUSED(a); }
    { xo::AVector4 a(_a); XO_UNUSED(a); }
    { xo::AVector4 a(xo::AVector4(1.f)); XO_UNUSED(a); }
    { xo::AVector4 a = _a; XO_UNUSED(a); }
    { xo::AVector4 a = xo::AVector4(_a); XO_UNUSED(a); }
    { xo::AVector4 a(v3); XO_UNUSED(a); }
    { xo::AVector4 a(v3, 1.f); XO_UNUSED(a); }
    { xo::AVector4 a = _a + _b; XO_UNUSED(a); }
    { xo::AVector4 a = _a - _b; XO_UNUSED(a); }
    { xo::AVector4 a = _a * _b; XO_UNUSED(a); }
    { xo::AVector4 a = _a / _b; XO_UNUSED(a); }
    { xo::AVector4 a = _a; a += _b; XO_UNUSED(a); }
    { xo::AVector4 a = _a; a -= _b; XO_UNUSED(a); }
    { xo::AVector4 a = _a; a *= _b; XO_UNUSED(a); }
    { xo::AVector4 a = _a; a /= _b; XO_UNUSED(a); }
    { xo::AVector4 a = -_a; XO_UNUSED(a); }
    { float a = _a[0]; XO_UNUSED(a); }
    { xo::AVector4 a = _a; a[0] = 1.f; XO_UNUSED(a); }
    { float a = _a.Sum(); XO_UNUSED(a); }
    { float a = _a.Magnitude(); XO_UNUSED(a); }
    { float a = _a.MagnitudeSquared(); XO_UNUSED(a); }
    { xo::AVector4 a = _a.Normalized(); XO_UNUSED(a); }
    { xo::AVector4 a = _a; a.Normalize(); XO_UNUSED(a); }
    { bool a = xo::AVector4::RoughlyEqual(_a, _b); XO_UNUSED(a); }
    { bool a = xo::AVector4::ExactlyEqual(_a, _b); XO_UNUSED(a); }
    { bool a = xo::AVector4::RoughlyEqual(_a, 1.f); XO_UNUSED(a); }
    { bool a = xo::AVector4::ExactlyEqual(_a, 1.f); XO_UNUSED(a); }
    { float a = xo::AVector4::DotProduct(_a, _b); XO_UNUSED(a); }
    { xo::AVector4 a = xo::AVector4::Lerp(_a, _b, 0.5f); XO_UNUSED(a); }
}

void AllAMatrix4x4Features() {
    xo::AMatrix4x4 const _a(xo::AMatrix4x4::Identity);
    xo::AMatrix4x4 const _b(xo::AMatrix4x4::Identity);
    xo::AVector4 const _v[4] = {
        xo::AVector4(1.f, 0.f, 0.f, 0.f),
        xo::AVector4(0.f, 1.f, 0.f, 0.f),
        xo::AVector4(0.f, 0.f, 1.f, 0.f),
        xo::AVector4(0.f, 0.f, 0.f, 1.f)
    };

    { xo::AMatrix4x4 a(_v); XO_UNUSED(a); }
    { xo::AMatrix4x4 a(_v[0], _v[1], _v[2], _v[3]); XO_UNUSED(a); }
    { xo::AMatrix4x4 a(0.f); XO_UNUSED(a); }
    { xo::AMatrix4x4 a; XO_UNUSED(a); }
    { xo::AMatrix4x4 a(_a); XO_UNUSED(a); }
    { xo::AMatrix4x4 a(xo::AMatrix4x4(0.f)); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = _a; XO_UNUSED(a); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4(0.f); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = _a * _b; XO_UNUSED(a); }
    { xo::AMatrix4x4 a = _a; a *= _b; XO_UNUSED(a); }
    { xo::AVector4 a = _a[0]; XO_UNUSED(a); }
    { xo::AMatrix4x4 a = _a; a[0] = xo::AVector4::One; XO_UNUSED(a); }
    { xo::AVector3 a = _a.Up(); XO_UNUSED(a); }
    { xo::AVector3 a = _a.Down(); XO_UNUSED(a); }
    { xo::AVector3 a = _a.Left(); XO_UNUSED(a); }
    { xo::AVector3 a = _a.Right(); XO_UNUSED(a); }
    { xo::AVector3 a = _a.Forward(); XO_UNUSED(a); }
    { xo::AVector3 a = _a.Backward(); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4::Transpose(_a); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4::Invert(_a); XO_UNUSED(a); }
    { xo::AMatrix4x4 a;  bool b = xo::AMatrix4x4::InvertSafe(_a, a); XO_UNUSED(a); XO_UNUSED(b); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4::Translation(xo::AVector3::Up); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4::Scale(xo::AVector3::Up); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4::RotationYaw(0.f); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4::RotationPitch(0.f); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4::RotationRoll(0.f); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4::RotationYawPitchRoll(0.f, 0.f, 0.f); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4::RotationAxisAngle(xo::AVector3::Up, 0.f); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4::PerspectiveFOV(0.f, 0.f); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4::PerspectiveFOV(0.f, 0.f, 0.f); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4::PerspectiveFOV(0.f, 0.f, 0.f, 0.f); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4::Perspective(0.f, 0.f, 0.f); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4::Perspective(0.f, 0.f, 0.f, 0.f); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = xo::AMatrix4x4::Perspective(0.f, 0.f, 0.f, 0.f, 0.f); XO_UNUSED(a); }
    { bool a = xo::AMatrix4x4::RoughlyEqual(_a, _b); XO_UNUSED(a); }
    { bool a = xo::AMatrix4x4::ExactlyEqual(_a, _b); XO_UNUSED(a); }
    { xo::AMatrix4x4 const& a = xo::AMatrix4x4::One; XO_UNUSED(a); }
    { xo::AMatrix4x4 const& a = xo::AMatrix4x4::Zero; XO_UNUSED(a); }
    { xo::AMatrix4x4 const& a = xo::AMatrix4x4::Identity; XO_UNUSED(a); }
}

void AllAQuaternionFeatures() {
    xo::AQuaternion const _a(xo::AQuaternion::Identity);
    xo::AQuaternion const _b(xo::AQuaternion::Identity);

    { xo::AQuaternion a(0.f, 0.f, 0.f, 1.f); XO_UNUSED(a); }
    { xo::AQuaternion a(0.f); XO_UNUSED(a); }
    { xo::AQuaternion a(xo::AVector4::One); XO_UNUSED(a); }
    { xo::AQuaternion a; XO_UNUSED(a); }
    { xo::AQuaternion a(xo::AQuaternion::Identity); XO_UNUSED(a); }
    { xo::AQuaternion a(xo::AQuaternion(1.f)); XO_UNUSED(a); }
    { xo::AQuaternion a = xo::AQuaternion::Identity; XO_UNUSED(a); }
    { xo::AQuaternion a = xo::AQuaternion(0.f); XO_UNUSED(a); }
    { xo::AQuaternion a = _a + _b; XO_UNUSED(a); }
    { xo::AQuaternion a = _a * 2.f; XO_UNUSED(a); }
    { xo::AQuaternion a = -_a; XO_UNUSED(a); }
    { float a = _a.Magnitude(); XO_UNUSED(a); }
    { float a = _a.MagnitudeSquared(); XO_UNUSED(a); }
    { xo::AQuaternion a = _a.Normalized(); XO_UNUSED(a); }
    { xo::AQuaternion a = _a; a.Normalize(); XO_UNUSED(a); }
    { xo::AMatrix4x4 a = _a.ToMatrix(); XO_UNUSED(a); }
    { xo::AQuaternion a = xo::AQuaternion::Invert(_a); XO_UNUSED(a); }
    { xo::AQuaternion a = xo::AQuaternion::RotationAxisAngle(xo::AVector3::Up, 0.f); XO_UNUSED(a); }
    { xo::AQuaternion a = xo::AQuaternion::RotationEuler({ 0.f, 0.f, 0.f }); XO_UNUSED(a); }
    { float a = xo::AQuaternion::DotProduct(_a, _b); XO_UNUSED(a); }
    { xo::AQuaternion a = xo::AQuaternion::Lerp(_a, _b, 0.f); XO_UNUSED(a); }
    { xo::AQuaternion a = xo::AQuaternion::Slerp(_a, _b, 0.f); XO_UNUSED(a); }
    { bool a = xo::AQuaternion::RoughlyEqual(_a, _b); XO_UNUSED(a); }
    { bool a = xo::AQuaternion::ExactlyEqual(_a, _b); XO_UNUSED(a); }
    { xo::AQuaternion a = xo::AQuaternion::Zero; XO_UNUSED(a); }
    { xo::AQuaternion a = xo::AQuaternion::Identity; XO_UNUSED(a); }
}

}