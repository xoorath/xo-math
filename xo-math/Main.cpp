#include <iostream>
#include <cmath>
using namespace std;

#include "../xo-math.h"
using namespace xo::math;

#define XO_TEST_CLOSE 1
#include "Test.h"
#undef XO_TEST_CLOSE

#include "Reference/bullet3-master/src/LinearMath/btVector3.h"

Test test;

void ShouldEqual(const Vector3& xov, const btVector3& btv, const char* name, float epsilon) {
    float diffx = Abs(xov.x - btv.getX());
    float diffy = Abs(xov.y - btv.getY());
    float diffz = Abs(xov.z - btv.getZ());
    float worst = diffx > diffy ? (diffx > diffz ? diffx : diffz) : (diffy > diffz ? diffy : diffz);
    test.ReportSuccessIf(worst <= epsilon, name);
}

void ShouldEqual(bool xob, bool btv, const char* name) {
    test.ReportSuccessIf(xob == btv, name);
}

void CompareBullet3() {
    test("Vecotr3 basic math operations", [] {
        for (int i = 0; i < 100; i++) {
            float x1 = RandomRange(-10000.0f, 10000.0f);
            float y1 = RandomRange(-10000.0f, 10000.0f);
            float z1 = RandomRange(-10000.0f, 10000.0f);

            float x2 = RandomRange(-10000.0f, 10000.0f);
            float y2 = RandomRange(-10000.0f, 10000.0f);
            float z2 = RandomRange(-10000.0f, 10000.0f);
            Vector3 xov1    = { x1, y1, z1 };
            btVector3 btv1  = { x1, y1, z1 };
            Vector3 xov2    = { x2, y2, z2 };
            btVector3 btv2  = { x2, y2, z2 };

            ShouldEqual(xov1 + xov2, btv1 + btv2, "addition", FloatEpsilon);
            ShouldEqual(xov1 - xov2, btv1 - btv2, "subtraction", FloatEpsilon);
            ShouldEqual(xov1 * xov2, btv1 * btv2, "multiplication", FloatEpsilon);
            ShouldEqual(xov1 / xov2, btv1 / btv2, "division", SSE::SSEFloatEpsilon); // failing with an error level too far from acceptable.
        }
    });

    test("Vecotr3 addative math operations", [] {
        for (int i = 0; i < 100; i++) {
            float x1 = RandomRange(-10000.0f, 10000.0f);
            float y1 = RandomRange(-10000.0f, 10000.0f);
            float z1 = RandomRange(-10000.0f, 10000.0f);

            float x2 = RandomRange(-10000.0f, 10000.0f);
            float y2 = RandomRange(-10000.0f, 10000.0f);
            float z2 = RandomRange(-10000.0f, 10000.0f);
            Vector3 xov1 = { x1, y1, z1 };
            btVector3 btv1 = { x1, y1, z1 };
            Vector3 xov2 = { x2, y2, z2 };
            btVector3 btv2 = { x2, y2, z2 };

            Vector3 xov0 = xov1;
            btVector3 btv0 = btv1;

            ShouldEqual(xov1 += xov2, btv1 += btv2, "addition", FloatEpsilon);
            xov1 = xov0;
            btv1 = btv0;
            ShouldEqual(xov1 -= xov2, btv1 -= btv2, "subtraction", FloatEpsilon);
            xov1 = xov0;
            btv1 = btv0;
            ShouldEqual(xov1 *= xov2, btv1 *= btv2, "multiplication", FloatEpsilon);
            
            xov1 = xov0;
            btv1 = btv0;
            // Note: Bullet lacks vec3/=vec3. 
            // Let's just compare vec3/vec3 for accuracy.
            ShouldEqual(xov1 /= xov2, btv1 / btv2, "division", SSE::SSEFloatEpsilon);
        }
    });

    test("Vecotr3 comparison operations", [] {
        for (int i = 0; i < 100; i++) {
            float x1 = RandomRange(-10000.0f, 10000.0f);
            float y1 = RandomRange(-10000.0f, 10000.0f);
            float z1 = RandomRange(-10000.0f, 10000.0f);

            float x2 = RandomRange(-10000.0f, 10000.0f);
            float y2 = RandomRange(-10000.0f, 10000.0f);
            float z2 = RandomRange(-10000.0f, 10000.0f);
            Vector3 xov1 = { x1, y1, z1 };
            btVector3 btv1 = { x1, y1, z1 };
            Vector3 xov2 = { x2, y2, z2 };
            btVector3 btv2 = { x2, y2, z2 };

            // Huge fail, bullet lets you check <, >, <= and >=, but all of them compare pointers?! 
            // I get that comparing magnitude is subjectively useful, but at least it's sensible.
            //ShouldEqual(xov1 < xov2, btv1 < btv2, "less than");
            //ShouldEqual(xov1 <= xov2, btv1 <= btv2, "less than or equal");
            //ShouldEqual(xov1 > xov2, btv1 > btv2, "greater than");
            //ShouldEqual(xov1 >= xov2, btv1 >= btv2, "greater than or equal");

            ShouldEqual(xov1 == xov2, btv1 == btv2, "equal (other vec)");
            ShouldEqual(xov1 != xov2, btv1 != btv2, "not equal (other vec)");

            ShouldEqual(xov1 == xov1, btv1 == btv1, "equal (same vec)");
            ShouldEqual(xov1 != xov1, btv1 != btv1, "not equal (same vec)");
        }
    });
}

int main() {
    SSE::UpdateControlWord();       // updates the thread-local state.
    SSE::SetDenormalsAreZero(true); // force all denormal values to 0
    SSE::SetFlushToZero(true);      // underflowing operations produce 0

    //xo_sse1::SSE::ThrowAllExceptions();
    SSE::ThrowNoExceptions();

    CompareBullet3();

    SSE::GetAllMXCSRInfo(cout);
    cout << "\n" << XO_MATH_VERSION_TXT << "\n" << XO_MATH_COMPILER_INFO << endl;

#if defined(_MSC_VER)
    system("pause");
#endif

    return test.GetTotalFailures();
}