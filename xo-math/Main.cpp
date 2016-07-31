#include <iostream>
#include <chrono>
#include <ctime>

#define XO_NO_NS 1
//#define XO_NO_SIMD 1              // Turn off SSE instructions
//#define XO_NO_FAST 1              // Turn off __vectorcall
//#define XO_NO_INLINE 1            // Turn off __forceinline
//#define XO_NO_INVERSE_DIVISION 1  // Turn off (1/x)*Vec in place of (x,x,x)/Vec
#include "../GameMath.h"

#ifdef XO_NO_NS
#   undef XO_NO_NS
#endif
#ifdef XO_NO_SIMD
#   undef XO_NO_SIMD
#endif
#ifdef XO_NO_FAST
#   undef XO_NO_FAST
#endif
#ifdef XO_NO_INLINE
#   undef XO_NO_INLINE
#endif
#ifdef XO_NO_INVERSE_DIVISION
#   undef XO_NO_INVERSE_DIVISION
#endif

#define XO_TEST_CLOSE 1
#include "Test.h"

#ifdef XO_TEST_CLOSE
#   undef XO_TEST_CLOSE
#endif

void TestVector2(Test& t) {
    t("Vector2 AngleDegrees", [&t] {
        REPORT_SUCCESS_IF(t, Vector2::Up.AngleDegrees(Vector2::Right), -90.0f);
        REPORT_SUCCESS_IF(t, Vector2::Right.AngleDegrees(Vector2::Down), -90.0f);
        REPORT_SUCCESS_IF(t, Vector2::Up.AngleDegrees(Vector2::Down), 180.0f);
    });

    t("Vector2 Dot", [&t] {
        REPORT_SUCCESS_IF(t, Vector2::Up.Dot(Vector2::Up), 1.0f);
        REPORT_SUCCESS_IF(t, Vector2::Up.Dot(Vector2::Right), 0.0f);
        REPORT_SUCCESS_IF(t, Vector2::Up.Dot(Vector2::Down), -1.0f);
        REPORT_SUCCESS_IF(t, Vector2::Up.Dot(Vector2::Up * 2), 2.0f);
    });

    t("Vector2 Orthogonal", [&t] {
        REPORT_SUCCESS_IF(t, Vector2::Up.OrthogonalCW(), Vector2::Right);
        REPORT_SUCCESS_IF(t, Vector2::Up.OrthogonalCCW(), Vector2::Left);
    });

    t("Vector2 Lerp", [&t] {
        REPORT_SUCCESS_IF(t, Vector2::Up.Lerp(Vector2::Up*2.0f, -0.5f), Vector2(0.0f, 0.5f));
        REPORT_SUCCESS_IF(t, Vector2::Up.Lerp(Vector2::Up*2.0f, 0.0f), Vector2(0.0f, 1.0f));
        REPORT_SUCCESS_IF(t, Vector2::Up.Lerp(Vector2::Up*2.0f, 0.5f), Vector2(0.0f, 1.5f));
        REPORT_SUCCESS_IF(t, Vector2::Up.Lerp(Vector2::Up*2.0f, 1.0f), Vector2(0.0f, 2.0f));
        REPORT_SUCCESS_IF(t, Vector2::Up.Lerp(Vector2::Up*2.0f, 1.5f), Vector2(0.0f, 2.5f));
    });

    t("Vector2 Operators", [&t] {
        const Vector2 v2a = { 1.1f, 2.2f };
        const Vector2 v2b = { 3.3f, 4.4f };
        const float fa = 5.5f;
        REPORT_SUCCESS_IF(t, v2a / v2b, Vector2(v2a.x / v2b.x, v2a.y / v2b.y));
        REPORT_SUCCESS_IF(t, v2a + v2b, Vector2(v2a.x + v2b.x, v2a.y + v2b.y));
        REPORT_SUCCESS_IF(t, v2a * v2b, Vector2(v2a.x * v2b.x, v2a.y * v2b.y));
        REPORT_SUCCESS_IF(t, v2a - v2b, Vector2(v2a.x - v2b.x, v2a.y - v2b.y));
        REPORT_SUCCESS_IF(t, v2a / fa, Vector2(v2a.x / fa, v2a.y / fa));
        REPORT_SUCCESS_IF(t, v2a + fa, Vector2(v2a.x + fa, v2a.y + fa));
        REPORT_SUCCESS_IF(t, v2a * fa, Vector2(v2a.x * fa, v2a.y * fa));
        REPORT_SUCCESS_IF(t, v2a - fa, Vector2(v2a.x - fa, v2a.y - fa));

        Vector2 v2c;
        v2c = v2a;  // reset
        v2c /= v2b; // perform operation
        REPORT_SUCCESS_IF(t, v2c, Vector2(v2a.x / v2b.x, v2a.y / v2b.y));
        v2c = v2a;  // reset
        v2c += v2b; // perform operation
        REPORT_SUCCESS_IF(t, v2c, Vector2(v2a.x + v2b.x, v2a.y + v2b.y));
        v2c = v2a;  // reset
        v2c *= v2b; // perform operation
        REPORT_SUCCESS_IF(t, v2c, Vector2(v2a.x * v2b.x, v2a.y * v2b.y));
        v2c = v2a;  // reset
        v2c -= v2b; // perform operation
        REPORT_SUCCESS_IF(t, v2c, Vector2(v2a.x - v2b.x, v2a.y - v2b.y));


        REPORT_SUCCESS_IF(t, ~v2a, Vector2(v2a.y, v2a.x));
        REPORT_SUCCESS_IF(t, -v2a, Vector2(-v2a.x, -v2a.y));

        REPORT_SUCCESS_IF(t, v2a < v2b, true);
        REPORT_SUCCESS_IF(t, v2a <= v2b, true);
        REPORT_SUCCESS_IF(t, v2b > v2a, true);
        REPORT_SUCCESS_IF(t, v2b >= v2a, true);

        REPORT_SUCCESS_IF(t, v2a > 1, true);
        REPORT_SUCCESS_IF(t, v2a < 4, true);
        REPORT_SUCCESS_IF(t, v2a < -4, true); // when checking magnitude, sign is ignored
    });
}

