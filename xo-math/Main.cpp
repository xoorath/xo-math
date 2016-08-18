#include <iostream>
#include <chrono>
#include <ctime>

//#define XOMATH_VEC2_CW 1            // All 2d math using relative angles will assume positive to be clockwise, and that rotations start at (0, 1)
#define XO_NO_NS 1                  // Turn off all namespace options for xomath
//#define XO_NO_INLINE 1            // Turn off __forceinline
//#define XO_NO_INVERSE_DIVISION 1  // Turn off (1/x)*Vec in place of (x,x,x)/Vec

//////////////////////////////////////////////////////////// In use
// Allow SSE (1999) Intel Katmai, (2001) AMD Palomino
#define XO_SSE 1
// Allow SSE2 (2001) Intel Willamette, (2003) AMD Opteron
#define XO_SSE2 1
// Allow SSE3 (2004) Intel Prescott, (2005) AMD Venice/San Diego
#define XO_SSE3 1
// Allow SSE4 (2008) Intel Nehalem, (2011) AMD Bulldozer, Playstation 4, Xbox One
#define XO_SSE4 1
//////////////////////////////////////////////////////////// Not yet used
// Allow SSSE3 (2006) Intel Woodcrest, (2011) AMD Bobcat
#define XO_SSSE3 1
// Allow AVX (2011) Intel Sandy Bridge, (2011) AMD Bulldozer
#define XO_AVX 1
// Allow AVX2 (2013) Intel Haswell, (2015) AMD Excavator, Playstation 4, Xbox One
#define XO_AVX2 1
// Allow AVX512 (2016) Intel Knights Landing
#define XO_AVX512 1

#include "../xo-math.h"

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

template<class functype>
struct ExecutionComparison {
    const char* LeftName;
    const char* RightName;
    std::function<functype> LeftFunc;
    std::function<functype> RightFunc;
};

template<class functype>
void RunComparison(std::function<functype> Func, unsigned long long* Count, bool* KeepRunning) {
    while(*KeepRunning) {
        Func();
        ++*Count;
    }
}

void CompareResults(const ExecutionComparison<void()>& Comparison, int Seconds) {
    std::cout << "Comparing functions " << Comparison.LeftName << " to " << Comparison.RightName << std::endl;
    unsigned long long LeftCount = 0, RightCount = 0;
    bool KeepRunning = true;

    std::thread t1(std::bind(&RunComparison<void()>, Comparison.LeftFunc, &LeftCount, &KeepRunning));
    std::thread t2(std::bind(&RunComparison<void()>, Comparison.RightFunc, &RightCount, &KeepRunning));


    const int secondsPerUpdate = 1;
    for(int i = 0; i < Seconds; i += secondsPerUpdate) {
        std::cout << "Running... " << (i+1) << "/" << Seconds << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(secondsPerUpdate));
    }

    KeepRunning = false;

    t1.join();
    t2.join();

    const char* winner;
    const char* loser;
    bool tie = false;
    float delta = 0.0f;

    if(LeftCount == RightCount) {
        delta = 0.0f;
        tie = true;
    } else if(LeftCount > RightCount) {
        delta = ((float)LeftCount / (float)RightCount) * 100.0f;
        winner = Comparison.LeftName;
        loser = Comparison.RightName;
    } else {
        delta = ((float)RightCount / (float)LeftCount) * 100.0f;
        winner = Comparison.RightName;
        loser = Comparison.LeftName;
    }

    if(tie) {
        std:: cout << "It was a tie. ";
    }
    else {
        std::cout.precision(3);
        std::cout << winner << " won, and is %" << std::fixed << delta << " faster.\n"
        << Comparison.LeftName << " [" << ((double)Seconds / (double)(LeftCount/1000) * 1000000.0) << "ns] ";

        std::cout << Comparison.RightName << " [" <<((double)Seconds / (double)(RightCount/1000) * 1000000.0) << "ns]" << std::endl ;
    }
}

