#include <iostream>

#define XO_MATH_IMPL
#include "xo-math.h"

#define TestScalar(exec, expect) \
    { float res = exec; if(xo::CloseEnough(res, expect) == false) { Fail(#exec " != " #expect, __LINE__); cout << "\tgot:" << res << endl; } else { Pass(#exec " == " #expect, __LINE__); } }

int main()
{
	using namespace std;
	using namespace xo;
    using namespace xo::simd;
	cout << "Compiling with sse: " << SSEVersionName << endl;
	cout << "Compiling with neon: " << NEONVersionName << endl;
    int passed = 0, failed = 0;
    auto Fail = [&failed](char const* statement, int line) {
        cout << "main.cpp(" << line << ") failed: " << statement << endl;
        failed++;
    };
    auto Pass = [&passed](char const* statement, int line) {
        (void)statement;
        (void)line;
        passed++;
    };

    TestScalar(Abs(1.f), 1.f);
    TestScalar(Abs(-1.f), 1.f);
    TestScalar(Abs(0.f), 0.f);
    TestScalar(Max(0.f, 10.f), 10.f);
    TestScalar(Max(0.f, -10.f), 0.f);
    TestScalar(Max(10.f, -10.f), 10.f);
    TestScalar(Max(5.f, 0.f, 10.f), 10.f);
    TestScalar(Max(0.f, 5.f, 10.f), 10.f);
    TestScalar(Max(0.f, 10.f, 5.f), 10.f);
    TestScalar(Min(0.f, 10.f), 0.f);
    TestScalar(Min(0.f, -10.f), -10.f);
    TestScalar(Min(10.f, -10.f), -10.f);
    TestScalar(Min(5.f, 0.f, 10.f), 0.f);
    TestScalar(Min(0.f, 5.f, 10.f), 0.f);
    TestScalar(Min(0.f, 10.f, 5.f), 0.f);
    TestScalar(Clamp(-1.f, 1.f, 0.5f), 0.5f);
    TestScalar(Clamp(-1.f, 1.f, 1.5f), 1.f);
    TestScalar(Clamp(-1.f, 1.f, -1.5f), -1.f);
    TestScalar(Lerp(0.f, 1.f, 0.5f), 0.5f);
    TestScalar(Lerp(-10.f, 10.f, 0.5f), 0.f);
    TestScalar(Lerp(0.f, 10.f, 0.f), 0.f);
    TestScalar(Lerp(0.f, 10.f, 1.f), 10.f);
    TestScalar(Lerp(0.f, 10.f, 2.f), 20.f);
    TestScalar(Lerp(0.f, 10.f, -1.f), -10.f);
    
    //{float a = xo::Lerp(1.f, 2.f, 0.5f); XO_UNUSED(a); }
    //{float a = xo::RelativeEpsilon(10.f); XO_UNUSED(a); }
    //{float a = xo::RelativeEpsilon(10.f, 20.f); XO_UNUSED(a); }
    //{float a = xo::CloseEnough(1.f, 1.000005f); XO_UNUSED(a); }
    //{float a = xo::Sqrt(1.f); XO_UNUSED(a); }
    //{float a = xo::Pow<1>(1.f); XO_UNUSED(a); }
    //{float a = xo::Pow<2>(1.f); XO_UNUSED(a); }
    //{float a = xo::Pow<3>(1.f); XO_UNUSED(a); }
    //{float a = xo::Pow<4>(1.f); XO_UNUSED(a); }
    //{float a = xo::Sin(1.f); XO_UNUSED(a); }
    //{float a = xo::Cos(1.f); XO_UNUSED(a); }
    //{float a = xo::ASin(1.f); XO_UNUSED(a); }
    //{float a = xo::ACos(1.f); XO_UNUSED(a); }
    //{float a, b; xo::SinCos(1.f, a, b); XO_UNUSED(a); XO_UNUSED(b); }
    //{float a, b; xo::ASinACos(1.f, a, b); XO_UNUSED(a); XO_UNUSED(b); }

    cout << passed << " passed " << failed << " failed." << endl;
	return 0;
}