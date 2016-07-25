#include <iostream>

#define NO_XO_NS 1
    #include "../GameMath.h"
#undef NO_XO_NS

int main() {
    std::cout.precision(2);
    Vector3 a = { 0.0f, 1.0f, 2.0f };
    Vector3 b = { 3.0f, 4.0f, 5.0f };

    a + b;

    std::cout << a << std::endl;
    return 0;
}