#include <iostream>

#include "../include/xo-math.h"
#include "../include/Units.h"
#include "xo-test.h"

void TestVector3(Test& test);
void TestVector4(Test& test);
void TestMathFuncs(Test& test);


#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)

int main() {
  Test test;
  test.SetFloatCompare([](float a, float b) { return xo::CloseEnough(a, b, xo::simd::Epsilon); });

  TestVector3(test);
  TestVector4(test);
  TestMathFuncs(test);

  return 0;
}

void TestVector3(Test& test) {
  test("Vector3", [&test] {
    const xo::Vector3 a(1.0f, 2.0f, 3.0f);
    const xo::Vector3 b(4.0f, 5.0f, 6.0f);
    xo::Vector3 c;
    float d;

    c = a+b;
    test.ReportSuccessIf(c.x, 5.0f, "1+4 should equal 5");
    test.ReportSuccessIf(c.y, 7.0f, "2+5 should equal 7");
    test.ReportSuccessIf(c.z, 9.0f, "3+9 should equal 8");

    c = a-b;
    test.ReportSuccessIf(c.x, -3.0f, "1-4 should equal -3");
    test.ReportSuccessIf(c.y, -3.0f, "2-5 should equal -3");
    test.ReportSuccessIf(c.z, -3.0f, "3-9 should equal -3");

    c = a*b;
    test.ReportSuccessIf(c.x, 4.0f, "1*4 should equal 4");
    test.ReportSuccessIf(c.y, 10.0f, "2*5 should equal 10");
    test.ReportSuccessIf(c.z, 18.0f, "3*9 should equal 18");

    c = a/b;
    test.ReportSuccessIf(c.x, 0.25f, "1/4 should equal 0.25");
    test.ReportSuccessIf(c.y, 0.4f, "2/5 should equal 0.4");
    test.ReportSuccessIf(c.z, 0.5f, "3/9 should equal 0.5");

    c = a;
    c += b;
    test.ReportSuccessIf(c.x, 5.0f, "1+=4 should equal 5");
    test.ReportSuccessIf(c.y, 7.0f, "2+=5 should equal 7");
    test.ReportSuccessIf(c.z, 9.0f, "3+=9 should equal 8");

    c = a;
    c -= b;
    test.ReportSuccessIf(c.x, -3.0f, "1-=4 should equal -3");
    test.ReportSuccessIf(c.y, -3.0f, "2-=5 should equal -3");
    test.ReportSuccessIf(c.z, -3.0f, "3-=9 should equal -3");

    c = a;
    c *= b;
    test.ReportSuccessIf(c.x, 4.0f, "1*=4 should equal 4");
    test.ReportSuccessIf(c.y, 10.0f, "2*=5 should equal 10");
    test.ReportSuccessIf(c.z, 18.0f, "3*=9 should equal 18");

    c = a;
    c /= b;
    test.ReportSuccessIf(c.x, 0.25f, "1/=4 should equal 0.25");
    test.ReportSuccessIf(c.y, 0.4f, "2/=5 should equal 0.4");
    test.ReportSuccessIf(c.z, 0.5f, "3/=9 should equal 0.5");

    c = a + 1.0f;
    test.ReportSuccessIf(c.x, 2.0f, "1+1 should equal 2");
    test.ReportSuccessIf(c.y, 3.0f, "2+1 should equal 3");
    test.ReportSuccessIf(c.z, 4.0f, "3+1 should equal 4");

    c = a - 1.0f;
    test.ReportSuccessIf(c.x, 0.0f, "1-1 should equal 0");
    test.ReportSuccessIf(c.y, 1.0f, "2-1 should equal 1");
    test.ReportSuccessIf(c.z, 2.0f, "3-1 should equal 2");

    c = a * 10.0f;
    test.ReportSuccessIf(c.x, 10.0f, "1*10 should equal 10");
    test.ReportSuccessIf(c.y, 20.0f, "2*10 should equal 20");
    test.ReportSuccessIf(c.z, 30.0f, "3*10 should equal 30");

    c = a / 2.0f;
    test.ReportSuccessIf(c.x, 0.5f, "1/2 should equal 0.5");
    test.ReportSuccessIf(c.y, 1.0f, "2/2 should equal 1.0");
    test.ReportSuccessIf(c.z, 1.5f, "3/2 should equal 1.5");

    c = a;
    c += 1.0f;
    test.ReportSuccessIf(c.x, 2.0f, "1+=1 should equal 2");
    test.ReportSuccessIf(c.y, 3.0f, "2+=1 should equal 3");
    test.ReportSuccessIf(c.z, 4.0f, "3+=1 should equal 4");

    c = a;
    c -= 1.0f;
    test.ReportSuccessIf(c.x, 0.0f, "1-=1 should equal 0");
    test.ReportSuccessIf(c.y, 1.0f, "2-=1 should equal 1");
    test.ReportSuccessIf(c.z, 2.0f, "3-=1 should equal 2");

    c = a;
    c *= 10.0f;
    test.ReportSuccessIf(c.x, 10.0f, "1*=10 should equal 10");
    test.ReportSuccessIf(c.y, 20.0f, "2*=10 should equal 20");
    test.ReportSuccessIf(c.z, 30.0f, "3*=10 should equal 30");

    c = a;
    c /= 2.0f;
    test.ReportSuccessIf(c.x, 0.5f, "1/=2 should equal 0.5");
    test.ReportSuccessIf(c.y, 1.0f, "2/=2 should equal 1.0");
    test.ReportSuccessIf(c.z, 1.5f, "3/=2 should equal 1.5");

    test.ReportSuccessIf(a[0], 1.0f, "[] operator should produce the value x at 0");
    test.ReportSuccessIf(a[1], 2.0f, "[] operator should produce the value y at 1");
    test.ReportSuccessIf(a[2], 3.0f, "[] operator should produce the value z at 2");

    c = -a;
    test.ReportSuccessIf(c.x, -1.0f, "- operator should produce -1 at x.");
    test.ReportSuccessIf(c.y, -2.0f, "- operator should produce -2 at y.");
    test.ReportSuccessIf(c.z, -3.0f, "- operator should produce -3 at z.");

    c = ~a;
    test.ReportSuccessIf(c.x, 3.0f, "~ operator should flip x and z.");
    test.ReportSuccessIf(c.y, 2.0f, "~ operator should  not change y.");
    test.ReportSuccessIf(c.z, 1.0f, "~ operator should flip z and x.");

    test.ReportSuccessIf(a < b, "a should be < b");
    test.ReportSuccessIf(a <= b, "a should be <= b");
    test.ReportSuccessIf(a <= a, "a should be <= a");
    test.ReportSuccessIf(b > a, "b should be > a");
    test.ReportSuccessIf(b >= a, "b should be >= a");
    test.ReportSuccessIf(a >= a, "a should be >= a");

    test.ReportSuccessIf(a == a, "a should be == a");
    test.ReportSuccessIf(!(a == b), "a should not be == b");

    test.ReportSuccessIf(!(a != a), "a should not be be != a");
    test.ReportSuccessIf(a != b, "a should not be != b");

    c = a.Normalized();
    test.ReportSuccessIf(c.Magnitude(), 1.0f, "a normalized (Normalized()) vector should have a magnitude of 1");

    c = a;
    c.Normalize();
    test.ReportSuccessIf(c.Magnitude(), 1.0f, "a normalized (Normalize()) vector should have a magnitude of 1");

    c = xo::Vector3::Cross(xo::Vector3::Forward, xo::Vector3::Right);
    test.ReportSuccessIf(c == xo::Vector3::Up, "forward cross right is up");

    d = xo::Vector3::Dot(xo::Vector3::Up, xo::Vector3::Up);
    test.ReportSuccessIf(d, 1.0f, "up dot up should equal 1");

    d = xo::Vector3::Dot(xo::Vector3::Up, xo::Vector3::Right);
    test.ReportSuccessIf(d, 0.0f, "up dot up should equal 0");

    d = xo::Vector3::Dot(xo::Vector3::Up, xo::Vector3::Forward);
    test.ReportSuccessIf(d, 0.0f, "up dot up should equal 0");

    d = xo::Vector3::Dot(xo::Vector3::Up, xo::Vector3::Down);
    test.ReportSuccessIf(d, -1.0f, "up dot up should equal -1");

    c = xo::Vector3::Lerp(xo::Vector3::Left, xo::Vector3::Right, 0.5f);
    test.ReportSuccessIf(c == xo::Vector3::Zero, "lerp(left, right, 0.5) should be zero");

    c = xo::Vector3::Lerp(xo::Vector3::Left, xo::Vector3::Right, 0.0f);
    test.ReportSuccessIf(c == xo::Vector3::Left, "lerp(left, right, 0) should be left");

    c = xo::Vector3::Lerp(xo::Vector3::Left, xo::Vector3::Right, 1.0f);
    test.ReportSuccessIf(c == xo::Vector3::Right, "lerp(left, right, 1) should be right");

    d = xo::Vector3::Distance(xo::Vector3::Left, xo::Vector3::Right);
    test.ReportSuccessIf(d, 2.0f ,"distance from left to right should be 2.");

    d = xo::Vector3::Distance(xo::Vector3::Left, xo::Vector3::Zero);
    test.ReportSuccessIf(d, 1.0f ,"distance from left to zero should be 1.");

    d = xo::Vector3::DistanceSquared(xo::Vector3::Left, xo::Vector3::Right);
    test.ReportSuccessIf(d, 4.0f ,"distance squared from left to right should be 4.");

    d = xo::Vector3::Distance(xo::Vector3::Left, xo::Vector3::Zero);
    test.ReportSuccessIf(d, 1.0f ,"distance squared from left to zero should be 1.");
  });
}

