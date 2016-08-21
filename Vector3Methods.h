#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector3Methods.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

Vector3::Vector3() {
}

Vector3::Vector3(float f) :
#if XO_SSE
    m(_mm_set1_ps(f))
#else
    x(f), y(f), z(f)
#endif
{
}

Vector3::Vector3(float x, float y, float z) :
#if XO_SSE
    m(_mm_set_ps(0.0f, z, y, x))
#else
    x(x), y(y), z(z)
#endif
{
}

Vector3::Vector3(const Vector3& vec) :
#if XO_SSE
    m(vec) 
#else
    x(vec.x), y(vec.y), z(vec.z) 
#endif
{
}

#if XO_SSE
Vector3::Vector3(const __m128& vec) : 
    m(vec) 
{
}
#endif

Vector3::Vector3(const Vector2& v) :
#if XO_SSE
    m(_mm_set_ps(0.0f, 0.0f, v.y, v.x))
#else
    x(v.x), y(v.y), z(0.0f)
#endif
{
}

Vector3::Vector3(const Vector4& v) :
#if XO_SSE
    m(v.m)
#else
    x(v.x), y(v.y), z(v.z)
#endif
{
}

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

_XOINL float Vector3::Sum() const {
#if XO_SSE3
    __m128 x = _mm_and_ps(m, MASK);
    x = _mm_hadd_ps(x, x);
    x = _mm_hadd_ps(x, x);
    return _mm_cvtss_f32(x);
#else
    return x+y+z;
#endif
}

float Vector3::MagnitudeSquared() const {
    Vector3 x = *this;
    x *= x;
    return x.Sum();
}

float Vector3::Magnitude() const {
    return Sqrt(MagnitudeSquared());
}

const Vector3& Vector3::Normalize() {
    float magnitude = MagnitudeSquared();
    if (CloseEnough(magnitude, 1.0f, Epsilon)) {
        return *this; // already normalized
    }
    else if (CloseEnough(magnitude, 0.0f, Epsilon)) {
        return *this; // zero vec
    }
    else {
        magnitude = Sqrt(magnitude);
        return (*this) /= magnitude;
    }
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
 
float Vector3::Dot(const Vector3& a, const Vector3& b) {
#if XO_SSE4
    return _mm_cvtss_f32(_mm_dp_ps(a, b, 0x7f));
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
 
void Vector3::Cross(const Vector3& a, const Vector3& b, Vector3& outVec) {
#if XO_SSE
    // Todo: There's a trick to do this with three shuffles. Look into that.
    __m128 l = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)));
    __m128 r = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)));
    outVec.m = _mm_sub_ps(l, r);
#else
    outVec.x = (a.y*b.z)-(a.z*b.y);
    outVec.y = (a.z*b.x)-(a.x*b.z);
    outVec.z = (a.x*b.y)-(a.y*b.x);
#endif
}

void Vector3::Max(const Vector3& a, const Vector3& b, Vector3& outVec) {
    outVec = a >= b ? a : b;
}
 
void Vector3::Min(const Vector3& a, const Vector3& b, Vector3& outVec) {
    outVec = a <= b ? a : b;
}

void Vector3::Lerp(const Vector3& a, const Vector3& b, float t, Vector3& outVec) {
    if(CloseEnough(t, 0.0f)) {
        outVec = a;
    }
    else if(CloseEnough(t, 1.0f)) {
        outVec = b;
    } 
    else {
        outVec = a + ((b - a) * t);
    }
}

void Vector3::RotateRadians(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec) {
    // Rodrigues' rotation formula
    // https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
    Vector3 axv, adv;
    float sinAng = Sin(angle);
    float cosAng = Cos(angle);
    Vector3::Cross(axis, v, axv);
    Vector3::Cross(axis, v, adv);
    outVec = v * cosAng + axv * sinAng + axis * adv * (1.0f - cosAng);
}

void Vector3::RotateDegrees(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec) {
    RotateRadians(v, axis, angle * Deg2Rad, outVec);
}

// TODO: test results with non-normalized forward. Do this for all random cone methods.
void Vector3::RandomOnConeRadians(const Vector3& forward, float angle, Vector3& outVec) {
    Vector3 crossed, tilted;
    Vector3 other = forward == Up ? Left : Up; // anything unit length but the forward vector
    Vector3::Cross(forward, other, crossed);
    Vector3::RotateRadians(forward, crossed, angle, tilted);
    Vector3::RotateRadians(tilted, forward, RandomRange(-PI, PI), outVec);
}

