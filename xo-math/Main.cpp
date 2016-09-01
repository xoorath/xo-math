#include <vector>
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;
#include "../xo-math.h"
using namespace xo::math;

#include "xo-test.h"

Test test;

#define TEST_MSG(x)  "main.cpp(" _XO_MATH_STRINGIFY(__LINE__) ") " x

void TestVector3Operators() {
    test("Vector3 Operators", []{
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
            test.ReportSuccessIf(   left op right, Vector3(__VA_ARGS__), TEST_MSG("We expected this vec3" #op "vec3 to be equal to the expected input param."));\
            test.ReportSuccessIfNot(left op right, Vector3(1.0f, 2.0f, 3.0f), TEST_MSG("We expected this vec" #op "vec to be not equal to the expected input param."));

        _XO_BASIC_OP(+, 10.0f, 2.0f, -3.0f);
        _XO_BASIC_OP(-, -12.2f, 2.4f, 3.66f);
        _XO_BASIC_OP(*, -12.21f, -0.44f, -1.0989f);
        _XO_BASIC_OP(/, -0.0990990991f, -11.0f, -0.0990990991f);
#undef _XO_BASIC_OP
        
#define _XO_BASIC_OPEQ(op, ...) \
            tempL = left; \
            tempL op right;\
            test.ReportSuccessIf(   tempL, Vector3(__VA_ARGS__), TEST_MSG("We expected this vec3" #op "vec3 to be equal to the expected input param.")); \
            test.ReportSuccessIfNot(tempL, Vector3(1.0f, 2.0f, 3.0f), TEST_MSG("We expected this vec" #op "vec to be not equal to the expected input param."));

        _XO_BASIC_OPEQ(+=, 10.0f, 2.0f, -3.0f);
        _XO_BASIC_OPEQ(-=, -12.2f, 2.4f, 3.66f);
        _XO_BASIC_OPEQ(*=, -12.21f, -0.44f, -1.0989f);
        _XO_BASIC_OPEQ(/=, -0.0990990991f, -11.0f, -0.0990990991f);
#undef _XO_BASIC_OPEQ

        test.ReportSuccessIf(-left, Vector3(1.1f, -2.2f, -0.33f), TEST_MSG("We expected -vec3 to be equal to the expected input param"));
        test.ReportSuccessIf(~left, Vector3(0.33f, 2.2f, -1.1f), TEST_MSG("We expected ~vec3 to be equal to the expected input param"));

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

        test.ReportSuccessIf(left < leftKnownMag, false, TEST_MSG("Vector3<float failed. Left should be equal to the known magnitude."));
        // Explicitly handling epsilon in cases where it matters.
        // If you need a value larger than epsilon to correct: it's probably wrong.
        test.ReportSuccessIf(left > leftKnownMag+FloatEpsilon, false, TEST_MSG("Vector3>float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left <= leftKnownMag+FloatEpsilon, true, TEST_MSG("Vector3<=float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left >= leftKnownMag, true, TEST_MSG("Vector3>=float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left < rightKnownMag, true, TEST_MSG("Vector3<float failed. Left should be smaller than the right magnitude"));
        test.ReportSuccessIf(left > rightKnownMag, false, TEST_MSG("Vector3>float failed. Left should be smaller than the right magnitude"));

        test.ReportSuccessIf(left == leftKnownMag, true, TEST_MSG("Vector3==float failed. Left should be equal to the known magnitude."));
        test.ReportSuccessIf(left != leftKnownMag, false, TEST_MSG("Vector3!=float failed. Left should be equal to the known magnitude."));

        test.ReportSuccessIf(left == rightKnownMag, false, TEST_MSG("Vector3==float failed. Left should not be equal to the right magnitude."));
        test.ReportSuccessIf(left != rightKnownMag, true, TEST_MSG("Vector3!=float failed. Left should not be equal to the right magnitude."));

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
        test.ReportSuccessIf(Vector3(1.1f), Vector3(1.1f, 1.1f, 1.1f), TEST_MSG("Constructor (float) did not set all elements."));
        test.ReportSuccessIf(Vector3(1.0f, 1.0f, 1.0f), Vector3::One, TEST_MSG("Constructor (x, y, z) did not set all elements."));
        test.ReportSuccessIf(Vector3(Vector3(1.0f, 1.0f, 1.0f)), Vector3::One, TEST_MSG("Copy constructor (Vector3) did not copy as expected."));
#if XO_SSE
        __m128 m = _mm_set_ps1(1.1f);
        test.ReportSuccessIf(Vector3(m), Vector3(1.1f, 1.1f, 1.1f), TEST_MSG("Constructor (__m128) did not set all elements."));
#endif
        test.ReportSuccessIf(Vector3(Vector4(1.1f, 2.2f, 3.3f, 4.4f)), Vector3(1.1f, 2.2f, 3.3f), TEST_MSG("Constructor(Vector4) didn't set all elements as expected"));
        test.ReportSuccessIf(Vector3(Vector2(1.1f, 2.2f)), Vector3(1.1f, 2.2f, 0.0f), TEST_MSG("Constructor(Vector2) didn't set all elements as expected"));

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

        _MM_ALIGN16 float f[3];
        temp.Get(f);
        test.ReportSuccessIf(f[0], 1.1f, TEST_MSG("Get(f) did not extract a correct x value."));
        test.ReportSuccessIf(f[1], 2.2f, TEST_MSG("Get(f) did not extract a correct y value."));
        test.ReportSuccessIf(f[2], 3.3f, TEST_MSG("Get(f) did not extract a correct z value."));

        test.ReportSuccessIf(temp.ZYX(), Vector3(3.3f, 2.2f, 1.1f), TEST_MSG("ZYX() did not swizzle correctly."));
        test.ReportSuccessIf(temp.Sum(), 6.6f, TEST_MSG("Sum() did not accumulate correctly."));
        
        constexpr auto knownMagSq = 1.1f*1.1f + 2.2f*2.2f + 3.3f*3.3f;
        const auto knownMag = Sqrt(knownMagSq);

        test.ReportSuccessIf(temp.MagnitudeSquared(), knownMagSq, TEST_MSG("Magnitude squared did not match the knownMagSq"));
        test.ReportSuccessIf(temp.Magnitude(), knownMag, TEST_MSG("Magnitude did not match the knownMag"));

        float knownNormalization[3] = {0.267261, 0.534523, 0.801784};
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
        

        test.ReportSuccessIf(Vector3::Max(temp, temp2), temp2, TEST_MSG("Max of two vectors is not as expected."));
        test.ReportSuccessIf(Vector3::Min(temp, temp2), temp, TEST_MSG("Min of two vectors is not as expected."));
        test.ReportSuccessIf(Vector3::Max(temp2, temp), temp2, TEST_MSG("Max of two vectors is not as expected."));
        test.ReportSuccessIf(Vector3::Min(temp2, temp), temp, TEST_MSG("Min of two vectors is not as expected."));

        Vector3::Max(temp, temp2, temp3);
        test.ReportSuccessIf(temp3, temp2, TEST_MSG("Max of two vectors is not as expected."));
        Vector3::Min(temp, temp2, temp3);
        test.ReportSuccessIf(temp3, temp, TEST_MSG("Max of two vectors is not as expected."));

#define _XO_LERP(t, expected, msg) \
            test.ReportSuccessIf(Vector3::Lerp(temp, temp2, t), expected, TEST_MSG(msg)); \
            Vector3::Lerp(temp, temp2, t, temp3); \
            test.ReportSuccessIf(temp3, expected, TEST_MSG(msg)); \
            test.ReportSuccessIf(temp.Lerp(temp2, t), expected, TEST_MSG(msg));

        _XO_LERP(1.0f, temp2, "Lerp of 1.0 didnt return the right param.");
        _XO_LERP(0.0f, temp, "Lerp of 0.0 didnt return the left param.");
        _XO_LERP(0.5f, Vector3(2.75f, 3.85f, 4.95f), "Lerp of 0.5 didnt return the half way between left and right params.");

#undef _XO_LERP

        


    });
}

int main() {

#if defined(XO_SSE)
    SSE::ThrowNoExceptions();
#endif
    cout << XO_MATH_COMPILER_INFO << endl;

#if defined(XO_SSE)
    //SSE::GetAllMXCSRInfo(cout);
#endif

    TestVector3Operators();
    TestVector3Methods();

#if defined(_MSC_VER)
    system("pause");
#endif
    return 0;
}