void TestVector4(Test& test) {
  test("Vector4", [&test] {
    const xo::Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    const xo::Vector4 b(5.0f, 6.0f, 7.0f, 8.0f);
    xo::Vector4 c;
    //float d;

    c = a+b;
    test.ReportSuccessIf(c.x, 6.0f, "1+5 should equal 6");
    test.ReportSuccessIf(c.y, 8.0f, "2+6 should equal 8");
    test.ReportSuccessIf(c.z, 10.0f, "3+7 should equal 10");
    test.ReportSuccessIf(c.w, 12.0f, "4+8 should equal 12");

    c = a-b;
    test.ReportSuccessIf(c.x, -4.0f, "1-5 should equal -4");
    test.ReportSuccessIf(c.y, -4.0f, "2-6 should equal -4");
    test.ReportSuccessIf(c.z, -4.0f, "3-7 should equal -4");
    test.ReportSuccessIf(c.w, -4.0f, "4-8 should equal -4");

    c = a*b;
    test.ReportSuccessIf(c.x, 5.0f, "1*5 should equal 5");
    test.ReportSuccessIf(c.y, 12.0f, "2*6 should equal 12");
    test.ReportSuccessIf(c.z, 21.0f, "3*7 should equal 21");
    test.ReportSuccessIf(c.w, 32.0f, "4*8 should equal 32");

    c = a/b;
    test.ReportSuccessIf(c.x, 0.2f, "1/5 should equal 0.2");
    test.ReportSuccessIf(c.y, (2.0f/6.0f), "2/6 should equal 2/6");
    test.ReportSuccessIf(c.z, (3.0f/7.0f), "3/7 should equal 3/7");
    test.ReportSuccessIf(c.w, 0.5f, "4/8 should equal 0.5");

    c = a;
    c += b;
    test.ReportSuccessIf(c.x, 6.0f, "1+5 should equal 6");
    test.ReportSuccessIf(c.y, 8.0f, "2+6 should equal 8");
    test.ReportSuccessIf(c.z, 10.0f, "3+7 should equal 10");
    test.ReportSuccessIf(c.w, 12.0f, "4+8 should equal 12");

    c = a;
    c -= b;
    test.ReportSuccessIf(c.x, -4.0f, "1-5 should equal -4");
    test.ReportSuccessIf(c.y, -4.0f, "2-6 should equal -4");
    test.ReportSuccessIf(c.z, -4.0f, "3-7 should equal -4");
    test.ReportSuccessIf(c.w, -4.0f, "4-8 should equal -4");

    c = a;
    c *= b;
    test.ReportSuccessIf(c.x, 5.0f, "1*5 should equal 5");
    test.ReportSuccessIf(c.y, 12.0f, "2*6 should equal 12");
    test.ReportSuccessIf(c.z, 21.0f, "3*7 should equal 21");
    test.ReportSuccessIf(c.w, 32.0f, "4*8 should equal 32");

    c = a;
    c /= b;
    test.ReportSuccessIf(c.x, 0.2f, "1/5 should equal 0.2");
    test.ReportSuccessIf(c.y, (2.0f/6.0f), "2/6 should equal 2/6");
    test.ReportSuccessIf(c.z, (3.0f/7.0f), "3/7 should equal 3/7");
    test.ReportSuccessIf(c.w, 0.5f, "4/8 should equal 0.5");
    
    c = a + 1.0f;
    test.ReportSuccessIf(c.x, 2.0f, "1+1 should equal 2");
    test.ReportSuccessIf(c.y, 3.0f, "2+1 should equal 3");
    test.ReportSuccessIf(c.z, 4.0f, "3+1 should equal 4");
    test.ReportSuccessIf(c.w, 5.0f, "4+1 should equal 5");

    c = a - 1.0f;
    test.ReportSuccessIf(c.x, 0.0f, "1-1 should equal 0");
    test.ReportSuccessIf(c.y, 1.0f, "2-1 should equal 1");
    test.ReportSuccessIf(c.z, 2.0f, "3-1 should equal 2");
    test.ReportSuccessIf(c.w, 3.0f, "4-1 should equal 3");

    c = a * 10.0f;
    test.ReportSuccessIf(c.x, 10.0f, "1*10 should equal 10");
    test.ReportSuccessIf(c.y, 20.0f, "2*10 should equal 20");
    test.ReportSuccessIf(c.z, 30.0f, "3*10 should equal 30");
    test.ReportSuccessIf(c.w, 40.0f, "4*10 should equal 40");

    c = a / 2.0f;
    test.ReportSuccessIf(c.x, 0.5f, "1/2 should equal 0.5");
    test.ReportSuccessIf(c.y, 1.0f, "2/2 should equal 1.0");
    test.ReportSuccessIf(c.z, 1.5f, "3/2 should equal 1.5");
    test.ReportSuccessIf(c.w, 2.0f, "4/2 should equal 2.0");

    c = a;
    c += 1.0f;
    test.ReportSuccessIf(c.x, 2.0f, "1+=1 should equal 2");
    test.ReportSuccessIf(c.y, 3.0f, "2+=1 should equal 3");
    test.ReportSuccessIf(c.z, 4.0f, "3+=1 should equal 4");
    test.ReportSuccessIf(c.w, 5.0f, "4+=1 should equal 5");

    c = a;
    c -= 1.0f;
    test.ReportSuccessIf(c.x, 0.0f, "1-=1 should equal 0");
    test.ReportSuccessIf(c.y, 1.0f, "2-=1 should equal 1");
    test.ReportSuccessIf(c.z, 2.0f, "3-=1 should equal 2");
    test.ReportSuccessIf(c.w, 3.0f, "4-=1 should equal 3");

    c = a;
    c *= 10.0f;
    test.ReportSuccessIf(c.x, 10.0f, "1*=10 should equal 10");
    test.ReportSuccessIf(c.y, 20.0f, "2*=10 should equal 20");
    test.ReportSuccessIf(c.z, 30.0f, "3*=10 should equal 30");
    test.ReportSuccessIf(c.w, 40.0f, "4*=10 should equal 40");

    c = a;
    c /= 2.0f;
    test.ReportSuccessIf(c.x, 0.5f, "1/=2 should equal 0.5");
    test.ReportSuccessIf(c.y, 1.0f, "2/=2 should equal 1.0");
    test.ReportSuccessIf(c.z, 1.5f, "3/=2 should equal 1.5");
    test.ReportSuccessIf(c.w, 2.0f, "4/=2 should equal 2.0");

    test.ReportSuccessIf(a[0], 1.0f, "[] operator should produce the value x at 0");
    test.ReportSuccessIf(a[1], 2.0f, "[] operator should produce the value y at 1");
    test.ReportSuccessIf(a[2], 3.0f, "[] operator should produce the value z at 2");
    test.ReportSuccessIf(a[3], 4.0f, "[] operator should produce the value w at 3");

    c = -a;
    test.ReportSuccessIf(c.x, -1.0f, "- operator should produce -1 at x.");
    test.ReportSuccessIf(c.y, -2.0f, "- operator should produce -2 at y.");
    test.ReportSuccessIf(c.z, -3.0f, "- operator should produce -3 at z.");
    test.ReportSuccessIf(c.w, -4.0f, "- operator should produce -4 at w.");

    c = ~a;
    test.ReportSuccessIf(c.x, 4.0f, "~ operator should flip w and z.");
    test.ReportSuccessIf(c.y, 3.0f, "~ operator should flip y and x.");
    test.ReportSuccessIf(c.z, 2.0f, "~ operator should flip x and y.");
    test.ReportSuccessIf(c.w, 1.0f, "~ operator should flip z and w.");

    test.ReportSuccessIf(a < b, "a should be < b");
    test.ReportSuccessIf(a <= b, "a should be <= b");
    test.ReportSuccessIf(a <= a, "a should be <= a");
    test.ReportSuccessIf(b > a, "b should be > a");
    test.ReportSuccessIf(b >= a, "b should be >= a");
    test.ReportSuccessIf(a >= a, "a should be >= a");

    test.ReportSuccessIf(a == a, "a should be == a");
    test.ReportSuccessIf(!(a == b), "a should not be == b");

    test.ReportSuccessIf(!(a != a), "a should not be be != a");
    test.ReportSuccessIf(a != b, "a should not be != b");

    c = a.Normalized();
    test.ReportSuccessIf(c.Magnitude(), 1.0f, "a normalized (Normalized()) vector should have a magnitude of 1");

    c = a;
    c.Normalize();
    test.ReportSuccessIf(c.Magnitude(), 1.0f, "a normalized (Normalize()) vector should have a magnitude of 1");

    // TODO: test Dot
    // TODO: test Lerp
    // TODO: test Distance
    // TODO: test DistanceSquared
  });
}

