[![Build Status](https://semaphoreci.com/api/v1/xoorath/xo-math/branches/master/badge.svg)](https://semaphoreci.com/xoorath/xo-math)

:warning: **xo-math is currently in progress and not ready for production use.**

# What is xo-math

xo-math is a public domain single header file module for optimized game math. C++11 or newer required.

## Mission

xo-math is designed to be as **fast** as possible on your target platform, and thoroughly tested for performance against other alternatives.

xo-math is designed to be **future-looking**. We push simd math past what currently adopted platforms support and build solutions for the up and coming.

xo-math is designed to be **friendly**. Easy to use and easy to understand, xo-math has built it's license and API for mere mortals to comprehend. You should be able to understand what we're all about in *10 minutes or less*, drag and drop xo-math.h into your project and start coding in just minutes more.

If you feel I've failed my mission here, [please let me know](https://github.com/xoorath/xo-math/issues).

# Platform Support

xo-math supports a variety of compilers with C++11 support. The [SIMD](https://en.wikipedia.org/wiki/SIMD) support is currently limited to x86, but ARM is planned soon.

**Actively tested platforms:**

| Building OS -> Running OS | Compiler         | Supported          |
|---------------------------|------------------|--------------------|
| Windows -> Windows        | msvc 14          | :white_check_mark: |
| Windows -> PS4            | msvc 14          | :warning: |
| Windows -> Xbox One       | msvc 14          | :warning: |
| Windows -> Windows        | gcc 4.8+         | :white_check_mark: |
| Windows -> Windows        | clang 3.3+       | :white_check_mark: |
| Windows -> Windows        | icc              | :warning: |
| linux -> linux            | gcc 4.8+         | :white_check_mark: |
| linux -> linux            | clang 3.3+       | :white_check_mark: |
| osx -> osx                | apple clang 5.x  | :warning: |
| osx -> osx                | apple clang 6.x  | :white_check_mark: |
| osx -> osx                | apple clang 7.x  | :warning: |

:warning:: This configuration is planned to work, but currently untested. If you've have experience running xo-math under one of these configurations, [please let me know](http://twitter.com/xoorath).

# Quick start guide

Download and include `xo-math.h` from the head revision of the repo. No other files are required to use xo-math.

Once xo-math is included, you can begin using the math types included. Example:

```c++

#include <iostream>
#include "xo-math.h"

using namespace std;
using namespace xo;
using namespace xo::math;

int main() {
  
  Vector3 hello = Vector3::RandomOnSphere();
  Vector3 world = Vector3::RandomInConeRadians(Vector3::Forward, HalfPI/2.0f);

  cout << hello << endl << world << endl;

  return 0;
}

```

_note:_ The only header to ever be included for xo-math is `xo-math.h`, even when using the sources from the `xo-math/include/` directory.


# Documentation

[wiki home](https://github.com/xoorath/xo-math/wiki)
[configuration](https://github.com/xoorath/xo-math/wiki/Configuration)

