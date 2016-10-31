**xo-math**
===============================================================================

What is xo-math
----------

xo-math is an `open source <#license>`_ single header file module for optimized game math. C++11 or newer required. 

Mission
----------

xo-math is designed to be as **fast** as possible on your target platform, and thoroughly tested for performance against other alternatives.

xo-math is designed to be **future-looking**. We push simd math past what currently adopted platforms support and build solutions for the up and coming that will out perform on launch day.

xo-math is designed to be **friendly**. Easy to use and easy to understand, xo-math has built it's license and API for mere mortals to comprehend. You should be able to understand what we're all about in *10 minutes or less*, drag and drop xo-math.h into your project and start coding in just minutes more.

If you feel I've failed my mission here, please let me know `on github`_.

Quick Start
----------

Download `xo-math.h`_, `xo-math.cpp`_ and `xo-math-config.h`_ from the root of the repo, include them in your project and you're good to go.

.. code ::

    #include <iostream>
    #include "xo-math.h"

    using namespace std;
    using namespace xo;

    int main() {
        
        Vector3 vec = Vector3::RandomOnConeRadians(Vector3::Up, HalfPI/2.0f);
        cout << "random 45 degree cone vector from Vector3::Up: " << vec << endl;

        if(vec == 1.0f) {
            cout << "The vector is normalized." << endl;
        }

        Matrix4x4 tilt = Matrix4x4::RotationXDegrees(-90.0f);
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
msvc++                 Win32                11.0 (VS2012)
g++                    Linux, OSX, Win32    4.8
clang++                Linux, OSX, Win32    3.2
apple-clang            OSX                  4.2 (Xcode 4.6)
====================== ==================== =======================

**Desired Support**

====================== ================================= =======================
Compiler               Targets                           Minimum Version
====================== ================================= =======================
msvc++                 `Android`_, ARM, Win32, `XB1`_    11.0 (VS2012)
g++                    Linux, OSX, Win32                 4.8
clang++                Linux, OSX, `PS4`_, Win32         3.2
apple-clang            OSX, `iOS`_                       4.2 (Xcode 4.6)
SNC                    `PS Vita`_                        
icc++                  Linux, OSX, Win32, `PS4`_         11.1
MULTI                  `Wii U`_                          ?
ARMCC                  `3DS`_                            5.05
====================== ================================= =======================

*Note:* There are some simplifications here. Minimum versions are based on the earliest known compiler with basic c++11 support. Some targets require special compilers that may have a different minimum version based on SDK availability. In these cases, the compiler for the lowest supported SDK version is our target.

**Key Challenges**

Although all above platforms should be supported, I have a lack of testing bandwidth to ensure compilation is seamless on these platforms. In some cases compilation will fail on a lack of a specific c++ feature.

On ARM devices `NEON`_ should be supported, but currently is not. At this time ARM devices should be able to use xo-math and will default to floating point math without SIMD support.

`Intel parallel studio for xo-math`_
----------

The intel c++ compiler is not yet supported, but you can help. I have a crowdrise campaign for a single user license of `Intel parallel studio for xo-math`_. For more details on the campaign, see the campaign description.

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
.. _xo-math-config.h: https://raw.githubusercontent.com/xoorath/xo-math/master/xo-math-config.h
.. _xo-math.cpp: https://raw.githubusercontent.com/xoorath/xo-math/master/xo-math.cpp
.. _on github: https://github.com/xoorath/xo-math/issues
.. _Intel parallel studio for xo-math: https://www.crowdrise.com/intel-parallel-studio-for-xo-math
.. _NEON: http://www.arm.com/products/processors/technologies/neon.php
.. _XB1: http://www.xbox.com/en-US/developers
.. _PS4: https://www.playstation.com/en-us/develop/
.. _PS Vita: https://www.playstation.com/en-us/develop/
.. _Wii U: https://developer.nintendo.com
.. _3DS: https://developer.nintendo.com
.. _iOS: https://developer.apple.com/programs/
.. _Android: https://developer.android.com/index.html