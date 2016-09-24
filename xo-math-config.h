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

//! Not recomended, used for the development of xo-math. 
//! This option doesn't undef the internal macros in xo-math.h
#define XO_EXPORT_ALL

#define XO_ASSERT(condition, message) //if(condition) { std::cout << message << std::endl; }

//! Use left hand space for xo-math.
//! Variants: XO_SPACE_RIGHTHAND
#define XO_SPACE_LEFTHAND

//! Use the positive y axis to represent up in xo-math.
//! Variants: XO_SPACE_ZUP
#define XO_SPACE_YUP

#endif // XO_MATH_CONFIG_H_
