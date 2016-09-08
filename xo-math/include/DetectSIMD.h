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

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include DetectSIMD.h directly. Include GameMath.h.");
#else // XOMATH_INTERNAL

#if !defined(XO_SSE) && !defined(XO_SSE2) && !defined(XO_SSE3) && !defined(XO_SSSE3) && !defined(XO_SSE4_1) && !defined(XO_SSE4_2) && !defined(XO_AVX) && !defined(XO_AVX2) && !defined(XO_AVX512)
#   if defined(_MSC_VER)
#       if defined(_M_IX86_FP)
#           if _M_IX86_FP == 1
#               define XO_SSE 1
#           elif _M_IX86_FP == 2
#               define XO_SSE 1
#               define XO_SSE2 1
#           endif
#       endif
#       if defined(__AVX__)
#           define XO_SSE 1
#           define XO_SSE2 1
#           define XO_SSE3 1
#           define XO_SSSE3 1
#           define XO_SSE4_1 1
#           define XO_SSE4_2 1
#           define XO_AVX 1
#       endif
#       if defined(__AVX2__)
#           define XO_SSE 1
#           define XO_SSE2 1
#           define XO_SSE3 1
#           define XO_SSSE3 1
#           define XO_SSE4_1 1
#           define XO_SSE4_2 1
#           define XO_AVX 1
#           define XO_AVX2 1
#       endif
// TODO: add AVX512 for msvc when it exists.
#   elif defined(__clang__) || defined (__GNUC__)
#       if defined(__SSE__)
#           define XO_SSE 1
#       endif
#       if defined(__SSE2__)
#           define XO_SSE2 1
#       endif
#       if defined(__SSE3__)
#           define XO_SSE3 1
#       endif
#       if defined(__SSSE3__)
#           define XO_SSSE3 1
#       endif
#       if defined(__SSE4_1__)
#           define XO_SSE4_1 1
#       endif
#       if defined(__SSE4_2__)
#           define XO_SSE4_2 1
#       endif
#       if defined(__AVX__)
#           define XO_AVX 1
#       endif
#       if defined(__AVX2__)
#           define XO_AVX2 1
#       endif
#       if defined(__AVX512__) || defined(__AVX512F__)
#           define XO_AVX512 1
#       endif
#   endif
// If the user really knows what they're doing, they should be able to override seriously any of these defines they want.
// If XO_NO_VECTOR_DETECT_WARNING is not defined then we'll tell them about the lack of support on their platform 
#elif !defined(XO_NO_VECTOR_DETECT_WARNING)
#   if defined(_MSC_VER)
#       if defined(_XO_MATH_MSC_PRAGMALINK)
_XOMATH_INTERNAL_MACRO_WARNING
#       else
#           define _XO_MATH_MSC_PRAGMALINK __FILE__ "(" _XO_MATH_STRINGIFY(__LINE__) ") : "
#       endif
#       if defined(_XO_MATH_WARN)
_XOMATH_INTERNAL_MACRO_WARNING
#       else
#           define _XO_MATH_WARN(exp) (_XO_MATH_MSC_PRAGMALINK "WARNING: " exp)
#       endif

#       if !(defined(__AVX__) || defined(__AVX2__) || defined(_M_IX86_FP) || _M_IX86_FP < 1) && defined(XO_SSE)
#           undef XO_SSE
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSE is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__) || defined(_M_IX86_FP) || _M_IX86_FP < 2) && defined(XO_SSE2)
#           undef XO_SSE2
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSE2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_SSE3)
#           undef XO_SSE3
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSE3 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_SSSE3)
#           undef XO_SSSE3
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSSE3 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_SSE4_1)
#           undef XO_SSE4_1
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSE4_1 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_SSE4_2)
#           undef XO_SSE4_2
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSE4_2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_AVX)
#           undef XO_AVX
#           pragma message _XO_MATH_WARN("xo-math detected that XO_AVX is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !defined(__AVX2__) && defined(XO_AVX2)
#           undef XO_AVX2
#           pragma message _XO_MATH_WARN("xo-math detected that XO_AVX2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
        // Hey, if you know for a fact that AVX512 is supported on your version of msvc
        // could you please find out if there's a macro to detect it? (such as __AVX512__?)
        // At the time of writing, there is no such macro documented.
        // If there is I'd appreciate a quick email or pull request. I can be reached at jared@xoorath.com
        
        // Assuming __AVX512__ is the macro for example, you can substitute:

//#     if defined(XO_AVX512)
        // for
//#     !defined(__AVX512__) && defined(XO_AVX512)

        // to get your code to stop throwing this warning.
#       if defined(XO_AVX512)
#           undef XO_AVX512
#           pragma message _XO_MATH_WARN("xo-math detected that XO_AVX512 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif

#   undef _XO_MATH_MSC_PRAGMALINK
#   undef _XO_MATH_WARN
#   elif defined(__clang__) || defined (__GNUC__)
#       if !defined(__SSE__) && defined(XO_SSE)
#           undef XO_SSE
#           warning "xo-math detected that XO_SSE is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__SSE2__) && defined(XO_SSE2)
#           undef XO_SSE2
#           warning "xo-math detected that XO_SSE2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__SSE3__) && defined(XO_SSE3)
#           undef XO_SSE3
#           warning "xo-math detected that XO_SSE3 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__SSSE3__) && defined(XO_SSSE3)
#           undef XO_SSSE3
#           warning "xo-math detected that XO_SSSE3 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__SSE4_1__) && defined(XO_SSE4_1)
#           undef XO_SSE4_1
#           warning "xo-math detected that XO_SSE4_1 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__SSE4_2__) && defined(XO_SSE4_2)
#           undef XO_SSE4_2
#           warning "xo-math detected that XO_SSE4_2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__AVX__) && defined(XO_AVX)
#           undef XO_AVX
#           warning "xo-math detected that XO_AVX is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__AVX2__) && defined(XO_AVX2)
#           undef XO_AVX2
#           warning "xo-math detected that XO_AVX2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__AVX512__) && defined(XO_AVX512)
#           undef XO_AVX512
#           warning "xo-math detected that XO_AVX512 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#   endif
#endif

#if defined(XO_AVX512)
#   define XO_MATH_HIGHEST_SIMD "avx512"
#elif defined(XO_AVX2)
#   define XO_MATH_HIGHEST_SIMD "avx2"
#elif defined(XO_AVX)
#   define XO_MATH_HIGHEST_SIMD "avx"
#elif defined(XO_SSE4_2)
#   define XO_MATH_HIGHEST_SIMD "sse4.2"
#elif defined(XO_SSE4_1)
#   define XO_MATH_HIGHEST_SIMD "sse4.1"
#elif defined(XO_SSSE3)
#   define XO_MATH_HIGHEST_SIMD "ssse3"
#elif defined(XO_SSE3)
#   define XO_MATH_HIGHEST_SIMD "sse3"
#elif defined(XO_SSE2)
#   define XO_MATH_HIGHEST_SIMD "sse2"
#elif defined(XO_SSE)
#   define XO_MATH_HIGHEST_SIMD "sse"
#else
#   define XO_MATH_HIGHEST_SIMD "none"
#endif

#endif // XOMATH_INTERNAL