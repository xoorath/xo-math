#include <iostream>

#include <xo-math.h>

using std::cout;
using std::endl;

constexpr int hi = 6;

int main() {
  xo::Vector3 a(1.0f, 2.0f, 3.0f);
  xo::Vector3 b(xo::Vector3::Up);
  a.Normalize();
  a *= b;
  b += a;
  float f = a.Sum();
  (void)f;
  
  cout << "xo-math compiling with simd: " << XO_MATH_HIGHEST_SIMD << endl;
  
  return 0;
}