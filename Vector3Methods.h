

Vector3::Vector3() :
#if XO_SSE
    m(_mm_setzero_ps())
{
}
#else
    x(0.0f), y(0.0f), z(0.0f)
{
}
#endif

Vector3::Vector3(float f) :
#if XO_SSE
    m(_mm_set1_ps(f))
{
}
#else
    x(f), y(f), z(f), w(0.0f)
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
    m(vec.m) 
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

void Vector3::Set(float x, float y, float z) {
#if XO_SSE
    m = _mm_set_ps(0.0f, z, y, x);
#else
    this->x = x;
    this->y = y;
    this->z = z;
#endif
}

void Vector3::Set(float f) {
#if XO_SSE
    m = _mm_set1_ps(f);
#else
    this->x = f;
    this->y = f;
    this->z = f;
#endif
}

void Vector3::Set(const Vector3& vec) {
#if XO_SSE
    m = vec.m;
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
#endif
}

#if XO_SSE
void Vector3::Set(const __m128& vec) {
    m = vec;
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