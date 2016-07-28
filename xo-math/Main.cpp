#include <iostream>

#define NO_XO_NS 1
#include "../GameMath.h"
#undef NO_XO_NS

#include "Test.h"

int main() {
    Test t;

    const int itterations = 1;

    t("Vector3: Basic assignment", [&t, &itterations] {
        for (int i = 0; i < itterations; ++i) {
            Vector3 v3 = { 1.0f, 2.0f, 3.0f };

            t.ReportSuccessIf(v3.x, 1.0f, "x component does not match initializer list.");
            t.ReportSuccessIf(v3.y, 2.0f, "y component does not match initializer list.");
            t.ReportSuccessIf(v3.z, 3.0f, "z component does not match initializer list.");
            float x, y, z;
            v3.Get(x, y, z);
            t.ReportSuccessIf(x, 1.0f, "x component from Get does not match initializer list.");
            t.ReportSuccessIf(y, 2.0f, "y component from Get does not match initializer list.");
            t.ReportSuccessIf(z, 3.0f, "z component from Get does not match initializer list.");

            struct WithOffset {
                char offset;
                Vector3 v3;
            };

            WithOffset s = { ' ', {1.0f, 2.0f, 3.0f} };

            t.ReportSuccessIf(s.v3.x, 1.0f, "x component does not match initializer list with offset.");
            t.ReportSuccessIf(s.v3.y, 2.0f, "y component does not match initializer list with offset.");
            t.ReportSuccessIf(s.v3.z, 3.0f, "z component does not match initializer list with offset.");
            s.v3.Get(x, y, z);
            t.ReportSuccessIf(x, 1.0f, "x component from Get does not match initializer list with offset.");
            t.ReportSuccessIf(y, 2.0f, "y component from Get does not match initializer list with offset.");
            t.ReportSuccessIf(z, 3.0f, "z component from Get does not match initializer list with offset.");
        }
    });

    t("Vector3: Basic math", [&t, &itterations] {
        for (int i = 0; i < itterations; ++i) {
#define VerifyOperator(op, expectation) t.ReportSuccessIf(Vector3(1.0f, 2.0f, 3.0f) op Vector3(1.0f, 2.0f, 3.0f), expectation, "Failed expectation for operator " #op );
            VerifyOperator(+, Vector3(2.0f, 4.0f, 6.0f));
            VerifyOperator(-, Vector3(0.0f, 0.0f, 0.0f));
            VerifyOperator(*, Vector3(1.0f, 4.0f, 9.0f));
            VerifyOperator(/ , Vector3(1.0f, 1.0f, 1.0f));
#undef VerifyOperator
            {
                Vector3 v3;
#define VerifyOperatorAdd(op, expectation) v3 = {1.0f, 2.0f, 3.0f}; v3 op Vector3(1.0f, 2.0f, 3.0f); t.ReportSuccessIf(v3, expectation, "Failed expectation for operator " #op );
                VerifyOperatorAdd(+=, Vector3(2.0f, 4.0f, 6.0f));
                VerifyOperatorAdd(-=, Vector3(0.0f, 0.0f, 0.0f));
                VerifyOperatorAdd(*=, Vector3(1.0f, 4.0f, 9.0f));
                VerifyOperatorAdd(/=, Vector3(1.0f, 1.0f, 1.0f));
            }
#undef VerifyOperatorAdd
            t.ReportSuccessIf(Vector3(1.0f, 2.0f, 3.0f) + 1.0f, Vector3(2.0f, 3.0f, 4.0f), "Float addition failed.");
            t.ReportSuccessIf(-Vector3(1.0f, 2.0f, 3.0f), Vector3(-1.0f, -2.0f, -3.0f), "Failed expectation for operator -v");
            t.ReportSuccessIf(~Vector3(1.0f, 2.0f, 3.0f), Vector3(3.0f, 2.0f, 1.0f), "Failed expectation for operator ~v");
        }
    });

    t("Vector3: Rotations", [&t, &itterations] {
#define VerifyAngle(from, to, expected) t.ReportSuccessIf(Vector3:: from .AngleDegrees(Vector3:: to ), expected, #from " and " #to " are not at " #expected " degrees");
        VerifyAngle(Up, Right, 90.0f)
        VerifyAngle(Up, Left, 90.0f)
        VerifyAngle(Up, Forward, 90.0f)
        VerifyAngle(Up, Backward, 90.0f)
        VerifyAngle(Down, Right, 90.0f)
        VerifyAngle(Down, Left, 90.0f)
        VerifyAngle(Down, Forward, 90.0f)
        VerifyAngle(Down, Backward, 90.0f)
        // should work at dot -1
        VerifyAngle(Up, Down, 179.999969f)
        // should not be effected by length
        VerifyAngle(Up, Right*2.0f, 90.0f)
#undef VerifyAngle
    });

    t("Vector3: Magnitude", [&t, &itterations] {
#define VerifyMagnitude(v, expected) \
        t.ReportSuccessIf(v .Magnitude(), expected, #v " does not have a magnitude of " #expected ".");\
        t.ReportSuccessIf(v .MagnitudeSquared(), expected * expected, #v " does not have a magnitude squared of " #expected "^2.");

        VerifyMagnitude(Vector3::Up, 1.0f);
        VerifyMagnitude(Vector3::Down, 1.0f);
        VerifyMagnitude(Vector3::Left, 1.0f);
        VerifyMagnitude(Vector3::Right, 1.0f);
        VerifyMagnitude(Vector3::Forward, 1.0f);
        VerifyMagnitude(Vector3::Backward, 1.0f);
        VerifyMagnitude((Vector3::Up*2.0f), 2.0f);
        VerifyMagnitude((Vector3::Down*2.0f), 2.0f);

#undef VerifyMagnitude

    });

    return t.GetTotalFailures();
}