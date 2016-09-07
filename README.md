### Status
[![Build Status](https://semaphoreci.com/api/v1/xoorath/xo-math/branches/master/shields_badge.svg)](https://semaphoreci.com/xoorath/xo-math)

xo-math is currently in progress and not ready for production use.

# quick start guide

Download and include `xo-math.h` from the head revision of the repo. No other files are required to use xo-math.

Once xo-math is included, you can begin using the math types included. Example:

```

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

# contributing

The `xo-math.h` header in the root of the repo is just a distribution file. For contributors we have a much more split-up version of the library inside the `xo-math` directory.

## Project Layout

We keep various project files in `xo-math/`. We actively support xcode7, sublime text 3 and visual studio 2015 builds. If you find one particular IDE is not compiling (out of date included files, for example) please take a moment to update them and submit the fix alone in its own pull request. It can help un-block other developers quickly and is easy to verify.

`xo-math.h` contains any project-wide includes, small utility functions and project-wide information such as `XO_MATH_VERSION_TXT`.

## Macros

We use macros in two flavours in this project:

- `_XO_MACRONAME` indicates a macro that is internal to xo-math. It should be undefined before it leaves file scope.
- `XO_MACRONAME` indicates a macro that is intended for use by xo-math users. It can be a macro to configure xo-math with, or a macro provided by xo-math for the user to use. These will not be undefined from within xo-math.

## Unit tests

In xo-math we use `xo-math/xo-test.h` to perform unit tests in `xo-math/Main.cpp`. There's no guarantee that xo-test is or ever will be unmodified from the original xo-test project. If there are testing features specific to xo-math, it's perfectly acceptable to modify xo-test.h for our needs here.

Adding unit tests to Main.cpp is encouraged. There is no need to have a super-clean and idealistic pattern to the unit tests in Main.cpp. The purpose is simply to catch human error and ensure reliability of xo-math to its users. If the error messages or names are inconsistent but extra throughput is tested, then it's accomplishing its goals none the less. That said: whenever possible deterministic tests are preferred. Any failing test should fail every time if at all possible.