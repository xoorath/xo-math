#pragma once
#include <inttypes.h>
#include <cstdlib>
#include "xo-math-macros.h"
#include "xo-math-constants.h"
// $inline_begin
namespace xo
{
constexpr XO_INL float Abs(float num)                    { return num >= 0 ? num : -num; }
constexpr XO_INL float Max(float a, float b)             { return a > b ? a : b; }
constexpr XO_INL float Max(float a, float b, float c)    { return Max(Max(a, b), c); }
constexpr XO_INL float Min(float a, float b)             { return a < b ? a : b; }
constexpr XO_INL float Min(float a, float b, float c)    { return Min(Min(a, b), c); }

constexpr XO_INL float RelativeEpsilon(float a)          { return MachineEpsilon * Max(1.f, Abs(a)); }
constexpr XO_INL float RelativeEpsilon(float a, float b) { return MachineEpsilon * Max(1.f, Abs(a), Abs(b)); }

// See: http://realtimecollisiondetection.net/blog/?p=89
// Example accuracy:
// CloseEnough(0.00000001f, 0.00000009f) == true
// CloseEnough(0.0000001f,  0.0000009f) == false
constexpr XO_INL bool CloseEnough(float left, float right) {
    return Abs(left - right) <= RelativeEpsilon(left, right);
}

XO_INL float Sqrt(float val)                   { return std::sqrt(val); }
XO_INL float Pow(float val, int power)         { return std::pow(val, power); }
XO_INL float Sin(float val)                    { return std::sin(val); }
XO_INL float Cos(float val)                    { return std::cos(val); }
XO_INL float ASin(float val)                   { return std::asin(val); }
XO_INL float ACos(float val)                   { return std::acos(val); }
XO_INL void SinCos(float val, float& sinOut, float& cosOut) {
    sinOut = Sin(val);
    cosOut = Cos(val);
}
XO_INL float ASinACos(float val, float& asinOut, float& acosOut) {
    asinOut = ASin(val);
    acosOut = ACos(val);
}

} // ::xo