void TestMathFuncs(Test& test) {
  test("Math funcs", [&test] {
    using xo::PI;
    using xo::HALFPI;
    using xo::QUARTERPI;
#define XO_TEST_MATH_FUNC(func, in, out) test.ReportSuccessIf(xo:: func (in), out, STRINGIFY(func) " of " STRINGIFY(in) " should be " STRINGIFY(out));
#define XO_TEST_MATH_FUNC2(func, in1, in2, out) test.ReportSuccessIf(xo:: func (in1, in2), out, STRINGIFY(func) " of (" STRINGIFY(in1) ", " STRINGIFY(in2) ") should be " STRINGIFY(out));
    XO_TEST_MATH_FUNC(Abs, 1.0f, 1.0f);
    XO_TEST_MATH_FUNC(Abs, -1.0f, 1.0f);
    XO_TEST_MATH_FUNC(Abs, 0.0f, 0.0f);

    XO_TEST_MATH_FUNC(ACos, -1.0f, PI);
    XO_TEST_MATH_FUNC(ACos, 1.0f, 0.0f);
    XO_TEST_MATH_FUNC(ACos, 0.0f, HALFPI);

    XO_TEST_MATH_FUNC(ASin, -1.0f, -HALFPI);
    XO_TEST_MATH_FUNC(ASin, 1.0f, HALFPI);
    XO_TEST_MATH_FUNC(ASin, 0.0f, 0.0f);

    XO_TEST_MATH_FUNC(ATan, 0.0f, 0.0f);
    XO_TEST_MATH_FUNC(ATan, 1.0f, QUARTERPI);

    XO_TEST_MATH_FUNC2(ATan2, 0.0f, 0.0f, 0.0f);
    XO_TEST_MATH_FUNC2(ATan2, 1.0f, 0.0f, HALFPI);
    XO_TEST_MATH_FUNC2(ATan2, 0.0f, 1.0f, 0.0f);

    // TODO: more tests for cbrt
    XO_TEST_MATH_FUNC(Cbrt, 0.0f, 0.0f);
    XO_TEST_MATH_FUNC(Cbrt, 1.0f, 1.0f);
    XO_TEST_MATH_FUNC(Cbrt, -1.0f, -1.0f);

    XO_TEST_MATH_FUNC(Cos, 0.0f, 1.0f);
    XO_TEST_MATH_FUNC(Cos, PI, -1.0f);

    XO_TEST_MATH_FUNC2(Difference, 0.0f, 1.0f, 1.0f);
    XO_TEST_MATH_FUNC2(Difference, -1.0f, 1.0f, 2.0f);
    XO_TEST_MATH_FUNC2(Difference, -1.0f, 0.0f, 1.0f);
    XO_TEST_MATH_FUNC2(Difference, 5.0f, 5.0f, 0.0f);

    XO_TEST_MATH_FUNC2(Max, 1.0f, 0.0f, 1.0f);
    XO_TEST_MATH_FUNC2(Max, 0.0f, 1.0f, 1.0f);
    XO_TEST_MATH_FUNC2(Max, -1.0f, 1.0f, 1.0f);
    XO_TEST_MATH_FUNC2(Max, -2.0f, -1.0f, -1.0f);

    XO_TEST_MATH_FUNC2(Min, 1.0f, 0.0f, 0.0f);
    XO_TEST_MATH_FUNC2(Min, 0.0f, 1.0f, 0.0f);
    XO_TEST_MATH_FUNC2(Min, -1.0f, 1.0f, -1.0f);
    XO_TEST_MATH_FUNC2(Min, -2.0f, -1.0f, -2.0f);

    XO_TEST_MATH_FUNC(Sin, 0.0f, 0.0f);
    XO_TEST_MATH_FUNC(Sin, HALFPI, 1.0f);

    XO_TEST_MATH_FUNC(Sqrt, 1.0f, 1.0f);
    XO_TEST_MATH_FUNC(Sqrt, 4.0f, 2.0f);
    XO_TEST_MATH_FUNC(Sqrt, 16.0f, 4.0f);
    XO_TEST_MATH_FUNC(Sqrt, 64.0f, 8.0f);

    XO_TEST_MATH_FUNC(Tan, 0.0f, 0.0f);

    {
      float s, c;
      xo::SinCos(0.0f, s, c);
      test.ReportSuccessIf(s, 0.0f, "sincos(0) should have a sin of 0");
      test.ReportSuccessIf(c, 1.0f, "sincos(0) should have a cos of 1");
      
      xo::SinCos(HALFPI, s, c);
      test.ReportSuccessIf(s, 1.0f, "sincos(HALFPI) should have a sin of 1");

      xo::SinCos(PI, s, c);
      test.ReportSuccessIf(c, -1.0f, "sincos(PI) should have a sin of 1");
    }

    {
      float s[4], c[4];
      {
        float in[] = {0, 0, 0, 0};
        xo::SinCos4(in, s, c);
        for(int i = 0; i < 4; ++i) {
          test.ReportSuccessIf(s[i], 0.0f, "sincos(0) should have a sin of 0");
          test.ReportSuccessIf(c[i], 1.0f, "sincos(0) should have a cos of 1");
        }
      }
      {
        float in[] = {HALFPI, HALFPI, HALFPI, HALFPI};
        xo::SinCos4(in, s, c);
        for(int i = 0; i < 4; ++i) {
          test.ReportSuccessIf(s[i], 1.0f, "sincos(HALFPI) should have a sin of 1");
        }
      }
      {
        float in[] = {PI, PI, PI, PI};
        xo::SinCos4(in, s, c);
        for(int i = 0; i < 4; ++i) {
          test.ReportSuccessIf(c[i], -1.0f, "sincos(PI) should have a sin of 1");
        }
      }
    }

  XO_TEST_MATH_FUNC2(CloseEnough, 1.0f, 1.0f, true);
  XO_TEST_MATH_FUNC2(CloseEnough, 1.0f, 1.01f, false);
  XO_TEST_MATH_FUNC2(CloseEnough, 1.0f, 1.001f, false);
  XO_TEST_MATH_FUNC2(CloseEnough, 1.0f, 1.0001f, false);
  XO_TEST_MATH_FUNC2(CloseEnough, 1.0f, 1.00001f, false);
  XO_TEST_MATH_FUNC2(CloseEnough, 1.0f, 1.000001f, false);
  XO_TEST_MATH_FUNC2(CloseEnough, 1.0f, 1.0000001f, true);

  XO_TEST_MATH_FUNC2(CloseEnough, 10.0f, 10.0f, true);
  XO_TEST_MATH_FUNC2(CloseEnough, 10.0f, 10.01f, false);
  XO_TEST_MATH_FUNC2(CloseEnough, 10.0f, 10.001f, false);
  XO_TEST_MATH_FUNC2(CloseEnough, 10.0f, 10.0001f, false);
  XO_TEST_MATH_FUNC2(CloseEnough, 10.0f, 10.00001f, false);
  XO_TEST_MATH_FUNC2(CloseEnough, 10.0f, 10.000001f, true);
  XO_TEST_MATH_FUNC2(CloseEnough, 10.0f, 10.0000001f, true);

  XO_TEST_MATH_FUNC2(CloseEnough, 100.0f, 100.0f, true);
  XO_TEST_MATH_FUNC2(CloseEnough, 100.0f, 100.01f, false);
  XO_TEST_MATH_FUNC2(CloseEnough, 100.0f, 100.001f, false);
  XO_TEST_MATH_FUNC2(CloseEnough, 100.0f, 100.0001f, false);
  XO_TEST_MATH_FUNC2(CloseEnough, 100.0f, 100.00001f, true);
  XO_TEST_MATH_FUNC2(CloseEnough, 100.0f, 100.000001f, true);
  XO_TEST_MATH_FUNC2(CloseEnough, 100.0f, 100.0000001f, true);

#undef XO_TEST_MATH_FUNC
#undef XO_TEST_MATH_FUNC2
  });
}