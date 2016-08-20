#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector3Methods.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

Vector3::Vector3() {
}

Vector3::Vector3(float f) :
#if XO_SSE
    m(_mm_set1_ps(f))
{
}
#else
    x(f), y(f), z(f)
{
}
#endif

Vector3::Vector3(float x, float y, float z) :
#if XO_SSE
    m(_mm_set_ps(0.0f, z, y, x))
{
}
#else
    x(x), y(y), z(z)
{
}
#endif

Vector3::Vector3(const Vector3& vec) :
#if XO_SSE
    m(vec) 
{
}
#else
    x(vec.x), y(vec.y), z(vec.z) 
{ 
}
#endif

#if XO_SSE
Vector3::Vector3(const __m128& vec) : m(vec) {

}
#endif

Vector3::Vector3(const Vector2& v) :
#if XO_SSE
    m(_mm_set_ps(0.0f, 0.0f, v.y, v.x))
{
}
#else
    x(v.x), y(v.y), z(0.0f)
{
}
#endif

Vector3::Vector3(const Vector4& v) :
#if XO_SSE
    m(v.m)
{
}
#else
    x(v.x), y(v.y), z(v.z)
{
}
#endif

const Vector3& Vector3::Set(float x, float y, float z) {
#if XO_SSE
    m = _mm_set_ps(0.0f, z, y, x);
#else
    this->x = x;
    this->y = y;
    this->z = z;
#endif
    return *this;
}

const Vector3& Vector3::Set(float f) {
#if XO_SSE
    m = _mm_set1_ps(f);
#else
    this->x = f;
    this->y = f;
    this->z = f;
#endif
    return *this;
}

const Vector3& Vector3::Set(const Vector3& vec) {
#if XO_SSE
    m = vec.m;
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
#endif
    return *this;
}

#if XO_SSE
const Vector3& Vector3::Set(const __m128& vec) {
    m = vec;
    return *this;
}
#endif

void Vector3::Get(float& x, float& y, float &z) const {
    x = this->x;
    y = this->y;
    z = this->z;
}

void Vector3::Get(float* f) const {
#if XO_SSE
    _mm_store_ps(f, m);
#else
    f[0] = this->x;
    f[1] = this->y;
    f[2] = this->z;
#endif
}


Vector3 Vector3::ZYX() const {
#if XO_SSE
    return Vector3(_mm_shuffle_ps(m, m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Y, IDX_Z)));
#else
    return Vector3(z, y, x);
#endif
}

float Vector3::MagnitudeSquared() const {
#if XO_SSE3
    auto square = _mm_and_ps(_mm_mul_ps(m, m), MASK);
    square = _mm_hadd_ps(square, square);
    square = _mm_hadd_ps(square, square);
    return _mm_cvtss_f32(square);
#elif XO_SSE
    auto square = _mm_mul_ps(m, m);
    _MM_ALIGN16 float t[4];
    _mm_store_ps(t, square);
    return t[IDX_X] + t[IDX_Y] + t[IDX_Z];
#else
    return (x*x) + (y*y) + (z*z);
#endif
}


float Vector3::Magnitude() const {
    return Sqrt(MagnitudeSquared());
}

const Vector3& Vector3::Normalize() {
    float magnitude = MagnitudeSquared();
    if (CloseEnough(magnitude, 1.0f, Epsilon))
        return *this; // already normalized
    if (CloseEnough(magnitude, 0.0f, Epsilon))
        return *this; // zero vec
    magnitude = Sqrt(magnitude);
    return (*this) /= magnitude;
}


Vector3 Vector3::Normalized() const {
    return Vector3(*this).Normalize();
}


bool Vector3::IsZero() const {
    return CloseEnough(MagnitudeSquared(), 0.0f, Epsilon);
}


bool Vector3::IsNormalized() const {
    return CloseEnough(MagnitudeSquared(), 1.0f, Epsilon);
}
 
Vector3 Vector3::Max(const Vector3& a, const Vector3& b) {
    return a >= b ? a : b;
}
 
Vector3 Vector3::Min(const Vector3& a, const Vector3& b) {
    return a <= b ? a : b;
}
 
float Vector3::Dot(const Vector3& a, const Vector3& b) {
#if XO_SSE4
    return _mm_cvtss_f32(_mm_dp_ps(a, b, 0xff));
#elif XO_SSE3
    auto d = _mm_and_ps(_mm_mul_ps(a.m, b.m), MASK);
    d = _mm_hadd_ps(d, d);
    d = _mm_hadd_ps(d, d);
    return _mm_cvtss_f32(d);
#elif XO_SSE
    auto d = _mm_mul_ps(a.m, b.m);
    _MM_ALIGN16 float t[4];
    _mm_store_ps(t, d);
    return t[IDX_X] + t[IDX_Y] + t[IDX_Z];
#else
    return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
#endif
}
 
Vector3 Vector3::Cross(const Vector3& a, const Vector3& b) {
#if XO_SSE
        // Todo: There's a trick to do this with three shuffles. Look into that.
        auto l = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)));
        auto r = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)));
        return Vector3(_mm_sub_ps(l, r));
#else
        return Vector3((a.y*b.z)-(a.z*b.y), (a.z*b.x)-(a.x*b.z), (a.x*b.y)-(a.y*b.x));
#endif
}

