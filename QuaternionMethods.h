#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include QuaternionMethods.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

#if defined(_XO_ASSIGN_QUAT)
_XOMATH_INTERNAL_MACRO_WARNING
#   undef _XO_ASSIGN_QUAT
#endif
#if defined(_XO_ASSIGN_QUAT_Q)
_XOMATH_INTERNAL_MACRO_WARNING
#   undef _XO_ASSIGN_QUAT_Q
#endif

#if XO_SSE
#define _XO_ASSIGN_QUAT(W, X, Y, Z) m = _mm_set_ps(W, Z, Y, X);
#define _XO_ASSIGN_QUAT_Q(Q, W, X, Y, Z) Q.m = _mm_set_ps(W, Z, Y, X);
#else
#define _XO_ASSIGN_QUAT(W, X, Y, Z) this->w = W; this->x = X; this->y = Y; this->z = Z;
#define _XO_ASSIGN_QUAT_Q(Q, W, X, Y, Z) q.w = W; q.x = X; q.y = Y; q.z = Z;
#endif

Quaternion::Quaternion() {
}

Quaternion::Quaternion(const Matrix4x4& mat) {
    Vector3 xAxis(mat[0]);
    Vector3 yAxis(mat[1]);
    Vector3 zAxis(mat[2]);

    Vector3 scale(xAxis.Magnitude(), yAxis.Magnitude(), zAxis.Magnitude());

    // don't use close enough, skip the abs since we're all positive value.
    if(scale.x <= FloatEpsilon || scale.y <= FloatEpsilon || scale.z <= FloatEpsilon) {
#if XO_SSE

#else
        w = 1.0f;
        x = y = z = 0.0f;
#endif
        return; // too close.
    }

#if XO_SSE
#   if XO_NO_INVERSE_DIVISION
    Vector3 recipScale = Vector3(_mm_div_ps(Vector4::One.m, scale.m));
#   else
    Vector3 recipScale = Vector3(_mm_rcp_ps(scale.m));
#   endif
#else
    Vector3 recipScale = Vector3(1.0f/scale.x, 1.0f/scale.y, 1.0f/scale.z);
#endif
    xAxis *= recipScale.x;
    yAxis *= recipScale.y;
    zAxis *= recipScale.z;

    // Now calculate the rotation from the resulting matrix (axes).
    float trace = xAxis.x + yAxis.y + zAxis.z + 1.0f;

    if (trace > 1.0f)
    {
        float s = 0.5f / Sqrt(trace);
        _XO_ASSIGN_QUAT(
            0.25f / s,
            (yAxis.z- zAxis.y) * s,
            (xAxis.y - yAxis.x) * s,
            (zAxis.x - xAxis.z) * s);
    }
    else
    {
        if (xAxis.x > yAxis.y && xAxis.x > zAxis.z)
        {
            float s = 0.5f / Sqrt(1.0f + xAxis.x - yAxis.y - zAxis.z);
            _XO_ASSIGN_QUAT(
                (yAxis.z - zAxis.y) * s,
                0.25f / s,
                (zAxis.x + xAxis.z) * s,
                (yAxis.x + xAxis.y) * s);
        }
        else if (yAxis.y > zAxis.z)
        {
            float s = 0.5f / Sqrt(1.0f + yAxis.y - xAxis.x - zAxis.z);
            _XO_ASSIGN_QUAT(
                (zAxis.x - xAxis.z) * s,
                (yAxis.x + xAxis.y) * s,
                (zAxis.y + yAxis.z) * s,
                0.25f / s);
        }
        else
        {
            float s = 0.5f / Sqrt(1.0f + zAxis.z - xAxis.x - yAxis.y );
            _XO_ASSIGN_QUAT(
                (xAxis.y - yAxis.x ) * s,
                (zAxis.x + xAxis.z ) * s,
                0.25f / s,
                (zAxis.y + yAxis.z ) * s);
        }
    }
}

Quaternion::Quaternion(float x, float y, float z, float w)  :
#if XO_SSE
    m(_mm_set_ps(w, z, y, x))
#else
    x(x), y(y), z(z), w(w)
#endif
{
}

Quaternion Quaternion::Inverse() const {
    return Quaternion(*this).MakeInverse();
}

