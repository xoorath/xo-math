#pragma once
#include <inttypes.h>
#include <cstdlib>
#include "xo-math-macros.h"
#include "xo-math-constants.h"
// $inline_begin
namespace xo
{
template<typename T> constexpr XO_INL T Abs(T num)          { return num >= 0 ? num : -num; }
template<typename T> constexpr XO_INL T Max(T a, T b)       { return a > b ? a : b; }
template<typename T> constexpr XO_INL T Max(T a, T b, T c)  { return Max(Max(a, b), c); }
template<typename T> constexpr XO_INL T Min(T a, T b)       { return a < b ? a : b; }
template<typename T> constexpr XO_INL T Min(T a, T b, T c)  { return Min(Min(a, b), c); }

constexpr XO_INL float Clamp(float val, float minVal, float maxVal) {
    return Max(Min(val, maxVal), minVal);
}

float WrapMinMax(float val, float minVal, float maxVal);

constexpr XO_INL float Lerp(float start, float end, float t) {
    return start + t * (end-start);
}

constexpr XO_INL float RelativeEpsilon(float a)          { return MachineEpsilon * Max(1.f, Abs(a)); }
constexpr XO_INL float RelativeEpsilon(float a, float b) { return MachineEpsilon * Max(1.f, Abs(a), Abs(b)); }

// See: http://realtimecollisiondetection.net/blog/?p=89
// Example accuracy:
// CloseEnough(0.00000001f, 0.00000009f) == true
// CloseEnough(0.0000001f,  0.0000009f) == false
constexpr XO_INL bool CloseEnough(float left, float right) {
    return Abs(left - right) <= RelativeEpsilon(left, right);
}

float Sqrt(float val);
float Pow(float val, int power);
template<int power> XO_INL float Pow(float val) { return Pow(val, power); }
template<> float XO_INL Pow<2>(float val) { return val*val; }
template<> float XO_INL Pow<3>(float val) { return val * val * val; }

float Sin(float val);
float Cos(float val);
float ASin(float val);
float ACos(float val);
void SinCos(float val, float& sinOut, float& cosOut);
void ASinACos(float val, float& asinOut, float& acosOut);

#if defined(XO_MATH_IMPL)
float WrapMinMax(float val, float minVal, float maxVal) {
    if (CloseEnough(val, minVal) || CloseEnough(val, maxVal)) {
        return val;
    }
    else if (val > 0.f) {
        return fmod(val, maxVal) + minVal;
    }
    else {
        return maxVal - fmod(Abs(val), maxVal) + minVal;
    }
}

float Sqrt(float val) { return std::sqrt(val); }
float Pow(float val, int power) { return std::pow(val, power); }
float Sin(float val) { return std::sin(val); }
float Cos(float val) { return std::cos(val); }
float ASin(float val) { return std::asin(val); }
float ACos(float val) { return std::acos(val); }

void SinCos(float val, float& sinOut, float& cosOut) {
    sinOut = Sin(val);
    cosOut = Cos(val);
}

void ASinACos(float val, float& asinOut, float& acosOut) {
    asinOut = ASin(val);
    acosOut = ACos(val);
}
#endif

XO_INL constexpr float operator "" _deg2rad(long double num) {
    return static_cast<float>(num) * xo::Deg2Rad;
}

} // ::xo
