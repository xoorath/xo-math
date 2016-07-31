#include <iostream>
#include <chrono>
#include <ctime>

#define XO_NO_NS 1
//#define XO_NO_SIMD 1              // Turn off SSE instructions
//#define XO_NO_FAST 1              // Turn off __vectorcall
//#define XO_NO_INLINE 1            // Turn off __forceinline
//#define XO_NO_INVERSE_DIVISION 1  // Turn off (1/x)*Vec in place of (x,x,x)/Vec
#include "../GameMath.h"


#ifdef XO_NO_SIMD
#   undef XO_NO_SIMD
#endif
#ifdef XO_NO_NS
#   undef XO_NO_NS
#endif
#ifdef XO_NO_FAST
#   undef XO_NO_FAST
#endif
#ifdef XO_NO_INLINE
#   undef XO_NO_INLINE
#endif


#include "Test.h"

void TestPerformance() {
    typedef std::chrono::time_point<std::chrono::system_clock> TTime;
    typedef std::chrono::duration<double> TDuration;

    const int itterations = 8000;
    const int vecCount = 4096;
    Vector3 arr[vecCount];
    int opIndex[vecCount];
    srand((unsigned)time(nullptr));
    for (int i = 0; i < vecCount; ++i) {
        // give us random non-zero vectors
        arr[i] = Vector3(
            (((float)rand() / (float)RAND_MAX)*1000.0f) + 1.0f,
            (((float)rand() / (float)RAND_MAX)*1000.0f) + 1.0f,
            (((float)rand() / (float)RAND_MAX)*1000.0f) + 1.0f);
        opIndex[i] = rand() % (vecCount - 1);
    }

    TTime start, end;
    start = std::chrono::system_clock::now();
    for (int i = 0; i < itterations; ++i) {
        for (int j = 0; j < vecCount; ++j) {
            arr[j] /= arr[opIndex[j]].x;
            arr[j] /= arr[opIndex[j]].y;
            arr[j] /= arr[opIndex[j]].z;
            arr[j] /= arr[opIndex[j]].x;
            arr[j] /= arr[opIndex[j]].y;
            arr[j] /= arr[opIndex[j]].z;
            arr[j] /= arr[opIndex[j]].x;
            arr[j] /= arr[opIndex[j]].y;
            arr[j] /= arr[opIndex[j]].z;
            arr[j] /= arr[opIndex[j]].x;
            arr[j] /= arr[opIndex[j]].y;
            arr[j] /= arr[opIndex[j]].z;
        }
    }
    end = std::chrono::system_clock::now();
    TDuration seconds = end - start;

    std::cout << "Perf tests took (" << seconds.count() << ") seconds. \n\n";
}

#define RUN_TESTS 1
#define RUN_PERF_TESTS 0
int main() {
#if RUN_TESTS
    Test t;

    // set to one when testing, increase when finding speed of tests.
    const int itterations = 1;
    double time = 0.0f;
    time += t("Vector3: Basic assignment", [&t, &itterations] {
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

    time += t("Vector3: Basic math", [&t, &itterations] {
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

    time += t("Vector3: Rotations", [&t, &itterations] {
        for (int i = 0; i < itterations; ++i) {
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
        }
    });

    time += t("Vector3: Magnitude", [&t, &itterations] {
        for (int i = 0; i < itterations; ++i) {
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
            VerifyMagnitude(Vector3::One, 1.7320508075688772f);
            VerifyMagnitude((Vector3::Zero + 1.0f), 1.7320508075688772f);
        #undef VerifyMagnitude
        }
    });

    time += t("Vector3: Dot", [&t, &itterations] {
        for (int i = 0; i < itterations; ++i) {
            t.ReportSuccessIf(Vector3::Up.Dot(Vector3::Right), 0.0f, "Dot product was not as we expected. Up.Right");
            t.ReportSuccessIf(Vector3::Up.Dot(Vector3::Left), 0.0f, "Dot product was not as we expected. Up.Left");
            t.ReportSuccessIf(Vector3::Up.Dot(Vector3::Forward), 0.0f, "Dot product was not as we expected. Up.Forward");
            t.ReportSuccessIf(Vector3::Up.Dot(Vector3::Backward), 0.0f, "Dot product was not as we expected.Up.Backward");
            t.ReportSuccessIf(Vector3::Up.Dot(Vector3::Up), 1.0f, "Dot product was not as we expected. Up.Up");
            t.ReportSuccessIf(Vector3::Up.Dot(Vector3::Down), -1.0f, "Dot product was not as we expected. Up.Down");
            t.ReportSuccessIf(Vector3::Up.Dot(Vector3::Up * 2.0f), 2.0f, "Dot product was not as we expected. Up.(Upx2)");
            t.ReportSuccessIf(Vector3::Up.Dot(Vector3::Down * 2.0f), -2.0f, "Dot product was not as we expected. (Up.Downx2)");

            auto upWithW = Vector3::Up + 1.0f;
            upWithW.x = 0.0f;
            upWithW.y = 1.0f;
            upWithW.z = 0.0f;
            t.ReportSuccessIf(Vector3::Up.Dot(upWithW), 1.0f, "Dot product was not as we expected. Up.(Up with W)");
        }
    });

    time += t("Vector3: Cross", [&t, &itterations] {
        for (int i = 0; i < itterations; ++i) {
            t.ReportSuccessIf(Vector3::Up.Cross(Vector3::Forward), Vector3::Right, "Cross product was not as we expected. upxforward");
            t.ReportSuccessIf(Vector3::Forward.Cross(Vector3::Up), Vector3::Left, "Cross product was not as we expected. forwardxup");

            // todo: double check expectation
            t.ReportSuccessIf(Vector3::Up.Cross(Vector3::Forward*2.0f), Vector3::Right*2.0f, "Cross product was not as we expected. upx(forwardx2)");
        }
    });

    t("Vector3: Heap", [&t, &itterations] {
        for (int i = 0; i < itterations; ++i) {
            Vector3* v3 = new Vector3(1.0f, 2.0f, 3.0f);
            t.ReportSuccessIf(*v3, Vector3(1.0f, 2.0f, 3.0f), "Heap vector does not match expectation.");
            struct WithOffset {
                char offset;
                Vector3 v3;
            };

            WithOffset* s = new WithOffset();
            s->v3.Set(1.0f, 2.0f, 3.0f);

            t.ReportSuccessIf(s->v3, Vector3(1.0f, 2.0f, 3.0f), "Heap vector in offset struct does not match expectation.");

            delete s;
            delete v3;
        }
    });

    std::cout << std::fixed << "Test execution time: " << time << std::endl;
#endif

#if RUN_PERF_TESTS
    TestPerformance();
#endif

#if defined(_MSC_VER)
    system("pause");
#endif

#if RUN_TESTS
    return t.GetTotalFailures();
#else
    return 0;
#endif
}