void TestVector3(Test& t) {
    t("Vector3 AngleDegrees", [&t] {
        REPORT_SUCCESS_IF(t, Vector3::Up.AngleDegrees(Vector3::Right), 90.0f);
        REPORT_SUCCESS_IF(t, Vector3::Right.AngleDegrees(Vector3::Down), 90.0f);
        REPORT_SUCCESS_IF(t, Vector3::Up.AngleDegrees(Vector3::Down), 180.0f);
    });

    t("Vector3 Dot", [&t] {
        REPORT_SUCCESS_IF(t, Vector3::Up.Dot(Vector3::Up), 1.0f);
        REPORT_SUCCESS_IF(t, Vector3::Up.Dot(Vector3::Right), 0.0f);
        REPORT_SUCCESS_IF(t, Vector3::Up.Dot(Vector3::Down), -1.0f);
        REPORT_SUCCESS_IF(t, Vector3::Up.Dot(Vector3::Up * 2), 2.0f);
    });

    t("Vector3 Cross", [&t] {
        REPORT_SUCCESS_IF(t, Vector3::Up.Cross(Vector3::Forward), Vector3::Right);
        REPORT_SUCCESS_IF(t, Vector3::Forward.Cross(Vector3::Up), Vector3::Left);
    });

    t("Vector3 Lerp", [&t] {
        REPORT_SUCCESS_IF(t, Vector3::Up.Lerp(Vector3::Up*2.0f, -0.5f), Vector3(0.0f, 0.5f, 0.0f));
        REPORT_SUCCESS_IF(t, Vector3::Up.Lerp(Vector3::Up*2.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
        REPORT_SUCCESS_IF(t, Vector3::Up.Lerp(Vector3::Up*2.0f, 0.5f), Vector3(0.0f, 1.5f, 0.0f));
        REPORT_SUCCESS_IF(t, Vector3::Up.Lerp(Vector3::Up*2.0f, 1.0f), Vector3(0.0f, 2.0f, 0.0f));
        REPORT_SUCCESS_IF(t, Vector3::Up.Lerp(Vector3::Up*2.0f, 1.5f), Vector3(0.0f, 2.5f, 0.0f));
    });

    t("Vector3 Operators", [&t] {
        const Vector3 v2a = { 1.1f, 2.2f, 3.3f };
        const Vector3 v2b = { 4.4f, 5.5f, 6.6f };
        const float fa = 7.7f;
        REPORT_SUCCESS_IF(t, v2a / v2b, Vector3(v2a.x / v2b.x, v2a.y / v2b.y, v2a.z / v2b.z));
        REPORT_SUCCESS_IF(t, v2a + v2b, Vector3(v2a.x + v2b.x, v2a.y + v2b.y, v2a.z + v2b.z));
        REPORT_SUCCESS_IF(t, v2a * v2b, Vector3(v2a.x * v2b.x, v2a.y * v2b.y, v2a.z * v2b.z));
        REPORT_SUCCESS_IF(t, v2a - v2b, Vector3(v2a.x - v2b.x, v2a.y - v2b.y, v2a.z - v2b.z));
        REPORT_SUCCESS_IF(t, v2a / fa, Vector3(v2a.x / fa, v2a.y / fa, v2a.z / fa));
        REPORT_SUCCESS_IF(t, v2a + fa, Vector3(v2a.x + fa, v2a.y + fa, v2a.z + fa));
        REPORT_SUCCESS_IF(t, v2a * fa, Vector3(v2a.x * fa, v2a.y * fa, v2a.z * fa));
        REPORT_SUCCESS_IF(t, v2a - fa, Vector3(v2a.x - fa, v2a.y - fa, v2a.z - fa));

        Vector3 v2c;
        v2c = v2a;  // reset
        v2c /= v2b; // perform operation
        REPORT_SUCCESS_IF(t, v2c, Vector3(v2a.x / v2b.x, v2a.y / v2b.y, v2a.z / v2b.z));
        v2c = v2a;  // reset
        v2c += v2b; // perform operation
        REPORT_SUCCESS_IF(t, v2c, Vector3(v2a.x + v2b.x, v2a.y + v2b.y, v2a.z + v2b.z));
        v2c = v2a;  // reset
        v2c *= v2b; // perform operation
        REPORT_SUCCESS_IF(t, v2c, Vector3(v2a.x * v2b.x, v2a.y * v2b.y, v2a.z * v2b.z));
        v2c = v2a;  // reset
        v2c -= v2b; // perform operation
        REPORT_SUCCESS_IF(t, v2c, Vector3(v2a.x - v2b.x, v2a.y - v2b.y, v2a.z - v2b.z));


        REPORT_SUCCESS_IF(t, ~v2a, Vector3(v2a.z, v2a.y, v2a.x));
        REPORT_SUCCESS_IF(t, -v2a, Vector3(-v2a.x, -v2a.y, -v2a.z));

        REPORT_SUCCESS_IF(t, v2a < v2b, true);
        REPORT_SUCCESS_IF(t, v2a <= v2b, true);
        REPORT_SUCCESS_IF(t, v2b > v2a, true);
        REPORT_SUCCESS_IF(t, v2b >= v2a, true);

        REPORT_SUCCESS_IF(t, v2a > 1, true);
        REPORT_SUCCESS_IF(t, v2a < 8, true);
        REPORT_SUCCESS_IF(t, v2a < -8, true); // when checking magnitude, sign is ignored
    });
}

int main() {
 
    Test t;

    TestVector2(t);
    TestVector3(t);

#if defined(_MSC_VER)
    system("pause");
#endif

    return t.GetTotalFailures();
}