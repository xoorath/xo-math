#include <vector>
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;
#include "../xo-math.h"
using namespace xo::math;

#include "xo-test.h"

Test test;

void TestVector3Operators() {
    test("Vector3 Operators", []{
        Vector3 left    = Vector3(-1.1f, 2.2f, 0.33f); // mag: 2.48171
        Vector3 right   = Vector3(11.1f, -0.2f, -3.33f); // mag: 11.5905

#define _XO_BASIC_OP(op, ...) \
            test.ReportSuccessIf(   left op right, Vector3(__VA_ARGS__), "We expected this vec3" #op "vec3 to be equal to the expected input param."); \
            test.ReportSuccessIfNot(left op right, Vector3(1.0f, 2.0f, 3.0f), "We expected this vec" #op "vec to be not equal to the expected input param.");

        _XO_BASIC_OP(+, 10.0f, 2.0f, -3.0f);
        _XO_BASIC_OP(-, -12.2f, 2.4f, 3.66f);
        _XO_BASIC_OP(*, -12.21f, -0.44f, -1.0989f);
        _XO_BASIC_OP(/, -0.0990990991f, -11.0f, -0.0990990991f);
#undef _XO_BASIC_OP

        Vector3 tempL;

#define _XO_BASIC_OPEQ(op, ...) \
            tempL = left; \
            tempL op right;\
            test.ReportSuccessIf(   tempL, Vector3(__VA_ARGS__), "We expected this vec3" #op "vec3 to be equal to the expected input param."); \
            test.ReportSuccessIfNot(tempL, Vector3(1.0f, 2.0f, 3.0f), "We expected this vec" #op "vec to be not equal to the expected input param.");

        _XO_BASIC_OPEQ(+=, 10.0f, 2.0f, -3.0f);
        _XO_BASIC_OPEQ(-=, -12.2f, 2.4f, 3.66f);
        _XO_BASIC_OPEQ(*=, -12.21f, -0.44f, -1.0989f);
        _XO_BASIC_OPEQ(/=, -0.0990990991f, -11.0f, -0.0990990991f);
#undef _XO_BASIC_OPEQ

        test.ReportSuccessIf(left[0], -1.1f, "extracting the x value from a vector did not produce what we expected.");
        test.ReportSuccessIf(left[1], 2.2f, "extracting the y value from a vector did not produce what we expected.");
        test.ReportSuccessIf(left[2], 0.33f, "extracting the z value from a vector did not produce what we expected.");

        test.ReportSuccessIf(-left, Vector3(1.1f, -2.2f, -0.33f), "We expected -vec3 to be equal to the expected input param");
        test.ReportSuccessIf(~left, Vector3(0.33f, 2.2f, -1.1f), "We expected ~vec3 to be equal to the expected input param");

#define _XO_BASIC_COMPARE_OP(op, expected) \
            test.ReportSuccessIf(left op right, expected, "We expected this vec" #op "vec to be equal to the expected input param");

            _XO_BASIC_COMPARE_OP(<, true);
            _XO_BASIC_COMPARE_OP(>, false);
#undef _XO_BASIC_COMPARE_OP

    });
}

int main() {

    TestVector3Operators();

#if defined(_MSC_VER)
    system("pause");
#endif
    return 0;
}