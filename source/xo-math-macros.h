#pragma once
#include <inttypes.h>
// $inline_begin
// xo-math calling convention.
// See License in third-party-licenses.h for https://github.com/Microsoft/DirectXMath
// applies to the exact version checking for __vectorcall
#include <new.h>

#if defined(_MSC_VER) && !defined(_M_ARM) && \
(((_MSC_FULL_VER >= 170065501) && (_MSC_VER < 1800)) || (_MSC_FULL_VER >= 180020418))
#   define XO_CC __vectorcall
#else
#   define XO_CC
#endif

#define XO_INL inline

#if defined(_MSC_VER)
#define XO_ALN_16               __declspec(align(16))
#define XO_ALN_16_MALLOC(size)  _aligned_malloc(size, 16)
#define XO_ALN_16_FREE(mem)     _aligned_free(mem)
#endif