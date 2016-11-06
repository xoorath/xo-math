// The MIT License (MIT)
//
// Copyright (c) 2016 Jared Thomson
//
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included 
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT 
// OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
// THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#if !defined(XO_MATH_CONFIG_H_)
#define XO_MATH_CONFIG_H_

// Options:
//  XO_NO_OSTREAM 
//      * disables ostream (such as cout) support of math types.
//  XO_ASSERT(condition, message)
//      * Called by xo-math for sanity checks.
//  XO_SPACE_LEFTHAND | XO_SPACE_RIGHTHAND
//      * Either left hand or right hand coord systems can be used. Do not define both. If neither is defined, the default is right hand.
//  XO_SPACE_YUP | XO_SPACE_ZUP
//      * Either y or z axis can be used for the 'up' axis. Do not define both. If neither is defined, the default is z.
//  XO_NO_INVERSE_DIVISION
//      * By default xo-math uses approximate division which is faster and less accurate. When this option is defined, standard float/simd division is used.
//      * See source code comments in the definition of Vector3::operator/= for more details.
//  XO_16ALIGNED_MALLOC(size)
//      * A 16 byte aligned allocator can be provided to xo-math by advanced end users.
//  XO_16ALIGNED_FREE(ptr)
//      * A free method can be provided to xo-math by advanced end users.
//  XO_EXPORT_ALL
//      * Not for typical end users. This prevents the undefining of internal macros such as _XOINL or XO_INTERNAL for example.

#define XO_EXPORT_ALL


#if defined(XO_NO_OSTREAM)
#   define XO_ASSERT(condition, message)
#else
#   include <iostream>
#   define XO_ASSERT(condition, message) if(!(condition)) { std::cerr << message << std::endl; }
#endif

#define XO_SPACE_LEFTHAND
#define XO_SPACE_YUP

////////////////////////////////////////////////////////////////////////// Defaults & Safeguards: Do not modify below

#if defined(XO_SPACE_LEFTHAND) && defined(XO_SPACE_RIGHTHAND)
static_assert(false, "left hand and right hand should not both be defined. Define one or neither.")
#elif !defined(XO_SPACE_LEFTHAND) && !defined(XO_SPACE_RIGHTHAND)
#   define XO_SPACE_RIGHTHAND 1 // Default space: right hand.
#endif

#if defined(XO_SPACE_YUP) && defined(XO_SPACE_ZUP)
static_assert(false, "y-up and z-up should not both be defined. Define one or neither.");
#elif !defined(XO_SPACE_YUP) && !defined(XO_SPACE_ZUP)
#   define XO_SPACE_ZUP //Default up: z
#endif

#endif // XO_MATH_CONFIG_H_
