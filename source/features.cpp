/*
    features.cpp

    The purpose of this file is to touch all features of xo-math, to ensure it compiles 
    under the current configuration. It's not intended to be a unit test, or produce any
    meaningful code. The purpose is simply for compilation testing.
*/
#include "xo-math.h"
#define XO_UNUSED(code) (void)code

void AllHelperFeatures()
{
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

void AllVector3Features()
{
    // some vectors to use in temp code below...
    xo::Vector3 const _a(xo::Vector3::Up);
    xo::Vector3 const _b(xo::Vector3::Down);

    {xo::Vector3 a(1.f, 2.f, 3.f); XO_UNUSED(a); }
    {xo::Vector3 a(1.f); XO_UNUSED(a); }
    {xo::Vector3 a = xo::Vector3(); XO_UNUSED(a); }
    {xo::Vector3 a(xo::Vector3::Up); XO_UNUSED(a); }
    {xo::Vector3 a; XO_UNUSED(a); }
    {xo::Vector3 a = xo::Vector3::Up; XO_UNUSED(a); }
    {xo::Vector3 a = xo::Vector3(); XO_UNUSED(a); }
    {xo::Vector3 a = _a + _b; XO_UNUSED(a); }
    {xo::Vector3 a = _a - _b; XO_UNUSED(a); }
    {xo::Vector3 a = _a * _b; XO_UNUSED(a); }
    {xo::Vector3 a = _a / _b; XO_UNUSED(a); }
    {xo::Vector3 a = _a; a += _b; XO_UNUSED(a); }
    {xo::Vector3 a = _a; a -= _b; XO_UNUSED(a); }
    {xo::Vector3 a = _a; a *= _b; XO_UNUSED(a); }
    {xo::Vector3 a = _a; a /= _b; XO_UNUSED(a); }
    {xo::Vector3 a = -_a; XO_UNUSED(a); }
    {float a = _a.Sum(); XO_UNUSED(a); }
    {float a = _a.Magnitude(); XO_UNUSED(a); }
    {float a = _a.MagnitudeSquared(); XO_UNUSED(a); }
    {xo::Vector3 a = _a; a.Normalize(); XO_UNUSED(a); }
    {xo::Vector3 a = _a.Normalized(); XO_UNUSED(a); }
    {bool a = xo::Vector3::RoughlyEqual(_a, _b); XO_UNUSED(a); }
    {bool a = xo::Vector3::ExactlyEqual(_a, _b); XO_UNUSED(a); }
    {bool a = xo::Vector3::RoughlyEqual(_a, 1.f); XO_UNUSED(a); }
    {bool a = xo::Vector3::ExactlyEqual(_a, 1.f); XO_UNUSED(a); }
    {float a = xo::Vector3::DotProduct(_a, _b); XO_UNUSED(a); }
    {xo::Vector3 a = xo::Vector3::CrossProduct(_a, _b); XO_UNUSED(a); }
    {xo::Vector3 a = xo::Vector3::Lerp(_a, _b, 0.5f); XO_UNUSED(a); }
    {float a = xo::Vector3::DistanceSquared(_a, _b); XO_UNUSED(a); }
    {float a = xo::Vector3::Distance(_a, _b); XO_UNUSED(a); }
    {xo::Vector3 const& a = xo::Vector3::Zero; XO_UNUSED(a); }
    {xo::Vector3 const& a = xo::Vector3::One; XO_UNUSED(a); }
    {xo::Vector3 const& a = xo::Vector3::Up; XO_UNUSED(a); }
    {xo::Vector3 const& a = xo::Vector3::Down; XO_UNUSED(a); }
    {xo::Vector3 const& a = xo::Vector3::Left; XO_UNUSED(a); }
    {xo::Vector3 const& a = xo::Vector3::Right; XO_UNUSED(a); }
    {xo::Vector3 const& a = xo::Vector3::Forward; XO_UNUSED(a); }
    {xo::Vector3 const& a = xo::Vector3::Backward; XO_UNUSED(a); }
}

void AllVector4Features()
{
    // some vectors to use in temp code below...
    xo::Vector4 const _a(xo::Vector4::One);
    xo::Vector4 const _b(-xo::Vector4::One);

    {xo::Vector4 a(1.f, 2.f, 3.f, 4.f); XO_UNUSED(a); }
    {xo::Vector4 a(1.f); XO_UNUSED(a); }
    {xo::Vector4 a; XO_UNUSED(a); }
    {xo::Vector4 a(_a); XO_UNUSED(a); }
    {xo::Vector4 a(xo::Vector4(1.f)); XO_UNUSED(a); }
    {xo::Vector4 a = _a; XO_UNUSED(a); }
    {xo::Vector4 a = xo::Vector4(_a); XO_UNUSED(a); }
    {xo::Vector4 a = _a + _b; XO_UNUSED(a); }
    {xo::Vector4 a = _a - _b; XO_UNUSED(a); }
    {xo::Vector4 a = _a * _b; XO_UNUSED(a); }
    {xo::Vector4 a = _a / _b; XO_UNUSED(a); }
    {xo::Vector4 a = _a; a += _b; XO_UNUSED(a); }
    {xo::Vector4 a = _a; a -= _b; XO_UNUSED(a); }
    {xo::Vector4 a = _a; a *= _b; XO_UNUSED(a); }
    {xo::Vector4 a = _a; a /= _b; XO_UNUSED(a); }
    {xo::Vector4 a = -_a; XO_UNUSED(a); }
    {float a = _a[0]; XO_UNUSED(a); }
    {xo::Vector4 a = _a; a[0] = 1.f; XO_UNUSED(a); }
    {float a = _a.Sum(); XO_UNUSED(a); }
    {float a = _a.Magnitude(); XO_UNUSED(a); }
    {float a = _a.MagnitudeSquared(); XO_UNUSED(a); }
    {xo::Vector4 a = _a.Normalized(); XO_UNUSED(a); }
    {xo::Vector4 a = _a; a.Normalize(); XO_UNUSED(a); }
    {bool a = xo::Vector4::RoughlyEqual(_a, _b); XO_UNUSED(a); }
    {bool a = xo::Vector4::ExactlyEqual(_a, _b); XO_UNUSED(a); }
    {bool a = xo::Vector4::RoughlyEqual(_a, 1.f); XO_UNUSED(a); }
    {bool a = xo::Vector4::ExactlyEqual(_a, 1.f); XO_UNUSED(a); }
    {float a = xo::Vector4::DotProduct(_a, _b); XO_UNUSED(a); }
    {xo::Vector4 a = xo::Vector4::Lerp(_a, _b, 0.5f); XO_UNUSED(a); }
}

void AllMatrix4x4Features()
{

}

void AllQuaternionFeatures()
{

}

void AllAVector3Features()
{

}

void AllAVector4Features()
{

}

void AllAMatrix4x4Features()
{

}

void AllAQuaternionFeatures()
{

}
