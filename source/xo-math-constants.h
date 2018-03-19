#pragma once
#include <inttypes.h>
#include <limits>
// $inline_begin
namespace xo{

// please read about epsilon before using it
// see: http://realtimecollisiondetection.net/blog/?p=89
constexpr float MachineEpsilon = std::numeric_limits<float>::epsilon();

} // ::xo
