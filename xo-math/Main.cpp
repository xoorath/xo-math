#include <vector>
#include <iostream>
#include <cmath>
using std::cout;
using std::endl;

#define XO_NO_INVERSE_DIVISION // temp hack to fix a couple tests in Vec4
#define XO_EXPORT_ALL // just to use internal macros below, such as _XOCONSTEXPR, not for a typical use case

#include "include/xo-math.h" // the development version of xo-math
//#include "../xo-math.h" // the distribution version of xo-math

using xo::math::Sqrt;
using xo::math::FloatEpsilon;
using xo::math::HalfPI;
using xo::math::PI;

#include "xo-test.h"

Test test;

#define TEST_MSG(x)  "Main.cpp(" _XO_MATH_STRINGIFY(__LINE__) ") " x

void TestVector2Operators() {
    test("Vector2 Operators", []{
        using xo::math::Vector2;

        Vector2 left    = Vector2(-1.1f, 2.2f);
        Vector2 right   = Vector2(11.1f, -0.2f);
        Vector2 tempL(left);

        test.ReportSuccessIf(left[0], -1.1f, TEST_MSG("extracting the x value from a vector did not produce what we expected."));
        test.ReportSuccessIf(left[1], 2.2f, TEST_MSG("extracting the y value from a vector did not produce what we expected."));

        tempL[0] = 1.0f;
        tempL[1] = 2.0f;

        test.ReportSuccessIf(tempL[0], 1.0f, TEST_MSG("operator [] failed to set an element."));
        test.ReportSuccessIf(tempL[1], 2.0f, TEST_MSG("operator [] failed to set an element."));

        const float leftKnownMag = Sqrt(left[0]*left[0] + left[1]*left[1]);

#define _XO_BASIC_OP(op, ...) \
            test.ReportSuccessIf(   left op right, Vector2(__VA_ARGS__), TEST_MSG("We expected this vec" #op "vec to be equal to the expected input param."));\
            test.ReportSuccessIfNot(left op right, Vector2(1.0f, 2.0f), TEST_MSG("We expected this vec" #op "vec to be not equal to the expected input param."));

        _XO_BASIC_OP(+, 10.0f, 2.0f);
        _XO_BASIC_OP(-, -12.2f, 2.4f);
        _XO_BASIC_OP(*, -12.21f, -0.44f);
        _XO_BASIC_OP(/, -0.0990990991f, -11.0f);
#undef _XO_BASIC_OP
        
#define _XO_BASIC_OPEQ(op, ...) \
            tempL = left; \
            tempL op right;\
            test.ReportSuccessIf(   tempL, Vector2(__VA_ARGS__), TEST_MSG("We expected this vec" #op "vec to be equal to the expected input param.")); \
            test.ReportSuccessIfNot(tempL, Vector2(1.0f, 2.0f), TEST_MSG("We expected this vec" #op "vec to be not equal to the expected input param."));

        _XO_BASIC_OPEQ(+=, 10.0f, 2.0f);
        _XO_BASIC_OPEQ(-=, -12.2f, 2.4f);
        _XO_BASIC_OPEQ(*=, -12.21f, -0.44f);
        _XO_BASIC_OPEQ(/=, -0.0990990991f, -11.0f);
#undef _XO_BASIC_OPEQ

        test.ReportSuccessIf(-left, Vector2(1.1f, -2.2f), TEST_MSG("We expected -vec to be equal to the expected input param"));
        test.ReportSuccessIf(~left, Vector2(2.2f, -1.1f), TEST_MSG("We expected ~vec to be equal to the expected input param"));

#define _XO_BASIC_COMPARE_OP(op, expected) \
            test.ReportSuccessIf(left op right, expected, TEST_MSG("We expected this vec" #op "vec to be equal to the expected input param"));

        _XO_BASIC_COMPARE_OP(<, true);
        _XO_BASIC_COMPARE_OP(>, false);
        _XO_BASIC_COMPARE_OP(<=, true);
        _XO_BASIC_COMPARE_OP(>=, false);
        _XO_BASIC_COMPARE_OP(==, false);
        _XO_BASIC_COMPARE_OP(!=, true);

        test.ReportSuccessIf(left >= left, true, TEST_MSG(">= failed. Left is equal to left."));
        test.ReportSuccessIf(left <= left, true, TEST_MSG("<= failed. Left is equal to left."));
        test.ReportSuccessIf(right <= left, false, TEST_MSG("<= failed. Right should have a larger magnitude than left."));
        test.ReportSuccessIf(left >= right, false, TEST_MSG("<= failed. Right should have a larger magnitude than left."));
        test.ReportSuccessIf(left == left, true, TEST_MSG("== failed. Left should be equal to left."));
        test.ReportSuccessIf(right == right, true, TEST_MSG("== failed. Right should be equal to right."));
        test.ReportSuccessIf(left != left, false, TEST_MSG("!= failed. Left should be equal to left."));
        test.ReportSuccessIf(right != right, false, TEST_MSG("!= failed. Right should be equal to right."));
#undef _XO_BASIC_COMPARE_OP
        
        const float rightKnownMag = 11.1018f;

        test.ReportSuccessIf(left < leftKnownMag, false, TEST_MSG("Vec<float failed. Left should be equal to the known magnitude."));
        // Explicitly handling epsilon in cases where it matters.
        // If you need a value larger than epsilon to correct: it's probably wrong.
        test.ReportSuccessIf(left > leftKnownMag+Vector2::Epsilon, false, TEST_MSG("Vec>float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left <= leftKnownMag+Vector2::Epsilon, true, TEST_MSG("Vec<=float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left >= leftKnownMag, true, TEST_MSG("Vec>=float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left < rightKnownMag, true, TEST_MSG("Vec<float failed. Left should be smaller than the right magnitude"));
        test.ReportSuccessIf(left > rightKnownMag, false, TEST_MSG("Vec>float failed. Left should be smaller than the right magnitude"));

        test.ReportSuccessIf(left == leftKnownMag, true, TEST_MSG("Vec==float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left != leftKnownMag, false, TEST_MSG("Vec!=float failed. Left should be equal to the known magnitude."));

        test.ReportSuccessIf(left == rightKnownMag, false, TEST_MSG("Vec==float failed. Left should not be equal to the right magnitude."));
        test.ReportSuccessIf(left != rightKnownMag, true, TEST_MSG("Vec!=float failed. Left should not be equal to the right magnitude."));
    });
}

void TestVector2Methods() {
    test("Vector2 Methods", []{
        using xo::math::Vector2;

        test.ReportSuccessIf(Vector2(1.1f), Vector2(1.1f, 1.1f), TEST_MSG("Constructor (float) did not set all elements."));
        test.ReportSuccessIf(Vector2(1.0f, 1.0f), Vector2::One, TEST_MSG("Constructor (x, y) did not set all elements."));
        test.ReportSuccessIf(Vector2(Vector2(1.0f, 1.0f)), Vector2::One, TEST_MSG("Copy constructor (Vector3) did not copy as expected."));

        test.ReportSuccessIf(Vector2(xo::math::Vector4(1.1f, 2.2f, 3.3f, 4.4f)), Vector2(1.1f, 2.2f), TEST_MSG("Constructor(Vector4) didn't set all elements as expected"));
        test.ReportSuccessIf(Vector2(xo::math::Vector3(1.1f, 2.2f, 3.3f)), Vector2(1.1f, 2.2f), TEST_MSG("Constructor(Vector3) didn't set all elements as expected"));

        Vector2 temp;
        test.ReportSuccessIf(temp.Set(1.1f, 2.2f), Vector2(1.1f, 2.2f), TEST_MSG("Set(x, y, z) did not set all elements."));
        test.ReportSuccessIf(temp.Set(1.1f), Vector2(1.1f, 1.1f), TEST_MSG("Set(float) did not set all elements."));
        test.ReportSuccessIf(temp.Set(Vector2::One), Vector2::One, TEST_MSG("Set(Vector2) did not set all elements."));

        temp.Set(1.1f, 2.2f);
        float x, y;
        temp.Get(x, y);
        test.ReportSuccessIf(x, 1.1f, TEST_MSG("Get(x, y, z) did not extract a correct x value."));
        test.ReportSuccessIf(y, 2.2f, TEST_MSG("Get(x, y, z) did not extract a correct y value."));

        _MM_ALIGN16 float f[4];
        temp.Get(f);
        test.ReportSuccessIf(f[0], 1.1f, TEST_MSG("Get(f) did not extract a correct x value."));
        test.ReportSuccessIf(f[1], 2.2f, TEST_MSG("Get(f) did not extract a correct y value."));

        test.ReportSuccessIf(temp.Sum(), 3.3f, TEST_MSG("Sum() did not accumulate correctly."));

        test.ReportSuccessIf(temp.MagnitudeSquared(), 6.05f, TEST_MSG("did not match known magnitude squared"));
        test.ReportSuccessIf(temp.Magnitude(), 2.45967f, TEST_MSG("did not match known magnitude"));

        test.ReportSuccessIf(temp.Normalize().Magnitude(), 1.0f, TEST_MSG("vector is not normal after normalize"));
        test.ReportSuccessIf(temp.Magnitude(), 1.0f, TEST_MSG("vector is not normal after normalize"));

        temp.Set(1.1f, 2.2f);
        test.ReportSuccessIf(temp.Normalized().Magnitude(), 1.0f, TEST_MSG("vector is not normal returning from normalized"));
        test.ReportSuccessIfNot(temp.Magnitude(), 1.0f, TEST_MSG("vector is normal after normalized"));

        test.ReportSuccessIfNot(temp.IsZero(), TEST_MSG("vector is zero when it shouldn't be."));
        temp = Vector2::Zero;
        test.ReportSuccessIf(temp.IsZero(), TEST_MSG("vector isn't zero when it shouldn be."));

        test.ReportSuccessIfNot(temp.IsNormalized(), TEST_MSG("vector is normal when it shouldn't be."));
        temp = Vector2::UnitX;
        test.ReportSuccessIf(temp.IsNormalized(), TEST_MSG("vector is not normal when it shouldn be."));

        test.ReportSuccessIf(Vector2(1.1f, 2.2f).Dot(Vector2(-0.1f, -22.0f)), -48.510002136230f, TEST_MSG("Dot did not match known dot"));
        test.ReportSuccessIf(Vector2::Dot(Vector2(1.1f, 2.2f), Vector2(-0.1f, -22.0f)), -48.510002136230f, TEST_MSG("Dot did not match known dot"));

        test.ReportSuccessIf(Vector2(1.1f, 2.2f).Cross(Vector2(-0.1f, -22.0f)), -23.980001449585f, TEST_MSG("Cross did not match known Cross"));
        test.ReportSuccessIf(Vector2::Cross(Vector2(1.1f, 2.2f), Vector2(-0.1f, -22.0f)), -23.980001449585f, TEST_MSG("Cross did not match known cross"));

        test.ReportSuccessIf(Vector2::AngleRadians(Vector2::Up, Vector2::Right), HalfPI, TEST_MSG("up to right should be a rotation of halfpi"));
        test.ReportSuccessIf(Vector2::AngleRadians(Vector2::Up*2.0f, Vector2::Right), HalfPI, TEST_MSG("up to right should be a rotation of halfpi"));
        test.ReportSuccessIf(Vector2::AngleRadians(Vector2::Up, Vector2::Right*2.0f), HalfPI, TEST_MSG("up to right should be a rotation of halfpi"));

        test.ReportSuccessIf(Vector2::Up.AngleRadians(Vector2::Right), HalfPI, TEST_MSG("up to right should be a rotation of halfpi"));
        test.ReportSuccessIf(Vector2::Up.AngleRadians(Vector2::Right), HalfPI, TEST_MSG("up to right should be a rotation of halfpi"));
        test.ReportSuccessIf((Vector2::Up*2.0f).AngleRadians(Vector2::Right*2.0f), HalfPI, TEST_MSG("up to right should be a rotation of halfpi"));

        test.ReportSuccessIf(Vector2::AngleDegrees(Vector2::Up, Vector2::Right), 90.0f, TEST_MSG("up to right should be a rotation of 90.0f"));
        test.ReportSuccessIf(Vector2::AngleDegrees(Vector2::Up*2.0f, Vector2::Right), 90.0f, TEST_MSG("up to right should be a rotation of 90.0f"));
        test.ReportSuccessIf(Vector2::AngleDegrees(Vector2::Up, Vector2::Right*2.0f), 90.0f, TEST_MSG("up to right should be a rotation of 90.0f"));

        test.ReportSuccessIf(Vector2::Up.AngleDegrees(Vector2::Right), 90.0f, TEST_MSG("up to right should be a rotation of 90.0f"));
        test.ReportSuccessIf(Vector2::Up.AngleDegrees(Vector2::Right), 90.0f, TEST_MSG("up to right should be a rotation of 90.0f"));
        test.ReportSuccessIf((Vector2::Up*2.0f).AngleDegrees(Vector2::Right*2.0f), 90.0f, TEST_MSG("up to right should be a rotation of 90.0f"));

        test.ReportSuccessIf(Vector2::OrthogonalCW(Vector2::Up), Vector2::Right, TEST_MSG("right should be 90 degrees cw to up"));
        test.ReportSuccessIf(Vector2::OrthogonalCCW(Vector2::Up), Vector2::Left, TEST_MSG("left should be 90 degrees ccw to up"));

        test.ReportSuccessIf(Vector2::Lerp(Vector2::Zero, Vector2::One*10.0f, 0.0f), Vector2::Zero, TEST_MSG("lerp of 0 should be the left param"));
        test.ReportSuccessIf(Vector2::Lerp(Vector2::Zero, Vector2::One*10.0f, 1.0f), Vector2::One*10.0f, TEST_MSG("lerp of 1 should be the right param"));
        test.ReportSuccessIf(Vector2::Lerp(Vector2::Zero, Vector2::One*10.0f, 0.5f), Vector2::One*5.0f, TEST_MSG("lerp of 0.5 should be the midpoint"));

    });
}

void TestVector3Operators() {
    test("Vector3 Operators", []{
        using xo::math::Vector3;

        Vector3 left    = Vector3(-1.1f, 2.2f, 0.33f); // mag: 2.481713056564
        Vector3 right   = Vector3(11.1f, -0.2f, -3.33f); // mag: 11.5905
        Vector3 tempL(left);

        test.ReportSuccessIf(left[0], -1.1f, TEST_MSG("extracting the x value from a vector did not produce what we expected."));
        test.ReportSuccessIf(left[1], 2.2f, TEST_MSG("extracting the y value from a vector did not produce what we expected."));
        test.ReportSuccessIf(left[2], 0.33f, TEST_MSG("extracting the z value from a vector did not produce what we expected."));

        tempL[0] = 1.0f;
        tempL[1] = 2.0f;
        tempL[2] = 3.0f;

        test.ReportSuccessIf(tempL[0], 1.0f, TEST_MSG("operator [] failed to set an element."));
        test.ReportSuccessIf(tempL[1], 2.0f, TEST_MSG("operator [] failed to set an element."));
        test.ReportSuccessIf(tempL[2], 3.0f, TEST_MSG("operator [] failed to set an element."));

        const float leftKnownMag = Sqrt(left[0]*left[0] + left[1]*left[1] + left[2]*left[2]);

#define _XO_BASIC_OP(op, ...) \
            test.ReportSuccessIf(   left op right, Vector3(__VA_ARGS__), TEST_MSG("We expected this vec" #op "vec to be equal to the expected input param."));\
            test.ReportSuccessIfNot(left op right, Vector3(1.0f, 2.0f, 3.0f), TEST_MSG("We expected this vec" #op "vec to be not equal to the expected input param."));

        _XO_BASIC_OP(+, 10.0f, 2.0f, -3.0f);
        _XO_BASIC_OP(-, -12.2f, 2.4f, 3.66f);
        _XO_BASIC_OP(*, -12.21f, -0.44f, -1.0989f);
        _XO_BASIC_OP(/, -0.0990990991f, -11.0f, -0.0990990991f);
#undef _XO_BASIC_OP
        
#define _XO_BASIC_OPEQ(op, ...) \
            tempL = left; \
            tempL op right;\
            test.ReportSuccessIf(   tempL, Vector3(__VA_ARGS__), TEST_MSG("We expected this vec" #op "vec to be equal to the expected input param.")); \
            test.ReportSuccessIfNot(tempL, Vector3(1.0f, 2.0f, 3.0f), TEST_MSG("We expected this vec" #op "vec to be not equal to the expected input param."));

        _XO_BASIC_OPEQ(+=, 10.0f, 2.0f, -3.0f);
        _XO_BASIC_OPEQ(-=, -12.2f, 2.4f, 3.66f);
        _XO_BASIC_OPEQ(*=, -12.21f, -0.44f, -1.0989f);
        _XO_BASIC_OPEQ(/=, -0.0990990991f, -11.0f, -0.0990990991f);
#undef _XO_BASIC_OPEQ

        test.ReportSuccessIf(-left, Vector3(1.1f, -2.2f, -0.33f), TEST_MSG("We expected -vec to be equal to the expected input param"));
        test.ReportSuccessIf(~left, Vector3(0.33f, 2.2f, -1.1f), TEST_MSG("We expected ~vec to be equal to the expected input param"));

#define _XO_BASIC_COMPARE_OP(op, expected) \
            test.ReportSuccessIf(left op right, expected, TEST_MSG("We expected this vec" #op "vec to be equal to the expected input param"));

        _XO_BASIC_COMPARE_OP(<, true);
        _XO_BASIC_COMPARE_OP(>, false);
        _XO_BASIC_COMPARE_OP(<=, true);
        _XO_BASIC_COMPARE_OP(>=, false);
        _XO_BASIC_COMPARE_OP(==, false);
        _XO_BASIC_COMPARE_OP(!=, true);

        test.ReportSuccessIf(left >= left, true, TEST_MSG(">= failed. Left is equal to left."));
        test.ReportSuccessIf(left <= left, true, TEST_MSG("<= failed. Left is equal to left."));
        test.ReportSuccessIf(right <= left, false, TEST_MSG("<= failed. Right should have a larger magnitude than left."));
        test.ReportSuccessIf(left >= right, false, TEST_MSG("<= failed. Right should have a larger magnitude than left."));
        test.ReportSuccessIf(left == left, true, TEST_MSG("== failed. Left should be equal to left."));
        test.ReportSuccessIf(right == right, true, TEST_MSG("== failed. Right should be equal to right."));
        test.ReportSuccessIf(left != left, false, TEST_MSG("!= failed. Left should be equal to left."));
        test.ReportSuccessIf(right != right, false, TEST_MSG("!= failed. Right should be equal to right."));
#undef _XO_BASIC_COMPARE_OP
        
        const float rightKnownMag = 11.5905f;

        test.ReportSuccessIf(left < leftKnownMag, false, TEST_MSG("Vec<float failed. Left should be equal to the known magnitude."));
        // Explicitly handling epsilon in cases where it matters.
        // If you need a value larger than epsilon to correct: it's probably wrong.
        test.ReportSuccessIf(left > leftKnownMag+Vector3::Epsilon, false, TEST_MSG("Vec>float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left <= leftKnownMag+Vector3::Epsilon, true, TEST_MSG("Vec<=float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left >= leftKnownMag, true, TEST_MSG("Vec>=float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left < rightKnownMag, true, TEST_MSG("Vec<float failed. Left should be smaller than the right magnitude"));
        test.ReportSuccessIf(left > rightKnownMag, false, TEST_MSG("Vec>float failed. Left should be smaller than the right magnitude"));

        test.ReportSuccessIf(left == leftKnownMag, true, TEST_MSG("Vec==float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left != leftKnownMag, false, TEST_MSG("Vec!=float failed. Left should be equal to the known magnitude."));

        test.ReportSuccessIf(left == rightKnownMag, false, TEST_MSG("Vec==float failed. Left should not be equal to the right magnitude."));
        test.ReportSuccessIf(left != rightKnownMag, true, TEST_MSG("Vec!=float failed. Left should not be equal to the right magnitude."));

#if XO_SSE
        __m128 m = left; // will generate a compiler error if the __m128 cast operator doesn't exist.

        // failure in the following could indicate a problem with the Vector3 constructor, the cast or the comparison.
        // some debugging by hand would be needed if these fail.
        test.ReportSuccessIf(Vector3(m), left, TEST_MSG("The implicitly casted __m128 was not assigned to correctly."));
        test.ReportSuccessIfNot(Vector3(m), right, TEST_MSG("The implicitly casted __m128 wasn't compared to correctly."));
#endif
    });
}

void TestVector3Methods() {
    test("Vector3 Methods", []{
        using xo::math::Vector3;
        test.ReportSuccessIf(Vector3(1.1f), Vector3(1.1f, 1.1f, 1.1f), TEST_MSG("Constructor (float) did not set all elements."));
        test.ReportSuccessIf(Vector3(1.0f, 1.0f, 1.0f), Vector3::One, TEST_MSG("Constructor (x, y, z) did not set all elements."));
        test.ReportSuccessIf(Vector3(Vector3(1.0f, 1.0f, 1.0f)), Vector3::One, TEST_MSG("Copy constructor (Vector3) did not copy as expected."));
#if XO_SSE
        __m128 m = _mm_set_ps1(1.1f);
        test.ReportSuccessIf(Vector3(m), Vector3(1.1f, 1.1f, 1.1f), TEST_MSG("Constructor (__m128) did not set all elements."));
#endif
        test.ReportSuccessIf(Vector3(xo::math::Vector4(1.1f, 2.2f, 3.3f, 4.4f)), Vector3(1.1f, 2.2f, 3.3f), TEST_MSG("Constructor(Vector4) didn't set all elements as expected"));
        test.ReportSuccessIf(Vector3(xo::math::Vector2(1.1f, 2.2f)), Vector3(1.1f, 2.2f, 0.0f), TEST_MSG("Constructor(Vector2) didn't set all elements as expected"));

        Vector3 temp;
        test.ReportSuccessIf(temp.Set(1.1f, 2.2f, 3.3f), Vector3(1.1f, 2.2f, 3.3f), TEST_MSG("Set(x, y, z) did not set all elements."));
        test.ReportSuccessIf(temp.Set(1.1f), Vector3(1.1f, 1.1f, 1.1f), TEST_MSG("Set(float) did not set all elements."));
        test.ReportSuccessIf(temp.Set(Vector3::One), Vector3::One, TEST_MSG("Set(Vector3) did not set all elements."));

#if XO_SSE
        test.ReportSuccessIf(temp.Set(_mm_set_ps1(1.1f)), Vector3(1.1f, 1.1f, 1.1f), TEST_MSG("Set(__m128) did not set all elements."));
#endif
        temp.Set(1.1f, 2.2f, 3.3f);
        float x, y, z;
        temp.Get(x, y, z);
        test.ReportSuccessIf(x, 1.1f, TEST_MSG("Get(x, y, z) did not extract a correct x value."));
        test.ReportSuccessIf(y, 2.2f, TEST_MSG("Get(x, y, z) did not extract a correct y value."));
        test.ReportSuccessIf(z, 3.3f, TEST_MSG("Get(x, y, z) did not extract a correct z value."));

        _MM_ALIGN16 float f[4];
        temp.Get(f);
        test.ReportSuccessIf(f[0], 1.1f, TEST_MSG("Get(f) did not extract a correct x value."));
        test.ReportSuccessIf(f[1], 2.2f, TEST_MSG("Get(f) did not extract a correct y value."));
        test.ReportSuccessIf(f[2], 3.3f, TEST_MSG("Get(f) did not extract a correct z value."));

        test.ReportSuccessIf(temp.ZYX(), Vector3(3.3f, 2.2f, 1.1f), TEST_MSG("ZYX() did not swizzle correctly."));
        test.ReportSuccessIf(temp.Sum(), 6.6f, TEST_MSG("Sum() did not accumulate correctly."));
        
        _XOCONSTEXPR auto knownMagSq = 1.1f*1.1f + 2.2f*2.2f + 3.3f*3.3f;
        const auto knownMag = Sqrt(knownMagSq);

        test.ReportSuccessIf(temp.MagnitudeSquared(), knownMagSq, TEST_MSG("Magnitude squared did not match the knownMagSq"));
        test.ReportSuccessIf(temp.Magnitude(), knownMag, TEST_MSG("Magnitude did not match the knownMag"));

        float knownNormalization[3] = {0.267261f, 0.534523f, 0.801784f};
        auto norm = temp.Normalized();
        test.ReportSuccessIfNot(norm, temp, TEST_MSG("temp.Normalized should not modify temp in any way."));
        test.ReportSuccessIf(norm.x, knownNormalization[0], TEST_MSG("Normalized didn't produce a known x value for its normalized input."));
        test.ReportSuccessIf(norm.y, knownNormalization[1], TEST_MSG("Normalized didn't produce a known y value for its normalized input."));
        test.ReportSuccessIf(norm.z, knownNormalization[2], TEST_MSG("Normalized didn't produce a known z value for its normalized input."));
        test.ReportSuccessIf(norm.MagnitudeSquared(), 1.0f, TEST_MSG("Normalized didn't produce a unit length vector."));

        temp.Normalize();
        test.ReportSuccessIf(norm, temp, TEST_MSG("Normalize didn't produce the same result as Normalized"));

        test.ReportSuccessIf(norm.IsNormalized(), TEST_MSG("IsNormalized returned false after Normalize"));

        test.ReportSuccessIfNot(temp.IsZero(), TEST_MSG("Temp was decided to be zero when it isn't zero."));
        temp.Set(0.0f);
        test.ReportSuccessIf(temp.IsZero(), TEST_MSG("Temp was decided to be non zero when it is zero."));
        test.ReportSuccessIfNot(temp.IsNormalized(), TEST_MSG("IsNormalized returned true for a zero vector."));

        Vector3 temp2;
        temp.Set(1.1f, 2.2f, 3.3f);
        temp2.Set(4.4f, 5.5f, 6.6f);

        test.ReportSuccessIf(Vector3::Dot(temp, temp2), 38.72f, TEST_MSG("dot product of (1.1,2.2,3.3) dot (4.4,5.5,6.6) expecetd to match known result."));
        test.ReportSuccessIf(Vector3::Dot(temp2, temp), 38.72f, TEST_MSG("dot product of (4.4,5.5,6.6) dot (1.1,2.2,3.3) expecetd to match known result."));

        test.ReportSuccessIf(temp.Dot(temp2), 38.72f, TEST_MSG("dot product of (1.1,2.2,3.3) dot (4.4,5.5,6.6) expecetd to match known result."));
        test.ReportSuccessIf(temp2.Dot(temp), 38.72f, TEST_MSG("dot product of (4.4,5.5,6.6) dot (1.1,2.2,3.3) expecetd to match known result."));

        test.ReportSuccessIf(temp.Dot(temp), temp.MagnitudeSquared(), TEST_MSG("dot product of a given vector by itself is expected to be its own magnitude squared."));
        test.ReportSuccessIf(temp2.Dot(temp2), temp2.MagnitudeSquared(), TEST_MSG("dot product of a given vector by itself is expected to be its own magnitude squared."));

        temp2 = -temp2;
        test.ReportSuccessIf(Vector3::Dot(temp, temp2), -38.72f, TEST_MSG("dot product of (1.1,2.2,3.3) dot (-4.4,-5.5,-6.6) expecetd to match known result."));
        test.ReportSuccessIf(Vector3::Dot(temp2, temp), -38.72f, TEST_MSG("dot product of (-4.4,-5.5,-6.6) dot (1.1,2.2,3.3) expecetd to match known result."));

        test.ReportSuccessIf(temp.Dot(temp2), -38.72f, TEST_MSG("dot product of (1.1,2.2,3.3) dot (-4.4,-5.5,-6.6) expecetd to match known result."));
        test.ReportSuccessIf(temp2.Dot(temp), -38.72f, TEST_MSG("dot product of (-4.4,-5.5,-6.6) dot (1.1,2.2,3.3) expecetd to match known result."));

        temp2.Set(4.4f, 5.5f, 6.6f);

        test.ReportSuccessIf(Vector3::Cross(temp, temp2), Vector3(-3.63f, 7.26f, -3.63f), TEST_MSG("dot product of (1.1,2.2,3.3) and (4.4,5.5,6.6) expected to match a known result."));
        Vector3 temp3;
        Vector3::Cross(temp, temp2, temp3);
        test.ReportSuccessIf(temp3, Vector3(-3.63f, 7.26f, -3.63f), TEST_MSG("dot product of (1.1,2.2,3.3) and (4.4,5.5,6.6) expected to match a known result."));
        test.ReportSuccessIf(temp.Cross(temp2), Vector3(-3.63f, 7.26f, -3.63f), TEST_MSG("dot product of (1.1,2.2,3.3) and (4.4,5.5,6.6) expected to match a known result."));

        test.ReportSuccessIf(Vector3::Cross(temp2, temp), Vector3(3.63f, -7.26f, 3.63f), TEST_MSG("dot product of (4.4,5.5,6.6) and (1.1,2.2,3.3) expected to match a known result."));
        Vector3::Cross(temp2, temp, temp3);
        test.ReportSuccessIf(temp3, Vector3(3.63f, -7.26f, 3.63f), TEST_MSG("dot product of (4.4,5.5,6.6) and (1.1,2.2,3.3) expected to match a known result."));
        test.ReportSuccessIf(temp2.Cross(temp), Vector3(3.63f, -7.26f, 3.63f), TEST_MSG("dot product of (4.4,5.5,6.6) and (1.1,2.2,3.3) expected to match a known result."));
        
        temp2.Set(4.4f, -5.5f, 6.6f);
        test.ReportSuccessIf(Vector3::Max(temp, temp2), Vector3(4.4f, 2.2f, 6.6f), TEST_MSG("Max of two vectors is not as expected."));
        test.ReportSuccessIf(Vector3::Min(temp, temp2), Vector3(1.1f, -5.5f, 3.3f), TEST_MSG("Min of two vectors is not as expected."));
        test.ReportSuccessIf(Vector3::Max(temp2, temp), Vector3(4.4f, 2.2f, 6.6f), TEST_MSG("Max of two vectors is not as expected."));
        test.ReportSuccessIf(Vector3::Min(temp2, temp), Vector3(1.1f, -5.5f, 3.3f), TEST_MSG("Min of two vectors is not as expected."));

        Vector3::Max(temp, temp2, temp3);
        test.ReportSuccessIf(temp3, Vector3(4.4f, 2.2f, 6.6f), TEST_MSG("Max of two vectors is not as expected."));
        Vector3::Min(temp, temp2, temp3);
        test.ReportSuccessIf(temp3, Vector3(1.1f, -5.5f, 3.3f), TEST_MSG("Max of two vectors is not as expected."));

        temp2.Set(4.4f, 5.5f, 6.6f);

#define _XO_LERP(t, expected, msg) \
            test.ReportSuccessIf(Vector3::Lerp(temp, temp2, t), expected, TEST_MSG(msg)); \
            Vector3::Lerp(temp, temp2, t, temp3); \
            test.ReportSuccessIf(temp3, expected, TEST_MSG(msg)); \
            test.ReportSuccessIf(temp.Lerp(temp2, t), expected, TEST_MSG(msg));

        _XO_LERP(1.0f, temp2, "Lerp of 1.0 didnt return the right param.");
        _XO_LERP(0.0f, temp, "Lerp of 0.0 didnt return the left param.");
        _XO_LERP(0.5f, Vector3(2.75f, 3.85f, 4.95f), "Lerp of 0.5 didnt return the half way between left and right params.");

#undef _XO_LERP

        

#define _XO_ROTATE_RADIANS(input, axis, angle, expected, msg) \
            temp = input; \
            test.ReportSuccessIf(Vector3::RotateRadians(temp, axis, angle), expected, TEST_MSG(msg)); \
            Vector3::RotateRadians(temp, axis, angle, temp3); \
            test.ReportSuccessIf(temp3, expected, TEST_MSG(msg)); \
            test.ReportSuccessIf(temp.RotateRadians(axis, angle), expected, TEST_MSG(msg)); \

#if defined XO_SPACE_LEFTHAND // positive is clockwise 
        _XO_ROTATE_RADIANS(Vector3::Up, Vector3::Right, HalfPI, Vector3::Forward, "Up rotated pi/2 radians by the right axis should face forward.");
        _XO_ROTATE_RADIANS(Vector3::Up, Vector3::Right, -HalfPI, Vector3::Backward, "Up rotated -pi/2 radians by the right axis should face backward.");
        _XO_ROTATE_RADIANS(Vector3::Up*2.0f, Vector3::Right, HalfPI, Vector3::Forward*2.0f, "Up rotated pi/2 radians by the right axis should face forward.");
        _XO_ROTATE_RADIANS(Vector3::Up*2.0f, Vector3::Right, -HalfPI, Vector3::Backward*2.0f, "Up rotated -pi/2 radians by the right axis should face backward.");
#elif defined XO_SPACE_RIGHTHAND // positive is counter clockwise
        _XO_ROTATE_RADIANS(Vector3::Up, Vector3::Right, HalfPI, Vector3::Backward, "Up rotated pi/2 radians by the right axis should face backward.");
        _XO_ROTATE_RADIANS(Vector3::Up, Vector3::Right, -HalfPI, Vector3::Forward, "Up rotated -pi/2 radians by the right axis should face forward.");
        _XO_ROTATE_RADIANS(Vector3::Up*2.0f, Vector3::Right, HalfPI, Vector3::Backward*2.0f, "Up rotated pi/2 radians by the right axis should face backward.");
        _XO_ROTATE_RADIANS(Vector3::Up*2.0f, Vector3::Right, -HalfPI, Vector3::Forward*2.0f, "Up rotated -pi/2 radians by the right axis should face forward.");
#endif

#undef _XO_ROTATE_RADIANS

#define _XO_ROTATE_DEGREES(input, axis, angle, expected, msg) \
            temp = input; \
            test.ReportSuccessIf(Vector3::RotateDegrees(temp, axis, angle), expected, TEST_MSG(msg)); \
            Vector3::RotateDegrees(temp, axis, angle, temp3); \
            test.ReportSuccessIf(temp3, expected, TEST_MSG(msg)); \
            test.ReportSuccessIf(temp.RotateDegrees(axis, angle), expected, TEST_MSG(msg)); \

#if defined XO_SPACE_LEFTHAND
        _XO_ROTATE_DEGREES(Vector3::Up, Vector3::Right, 90.0f, Vector3::Forward, "Up rotated 90 degrees radians by the right axis should face forward.");
        _XO_ROTATE_DEGREES(Vector3::Up, Vector3::Right, -90.0f, Vector3::Backward, "Up rotated -90 degrees radians by the right axis should face backward.");
        _XO_ROTATE_DEGREES(Vector3::Up*2.0f, Vector3::Right, 90.0f, Vector3::Forward*2.0f, "Up rotated 90 degrees radians by the right axis should face forward.");
        _XO_ROTATE_DEGREES(Vector3::Up*2.0f, Vector3::Right, -90.0f, Vector3::Backward*2.0f, "Up rotated -90 degrees radians by the right axis should face backward.");
#elif defined XO_SPACE_RIGHTHAND
        _XO_ROTATE_DEGREES(Vector3::Up, Vector3::Right, 90.0f, Vector3::Backward, "Up rotated 90 degrees radians by the right axis should face forward.");
        _XO_ROTATE_DEGREES(Vector3::Up, Vector3::Right, -90.0f, Vector3::Forward, "Up rotated -90 degrees radians by the right axis should face backward.");
        _XO_ROTATE_DEGREES(Vector3::Up*2.0f, Vector3::Right, 90.0f, Vector3::Backward*2.0f, "Up rotated 90 degrees radians by the right axis should face forward.");
        _XO_ROTATE_DEGREES(Vector3::Up*2.0f, Vector3::Right, -90.0f, Vector3::Forward*2.0f, "Up rotated -90 degrees radians by the right axis should face backward.");        
#endif

#undef _XO_ROTATE_DEGREES


#define _XO_TEST_RANDOM_SPHERE(method) \
            test.ReportSuccessIfNot(temp, temp2, TEST_MSG( #method " should return random results. We got the same vector twice, which is unlikely to be correct.")); \

#define _XO_TEST_RANDOM_SPHERE_EQ(method, radius) \
            test.ReportSuccessIf(temp == radius, TEST_MSG( #method " should return a sphere matching a known radius.")); \
            test.ReportSuccessIf(temp2 == radius, TEST_MSG( #method " should return a sphere matching a known radius."));

#define _XO_TEST_RANDOM_SPHERE_LT(method, radius) \
            test.ReportSuccessIf(temp <= radius, TEST_MSG( #method " should return a sphere with a smaller than given radius.")); \
            test.ReportSuccessIf(temp2 <= radius, TEST_MSG( #method " should return a sphere with a smaller than given radius."));

#define _XO_TEST_RANDOM_SPHERE_LT_GT(method, low, high) \
            test.ReportSuccessIf(temp <= high, TEST_MSG( #method " should return a sphere with a smaller than given radius.")); \
            test.ReportSuccessIf(temp2 <= high, TEST_MSG( #method " should return a sphere with a smaller than given radius.")); \
            test.ReportSuccessIf(temp >= low, TEST_MSG( #method " should return a sphere with a greater than given radius.")); \
            test.ReportSuccessIf(temp2 >= low, TEST_MSG( #method " should return a sphere with a greater than given radius."));

#define _XO_TEST_RANDOM_CONE(method, expectedOutMagSquared) \
            test.ReportSuccessIfNot(temp, Vector3::Up, TEST_MSG( #method " should return random results. we got the same vector back that we put in, which is unlikely to be correct.")); \
            test.ReportSuccessIfNot(temp2, Vector3::Up, TEST_MSG( #method " should return random results. we got the same vector back that we put in, which is unlikely to be correct.")); \
            test.ReportSuccessIfNot(temp, temp2, TEST_MSG( #method " should return random results. We got the same vector twice, which is unlikely to be correct.")); \
            test.ReportSuccessIf(temp.MagnitudeSquared(), expectedOutMagSquared, TEST_MSG( #method " gave us a vector with a different length than the input.")); \
            test.ReportSuccessIf(temp2.MagnitudeSquared(), expectedOutMagSquared, TEST_MSG( #method " gave us a vector with a different length than the input."));

#define _XO_TEST_CONE_EQ(method, angle) \
            test.ReportSuccessIf(temp.AngleRadians(Vector3::Up), angle, TEST_MSG( #method " did not respect the provided angle requested for a given cone.")); \
            test.ReportSuccessIf(temp2.AngleRadians(Vector3::Up), angle, TEST_MSG( #method " did not respect the provided angle requested for a given cone."));

#define _XO_TEST_CONE_LT(method, angle) \
            test.ReportSuccessIf(temp.AngleRadians(Vector3::Up) <= angle+FloatEpsilon, TEST_MSG( #method " did not respect the provided angle requested for a given cone.")); \
            test.ReportSuccessIf(temp2.AngleRadians(Vector3::Up) <= angle+FloatEpsilon, TEST_MSG( #method " did not respect the provided angle requested for a given cone."));

        temp = Vector3::RandomOnConeRadians(Vector3::Up, HalfPI/2);
        temp2 = Vector3::RandomOnConeRadians(Vector3::Up, HalfPI/2);
        _XO_TEST_RANDOM_CONE(RandomOnConeRadians, 1.0f);
        _XO_TEST_CONE_EQ(RandomOnConeRadians, HalfPI/2.0f);

        temp = Vector3::RandomOnConeRadians(Vector3::Up * 2.2f, HalfPI/2);
        temp2 = Vector3::RandomOnConeRadians(Vector3::Up * 2.2f, HalfPI/2);
        _XO_TEST_RANDOM_CONE(RandomOnConeRadians, 2.2f*2.2f);
        _XO_TEST_CONE_EQ(RandomOnConeRadians, HalfPI/2.0f);

        Vector3::RandomOnConeRadians(Vector3::Up, HalfPI/2, temp);
        Vector3::RandomOnConeRadians(Vector3::Up, HalfPI/2, temp2);
        _XO_TEST_RANDOM_CONE(RandomOnConeRadians, 1.0f);
        _XO_TEST_CONE_EQ(RandomOnConeRadians, HalfPI/2.0f);

        Vector3::RandomOnConeRadians(Vector3::Up * 2.2f, HalfPI/2, temp);
        Vector3::RandomOnConeRadians(Vector3::Up * 2.2f, HalfPI/2, temp2);
        _XO_TEST_RANDOM_CONE(RandomOnConeRadians, 2.2f * 2.2f);
        _XO_TEST_CONE_EQ(RandomOnConeRadians, HalfPI/2.0f);

        temp = Vector3::Up.RandomOnConeRadians(HalfPI/2.0f);
        temp2 = Vector3::Up.RandomOnConeRadians(HalfPI/2.0f);
        _XO_TEST_RANDOM_CONE(RandomOnConeRadians, 1.0f);
        _XO_TEST_CONE_EQ(RandomOnConeRadians, HalfPI/2.0f);

        temp = (Vector3::Up * 2.2f).RandomOnConeRadians(HalfPI/2.0f);
        temp2 = (Vector3::Up * 2.2f).RandomOnConeRadians(HalfPI/2.0f);
        _XO_TEST_RANDOM_CONE(RandomOnConeRadians, 2.2f * 2.2f);
        _XO_TEST_CONE_EQ(RandomOnConeRadians, HalfPI/2.0f);

        temp = Vector3::RandomInConeRadians(Vector3::Up, HalfPI/2.0f);
        temp2 = Vector3::RandomInConeRadians(Vector3::Up, HalfPI/2.0f);
        _XO_TEST_RANDOM_CONE(RandomInConeRadians, 1.0f);
        _XO_TEST_CONE_LT(RandomInConeRadians, HalfPI/2.0f);

        temp = Vector3::RandomInConeRadians(Vector3::Up * 2.2f, HalfPI/2.0f);
        temp2 = Vector3::RandomInConeRadians(Vector3::Up * 2.2f, HalfPI/2.0f);
        _XO_TEST_RANDOM_CONE(RandomInConeRadians, 2.2f*2.2f);
        _XO_TEST_CONE_LT(RandomInConeRadians, HalfPI/2.0f);

        Vector3::RandomInConeRadians(Vector3::Up, HalfPI/2.0f, temp);
        Vector3::RandomInConeRadians(Vector3::Up, HalfPI/2.0f, temp2);
        _XO_TEST_RANDOM_CONE(RandomInConeRadians, 1.0f);
        _XO_TEST_CONE_LT(RandomInConeRadians, HalfPI/2.0f);

        Vector3::RandomInConeRadians(Vector3::Up * 2.2f, HalfPI/2.0f, temp);
        Vector3::RandomInConeRadians(Vector3::Up * 2.2f, HalfPI/2.0f, temp2);
        _XO_TEST_RANDOM_CONE(RandomInConeRadians, 2.2f*2.2f);
        _XO_TEST_CONE_LT(RandomInConeRadians, HalfPI/2.0f);

        temp = Vector3::Up.RandomInConeRadians(HalfPI/2.0f);
        temp2 = Vector3::Up.RandomInConeRadians(HalfPI/2.0f);
        _XO_TEST_RANDOM_CONE(RandomInConeRadians, 1.0f);
        _XO_TEST_CONE_LT(RandomInConeRadians, HalfPI/2.0f);

        temp = (Vector3::Up * 2.2f).RandomInConeRadians(HalfPI/2.0f);
        temp2 = (Vector3::Up * 2.2f).RandomInConeRadians(HalfPI/2.0f);
        _XO_TEST_RANDOM_CONE(RandomInConeRadians, 2.2f*2.2f);
        _XO_TEST_CONE_LT(RandomInConeRadians, HalfPI/2.0f);

        temp = Vector3::RandomOnConeDegrees(Vector3::Up*2.2f, 45.0f);
        temp2 = Vector3::RandomOnConeDegrees(Vector3::Up*2.2f, 45.0f);
        _XO_TEST_RANDOM_CONE(RandomOnConeDegrees, 2.2f * 2.2f);
        _XO_TEST_CONE_EQ(RandomOnConeDegrees, 45.0f);

        Vector3::RandomOnConeDegrees(Vector3::Up, 45.0f, temp);
        Vector3::RandomOnConeDegrees(Vector3::Up, 45.0f, temp2);
        _XO_TEST_RANDOM_CONE(RandomOnConeDegrees, 1.0f);
        _XO_TEST_CONE_EQ(RandomOnConeDegrees, 45.0f);

        Vector3::RandomOnConeDegrees(Vector3::Up * 2.2f, 45.0f, temp);
        Vector3::RandomOnConeDegrees(Vector3::Up * 2.2f, 45.0f, temp2);
        _XO_TEST_RANDOM_CONE(RandomOnConeDegrees, 2.2f * 2.2f);
        _XO_TEST_CONE_EQ(RandomOnConeDegrees, 45.0f);

        temp = Vector3::Up.RandomOnConeDegrees(45.0f);
        temp2 = Vector3::Up.RandomOnConeDegrees(45.0f);
        _XO_TEST_RANDOM_CONE(RandomOnConeDegrees, 1.0f);
        _XO_TEST_CONE_EQ(RandomOnConeDegrees, 45.0f);

        temp = (Vector3::Up * 2.2f).RandomOnConeDegrees(45.0f);
        temp2 = (Vector3::Up * 2.2f).RandomOnConeDegrees(45.0f);
        _XO_TEST_RANDOM_CONE(RandomOnConeDegrees, 2.2f * 2.2f);
        _XO_TEST_CONE_EQ(RandomOnConeDegrees, 45.0f);

        temp = Vector3::RandomInConeDegrees(Vector3::Up, 45.0f);
        temp2 = Vector3::RandomInConeDegrees(Vector3::Up, 45.0f);
        _XO_TEST_RANDOM_CONE(RandomInConeDegrees, 1.0f);
        _XO_TEST_CONE_LT(RandomInConeDegrees, 45.0f);

        temp = Vector3::RandomInConeDegrees(Vector3::Up * 2.2f, 45.0f);
        temp2 = Vector3::RandomInConeDegrees(Vector3::Up * 2.2f, 45.0f);
        _XO_TEST_RANDOM_CONE(RandomInConeDegrees, 2.2f*2.2f);
        _XO_TEST_CONE_LT(RandomInConeDegrees, 45.0f);

        Vector3::RandomInConeDegrees(Vector3::Up, 45.0f, temp);
        Vector3::RandomInConeDegrees(Vector3::Up, 45.0f, temp2);
        _XO_TEST_RANDOM_CONE(RandomInConeDegrees, 1.0f);
        _XO_TEST_CONE_LT(RandomInConeDegrees, 45.0f);

        Vector3::RandomInConeDegrees(Vector3::Up*2.2f, 45.0f, temp);
        Vector3::RandomInConeDegrees(Vector3::Up*2.2f, 45.0f, temp2);
        _XO_TEST_RANDOM_CONE(RandomInConeDegrees, 2.2f*2.2f);
        _XO_TEST_CONE_LT(RandomInConeDegrees, 45.0f);

        temp = Vector3::Up.RandomInConeDegrees(45.0f);
        temp2 = Vector3::Up.RandomInConeDegrees(45.0f);
        _XO_TEST_RANDOM_CONE(RandomInConeDegrees, 1.0f);
        _XO_TEST_CONE_LT(RandomInConeDegrees, 45.0f);

        temp = (Vector3::Up*2.2f).RandomInConeDegrees(45.0f);
        temp2 = (Vector3::Up*2.2f).RandomInConeDegrees(45.0f);
        _XO_TEST_RANDOM_CONE(RandomInConeDegrees, 2.2f * 2.2f);
        _XO_TEST_CONE_LT(RandomInConeDegrees, 45.0f);

        temp = Vector3::RandomOnSphere();
        temp2 = Vector3::RandomOnSphere();
        _XO_TEST_RANDOM_SPHERE(RandomOnSphere);
        _XO_TEST_RANDOM_SPHERE_EQ(RandomOnSphere, 1.0f);

        Vector3::RandomOnSphere(temp);
        Vector3::RandomOnSphere(temp2);
        _XO_TEST_RANDOM_SPHERE(RandomOnSphere);
        _XO_TEST_RANDOM_SPHERE_EQ(RandomOnSphere, 1.0f);

        temp = Vector3::RandomInSphere();
        temp2 = Vector3::RandomInSphere();
        _XO_TEST_RANDOM_SPHERE(RandomInSphere);
        _XO_TEST_RANDOM_SPHERE_LT(RandomInSphere, 1.0f);

        Vector3::RandomInSphere(temp);
        Vector3::RandomInSphere(temp2);
        _XO_TEST_RANDOM_SPHERE(RandomInSphere);
        _XO_TEST_RANDOM_SPHERE_LT(RandomInSphere, 1.0f);

        temp = Vector3::RandomAtDistance(10.0f);
        temp2 = Vector3::RandomAtDistance(10.0f);
        _XO_TEST_RANDOM_SPHERE(RandomAtDistance);
        _XO_TEST_RANDOM_SPHERE_EQ(RandomAtDistance, 10.0f);

        Vector3::RandomAtDistance(10.0f, temp);
        Vector3::RandomAtDistance(10.0f, temp2);
        _XO_TEST_RANDOM_SPHERE(RandomAtDistance);
        _XO_TEST_RANDOM_SPHERE_EQ(RandomAtDistance, 10.0f);

        temp = Vector3::RandomInDistance(10.0f);
        temp2 = Vector3::RandomInDistance(10.0f);
        _XO_TEST_RANDOM_SPHERE(RandomInDistance);
        _XO_TEST_RANDOM_SPHERE_LT(RandomInDistance, 10.0f);

        Vector3::RandomInDistance(10.0f, temp);
        Vector3::RandomInDistance(10.0f, temp2);
        _XO_TEST_RANDOM_SPHERE(RandomInDistance);
        _XO_TEST_RANDOM_SPHERE_LT(RandomInDistance, 10.0f);

        temp = Vector3::RandomInRange(5.0f, 10.0f);
        temp2 = Vector3::RandomInRange(5.0f, 10.0f);
        _XO_TEST_RANDOM_SPHERE(RandomInRange);
        _XO_TEST_RANDOM_SPHERE_LT_GT(RandomInRange, 5.0f, 10.0f);

        Vector3::RandomInRange(5.0f, 10.0f, temp);
        Vector3::RandomInRange(5.0f, 10.0f, temp2);
        _XO_TEST_RANDOM_SPHERE(RandomInRange);
        _XO_TEST_RANDOM_SPHERE_LT_GT(RandomInRange, 5.0f, 10.0f);

#undef _XO_TEST_RANDOM_SPHERE
#undef _XO_TEST_RANDOM_SPHERE_EQ
#undef _XO_TEST_RANDOM_SPHERE_LT
#undef _XO_TEST_RANDOM_SPHERE_LT_GT
#undef _XO_TEST_RANDOM_CONE
#undef _XO_TEST_CONE_LT

        test.ReportSuccessIf(Vector3::Up.AngleRadians(Vector3::Right), HalfPI, TEST_MSG("Up and right should be pi/2 appart"));
        test.ReportSuccessIf(Vector3::Up.AngleRadians(Vector3::Left), HalfPI, TEST_MSG("Up and left should be pi/2 appart"));
        test.ReportSuccessIf(Vector3::AngleRadians(Vector3::Up, Vector3::Right), HalfPI, TEST_MSG("Up and right should be pi/2 appart"));
        test.ReportSuccessIf(Vector3::Up.AngleRadians(Vector3::Down), PI, TEST_MSG("Up and down should be pi appart"));
        test.ReportSuccessIf(Vector3::AngleRadians(Vector3::Up, Vector3::Down), PI, TEST_MSG("Up and down should be pi appart"));

        test.ReportSuccessIf(Vector3::Up.AngleDegrees(Vector3::Right), 90.0f, TEST_MSG("Up and right should be 90 degrees appart"));
        test.ReportSuccessIf(Vector3::Up.AngleDegrees(Vector3::Left), 90.0f, TEST_MSG("Up and left should be 90 degrees appart"));
        test.ReportSuccessIf(Vector3::AngleDegrees(Vector3::Up, Vector3::Right), 90.0f, TEST_MSG("Up and right should be 90 degrees appart"));
        test.ReportSuccessIf(Vector3::Up.AngleDegrees(Vector3::Down), 180.0f, TEST_MSG("Up and down should be 180 degrees appart"));
        test.ReportSuccessIf(Vector3::AngleDegrees(Vector3::Up, Vector3::Down), 180.0f, TEST_MSG("Up and down should be 180 degrees appart"));

        test.ReportSuccessIf(Vector3::DistanceSquared(Vector3::One, -Vector3::One), 3.464102f*3.464102f, TEST_MSG("(1,1,1) and (-1,-1,-1) should be 3.464102^2 units apart."));
        test.ReportSuccessIf(Vector3::DistanceSquared(-Vector3::One, Vector3::One), 3.464102f*3.464102f, TEST_MSG("(-1,-1,-1) and (1,1,1) should be 3.464102^2 units apart."));
        test.ReportSuccessIf(Vector3::DistanceSquared(Vector3::One, Vector3::One), 0.0f, TEST_MSG("(1,1,1) and (1,1,1) should be 0 units apart."));
        test.ReportSuccessIf(Vector3::Distance(Vector3::One, -Vector3::One), 3.464102f, TEST_MSG("(1,1,1) and (-1,-1,-1) should be 3.464102 units apart."));
        test.ReportSuccessIf(Vector3::Distance(-Vector3::One, Vector3::One), 3.464102f, TEST_MSG("(-1,-1,-1) and (1,1,1) should be 3.464102 units apart."));
        test.ReportSuccessIf(Vector3::Distance(Vector3::One, Vector3::One), 0.0f, TEST_MSG("(1,1,1) and (1,1,1) should be 0 units apart."));

    });
}

void TestVector4Operators() {
    test("Vector4 Operators", [] {
        using xo::math::Vector4;
        Vector4 left = Vector4(11.1f, -0.2f, -3.33f, -0.04f);
        Vector4 right = Vector4(-1.1f, 2.2f, 0.33f, 44.0f);

        Vector4 tempL(left);

        test.ReportSuccessIf(left[0], 11.1f, TEST_MSG("extracting the x value from a vector did not produce what we expected."));
        test.ReportSuccessIf(left[1], -0.2f, TEST_MSG("extracting the y value from a vector did not produce what we expected."));
        test.ReportSuccessIf(left[2], -3.33f, TEST_MSG("extracting the z value from a vector did not produce what we expected."));
        test.ReportSuccessIf(left[3], -0.04f, TEST_MSG("extracting the z value from a vector did not produce what we expected."));

        tempL[0] = 1.0f;
        tempL[1] = 2.0f;
        tempL[2] = 3.0f;
        tempL[3] = 4.0f;

        test.ReportSuccessIf(tempL[0], 1.0f, TEST_MSG("operator [] failed to set an element."));
        test.ReportSuccessIf(tempL[1], 2.0f, TEST_MSG("operator [] failed to set an element."));
        test.ReportSuccessIf(tempL[2], 3.0f, TEST_MSG("operator [] failed to set an element."));
        test.ReportSuccessIf(tempL[2], 4.0f, TEST_MSG("operator [] failed to set an element."));


#define _XO_BASIC_OP(op, ...) \
            test.ReportSuccessIf(   left op right, Vector4(__VA_ARGS__), TEST_MSG("We expected this vec" #op "vec to be equal to the expected input param."));\
            test.ReportSuccessIfNot(left op right, Vector4(1.0f, 2.0f, 3.0f, 4.0f), TEST_MSG("We expected this vec" #op "vec to be not equal to the expected input param."));

        _XO_BASIC_OP(+, 10.0f, 2.0f, -3.0f, 43.96f);
        _XO_BASIC_OP(-, 12.2f, -2.4f, -3.66f, -44.04f);
        _XO_BASIC_OP(*, -12.21f, -0.44f, -1.0989f, -1.76f);
        _XO_BASIC_OP(/, -10.09090909f, -0.09090909091f, -10.090909091f, -0.0009090909091f);
#undef _XO_BASIC_OP
        
#define _XO_BASIC_OPEQ(op, ...) \
            tempL = left; \
            tempL op right;\
            test.ReportSuccessIf(   tempL, Vector4(__VA_ARGS__), TEST_MSG("We expected this vec" #op "vec to be equal to the expected input param.")); \
            test.ReportSuccessIfNot(tempL, Vector4(1.0f, 2.0f, 3.0f, 4.0f), TEST_MSG("We expected this vec" #op "vec to be not equal to the expected input param."));

        _XO_BASIC_OPEQ(+=, 10.0f, 2.0f, -3.0f, 43.96f);
        _XO_BASIC_OPEQ(-=, 12.2f, -2.4f, -3.66f, -44.04f);
        _XO_BASIC_OPEQ(*=, -12.21f, -0.44f, -1.0989f, -1.76f);
        _XO_BASIC_OPEQ(/=, -10.09090909f, -0.09090909091f, -10.090909091f, -0.0009090909091f);
#undef _XO_BASIC_OPEQ

        test.ReportSuccessIf(-left, Vector4(-11.1f, 0.2f, 3.33f, 0.04f), TEST_MSG("We expected -vec to be equal to the expected input param"));
        test.ReportSuccessIf(~left, Vector4(-0.04f, -3.33f, -0.2f, 11.1f), TEST_MSG("We expected ~vec to be equal to the expected input param"));

#define _XO_BASIC_COMPARE_OP(op, expected) \
            test.ReportSuccessIf(left op right, expected, TEST_MSG("We expected this vec" #op "vec to be equal to the expected input param"));

        _XO_BASIC_COMPARE_OP(<, true);
        _XO_BASIC_COMPARE_OP(>, false);
        _XO_BASIC_COMPARE_OP(<=, true);
        _XO_BASIC_COMPARE_OP(>=, false);
        _XO_BASIC_COMPARE_OP(==, false);
        _XO_BASIC_COMPARE_OP(!=, true);

        test.ReportSuccessIf(left >= left, true, TEST_MSG(">= failed. Left is equal to left."));
        test.ReportSuccessIf(left <= left, true, TEST_MSG("<= failed. Left is equal to left."));
        test.ReportSuccessIf(right <= left, false, TEST_MSG("<= failed. Right should have a larger magnitude than left."));
        test.ReportSuccessIf(left >= right, false, TEST_MSG("<= failed. Right should have a larger magnitude than left."));
        test.ReportSuccessIf(left == left, true, TEST_MSG("== failed. Left should be equal to left."));
        test.ReportSuccessIf(right == right, true, TEST_MSG("== failed. Right should be equal to right."));
        test.ReportSuccessIf(left != left, false, TEST_MSG("!= failed. Left should be equal to left."));
        test.ReportSuccessIf(right != right, false, TEST_MSG("!= failed. Right should be equal to right."));
#undef _XO_BASIC_COMPARE_OP
        
        const float leftKnownMag = 11.590535163879f;
        const float rightKnownMag = 44.069931030273f;

        test.ReportSuccessIf(left < leftKnownMag, false, TEST_MSG("Vec<float failed. Left should be equal to the known magnitude."));
        // Explicitly handling epsilon in cases where it matters.
        // If you need a value larger than epsilon to correct: it's probably wrong.
        test.ReportSuccessIf(left > leftKnownMag+Vector4::Epsilon, false, TEST_MSG("Vec>float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left <= leftKnownMag+Vector4::Epsilon, true, TEST_MSG("Vec<=float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left >= leftKnownMag, true, TEST_MSG("Vec>=float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left < rightKnownMag, true, TEST_MSG("Vec<float failed. Left should be smaller than the right magnitude"));
        test.ReportSuccessIf(left > rightKnownMag, false, TEST_MSG("Vec>float failed. Left should be smaller than the right magnitude"));

        test.ReportSuccessIf(left == leftKnownMag, true, TEST_MSG("Vec==float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left != leftKnownMag, false, TEST_MSG("Vec!=float failed. Left should be equal to the known magnitude."));

        test.ReportSuccessIf(left == rightKnownMag, false, TEST_MSG("Vec==float failed. Left should not be equal to the right magnitude."));
        test.ReportSuccessIf(left != rightKnownMag, true, TEST_MSG("Vec!=float failed. Left should not be equal to the right magnitude."));

#if XO_SSE
        __m128 m = left; // will generate a compiler error if the __m128 cast operator doesn't exist.

        // failure in the following could indicate a problem with the constructor, the cast or the comparison.
        // some debugging by hand would be needed if these fail.
        test.ReportSuccessIf(Vector4(m), left, TEST_MSG("The implicitly casted __m128 was not assigned to correctly."));
        test.ReportSuccessIfNot(Vector4(m), right, TEST_MSG("The implicitly casted __m128 wasn't compared to correctly."));
#endif
    });
}

void TestVector4Methods() {
    test("Vector4 Methods", []{
        using xo::math::Vector4;
        test.ReportSuccessIf(Vector4(1.1f), Vector4(1.1f, 1.1f, 1.1f, 1.1f), TEST_MSG("Constructor (float) did not set all elements."));
        test.ReportSuccessIf(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4::One, TEST_MSG("Constructor (x, y, z, w) did not set all elements."));
        test.ReportSuccessIf(Vector4(Vector4(1.0f, 1.0f, 1.0f, 1.0f)), Vector4::One, TEST_MSG("Copy constructor (Vector3) did not copy as expected."));
#if XO_SSE
        __m128 m = _mm_set_ps1(1.1f);
        test.ReportSuccessIf(Vector4(m), Vector4(1.1f, 1.1f, 1.1f, 1.1f), TEST_MSG("Constructor (__m128) did not set all elements."));
#endif
        test.ReportSuccessIf(Vector4(xo::math::Vector3(1.1f, 2.2f, 3.3f)), Vector4(1.1f, 2.2f, 3.3f, 0.0f), TEST_MSG("Constructor(Vector3) didn't set all elements as expected"));
        test.ReportSuccessIf(Vector4(xo::math::Vector2(1.1f, 2.2f)), Vector4(1.1f, 2.2f, 0.0f, 0.0f), TEST_MSG("Constructor(Vector2) didn't set all elements as expected"));

        Vector4 temp;
        test.ReportSuccessIf(temp.Set(1.1f, 2.2f, 3.3f, 4.4f), Vector4(1.1f, 2.2f, 3.3f, 4.4f), TEST_MSG("Set(x, y, z, w) did not set all elements."));
        test.ReportSuccessIf(temp.Set(1.1f), Vector4(1.1f, 1.1f, 1.1f, 1.1f), TEST_MSG("Set(float) did not set all elements."));
        test.ReportSuccessIf(temp.Set(Vector4::One), Vector4::One, TEST_MSG("Set(Vector3) did not set all elements."));

#if XO_SSE
        test.ReportSuccessIf(temp.Set(_mm_set_ps1(1.1f)), Vector4(1.1f, 1.1f, 1.1f, 1.1f), TEST_MSG("Set(__m128) did not set all elements."));
#endif
        temp.Set(1.1f, 2.2f, 3.3f, 4.4f);
        float x, y, z, w;
        temp.Get(x, y, z, w);
        test.ReportSuccessIf(x, 1.1f, TEST_MSG("Get(x, y, z, w) did not extract a correct x value."));
        test.ReportSuccessIf(y, 2.2f, TEST_MSG("Get(x, y, z, w) did not extract a correct y value."));
        test.ReportSuccessIf(z, 3.3f, TEST_MSG("Get(x, y, z, w) did not extract a correct z value."));
        test.ReportSuccessIf(w, 4.4f, TEST_MSG("Get(x, y, z, w) did not extract a correct w value."));

        _MM_ALIGN16 float f[4];
        temp.Get(f);
        test.ReportSuccessIf(f[0], 1.1f, TEST_MSG("Get(f) did not extract a correct x value."));
        test.ReportSuccessIf(f[1], 2.2f, TEST_MSG("Get(f) did not extract a correct y value."));
        test.ReportSuccessIf(f[2], 3.3f, TEST_MSG("Get(f) did not extract a correct z value."));
        test.ReportSuccessIf(f[3], 4.4f, TEST_MSG("Get(f) did not extract a correct w value."));

        test.ReportSuccessIf(temp.Sum(), 11.0f, TEST_MSG("Sum() did not accumulate correctly."));
        
        _XOCONSTEXPR auto knownMagSq = 1.1f*1.1f + 2.2f*2.2f + 3.3f*3.3f + 4.4f*4.4f;
        const auto knownMag = Sqrt(knownMagSq);

        test.ReportSuccessIf(temp.MagnitudeSquared(), knownMagSq, TEST_MSG("Magnitude squared did not match the knownMagSq"));
        test.ReportSuccessIf(temp.Magnitude(), knownMag, TEST_MSG("Magnitude did not match the knownMag"));
        test.ReportSuccessIf(temp.Normalize().Magnitude(), 1.0f, TEST_MSG("Magnitude of vector was not 1 after normalize."));
        test.ReportSuccessIf(temp.Magnitude(), 1.0f, TEST_MSG("Magnitude of vector was not 1 after normalize."));

        temp.Set(1.1f, 2.2f, 3.3f, 4.4f);
        test.ReportSuccessIf(temp.Normalized().Magnitude(), 1.0f, TEST_MSG("Magnitude of vector was not 1 after normalized."));
        test.ReportSuccessIfNot(temp.Magnitude(), 1.0f, TEST_MSG("Magnitude of vector was 1 after normalized."));

        test.ReportSuccessIfNot(temp.IsZero(), TEST_MSG("non zero vector was reported as being zero."));
        test.ReportSuccessIf(Vector4::Zero.IsZero(), TEST_MSG("zero vector was reported as being non zero."));

        test.ReportSuccessIfNot(temp.IsNormalized(), TEST_MSG("non normal vector was reported as being normal."));
        test.ReportSuccessIf(Vector4::UnitX.IsNormalized(), TEST_MSG("normal vector was reported as being not normal."));

        test.ReportSuccessIf(Vector4::Zero.Lerp(Vector4::One*10.0f, 0.0f), Vector4::Zero, TEST_MSG("lerp of 0 should return the left hand."));
        test.ReportSuccessIf(Vector4::Zero.Lerp(Vector4::One*10.0f, 1.0f), Vector4::One*10.0f, TEST_MSG("lerp of 1 should return the right hand."));
        test.ReportSuccessIf(Vector4::Zero.Lerp(Vector4::One*10.0f, 0.5f), Vector4::One*5.0f, TEST_MSG("lerp of 0.5 should return the midpoint."));

        test.ReportSuccessIf(Vector4(1.1f, 2.2f, 3.3f, 4.4f).Dot(Vector4(-1.1f, 2.2f, -3.3f, -4.4f)), -26.62f, TEST_MSG("failed known dot product"));
        test.ReportSuccessIf(Vector4::Dot(Vector4(1.1f, 2.2f, 3.3f, 4.4f), Vector4(-1.1f, 2.2f, -3.3f, -4.4f)), -26.62f, TEST_MSG("failed known dot product"));

        test.ReportSuccessIf(Vector4::DistanceSquared(Vector4::Zero, Vector4::UnitX*10.0f), 10.0f*10.0f, TEST_MSG("failed known distance squared"));
        test.ReportSuccessIf(Vector4::Distance(Vector4::Zero, Vector4::UnitX*10.0f), 10.0f, TEST_MSG("failed known distance"));

        test.ReportSuccessIf(Vector4::Zero.DistanceSquared(Vector4::UnitX*10.0f), 10.0f*10.0f, TEST_MSG("failed known distance squared"));
        test.ReportSuccessIf(Vector4::Zero.Distance(Vector4::UnitX*10.0f), 10.0f, TEST_MSG("failed known distance"));
    });  
}

int main() {

#if defined(XO_SSE)
    xo::math::sse::ThrowNoExceptions();
#endif
    cout << XO_MATH_COMPILER_INFO << endl;

#if defined(XO_SSE)
    xo::math::sse::GetAllMXCSRInfo(cout);
#endif

    cout.precision(12);
    cout << std::fixed;

    TestVector2Operators();
    TestVector2Methods();
    TestVector3Operators();
    TestVector3Methods();
    TestVector4Operators();
    TestVector4Methods();

#if defined(_MSC_VER)
    system("pause");
#endif

    return test.GetTotalFailures();
}
