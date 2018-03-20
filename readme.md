# xo-math
_v. alpha_

_Fast math for game developers. No headaches, no cruft._

## Warnings
- xo-math is in alpha. Contribute, test, but don't integrate yet.
- Documentation does not exist yet.
- Source code is untested.

## Download
Download [*xo-math.h*](xo-math/xo-math.h).

In one cpp file, and one cpp file *only* define `XO_MATH_IMPL` before including xo-math.h. 

CPU specific optimizations are detected based on your compilation settings.

- [Compilation settings for gcc and clang.](https://gcc.gnu.org/onlinedocs/gcc-4.5.3/gcc/i386-and-x86_002d64-Options.html)
- [Compilation settings for msvc.](https://msdn.microsoft.com/en-us/library/7t5yh4fd.aspx)

## Contributing
- There is no contributing guide yet.
- Basically: keep it simple.
- Repo: https://github.com/xoorath/xo-math

## About
xo-math is being developed by Jared Thomson in an effort to make games a little faster and a little easier to make. Twitter: [@xoorath](http://twitter.com/xoorath)

xo-math is pronounced non-phonetically "ex oh math", and spelled without capitalization where fonts permit.

## License
```
    The MIT License (MIT)

    Copyright 2018 Jared Thomson

    Permission is hereby granted, free of charge, to any person obtaining a copy of this
    software and associated documentation files (the "Software"), to deal in the Software
    without restriction, including without limitation the rights to use, copy, modify,
    merge, publish, distribute, sub license, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be included in all copies
    or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
    CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```
See also: [third-party-licenses.h](source/third-party-licenses.h)
