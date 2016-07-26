#include <iostream>

#define NO_XO_NS 1
    #include "../GameMath.h"
#undef NO_XO_NS

int main() {
    std::cout.precision(2);
    Vector3 a = { 1.0f, 2.0f, 3.0f };
    Vector3 b = { 3.0f, 4.0f, 5.0f };

    a + b;

    float x, y, z;

    a.Get(x, y, z);


    std::cout << a << std::endl;
    return 0;
}