const Quaternion& Quaternion::MakeInverse() {
#if XO_SSE
    __m128 square = _mm_mul_ps(m, m);
    square = _mm_hadd_ps(square, square);
    square = _mm_hadd_ps(square, square);
    float n = _mm_cvtss_f32(square);
#else
    float n = x * x + y * y + z * z + w * w;
#endif
    
    if (CloseEnough(n, 1.0f))
    {
        // TODO: is there an intrinsic to negate
        x = -x;
        y = -y;
        z = -z;
        w = w;
        return *this;
    }
    if (CloseEnough(n, 0.0f))
        return *this;

#if XO_SSE
#   if XO_NO_INVERSE_DIVISION
    square = _mm_rcp_ps(square);
#   else
    // Todo: make and use SSE::One
    square = _mm_div_ps(Vector4::One.m, square);
#   endif
    x = -x;
    y = -y;
    z = -z;
    w = w;
    m = _mm_mul_ps(m, square);
#else
#   if XO_NO_INVERSE_DIVISION
    x = -x / n;
    y = -y / n;
    z = -z / n;
    w = w / n;
#   else
    n = 1.0f / n;
    x = -x * n;
    y = -y * n;
    z = -z * n;
    w = w * n;
#   endif
#endif
    return *this;
}

Quaternion Quaternion::Normalized() const {
    return Quaternion(*this).Normalize();
}

const Quaternion& Quaternion::Normalize() {
#if XO_SSE
    __m128 square = _mm_mul_ps(m, m);
    square = _mm_hadd_ps(square, square);
    square = _mm_hadd_ps(square, square);
    float n = _mm_cvtss_f32(square);
#else
    float n = x * x + y * y + z * z + w * w;
#endif
    // Already normalized.
    if (CloseEnough(n, 1.0f))
        return *this;

    n = Sqrt(n);
    // Too close to zero.
    if (CloseEnough(n, 0.0f))
        return *this;

    // let it take care of sse and no_inverse_division
    (*(Vector4*)this) /= n;

    return *this;
}

void Quaternion::RotationRadians(float x, float y, float z, Quaternion& outQuat) {
    RotationRadians(Vector3(x, y, z), outQuat);
}

void Quaternion::RotationRadians(const Vector3& v, Quaternion& outQuat) {
    Vector3 hv = v * 0.5f;
    // TODO: use intrinsics for sin/cos here
    Vector3 vs(Sin(hv.x), Sin(hv.y), Sin(hv.z));
    Vector3 vc(Sin(hv.x), Sin(hv.y), Sin(hv.z));
    _XO_ASSIGN_QUAT_Q(outQuat,
        vc.x * vc.y * vc.z + vs.x * vs.y * vs.z,
        vs.x * vc.y * vc.z - vc.x * vs.y * vs.z,
        vc.x * vs.y * vc.z + vs.x * vc.y * vs.z,
        vc.x * vc.y * vs.z - vs.x * vs.y * vc.z);
}

void Quaternion::AxisAngleRadians(const Vector3& axis, float radians, Quaternion& outQuat) {
    float hr = radians * 0.5f;
    float sr = Sin(hr);

    Vector3 n = axis.Normalized();
    n *= hr;
    // TODO: Assign with intrinsics here. We already have n.m equal to three quarters of the m128 we want.
    _XO_ASSIGN_QUAT_Q(outQuat, Cos(radians), n.x, n.y, n.z);
}

void Quaternion::LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up, Quaternion& outQuat) {
    LookAtFromDirection(to-from, up, outQuat);
}

void Quaternion::LookAtFromPosition(const Vector3& from, const Vector3& to, Quaternion& outQuat) {
    LookAtFromPosition(from, to, Vector3::Up, outQuat);
}

void Quaternion::LookAtFromDirection(const Vector3& direction, const Vector3& up, Quaternion& outQuat) {
    // Todo
}

void Quaternion::LookAtFromDirection(const Vector3& direction, Quaternion& outQuat) {
    LookAtFromDirection(direction, Vector3::Up, outQuat);
}

void Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t, Quaternion& outQuat) {
    // Fast slerp implementation by kwhatmough:
    // It contains no division operations, no trig, no inverse trig
    // and no sqrt. Not only does this code tolerate small constraint
    // errors in the input quaternions, it actually corrects for them.
    if (CloseEnough(t, 0.0f))
    {
        outQuat = a;
        return;
    }
    else if (CloseEnough(t, 1.0f))
    {
        outQuat = b;
        return;
    }
    if (a == b)
    {
        outQuat = a;
        return;
    }

    float halfY, alpha, beta;
    float u, f1, f2a, f2b;
    float ratio1, ratio2;
    float halfSecHalfTheta, versHalfTheta;
    float sqNotU, sqU;

    Vector4 * va = (Vector4*)&a;
    Vector4 * vb = (Vector4*)&b;

    float cosTheta = ((*va) * (*vb)).Sum();

    // As usual in all slerp implementations, we fold theta.
    alpha = cosTheta >= 0 ? 1.0f : -1.0f;
    halfY = 1.0f + alpha * cosTheta;

    // Here we bisect the interval, so we need to fold t as well.
    f2b = t - 0.5f;
    u = f2b >= 0 ? f2b : -f2b;
    f2a = u - f2b;
    f2b += u;
    u += u;
    f1 = 1.0f - u;

    // One iteration of Newton to get 1-cos(theta / 2) to good accuracy.
    halfSecHalfTheta = 1.09f - (0.476537f - 0.0903321f * halfY) * halfY;
    halfSecHalfTheta *= 1.5f - halfY * halfSecHalfTheta * halfSecHalfTheta;
    versHalfTheta = 1.0f - halfY * halfSecHalfTheta;

    // Evaluate series expansions of the coefficients.
    sqNotU = f1 * f1;
    ratio2 = 0.0000440917108f * versHalfTheta;
    ratio1 = -0.00158730159f + (sqNotU - 16.0f) * ratio2;
    ratio1 = 0.0333333333f + ratio1 * (sqNotU - 9.0f) * versHalfTheta;
    ratio1 = -0.333333333f + ratio1 * (sqNotU - 4.0f) * versHalfTheta;
    ratio1 = 1.0f + ratio1 * (sqNotU - 1.0f) * versHalfTheta;

    sqU = u * u;
    ratio2 = -0.00158730159f + (sqU - 16.0f) * ratio2;
    ratio2 = 0.0333333333f + ratio2 * (sqU - 9.0f) * versHalfTheta;
    ratio2 = -0.333333333f + ratio2 * (sqU - 4.0f) * versHalfTheta;
    ratio2 = 1.0f + ratio2 * (sqU - 1.0f) * versHalfTheta;

    // Perform the bisection and resolve the folding done earlier.
    f1 *= ratio1 * halfSecHalfTheta;
    f2a *= ratio2;
    f2b *= ratio2;
    alpha *= f1 + f2a;
    beta = f1 + f2b;

    // Apply final coefficients to a and b as usual.
    float w = alpha * a.w + beta * b.w;
    float x = alpha * a.x + beta * b.x;
    float y = alpha * a.y + beta * b.y;
    float z = alpha * a.z + beta * b.z;

    // This final adjustment to the quaternion's length corrects for
    // any small constraint error in the inputs q1 and q2 But as you
    // can see, it comes at the cost of 9 additional multiplication
    // operations. If this error-correcting feature is not required,
    // the following code may be removed.
    f1 = 1.5f - 0.5f * (w * w + x * x + y * y + z * z);
    _XO_ASSIGN_QUAT_Q(outQuat, w * f1, x * f1, y * f1, z * f1);
}

void Quaternion::Lerp(const Quaternion& a, const Quaternion& b, float t, Quaternion& outQuat) {
    if (CloseEnough(t, 0.0f))
    {
        outQuat = a;
        return;
    }
    else if (CloseEnough(t, 1.0f))
    {
        outQuat = b;
        return;
    }
    else {
        Vector4* vq = (Vector4*)&outQuat;
        Vector4* va = (Vector4*)&a;
        Vector4* vb = (Vector4*)&b;
        // Todo: give the vectors the same static 'outQuat' type interface, then just use the Vector4::Lerp for this lerp
        (*vq) = *va + ((*vb - *va) * t);
    }
}

void Quaternion::GetAxisAngleRadians(Vector3& axis, float& radians) const {
    
    Quaternion q = Normalized();

#if XO_SSE
    axis.m = q.m;
#else
    axis.x = q.x;
    axis.y = q.y;
    axis.z = q.z;
    axis.Normalize();
#endif
    radians = (2.0f * ACos(q.w));
}

#undef _XO_ASSIGN_QUAT
#undef _XO_ASSIGN_QUAT_Q

XOMATH_END_XO_NS

#endif