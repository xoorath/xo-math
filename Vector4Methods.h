#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector4Methods.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

Vector4::Vector4() :
#if XO_SSE
    m(_mm_setzero_ps())
{
}
#else
    x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}
#endif

Vector4::Vector4(float f) :
#if XO_SSE
    m(_mm_set1_ps(f))
{
}
#else
    x(f), y(f), z(f), w(f)
{
}
#endif

Vector4::Vector4(float x, float y, float z, float w) :
#if XO_SSE
    m(_mm_set_ps(w, z, y, x))
{
}
#else
    x(x), y(y), z(z), w(w)
{
}
#endif
Vector4::Vector4(const Vector4& vec) :
#if XO_SSE
    m(vec.m)
{
}
#else
    x(vec.x), y(vec.y), z(vec.z), w(vec.w)
{
}
#endif


#if XO_SSE
Vector4::Vector4(const __m128& vec) : 
    m(vec)
{
}
#endif

Vector4::Vector4(const Vector2& v) :
#if XO_SSE
    m(_mm_set_ps(0.0f, 0.0f, v.y, v.x)) 
{
}
#else
    x(v.x), y(v.y), z(0.0f), w(0.0f)
{
}
#endif

Vector4::Vector4(const Vector3& v) :
#if XO_SSE
    m(v.m)
{
}
#else
    x(v.x), y(v.y), z(v.z), w(0.0f)
{
}
#endif

void Vector4::Set(float x, float y, float z, float w) {
#if XO_SSE
    m = _mm_set_ps(w, z, y, x);
#else
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
#endif
}

void Vector4::Set(float f) {
#if XO_SSE
    m = _mm_set1_ps(f);
#else
    this->x = f;
    this->y = f;
    this->z = f;
    this->w = w;
#endif
}

void Vector4::Set(const Vector4& vec) {
#if XO_SSE
    m = vec.m;
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = vec.w;
#endif
}

#if XO_SSE
void Vector4::Set(const __m128& vec) {
    m = vec;
}
#endif

void Vector4::Get(float& x, float& y, float& z, float& w) const {
    x = this->x;
    y = this->y;
    z = this->z;
    w = this->w;
}

void Vector4::Get(float* f) const {
#if XO_SSE
    _mm_store_ps(f, m);
#else
    f[0] = this->x;
    f[1] = this->y;
    f[2] = this->z;
    f[3] = this->w;
#endif
}
 
float Vector4::Sum() const {
#if XO_SSE
    auto s = _mm_hadd_ps(m, m);
    s = _mm_hadd_ps(s, s);
    return _mm_cvtss_f32(s);
#else
    return x+y+z+w;
#endif
}
 
float Vector4::MagnitudeSquared() const {
#if XO_SSE
    auto square = _mm_mul_ps(m, m);
    square = _mm_hadd_ps(square, square);
    square = _mm_hadd_ps(square, square);
    return _mm_cvtss_f32(square);
#else
    return (x*x) + (y*y) + (z*z) + (w*w);
#endif
}
 
float Vector4::Magnitude() const {
    return Sqrt(MagnitudeSquared());
}

const Vector4& Vector4::Normalize() {
    float magnitude = MagnitudeSquared();
    if (CloseEnough(magnitude, 1.0f, Epsilon))
        return *this; // already normalized
    if (CloseEnough(magnitude, 0.0f, Epsilon))
        return *this; // zero vec
    magnitude = Sqrt(magnitude);
    return (*this) /= magnitude;
}
 
Vector4 Vector4::Normalized() const {
    return Vector4(*this).Normalize();
}

bool Vector4::IsZero() const {
    return CloseEnough(MagnitudeSquared(), 0.0f, Epsilon);
}

bool Vector4::IsNormalized() const {
    return CloseEnough(MagnitudeSquared(), 1.0f, Epsilon);
}
 
Vector4 Vector4::Max(const Vector4& a, const Vector4& b) {
    return a >= b ? a : b;
}

Vector4 Vector4::Min(const Vector4& a, const Vector4& b) {
    return a <= b ? a : b;
}

float Vector4::Dot(const Vector4& a, const Vector4& b) {
#if XO_SSE3
    auto d = _mm_mul_ps(a.m, b.m);
    d = _mm_hadd_ps(d, d);
    d = _mm_hadd_ps(d, d);
    return _mm_cvtss_f32(d);
#elif XO_SSE
    auto d = _mm_mul_ps(a.m, b.m);
    return d.m128_f32[IDX_X] + d.m128_f32[IDX_Y] + d.m128_f32[IDX_Z] + d.m128_f32[IDX_W];
#else
    return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
#endif
}


float Vector4::DistanceSquared(const Vector4& a, const Vector4& b) {
    return (b - a).MagnitudeSquared();
}

float Vector4::Distance(const Vector4&a, const Vector4&b) {
    return (b - a).Magnitude();
}

Vector4 Vector4::Lerp(const Vector4& a, const Vector4& b, float t) {
    return a + ((b - a) * t);
}

float Vector4::Dot(const Vector4& v) const                                { return Dot(*this, v); }
float Vector4::DistanceSquared(const Vector4& v) const                    { return DistanceSquared(*this, v); }
float Vector4::Distance(const Vector4& v) const                           { return Distance(*this, v); }
Vector4 Vector4::Lerp(const Vector4& v, float t) const                    { return Lerp(*this, v, t); }


XOMATH_END_XO_NS

#endif