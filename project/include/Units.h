//////////////////////////////////////////////////////////////////////
// Units.h 
//
// AUTHOR
//   Jared Thomson <twitter: @xoorath> <email:jared@xoorath.com>
//
// LICENSE
//   See end of file for license information.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "xo-math-defines.h"

XO_NAMESPACE_BEGIN

struct Radians {
  inline Radians(float val);
  inline Radians(const Radians& rad);
  inline Radians(const struct Degrees& deg);

  inline operator float&();
  inline operator const float&() const;

  float Value;
};

struct Degrees {
  inline Degrees(float val);
  inline Degrees(const Degrees& deg);
  inline Degrees(const Radians& rad);

  inline operator float&();
  inline operator const float&() const;

  float Value;
};

// inline
Radians::Radians(float val)
: Value(val) {
}

// inline
Radians::Radians(const Radians& rad)
: Value(rad.Value) {
}

// inline
Radians::Radians(const struct Degrees& deg)
: Value(deg.Value * Deg2Rad) {
}

// inline
Radians::operator float&() {
  return Value;
}

// inline
Radians::operator const float&() const {
  return Value;
}

// inline
Degrees::Degrees(float val)
: Value(val) {
}

// inline
Degrees::Degrees(const Degrees& deg)
: Value(deg.Value) {
}

// inline
Degrees::Degrees(const Radians& rad)
: Value(rad.Value * Rad2Deg) {
}

// inline
Degrees::operator float&() {
  return Value;
}

// inline
Degrees::operator const float&() const {
  return Value;
}

XO_NAMESPACE_END
//////////////////////////////////////////////////////////////////////
// This software is available under 2 licenses
// choose whichever you prefer.
//////////////////////////////////////////////////////////////////////

// MIT License
//////////////////////////////////////////////////////////////////////
/*
Copyright (c) 2017 Jared Thomson
Permission is hereby granted, free of charge, to any person obtaining 
a copy of this software and associated documentation files (the 
"Software"), to deal in the Software without restriction, including 
without limitation the rights to use, copy, modify, merge, publish, 
distribute, sublicense, and/or sell copies of the Software, and to 
permit persons to whom the Software is furnished to do so, subject to 
the following conditions:
The above copyright notice and this permission notice shall be 
included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////////////////////////////////

// The Unlicense
//////////////////////////////////////////////////////////////////////
/*
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.
In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
For more information, please refer to <http://unlicense.org/>
*/
//////////////////////////////////////////////////////////////////////