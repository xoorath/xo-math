#include <iostream>

#include "../include/xo-math.h"
#include "xo-test.h"

void TestVector3(Test& test);

int main() {
  Test test;
  TestVector3(test);
  return 0;
}

void TestVector3(Test& test) {
  test("Vector3", [&test] {
    const xo::Vector3 a(1.0f, 2.0f, 3.0f);
    const xo::Vector3 b(4.0f, 5.0f, 6.0f);
    xo::Vector3 c;
    float d;

    c = a+b;
    test.ReportSuccessIf(xo::CloseEnough(c.x, 5.0f), "1+4 should equal 5");
    test.ReportSuccessIf(xo::CloseEnough(c.y, 7.0f), "2+5 should equal 7");
    test.ReportSuccessIf(xo::CloseEnough(c.z, 9.0f), "3+9 should equal 8");

    c = a-b;
    test.ReportSuccessIf(xo::CloseEnough(c.x, -3.0f), "1-4 should equal -3");
    test.ReportSuccessIf(xo::CloseEnough(c.y, -3.0f), "2-5 should equal -3");
    test.ReportSuccessIf(xo::CloseEnough(c.z, -3.0f), "3-9 should equal -3");

    c = a*b;
    test.ReportSuccessIf(xo::CloseEnough(c.x, 4.0f), "1*4 should equal 4");
    test.ReportSuccessIf(xo::CloseEnough(c.y, 10.0f), "2*5 should equal 10");
    test.ReportSuccessIf(xo::CloseEnough(c.z, 18.0f), "3*9 should equal 18");

    c = a/b;
    test.ReportSuccessIf(xo::CloseEnough(c.x, 0.25f), "1/4 should equal 0.25");
    test.ReportSuccessIf(xo::CloseEnough(c.y, 0.4f), "2/5 should equal 0.4");
    test.ReportSuccessIf(xo::CloseEnough(c.z, 0.5f), "3/9 should equal 0.5");

    c = a;
    c += b;
    test.ReportSuccessIf(xo::CloseEnough(c.x, 5.0f), "1+=4 should equal 5");
    test.ReportSuccessIf(xo::CloseEnough(c.y, 7.0f), "2+=5 should equal 7");
    test.ReportSuccessIf(xo::CloseEnough(c.z, 9.0f), "3+=9 should equal 8");

    c = a;
    c -= b;
    test.ReportSuccessIf(xo::CloseEnough(c.x, -3.0f), "1-=4 should equal -3");
    test.ReportSuccessIf(xo::CloseEnough(c.y, -3.0f), "2-=5 should equal -3");
    test.ReportSuccessIf(xo::CloseEnough(c.z, -3.0f), "3-=9 should equal -3");

    c = a;
    c *= b;
    test.ReportSuccessIf(xo::CloseEnough(c.x, 4.0f), "1*=4 should equal 4");
    test.ReportSuccessIf(xo::CloseEnough(c.y, 10.0f), "2*=5 should equal 10");
    test.ReportSuccessIf(xo::CloseEnough(c.z, 18.0f), "3*=9 should equal 18");

    c = a;
    c /= b;
    test.ReportSuccessIf(xo::CloseEnough(c.x, 0.25f), "1/=4 should equal 0.25");
    test.ReportSuccessIf(xo::CloseEnough(c.y, 0.4f), "2/=5 should equal 0.4");
    test.ReportSuccessIf(xo::CloseEnough(c.z, 0.5f), "3/=9 should equal 0.5");

    test.ReportSuccessIf(xo::CloseEnough(a[0], 1.0f), "[] operator should produce the value x at 0");
    test.ReportSuccessIf(xo::CloseEnough(a[1], 2.0f), "[] operator should produce the value y at 1");
    test.ReportSuccessIf(xo::CloseEnough(a[2], 3.0f), "[] operator should produce the value z at 2");

    c = -a;
    test.ReportSuccessIf(xo::CloseEnough(c.x, -1.0f), "- operator should produce -1 at x.");
    test.ReportSuccessIf(xo::CloseEnough(c.y, -2.0f), "- operator should produce -2 at y.");
    test.ReportSuccessIf(xo::CloseEnough(c.z, -3.0f), "- operator should produce -3 at z.");

    c = ~a;
    test.ReportSuccessIf(xo::CloseEnough(c.x, 3.0f), "~ operator should flip x and z.");
    test.ReportSuccessIf(xo::CloseEnough(c.y, 2.0f), "~ operator should  not change y.");
    test.ReportSuccessIf(xo::CloseEnough(c.z, 1.0f), "~ operator should flip z and x.");

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
    test.ReportSuccessIf(xo::CloseEnough(c.Magnitude(), 1.0f), "a normalized (Normalized()) vector should have a magnitude of 1");

    c = a;
    c.Normalize();
    test.ReportSuccessIf(xo::CloseEnough(c.Magnitude(), 1.0f), "a normalized (Normalize()) vector should have a magnitude of 1");

    c = xo::Vector3::Cross(xo::Vector3::Forward, xo::Vector3::Right);
    test.ReportSuccessIf(c == xo::Vector3::Up, "forward cross right is up");

    d = xo::Vector3::Dot(xo::Vector3::Up, xo::Vector3::Up);
    test.ReportSuccessIf(xo::CloseEnough(d, 1.0f), "up dot up should equal 1");

    d = xo::Vector3::Dot(xo::Vector3::Up, xo::Vector3::Right);
    test.ReportSuccessIf(xo::CloseEnough(d, 0.0f), "up dot up should equal 0");

    d = xo::Vector3::Dot(xo::Vector3::Up, xo::Vector3::Forward);
    test.ReportSuccessIf(xo::CloseEnough(d, 0.0f), "up dot up should equal 0");

    d = xo::Vector3::Dot(xo::Vector3::Up, xo::Vector3::Down);
    test.ReportSuccessIf(xo::CloseEnough(d, -1.0f), "up dot up should equal -1");

    c = xo::Vector3::Lerp(xo::Vector3::Left, xo::Vector3::Right, 0.5f);
    test.ReportSuccessIf(c == xo::Vector3::Zero, "lerp(left, right, 0.5) should be zero");

    c = xo::Vector3::Lerp(xo::Vector3::Left, xo::Vector3::Right, 0.0f);
    test.ReportSuccessIf(c == xo::Vector3::Left, "lerp(left, right, 0) should be left");

    c = xo::Vector3::Lerp(xo::Vector3::Left, xo::Vector3::Right, 1.0f);
    test.ReportSuccessIf(c == xo::Vector3::Right, "lerp(left, right, 1) should be right");

    d = xo::Vector3::Distance(xo::Vector3::Left, xo::Vector3::Right);
    test.ReportSuccessIf(xo::CloseEnough(d, 2.0f) ,"distance from left to right should be 2.");

    d = xo::Vector3::Distance(xo::Vector3::Left, xo::Vector3::Zero);
    test.ReportSuccessIf(xo::CloseEnough(d, 1.0f) ,"distance from left to zero should be 1.");
  });
}