**xo-math**
===============================================================================

What is xo-math
----------

xo-math is an open source single header file module for optimized game math. C++11 or newer required.

Mission
----------

xo-math is designed to be as **fast** as possible on your target platform, and thoroughly tested for performance against other alternatives.

xo-math is designed to be **future-looking**. We push simd math past what currently adopted platforms support and build solutions for the up and coming.

xo-math is designed to be **friendly**. Easy to use and easy to understand, xo-math has built it's license and API for mere mortals to comprehend. You should be able to understand what we're all about in *10 minutes or less*, drag and drop xo-math.h into your project and start coding in just minutes more.

If you feel I've failed my mission here, please let me know `on github`_.

Quick Start
----------

Download `xo-math.h`_ from the root of the repo, include it in your project and you're good to go.

.. code ::

    #include <iostream>
    #include "xo-math.h"

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

Support
----------

Generally speaking, compilers supporting c++11 and later are supported out of the box.
The following configurations are known to work with no additional work.

====================== ==================== =======================
Compiler               Targets              Minimum Tested Version
====================== ==================== =======================
msvc++                 Win32                14.0 (VS2015)
g++                    Linux, OSX, Win32    4.8
clang++                Linux, OSX, Win32    3.2
apple-clang            OSX                  4.2 (Xcode 4.6)
====================== ==================== =======================

**Desired Support**

====================== ============================ =======================
Compiler               Targets                      Minimum Version
====================== ============================ =======================
msvc++                 Android, ARM, Win32, XB1     12.0 (VS2013)
g++                    Linux, OSX, Win32            4.8
clang++                Linux, OSX, Win32            3.2
apple-clang            OSX, iOS, tvOS, watchOS      4.2 (Xcode 4.6)
icc++                  Linux, OSX, Win32, PS4       11.1
====================== ============================ =======================

**`intel parallel studio for xo-math`_**
The intel c++ compiler is not yet supported, but you can help. I have a crowdrise campaign for a single user license of `intel parallel studio for xo-math`_. For more details on the campaign, see the campaign description.

Documentation
----------

*Classes:*

.. toctree::  

  classes/vector2.rst
  classes/vector3.rst
  classes/vector4.rst
  classes/matrix4x4.rst
  classes/quaternion.rst

*Definitions:*

.. toctree::
  functions.rst
  defines.rst

License
----------

The project is licensed under the MIT license.

.. _xo-math.h: https://raw.githubusercontent.com/xoorath/xo-math/master/xo-math.h
.. _on github: https://github.com/xoorath/xo-math/issues
.. _intel parallel studio for xo-math: https://www.crowdrise.com/intel-parallel-studio-for-xo-math