float Vector3::AngleRadians(const Vector3& a, const Vector3& b) {
#if XO_SSE3
    auto cross = Cross(a, b).m;
    cross = _mm_and_ps(_mm_mul_ps(cross, cross), MASK);
    cross = _mm_hadd_ps(cross, cross);
    cross = _mm_hadd_ps(cross, cross);
    return ATan2(Sqrt(_mm_cvtss_f32(cross)), Dot(a, b));
#elif XO_SSE
    auto cross = Cross(a, b).m;
    cross = _mm_mul_ps(cross, cross);
    _MM_ALIGN16 float t[4];
    _mm_store_ps(t, cross);
    return ATan2(Sqrt(t[IDX_X] + t[IDX_Y] + t[IDX_Z]), Dot(a, b));
#else
    auto cross = Cross(a, b);
    cross *= cross;
    return ATan2(Sqrt(cross.x + cross.y + cross.z), Dot(a, b));
#endif
}

float Vector3::AngleDegrees(const Vector3& a, const Vector3& b) {
    return AngleRadians(a, b) * Rad2Deg;
}

float Vector3::DistanceSquared(const Vector3& a, const Vector3& b) {
    return (b - a).MagnitudeSquared();
}
 
float Vector3::Distance(const Vector3&a, const Vector3&b) {
    return (b - a).Magnitude();
}

Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float t) {
    return a + ((b - a) * t);
}

Vector3 Vector3::RotateRadians(const Vector3& v, const Vector3& axis, float angle) {
    // Rodrigues' rotation formula
    // https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
    auto sinAng = Sin(angle);
    auto cosAng = Cos(angle);
    return v * cosAng + axis.Cross(v) * sinAng + axis * axis.Dot(v) * (1.0f - cosAng);
}

Vector3 Vector3::RotateDegrees(const Vector3& v, const Vector3& axis, float angle) {
    return RotateRadians(v, axis, angle * Deg2Rad);
}

Vector3 Vector3::RandomOnConeRadians(const Vector3& forward, float angle) {
    auto cross = forward.Cross(forward == Up ? Left : Up); // cross anything but itself will do. We just need an orthogonal vector
    auto tilted = forward.RotateRadians(cross, angle);
    return tilted.RotateRadians(forward, RandomRange(-PI, PI));
}

Vector3 Vector3::RandomInConeRadians(const Vector3& forward, float angle) {
    auto cross = forward.Cross(forward == Up ? Left : Up); // cross anything but itself will do. We just need an orthogonal vector
    auto tilted = forward.RotateRadians(cross, RandomRange(0.0f, angle));
    return tilted.RotateRadians(forward, RandomRange(-PI, PI));
}

Vector3 Vector3::RandomOnConeDegrees(const Vector3& forward, float angle) {
    return RandomOnConeRadians(forward, angle * Deg2Rad);
}

Vector3 Vector3::RandomInConeDegrees(const Vector3& forward, float angle) {
    return RandomInConeRadians(forward, angle * Deg2Rad);
}

Vector3 Vector3::RandomOnSphere() {
    return Vector3(
        RandomRange(-1.0f, 1.0f), 
        RandomRange(-1.0f, 1.0f), 
        RandomRange(-1.0f, 1.0f)).Normalized();
}

Vector3 Vector3::RandomInSphere() {
    return RandomOnSphere() * RandomRange(0.0f, 1.0f);
}

Vector3 Vector3::RandomAtDistance(float d) {
    return Vector3(
        RandomRange(-1.0f, 1.0f),
        RandomRange(-1.0f, 1.0f),
        RandomRange(-1.0f, 1.0f)).Normalized() * d;
}

Vector3 Vector3::RandomInDistance(float d) {
    return Vector3(
        RandomRange(-1.0f, 1.0f),
        RandomRange(-1.0f, 1.0f),
        RandomRange(-1.0f, 1.0f)).Normalized() * RandomRange(0.0f, d);
}

Vector3 Vector3::RandomInRange(float low, float high) {
    return Vector3(
        RandomRange(-1.0f, 1.0f),
        RandomRange(-1.0f, 1.0f),
        RandomRange(-1.0f, 1.0f)).Normalized() * RandomRange(low, high);
}

float Vector3::Dot(const Vector3& v) const                                { return Dot(*this, v); }


Vector3 Vector3::Cross(const Vector3& v) const                            { return Cross(*this, v); }

float Vector3::AngleRadians(const Vector3& v) const                       { return AngleRadians(*this, v); }

float Vector3::AngleDegrees(const Vector3& v) const                       { return AngleDegrees(*this, v); }

float Vector3::DistanceSquared(const Vector3& v) const                    { return DistanceSquared(*this, v); }

float Vector3::Distance(const Vector3& v) const                           { return Distance(*this, v); }

Vector3 Vector3::Lerp(const Vector3& v, float t) const                    { return Lerp(*this, v, t); }

Vector3 Vector3::RotateRadians(const Vector3& axis, float angle) const    { return RotateRadians(*this, axis, angle); }

Vector3 Vector3::RotateDegrees(const Vector3& axis, float angle) const    { return RotateDegrees(*this, axis, angle); }

Vector3 Vector3::RandomOnConeRadians(float angle) const                   { return RandomOnConeRadians(*this, angle); }

Vector3 Vector3::RandomInConeRadians(float angle) const                   { return RandomInConeRadians(*this, angle); }

Vector3 Vector3::RandomOnConeDegrees(float angle) const                   { return RandomOnConeDegrees(*this, angle); }

Vector3 Vector3::RandomInConeDegrees(float angle) const                   { return RandomInConeDegrees(*this, angle); }

XOMATH_END_XO_NS

#endif