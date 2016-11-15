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

#if defined(_MSC_VER)
#   if defined(_M_ARM)
        // note: directx defines _XM_ARM_NEON_INTRINSICS_ if _M_ARM is defined, 
        // so we're assuming under msvc that it's all that's required to determine neon support...
#       define XO_NEON 1
#   elif defined(_M_IX86_FP)
#       if _M_IX86_FP == 1
#           define XO_SSE 1
#       elif _M_IX86_FP == 2
#           define XO_SSE 1
#           define XO_SSE2 1
#       endif
#   endif
#   if defined(__AVX__)
#       define XO_SSE 1
#       define XO_SSE2 1
#       define XO_SSE3 1
#       define XO_SSSE3 1
#       define XO_SSE4_1 1
#       define XO_SSE4_2 1
#       define XO_AVX 1
#   endif
#   if defined(__AVX2__)
#       define XO_SSE 1
#       define XO_SSE2 1
#       define XO_SSE3 1
#       define XO_SSSE3 1
#       define XO_SSE4_1 1
#       define XO_SSE4_2 1
#       define XO_AVX 1
#       define XO_AVX2 1
#   endif
//! @todo add AVX512 for msvc when it exists.
#elif defined(__clang__) || defined (__GNUC__)
#   if defined(__SSE__)
#       define XO_SSE 1
#   endif
#   if defined(__SSE2__)
#       define XO_SSE2 1
#   endif
#   if defined(__SSE3__)
#       define XO_SSE3 1
#   endif
#   if defined(__SSSE3__)
#       define XO_SSSE3 1
#   endif
#   if defined(__SSE4_1__)
#       define XO_SSE4_1 1
#   endif
#   if defined(__SSE4_2__)
#       define XO_SSE4_2 1
#   endif
#   if defined(__AVX__)
#       define XO_AVX 1
#   endif
#   if defined(__AVX2__)
#       define XO_AVX2 1
#   endif
#   if defined(__AVX512__) || defined(__AVX512F__)
#       define XO_AVX512 1
#   endif
#   if defined(__arm__)
#       if defined(__ARM_NEON__)
#           define XO_NEON 1
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
#elif defined(XO_NEON)
#   define XO_MATH_HIGHEST_SIMD "neon"
#else
#   define XO_MATH_HIGHEST_SIMD "none"
#endif


#if defined(_MSC_VER) && !defined(_XO_MATH_OBJ)
#   pragma message("xo-math simd support: " XO_MATH_HIGHEST_SIMD)

#endif