#pragma once
#include <inttypes.h>
// $inline_begin
namespace xo { namespace simd {

#define XO_SSE_NONE 0x00
#define XO_SSE1     0x10
#define XO_SSE2     0x20
#define XO_SSE3     0x30
#define XO_SSSE3    0x31
#define XO_SSE4_1   0x41
#define XO_SSE4_2   0x42
#define XO_AVX      0x50
#define XO_AVX2     0x60
#define XO_AVX512   0x70

#if defined(_MSC_VER)
#   if defined(__AVX512__) // this define is a guess.
#       define XO_SSE_CURRENT XO_AVX512
#   elif defined(__AVX2__)
#       define XO_SSE_CURRENT XO_AVX2
#   elif defined(__AVX__)
#       define XO_SSE_CURRENT XO_AVX
#   elif defined(_M_IX86_FP)
#       if _M_IX86_FP == 1
#           define XO_SSE_CURRENT XO_SSE1
#       elif _M_IX86_FP == 2
#           define XO_SSE_CURRENT XO_SSE2
#       else
#           define XO_SSE_CURRENT XO_SSE1
#       endif
#   else
#       define XO_SSE_CURRENT XO_SSE_NONE
#       define XO_HAS_SSE 0
#   endif
#elif defined(__clang__) || defined (__GNUC__)
#   if defined(__AVX512__) || defined(__AVX512F__)
#       define XO_SSE_CURRENT XO_AVX512
#   elif defined(__AVX2__)
#       define XO_SSE_CURRENT XO_AVX2
#   elif defined(__AVX__)
#       define XO_SSE_CURRENT XO_AVX
#   elif defined(__SSE4_2__)
#       define XO_SSE_CURRENT XO_SSE4_2
#   elif defined(__SSE4_1__)
#       define XO_SSE_CURRENT XO_SSE4_1
#   elif defined(__SSSE3__)
#       define XO_SSE_CURRENT XO_SSSE3
#   elif defined(__SSE3__)
#       define XO_SSE_CURRENT XO_SSE3
#   elif defined(__SSE2__)
#       define XO_SSE_CURRENT XO_SSE2
#   elif defined(__SSE__)
#       define XO_SSE_CURRENT XO_SSE1
#else
#       define XO_SSE_CURRENT XO_SSE_NONE
#       define XO_HAS_SSE 0
#   endif
#else
#   define XO_SSE_CURRENT XO_SSE_NONE
#   define XO_HAS_SSE 0
#endif

#if !defined(XO_HAS_SSE)
#    define XO_HAS_SSE 1
#endif

enum class eXO_SSE : uint8_t
{
    eXO_SSE_NONE    = XO_SSE_NONE,
    eXO_SSE1        = XO_SSE1,
    eXO_SSE2        = XO_SSE2,
    eXO_SSE3        = XO_SSE3,
    eXO_SSSE3       = XO_SSSE3,
    eXO_SSE4_1      = XO_SSE4_1,
    eXO_SSE4_2      = XO_SSE4_2,
    eXO_AVX         = XO_AVX,
    eXO_AVX2        = XO_AVX2,
    eXO_AVX512      = XO_AVX512,
    eXO_SSE_CURRENT = XO_SSE_CURRENT
};

constexpr char const* SSEGetName(eXO_SSE version = eXO_SSE::eXO_SSE_CURRENT)
{
    switch (version)
    {
    case eXO_SSE::eXO_SSE_NONE: return "none";
    case eXO_SSE::eXO_SSE1:     return "SSE1";
    case eXO_SSE::eXO_SSE2:     return "SSE2";
    case eXO_SSE::eXO_SSE3:     return "SSE3";
    case eXO_SSE::eXO_SSSE3:    return "SSSE3";
    case eXO_SSE::eXO_SSE4_1:   return "SSE4.1";
    case eXO_SSE::eXO_SSE4_2:   return "SSE4.2";
    case eXO_SSE::eXO_AVX:      return "AVX";
    case eXO_SSE::eXO_AVX2:     return "AVX2";
    case eXO_SSE::eXO_AVX512:   return "AVX512";
    default:                    return "unknown";
    }
}

constexpr char const* SSEVersionName = SSEGetName();

#define XO_NEON_NONE 0x00
#define XO_NEON7     0x70

#if XO_HAS_SSE == 0
#   if defined(_MSC_VER)
#       if defined(_M_ARM)
#           define XO_NEON_CURRENT XO_NEON7
#       else
#           define XO_NEON_CURRENT XO_NEON_NONE
#           define XO_HAS_NEON 0
#       endif
#   elif defined(__clang__) || defined (__GNUC__)
#       if defined(__arm__) && defined(__ARM_NEON__)
#           define XO_NEON_CURRENT XO_NEON7
#       else
#           define XO_NEON_CURRENT XO_NEON_NONE
#           define XO_HAS_NEON 0
#       endif
#   else
#       define XO_NEON_CURRENT XO_NEON_NONE
#       define XO_HAS_NEON 0
#   endif
#else
#   define XO_NEON_CURRENT XO_NEON_NONE
#   define XO_HAS_NEON 0
#endif

#if !defined(XO_HAS_NEON)
#    define XO_HAS_NEON 1
#endif

enum class eXO_NEON : uint8_t
{
    eXO_NEON_NONE    = XO_NEON_NONE,
    eXO_NEON7        = XO_NEON7,
    eXO_NEON_CURRENT = XO_NEON_CURRENT
};

constexpr char const* NEONGetName(eXO_NEON version = eXO_NEON::eXO_NEON_CURRENT)
{
    switch (version)
    {
    case eXO_NEON::eXO_NEON_NONE: return "none";
    case eXO_NEON::eXO_NEON7:     return "v7";
    default:                      return "unknown";
    }
}

constexpr char const* NEONVersionName = NEONGetName();

} } // ::xo::simd