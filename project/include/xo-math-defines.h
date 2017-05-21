#pragma once

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(__arm__)
#   if defined(__ARM_NEON__)
#       include <arm_neon.h>
#   endif
#else
#   if defined(_MSC_VER)
#       if defined(_M_ARM)
            // note: directx defines _XM_ARM_NEON_INTRINSICS_ if _M_ARM is defined, 
            // so we're assuming under msvc that it's all that's required to determine neon support...
#           include <arm_neon.h>
#       else
#           include <pmmintrin.h>
#           include <xmmintrin.h>
#       endif
#   else
#       include <x86intrin.h>
#   endif
#endif

//////////////////////////////////////////////////////////////////////
// Standard xo library defines

#if !defined(XO_NAMESPACE_BEGIN) && !defined(XO_NAMESPACE_END)
#   define XO_NAMESPACE_BEGIN namespace xo {
#   define XO_NAMESPACE_END }
#endif

//////////////////////////////////////////////////////////////////////
// Configuration

// 1 if left handed, 0 if right handed. There is no XO_MATH_RIGHTHAND.
#define XO_MATH_LEFTHAND 1
// 1 if y is up, 0 if z is up. There is no XO_MATH_Z_IS_UP
#define XO_MATH_Y_IS_UP 1

//////////////////////////////////////////////////////////////////////
// Detect SIMD
#if defined(_MSC_VER)
#   if defined(_M_ARM)
        // note: directx defines _XM_ARM_NEON_INTRINSICS_ if _M_ARM is defined, 
        // so we're assuming under msvc that it's all that's required to determine neon support...
#       define XO_NEON 1
#   elif defined(__AVX2__)
#       define XO_SSE 1
#       define XO_SSE2 1
#       define XO_SSE3 1
#       define XO_SSSE3 1
#       define XO_SSE4_1 1
#       define XO_SSE4_2 1
#       define XO_AVX 1
#       define XO_AVX2 1
#   elif defined(__AVX__)
#       define XO_SSE 1
#       define XO_SSE2 1
#       define XO_SSE3 1
#       define XO_SSSE3 1
#       define XO_SSE4_1 1
#       define XO_SSE4_2 1
#       define XO_AVX 1
#   elif defined(_M_IX86_FP)
#       if _M_IX86_FP == 1
#           define XO_SSE 1
#       elif _M_IX86_FP == 2
#           define XO_SSE 1
#           define XO_SSE2 1
#       endif
#   endif
// TODO: add AVX512 for msvc when it exists.
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

XO_NAMESPACE_BEGIN
#if defined(XO_SSE)
#   define XO_SIMD 1
    typedef __m128 VectorRegister_t;
#   define XO_SSE_ONLY(code) code
#   define XO_NEON_ONLY(code)
#   define XO_SSE_OR_NEON(ssecode, neoncode) ssecode
#elif defined(XO_NEON)
#   define XO_SIMD 1
    typedef float32x4_t VectorRegister_t;
#   define XO_SSE_ONLY(code)
#   define XO_NEON_ONLY(code) code
#   define XO_SSE_OR_NEON(ssecode, neoncode) neoncode
#endif
XO_NAMESPACE_END

//////////////////////////////////////////////////////////////////////
// Define alignment macro
#if defined(XO_NEON)
#   define XO_SIMD_ALIGNMENT 8
#elif defined(XO_SSE)
#   define XO_SIMD_ALIGNMENT 16
#else
#   define XO_SIMD_ALIGNMENT 0
#endif

#if defined(_MSC_VER)
#   define XO_ALIGN __declspec(align(XO_SIMD_ALIGNMENT))
#else
#   define XO_ALIGN __attribute__((aligned(XO_SIMD_ALIGNMENT)))
#endif

//////////////////////////////////////////////////////////////////////
// Define aligned new/delete

#if XO_SIMD_ALIGNMENT
XO_NAMESPACE_BEGIN
namespace internal {
  static inline void* alligned_malloc(size_t size) {
    // todo: support neon
    return _mm_malloc(size, XO_SIMD_ALIGNMENT);
  }
  static inline void alligned_free(void* ptr) {
    // todo: support neon
    _mm_free(ptr);
  }
}
XO_NAMESPACE_END
#   define XO_OVERLOAD_NEW_DELETE() \
  static void* operator new (size_t size)   { return internal::alligned_malloc(size); } \
  static void* operator new[] (size_t size) { return internal::alligned_malloc(size); } \
  static void operator delete (void* ptr)        { internal::alligned_free(ptr); } \
  static void operator delete[] (void* ptr)      { internal::alligned_free(ptr); }
#else
#define XO_ALIGNED_MALLOC(size) malloc(size)
#define XO_ALIGNED_FREE(ptr) free(ptr)
#   define XO_OVERLOAD_NEW_DELETE()
#endif

#if defined(XO_SIMD)
XO_NAMESPACE_BEGIN
namespace simd {
  static inline float HexFloat(uint32_t u) {
    union {
      uint32_t u;
      float f;
    } Converter;
    Converter.u = u;
    return Converter.f;
  }

  static const VectorRegister_t AbsMask = _mm_set1_ps(HexFloat(0x7fffffff));
  //static const VectorRegister_t SignMask = _mm_set1_ps(HexFloat(0x80000000));

