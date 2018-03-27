#pragma once
#include <inttypes.h>
// $inline_begin
#include <new.h>
#include <cstdlib>
// xo-math calling convention.
// See License in third-party-licenses.h for https://github.com/Microsoft/DirectXMath
// applies to the exact version checking for __vectorcall
#if defined(_MSC_VER) && !defined(_M_ARM) && \
(((_MSC_FULL_VER >= 170065501) && (_MSC_VER < 1800)) || (_MSC_FULL_VER >= 180020418))
#   define XO_CC __vectorcall
#else
#   define XO_CC
#endif

#if defined(_MSC_VER)
#   define XO_INL __forceinline
#elif defined(__clang__) || defined (__GNUC__)
#   define XO_INL __attribute__((always_inline)) inline
#else
#   define XO_INL inline
#endif

#define XO_ALN_16               alignas(16)
#define XO_ALN_16_MALLOC(size)  _aligned_malloc(size, 16)
#define XO_ALN_16_FREE(mem)     free(mem)

// will never call a constructor, as it's intended for our math types...
#define XO_NEW_DEL_16(typeName) \
    void* operator new(size_t count) { \
        void* m = XO_ALN_16_MALLOC(count); \
        return static_cast<typeName*>(m); \
    } \
    void* operator new[](size_t count) { \
        void* m = XO_ALN_16_MALLOC(count*sizeof(typeName)); \
        return static_cast<typeName*>(m); \
    } 

#define XO_UNUSED(code) (void)code