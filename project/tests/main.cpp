#include <iostream>

#include <xo-math.h>
#include "xo-test.h"

void TestVector3(Test& test);

int main() {
  Test test;
  TestVector3(test);
  return 0;
}

void TestVector3(Test& test) {
  test("Vector3", [&test] {
    // you can manually call ReportSuccess/Failure for more complex or non-binary tests.
    if(2+2 == 4) {
      test.ReportSuccess();
    } else {
      test.ReportFailure("two plus two should equal four.");
    }

    // will report a failure if false
    test.ReportSuccessIf(10/2==5, "ten divided by two should be five.");

    int multiplyResult = 5*2;
    // will log expected vs. got to the console
    test.ReportSuccessIf(multiplyResult, 10, "multiplication result is incorrect");
  });
}