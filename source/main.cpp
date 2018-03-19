#include <iostream>

#define XO_MATH_IMPL
#include "xo-math.h"

int main()
{
	using namespace std;
	using namespace xo;
    using namespace xo::simd;
	cout << "Compiling with sse: " << SSEVersionName << endl;
	cout << "Compiling with neon: " << NEONVersionName << endl;
	
	//Vector3 a(1.0f), b(0.0f);
	//a = a + b + a;
	return 0;
}