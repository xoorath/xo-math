#pragma once
#include <inttypes.h>
// $inline_begin
// xo-math calling convention.
#if defined(_MSC_VER) && !defined(_M_ARM) && (((_MSC_FULL_VER >= 170065501) && (_MSC_VER < 1800)) || (_MSC_FULL_VER >= 180020418)) // See Licence in third-party-licences for https://github.com/Microsoft/DirectXMath 
#   define XO_CC __vectorcall
#else
#   define XO_CC
#endif

#define XO_INL inline