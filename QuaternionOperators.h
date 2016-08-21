#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include QuaternionOperators.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

float& Quaternion::operator [](int i) { 
  return f[i]; 
}

const float& Quaternion::operator [](int i) const { 
  return f[i]; 
}

const Quaternion& Quaternion::operator *= (const Quaternion& q) {

  return *this;
}

Quaternion Quaternion::operator * (const Quaternion& q) const {
  return Quaternion(*this) *= q;
}

bool Quaternion::operator == (const Quaternion& q) const {
#   if XO_SSE2
    return (_mm_movemask_ps(_mm_cmplt_ps(SSE::Abs(_mm_sub_ps(q.m, m)), SSE::Epsilon)) & 0b1111) == 0b1111;
#   elif XO_SSE
    // TODO: find a faster way with SSE to do a 'close enough' check.
    // I'm not sure if there's a way to do the sign bit masking like we have in SSE::Abs to acomplish
    // what we're doing in SSE2
    return 
        CloseEnough(x, q.x, SSE::SSEFloatEpsilon) && 
        CloseEnough(y, q.y, SSE::SSEFloatEpsilon) && 
        CloseEnough(z, q.z, SSE::SSEFloatEpsilon) &&
        CloseEnough(w, q.w, SSE::SSEFloatEpsilon);
#   else
    return 
        CloseEnough(x, q.x, Epsilon) && 
        CloseEnough(y, q.y, Epsilon) && 
        CloseEnough(z, q.z, Epsilon) &&
        CloseEnough(w, q.w, Epsilon);
#   endif
}

bool Quaternion::operator != (const Quaternion& q) const {
  return !((*this) == q);
}


XOMATH_END_XO_NS

#endif