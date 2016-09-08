[![Build Status](https://semaphoreci.com/api/v1/xoorath/xo-math/branches/master/badge.svg)](https://semaphoreci.com/xoorath/xo-math)

xo-math is currently in progress and not ready for production use.

# What is xo-math

xo-math is a public domain single header file module for optimized game math. C++11 or newer required.

# Mission

xo-math is designed to be *fast*, as fast as possible on your target platform.

xo-math is designed to be *future-looking*. We push simd math past what currently adopted platforms support and build solutions for the up and coming.

xo-math is designed to be *friendly*. Easy to use and easy to understand, xo-math has built it's licence and source code for mere mortals to comprehend. You should be able to understand what we're all about in *10 minutes or less*, drag and drop xo-math.h into your project and start coding in just minutes more.

If you feel I've failed my mission here, [please let me know](https://github.com/xoorath/xo-math/issues)

# Platform Support

xo-math supports a variety of compilers with C++11 support. The [SIMD](https://en.wikipedia.org/wiki/SIMD) support is currently limited to x86, but ARM is planned soon.

**Actively tested platforms:**

| Building OS -> Running OS | Compiler         | Supported          |
|---------------------------|------------------|--------------------|
| Windows -> Windows        | msvc 14          | :white_check_mark: |
| Windows -> PS4            | msvc 14          | :x: untested       |
| Windows -> Xbox One       | msvc 14          | :x: untested       |
| Windows -> Windows        | gcc 4.8+         | :white_check_mark: |
| Windows -> Windows        | clang 3.3+       | :white_check_mark: |
| Windows -> Windows        | icc              | :x: untested       |
| linux -> linux            | gcc 4.8+         | :white_check_mark: |
| linux -> linux            | clang 3.3+       | :white_check_mark: |
| osx -> osx                | apple clang 5.x  | :x: untested       |
| osx -> osx                | apple clang 6.x  | :white_check_mark: |
| osx -> osx                | apple clang 7.x  | :x: untested       |

**Note:**
Just because your target OS/compiler isn't listed above, or is listed as untested that doesn't mean xo-math won't work. It just means I don't develop for that platform/compiler specifically, or haven't configured CI to support that platform *yet*.

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


# Configuration

All configuration can be done with defines before including `xo-math.h`. It's suggested that you wrap xo-math.h in another header that does the config for you if you have configuration to be done. Something along the lines of:

```
project/xo-math/include/xo-math.h   | the real xo-math
project/xo-math/xo-math.h           | contains configurations and includes xo-math
```

## SIMD configuration

xo-math supports [SIMD](https://en.wikipedia.org/wiki/SIMD). If you've configured your compiler explicitly for x86 simd instructions then xo-math should detect that automatically.

See the [/arch option for visual studio](https://msdn.microsoft.com/en-us/library/7t5yh4fd.aspx).

See the [extended instruction switches for gcc](https://gcc.gnu.org/onlinedocs/gcc-4.5.3/gcc/i386-and-x86_002d64-Options.html). Search the page for `-msse4.2` and similar. 
These flags also work with supported clang versions.

If you know what you're doing, you can also explicitly define simd support manually. **If you do define any of these, xo-math expects you to define all the simd defines you want, not just the highest version.**

```c++
#define XO_SSE 1
#define XO_SSE2 1
#define XO_SSE3 1
#define XO_SSSE3 1
#define XO_SSE4_1 1
#define XO_SSE4_2 1
#define XO_AVX 1
#define XO_AVX2 1
```

If you do define any of the above, but our detection of your compiler settings disagrees with your defined configuration - we will warn you about it at compile time unless you define the following:

```c++
#define XO_NO_VECTOR_DETECT_WARNING 1
```

## Namespace configuration

### Custom namespace

```c++
#define XO_CUSTOM_NS my_namespace
#include "xo-math.h"
// ...
my_namespace::Vector3 v;
```

### Single namespace

```c++
#define XO_SINGLE_NS
#include "xo-math.h"
// ...
xo::Vector3 v;
```

### Simple namespace

```c++
#define XO_SIMPLE_NS
#include "xo-math.h"
// ...
xomath::Vector3 v;
```

### No namespace

```c++
#define XO_NO_NS
#include "xo-math.h"
// ...
Vector3 v;
```

### Without configuration

```c++
#include "xo-math.h"
// ...
xo::math::Vector3 v;
```

### Advanced: re-definable

For testing it can be useful to define multiple configurations of xo-math and re-include it inside different namespaces.

For this we provide `XO_REDEFINABLE` and can be used like so:

```c++
#define XO_REDEFINABLE 1

#define XO_CUSTOM_NS xo_sse1
#define XO_SSE 1
#include "xo-math.h"

#undef XO_CUSTOM_NS
#define XO_CUSTOM_NS xo_sse2
#define XO_SSE2 1
#include "xo-math.h"
// ...

xo_sse1::Vector3 GoodVec3;
xo_sse2::Vector3 BetterVec3;
```

If you're using this feature, you of course must be careful not to re-include `xo-math.h` twice with the same namespace. Wrap it in another file configured as you need be, and use proper include guards or pragma once practices within that file.

## No inverse division

*Note:* This option only has an effect when SSE is enabled. The documentation following in this section is only true when SSE is enabled.

To speed up vector division, we do an approximation. It's fairly inaccurate (somewhere on the order of a `0.00036` error), but is between 1.3x and 3.3x faster.

This optimization can be disabled with the following define:

```c++
#define XO_NO_INVERSE_DIVISION
#include "xo-math.h"
```