void Vector3::RandomInConeRadians(const Vector3& forward, float angle, Vector3& outVec) {
    Vector3 crossed, tilted;
    Vector3 other = forward == Up ? Left : Up; // anything unit length but the forward vector
    Vector3::Cross(forward, other, crossed);
    Vector3::RotateRadians(forward, crossed, RandomRange(0.0f, angle), tilted);
    Vector3::RotateRadians(tilted, forward, RandomRange(-PI, PI), outVec);
}

void Vector3::RandomOnConeDegrees(const Vector3& forward, float angle, Vector3& outVec) {
    RandomOnConeRadians(forward, angle * Deg2Rad, outVec);
}

void Vector3::RandomInConeDegrees(const Vector3& forward, float angle, Vector3& outVec) {
    RandomInConeRadians(forward, angle * Deg2Rad, outVec);
}

void Vector3::RandomOnSphere(Vector3& outVec) {
    // TODO: find a better method to do this.
    // In the off chance we randomly pick 0,0,0 this method will break.
    outVec.x = RandomRange(-1.0f, 1.0f);
    outVec.y = RandomRange(-1.0f, 1.0f);
    outVec.z = RandomRange(-1.0f, 1.0f);
    outVec.Normalize();
}

void Vector3::RandomInSphere(Vector3& outVec) {
    RandomOnSphere(outVec);
    outVec *= RandomRange(0.0f, 1.0f);
}

void Vector3::RandomAtDistance(float d, Vector3& outVec) {
    RandomOnSphere(outVec);
    outVec *= d;
}

void Vector3::RandomInDistance(float d, Vector3& outVec) {
    RandomOnSphere(outVec);
    outVec *= RandomRange(0.0f, d);
}

void Vector3::RandomInRange(float low, float high, Vector3& outVec) {
    RandomOnSphere(outVec);
    outVec *= RandomRange(low, high);
}

float Vector3::AngleRadians(const Vector3& a, const Vector3& b) {
    Vector3 cross;
    Vector3::Cross(a, b, cross);
    cross *= cross;
    return ATan2(Sqrt(cross.Sum()), Vector3::Dot(a, b));
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

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b) {
    Vector3 tempV;
    Cross(a, b, tempV);
    return tempV;
}
    
Vector3 Vector3::Max(const Vector3& a, const Vector3& b) {
    Vector3 tempV;
    Max(a, b, tempV);
    return tempV;
}
    
Vector3 Vector3::Min(const Vector3& a, const Vector3& b) {
    Vector3 tempV;
    Min(a, b, tempV);
    return tempV;
}
    
Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float t) {
    Vector3 tempV;
    Lerp(a, b, t, tempV);
    return tempV;
}
    
Vector3 Vector3::RotateRadians(const Vector3& v, const Vector3& axis, float angle) {
    Vector3 tempV;
    RotateRadians(v, axis, angle, tempV);
    return tempV;
}
    
Vector3 Vector3::RotateDegrees(const Vector3& v, const Vector3& axis, float angle) {
    Vector3 tempV;
    RotateDegrees(v, axis, angle, tempV);
    return tempV;
}
    
Vector3 Vector3::RandomOnConeRadians(const Vector3& forward, float angle) {
    Vector3 tempV;
    RandomOnConeRadians(forward, angle, tempV);
    return tempV;
}
    
Vector3 Vector3::RandomInConeRadians(const Vector3& forward, float angle) {
    Vector3 tempV;
    RandomInConeRadians(forward, angle, tempV);
    return tempV;
}
    
Vector3 Vector3::RandomOnConeDegrees(const Vector3& forward, float angle) {
    Vector3 tempV;
    RandomOnConeDegrees(forward, angle, tempV);
    return tempV;
}
    
Vector3 Vector3::RandomInConeDegrees(const Vector3& forward, float angle) {
    Vector3 tempV;
    RandomInConeDegrees(forward, angle, tempV);
    return tempV;
}
    
Vector3 Vector3::RandomOnSphere() {
    Vector3 tempV;
    RandomOnSphere(tempV);
    return tempV;
}
    
Vector3 Vector3::RandomInSphere() {
    Vector3 tempV;
    RandomInSphere(tempV);
    return tempV;
}
    
Vector3 Vector3::RandomAtDistance(float d) {
    Vector3 tempV;
    RandomAtDistance(d, tempV);
    return tempV;
}
    
Vector3 Vector3::RandomInDistance(float d) {
    Vector3 tempV;
    RandomInDistance(d, tempV);
    return tempV;
}
    
Vector3 Vector3::RandomInRange(float low, float high) {
    Vector3 tempV;
    RandomInRange(low, high, tempV);
    return tempV;
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