  inline VectorRegister_t Abs(VectorRegister_t v) {
    return _mm_and_ps(AbsMask, v);
  }

  //static const VectorRegister_t Zero = XO_SSE_OR_NEON(_mm_setzero_ps, neon_todo)();
  //static const VectorRegister_t One = XO_SSE_OR_NEON(_mm_set1_ps, neon_todo)(1.0f);
  static const VectorRegister_t NegativeOne = XO_SSE_OR_NEON(_mm_set1_ps, neon_todo)(-1.0f);
  static const float Epsilon = 0.000366210938f; // the quoted error on _mm_rcp_ps documentation
  static const VectorRegister_t VectorEpsilon = _mm_set_ps1(Epsilon);

  static inline void Add(const VectorRegister_t& a, const VectorRegister_t& b, VectorRegister_t& out) {
    out = XO_SSE_OR_NEON(_mm_add_ps, vaddq_f32)(a, b);
  }
  static inline void Subtract(const VectorRegister_t& a, const VectorRegister_t& b, VectorRegister_t& out) {
    out = XO_SSE_OR_NEON(_mm_sub_ps, vsubq_f32)(a, b);
  }
  static inline void Multiply(const VectorRegister_t& a, const VectorRegister_t& b, VectorRegister_t& out) {
    out = XO_SSE_OR_NEON(_mm_mul_ps, vmulq_f32)(a, b);
  }
  static inline void Divide(const VectorRegister_t& a, const VectorRegister_t& b, VectorRegister_t& out) {
#if defined(XO_SSE)
    // see: https://software.intel.com/sites/landingpage/IntrinsicsGuide
    // see: https://software.intel.com/en-us/articles/measuring-instruction-latency-and-throughput
    
    // _mm_div_ps
    // -----------------------------------
    // Architecture    Latency Throughput
    // Haswell         13      5
    // Ivy Bridge      13      6
    // Sandy Bridge    14      14
    // Westmere        14      12
    // Nehalem         14      12

    // _mm_rcp_ps
    // -----------------------------------
    // Architecture    Latency  Throughput
    // Haswell         5        1
    // Ivy Bridge      5        1
    // Sandy Bridge    5        1
    // Westmere        3        2
    // Nehalem         3        2

    // _mm_mul_ps
    // -----------------------------------
    // Architecture    Latency  Throughput
    // Haswell         5        0.5
    // Ivy Bridge      5        1
    // Sandy Bridge    5        1
    // Westmere        4        1
    // Nehalem         4        1

#   if 0
    // If speed is a concern, switch to using _mm_mul_ps/_mm_rcp_ps
    out = _mm_div_ps(a, b);
#   else
    // If accuracy is a concern, switch to using _mm_div_ps
    out = _mm_mul_ps(a, _mm_rcp_ps(b));
#   endif

#elif defined(XO_NEON)
    VectorRegister_t r = vrecpeq_f32(b);
    r = vmulq_f32(vrecpsq_f32(a, r), r);
    out = vmulq_f32(a, r);
#endif
  }
}
XO_NAMESPACE_END
#endif

//////////////////////////////////////////////////////////////////////
// Standard extensions and replacements
XO_NAMESPACE_BEGIN

static constexpr float PI = 3.141592653589793238462643383279502884197169399375105820f;
static constexpr float PIx2 = 2.0f * PI;
static constexpr float HALFPI = PI/2.0f;
static constexpr float QUARTERPI = PI/4.0f;
static constexpr float TAU = PIx2;
static constexpr float Rad2Deg = 360.0f / TAU;
static constexpr float Deg2Rad = TAU / 360.0f;
static constexpr float Epsilon = 0.0000001192092896f;

static inline float Abs(float f)                  { return f > 0.0f ? f : -f; }
static inline float ACos(float f)                 { return acosf(f); }
static inline float ASin(float f)                 { return asinf(f); }
static inline float ATan(float f)                 { return atanf(f); } 
static inline float ATan2(float y, float x)       { return atan2f(y, x); }
static inline float Cbrt(float f)                 { return cbrtf(f); }
static inline float Cos(float f)                  { return cosf(f); }
static inline float Difference(float x, float y)  { return Abs(x-y); }
static inline float Max(float x, float y)         { return x > y ? x : y; }
static inline float Min(float x, float y)         { return x < y ? x : y; }
static inline float Sin(float f)                  { return sinf(f); }
static inline float Sqrt(float f)                 { return sqrtf(f); }
static inline float Tan(float f)                  { return tanf(f); }

// TODO: implement with simd
static inline void SinCos(float r, float& outSin, float& outCos) {
  outSin = Sin(r);
  outCos = Cos(r);
}

// TODO: implement with simd
static inline void SinCos4(float* r, float* outSin, float* outCos) {
  SinCos(*(r++), *(outSin++), *(outCos++));
  SinCos(*(r++), *(outSin++), *(outCos++));
  SinCos(*(r++), *(outSin++), *(outCos++));
  SinCos(*(r++), *(outSin++), *(outCos++));
}

static inline bool CloseEnough(float a, float b, float tolerance = Epsilon) {
  return Difference(a, b) * (1.0f/tolerance) <= Min(Abs(a), Abs(b));
}

//////////////////////////////////////////////////////////////////////

XO_NAMESPACE_END