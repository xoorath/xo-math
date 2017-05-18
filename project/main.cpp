#include <iostream>

#include <xo-math.h>

using std::cout;
using std::endl;

constexpr int hi = 6;

int main() {
  auto hello = []() {
    cout << "hello world!!" << endl;
  };
  hello();
  //cout << "xo-math version " << XO_MATH_VERSION_MAJOR << "." << XO_MATH_VERSION_MINOR << endl;
  
  return 0;
}