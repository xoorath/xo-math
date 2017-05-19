//////////////////////////////////////////////////////////////////////////////////////////
// Test.h (version 0.1, July 2016)
//
//  A public domain single header file testing module. C++11 or newer required.
//
//  AUTHOR
//    Jared Thomson (@xoorath)
//
//  STREET CRED
//    Inspired by Sean Barrett's stb. https://github.com/nothings
//
//  LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <functional>
#include <iostream>
#include <chrono>
#include <ctime>

//////////////////////////////////////////////////////////////////////////////////////////
// Test
//////////////////////////////////////////////////////////////////////////////////////////
// This unit testing module is an exersize in minimalism.
// Example below.
//////////////////////////////////////////////////////////////////////////////////////////
/*

 Test test;
 // Run the basic math test.
 test("basic math", [&test]{

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
    test.ReportSuccesIf(multiplyResult, 10, "multiplication result is incorrect");
  });

  // You can get the number of all failures
  if(test.GetTotalFailures() > 0) 
    return -1;
  else 
    return 0;

*/
//////////////////////////////////////////////////////////////////////////////////////////
class Test {
  typedef std::function<void()> TTestFunc;
  typedef std::chrono::time_point<std::chrono::system_clock> TTime;
  typedef std::chrono::duration<double> TDuration;

public:

  void ReportSuccessIf(bool condition, const char* reason);

  template<typename T>
  void ReportSuccessIf(T got, T expected, const char* reason);

  void ReportSuccess();
  void ReportFailure(const char* reason);

  void operator ()(const char* testName, TTestFunc func);
  int GetTotalFailures() const;

private:
  int m_CurrentSuccess  = 0;
  int m_CurrentFailure  = 0;
  int m_TotalFailure    = 0;
};

//////////////////////////////////////////////////////////////////////////////////////////
// Test
//////////////////////////////////////////////////////////////////////////////////////////
void Test::ReportSuccessIf(bool condition, const char* reason) {
  if(condition) ReportSuccess(); else ReportFailure(reason);
}

template<typename T>
void Test::ReportSuccessIf(T got, T expected, const char* reason) {
  if(got == expected)
    ReportSuccess();
  else {
    ReportFailure(reason);
    std::cout << "expected: " << expected << " got: " << got << std::endl;
  }
}

void Test::ReportSuccess() {
  m_CurrentSuccess++;
}

void Test::ReportFailure(const char* reason) {
  m_CurrentFailure++;
  m_TotalFailure++;
  std::cout << "[FAIL] " << reason << std::endl;
}

void Test::operator ()(const char* testName, TTestFunc func) {
  TTime start, end;
  std::cout << "========== \"" << testName << "\" test started ==========\n";
  m_CurrentSuccess = m_CurrentFailure = 0;

  start = std::chrono::system_clock::now();
  func();
  end = std::chrono::system_clock::now();
  TDuration seconds = end-start;

  std::cout << m_CurrentSuccess << " passed. " << m_CurrentFailure;
  std::cout << " failed. took (" << seconds.count() << ") seconds. \n\n";
}

int Test::GetTotalFailures() const {
  return m_TotalFailure;
}