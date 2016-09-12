[![Build Status](https://semaphoreci.com/api/v1/xoorath/xo-math/branches/master/shields_badge.svg)](https://semaphoreci.com/xoorath/xo-math)
[![Documentation Status](https://readthedocs.org/projects/xo-math/badge/?version=latest)](http://xo-math.readthedocs.io/en/latest/?badge=latest)
[![MIT Licence](https://badges.frapsoft.com/os/mit/mit.png?v=103)](https://tldrlegal.com/license/mit-license) 

:warning: **xo-math is currently in progress and not ready for production use.**

# What is xo-math

xo-math is an open source single header file module for optimized game math. C++11 or newer required.

## Mission

xo-math is designed to be as **fast** as possible on your target platform, and thoroughly tested for performance against other alternatives.

xo-math is designed to be **future-looking**. We push simd math past what currently adopted platforms support and build solutions for the up and coming.

xo-math is designed to be **friendly**. Easy to use and easy to understand, xo-math has built it's license and API for mere mortals to comprehend. You should be able to understand what we're all about in *10 minutes or less*, drag and drop xo-math.h into your project and start coding in just minutes more.

If you feel I've failed my mission here, [please let me know](https://github.com/xoorath/xo-math/issues).

# Quick Start

Download [xo-math.h](https://raw.githubusercontent.com/xoorath/xo-math/master/xo-math.h) from the root of the repo, include it in your project and you're good to go.

```c++
#include <iostream>
#include "include/xo-math.h"

using namespace std;
using namespace xo;

int main() {
    
    math::Vector3 vec = math::Vector3::RandomOnConeRadians(math::Vector3::Up, math::HalfPI/2.0f);
    cout << "random 45 degree cone vector from Vector3::Up: " << vec << endl;

    if(vec == 1.0f) {
        cout << "The vector is normalized." << endl;
    }

    math::Matrix4x4 tilt = math::Matrix4x4::RotationXDegrees(-90.0f);
    tilt.Transform(vec);
    cout << "Now rotated -90 degrees by the x axis: " << vec << endl;

    float x, y, z;
    vec.Get(x, y, z);
    cout << "Extracted vector elements. x:" << x << " y:" << y << " z:" << z << endl;

    return 0;
}
```

# Documentation

Documentation is hosted by Read The Docs: **[xo-math.rtfd.io](http://xo-math.rtfd.io)**

