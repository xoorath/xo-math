#include <vector>
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;
#include "../xo-math.h"
using namespace xo::math;

#include "xo-test.h"

Test test;


#define TEST_MSG(x)  "main.cpp (" _XO_MATH_STRINGIFY(__LINE__) ")" x

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
        test.ReportSuccessIf(Vector3(m), left, "The implicitly casted __m128 was not assigned to correctly.");
        test.ReportSuccessIfNot(Vector3(m), right, "The implicitly casted __m128 wasn't compared to correctly.");
#endif
    });
}

void TestVector3Methods() {
    test("Vector3 Methods", []{
        test.ReportSuccessIf(Vector3(1.1f), Vector3(1.1f, 1.1f, 1.1f), "Constructor (float) did not set all elements.");
        test.ReportSuccessIf(Vector3(1.0f, 1.0f, 1.0f), Vector3::One, "Constructor (x, y, z) did not set all elements.");
        test.ReportSuccessIf(Vector3(Vector3(1.0f, 1.0f, 1.0f)), Vector3::One, "Copy constructor (Vector3) did not copy as expected.");
#if XO_SSE
        __m128 m = _mm_set_ps1(1.1f);
        test.ReportSuccessIf(Vector3(m), Vector3(1.1f, 1.1f, 1.1f), "Constructor (__m128) did not set all elements.");
#endif
    });
}

int main() {
    TestVector3Operators();
    TestVector3Methods();

#if defined(_MSC_VER)
    system("pause");
#endif
    return 0;
}