void TestVector2(Test& t) {
    t("Vector2 AngleDegrees", [&t] {
    #ifdef XOMATH_VEC2_CW
        REPORT_SUCCESS_IF(t, Vector2::Up.AngleDegrees(Vector2::Right), 90.0f);
        REPORT_SUCCESS_IF(t, Vector2::Right.AngleDegrees(Vector2::Down), 90.0f);
        REPORT_SUCCESS_IF(t, Vector2::Up.AngleDegrees(Vector2::Down), 180.0f);
    #else
        REPORT_SUCCESS_IF(t, Vector2::Up.AngleDegrees(Vector2::Right), -90.0f);
        REPORT_SUCCESS_IF(t, Vector2::Right.AngleDegrees(Vector2::Down), -90.0f);
        REPORT_SUCCESS_IF(t, Vector2::Up.AngleDegrees(Vector2::Down), -180.0f);
    #endif
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

    t("Vector2 Conversions", [&t] {
        REPORT_SUCCESS_IF(t, Vector2::Zero == Vector3::Zero, true);
        REPORT_SUCCESS_IF(t, Vector2::One == Vector3::One, true);
        REPORT_SUCCESS_IF(t, Vector2::One + Vector3::One, Vector2(2.0f, 2.0f));

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
        const Vector3 v3a = { 1.1f, 2.2f, 3.3f };
        const Vector3 v3b = { 4.4f, 5.5f, 6.6f };
        const float fa = 7.7f;
        REPORT_SUCCESS_IF(t, v3a / v3b, Vector3(v3a.x / v3b.x, v3a.y / v3b.y, v3a.z / v3b.z));
        REPORT_SUCCESS_IF(t, v3a + v3b, Vector3(v3a.x + v3b.x, v3a.y + v3b.y, v3a.z + v3b.z));
        REPORT_SUCCESS_IF(t, v3a * v3b, Vector3(v3a.x * v3b.x, v3a.y * v3b.y, v3a.z * v3b.z));
        REPORT_SUCCESS_IF(t, v3a - v3b, Vector3(v3a.x - v3b.x, v3a.y - v3b.y, v3a.z - v3b.z));
        REPORT_SUCCESS_IF(t, v3a / fa, Vector3(v3a.x / fa, v3a.y / fa, v3a.z / fa));
        REPORT_SUCCESS_IF(t, v3a + fa, Vector3(v3a.x + fa, v3a.y + fa, v3a.z + fa));
        REPORT_SUCCESS_IF(t, v3a * fa, Vector3(v3a.x * fa, v3a.y * fa, v3a.z * fa));
        REPORT_SUCCESS_IF(t, v3a - fa, Vector3(v3a.x - fa, v3a.y - fa, v3a.z - fa));

        Vector3 v2c;
        v2c = v3a;  // reset
        v2c /= v3b; // perform operation
        REPORT_SUCCESS_IF(t, v2c, Vector3(v3a.x / v3b.x, v3a.y / v3b.y, v3a.z / v3b.z));
        v2c = v3a;  // reset
        v2c += v3b; // perform operation
        REPORT_SUCCESS_IF(t, v2c, Vector3(v3a.x + v3b.x, v3a.y + v3b.y, v3a.z + v3b.z));
        v2c = v3a;  // reset
        v2c *= v3b; // perform operation
        REPORT_SUCCESS_IF(t, v2c, Vector3(v3a.x * v3b.x, v3a.y * v3b.y, v3a.z * v3b.z));
        v2c = v3a;  // reset
        v2c -= v3b; // perform operation
        REPORT_SUCCESS_IF(t, v2c, Vector3(v3a.x - v3b.x, v3a.y - v3b.y, v3a.z - v3b.z));


        REPORT_SUCCESS_IF(t, ~v3a, Vector3(v3a.z, v3a.y, v3a.x));
        REPORT_SUCCESS_IF(t, -v3a, Vector3(-v3a.x, -v3a.y, -v3a.z));

        REPORT_SUCCESS_IF(t, v3a < v3b, true);
        REPORT_SUCCESS_IF(t, v3a <= v3b, true);
        REPORT_SUCCESS_IF(t, v3b > v3a, true);
        REPORT_SUCCESS_IF(t, v3b >= v3a, true);

        REPORT_SUCCESS_IF(t, v3a > 1, true);
        REPORT_SUCCESS_IF(t, v3a < 8, true);
        REPORT_SUCCESS_IF(t, v3a < -8, true); // when checking magnitude, sign is ignored
    });

    t("Vector3 Conversions", [&t] {
        REPORT_SUCCESS_IF(t, Vector3::Zero == Vector2::Zero, true);
        REPORT_SUCCESS_IF(t, Vector3::One == Vector2::One, true);
        REPORT_SUCCESS_IF(t, Vector3::One + Vector2::One, Vector3(2.0f, 2.0f, 1.0f));
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