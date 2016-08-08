#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector4.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS
// TODO: align new/delete
class _MM_ALIGN16 Vector4 {
#if XO_SSE
        static const unsigned IDX_X = 0;
        static const unsigned IDX_Y = 1;
        static const unsigned IDX_Z = 2;
        static const unsigned IDX_W = 3;
#endif

public:
    constexpr static const float Epsilon = FloatEpsilon * 4.0f;

    _XOINL 
    Vector4() :
#if XO_SSE
        m(_mm_setzero_ps()) { }
#else
        x(0.0f), y(0.0f), z(0.0f), w(0.0f) { }
#endif

    _XOINL 
    Vector4(float f) :
#if XO_SSE
        m(_mm_set1_ps(f)) { }
#else
        x(f), y(f), z(f), w(f) { }
#endif

    _XOINL 
    Vector4(float x, float y, float z, float w) :
#if XO_SSE
        m(_mm_set_ps(w, z, y, x)) { }
#else
        x(x), y(y), z(z), w(w) { }
#endif
    _XOINL 
    Vector4(const Vector4& vec) :
#if XO_SSE
        m(vec.m) { }
 #else
        x(vec.x), y(vec.y), z(vec.z), w(vec.w) { }
#endif


#if XO_SSE
    _XOINL 
    Vector4(const __m128& vec) : 
        m(vec) { }
#endif

    Vector4(const class Vector2& v);
    Vector4(const class Vector3& v);

    _XOINL 
    float& operator [](int i) {
        return f[i];
    }

    _XOINL 
    const float& operator [](int i) const {
        return f[i];
    }

    _XOINL 
    void Set(float x, float y, float z, float w) {
#if XO_SSE
        m = _mm_set_ps(w, z, y, x);
#else
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
#endif
    }

    _XOINL 
    void Set(float f) {
#if XO_SSE
        m = _mm_set1_ps(f);
#else
        this->x = f;
        this->y = f;
        this->z = f;
        this->w = w;
#endif
    }

    _XOINL
    void Set(const Vector4& vec) {
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
    _XOINL 
    void Set(const __m128& vec) {
        m = vec;
    }
#endif

    _XOINL 
    void Get(float& x, float& y, float& z, float& w) const {
        x = this->x;
        y = this->y;
        z = this->z;
        w = this->w;
    }

    _XOINL
    void Get(float* f) const {
#if XO_SSE
            _mm_store_ps(f, m);
#else
            f[0] = this->x;
            f[1] = this->y;
            f[2] = this->z;
            f[3] = this->w;
#endif
    }

    VEC4D_SIMPLE_OP(+, _mm_add_ps)
    VEC4D_SIMPLE_OP(-, _mm_sub_ps)
    VEC4D_SIMPLE_OP(*, _mm_mul_ps)

    VEC4D_SIMPLE_OP_ADD(+=, +, _mm_add_ps)
    VEC4D_SIMPLE_OP_ADD(-=, -,  _mm_sub_ps)
    VEC4D_SIMPLE_OP_ADD(*=, *, _mm_mul_ps)

#if !defined(XO_NO_INVERSE_DIVISION)
    _XOINL 
    Vector4 operator / (const Vector4& v) const {
#   if XO_SSE
        return Vector4(_mm_div_ps(m, v.m));
#   else
        return Vector4(x/v.x, y/v.y, z/v.z, w/v.w);
#   endif
    } // no choice here, just divide each

    _XOINL 
    Vector4 operator / (float v) const       { return (*this) * (1.0f/v); }

    _XOINL 
    Vector4 operator / (double v) const      { return (*this) / (float)v; }

    _XOINL 
    Vector4 operator / (int v) const         { return (*this) / (float)v; }

    _XOINL 
    Vector4 operator / (const class  Vector2& v) const;

    _XOINL 
    Vector4 operator / (const class  Vector3& v) const;

    _XOINL 
    const Vector4& operator /= (const Vector4& v) {
#if XO_SSE
            m = _mm_div_ps(m, v.m);
#else
            x /= v.x; 
            y /= v.y; 
            z /= v.z;
#endif
        return *this;
    }

    _XOINL 
    const Vector4& operator /= (float v)     { return (*this) *= 1.0f / v; }

    _XOINL 
    const Vector4& operator /= (double v)    { return (*this) /= (float)v; }

    _XOINL 
    const Vector4& operator /= (int v)       { return (*this) /= (float)v; }

    _XOINL
    const Vector4& operator /= (const class Vector2& v);

    _XOINL
    const Vector4& operator /= (const class Vector3& v);

#else
    // scalar division is slower, so we try to avoid it unless XO_NO_INVERSE_DIVISION is defined.
    VEC4D_SIMPLE_OP(/ , _mm_div_ps)
    VEC4D_SIMPLE_OP_ADD(/=, / , _mm_div_ps)
#endif
    
    _XOINL 
    Vector4 operator - () const {
#if XO_SSE
        static const __m128 anti = _mm_set1_ps(-1.0f);
        return Vector4(_mm_mul_ps(m, anti));
#else
        return Vector4(-x, -y, -z, -w);
#endif
    }

    _XOINL
    Vector4 operator ~ () const {
#if XO_SSE
        return Vector4(_mm_shuffle_ps(m, m, _MM_SHUFFLE(IDX_X, IDX_Y, IDX_Z, IDX_W)));
#else
        return Vector4(w, z, y, x);
#endif
    }

    _XOINL 
    Vector4 ZYX() const {
        return ~(*this);
    }

    _XOINL 
    float Sum() const {
#if XO_SSE
        auto s = _mm_hadd_ps(m, m);
        s = _mm_hadd_ps(s, s);
        return _mm_cvtss_f32(s);
#else
        return x+y+z+w;
#endif
    }

    _XOINL 
    float MagnitudeSquared() const {
#if XO_SSE
            auto square = _mm_mul_ps(m, m);
            square = _mm_hadd_ps(square, square);
            square = _mm_hadd_ps(square, square);
            return _mm_cvtss_f32(square);
#else
            return (x*x) + (y*y) + (z*z) + (w*w);
#endif
    }

    _XOINL 
    float Magnitude() const {
        return Sqrt(MagnitudeSquared());
    }

    _XOINL
    const Vector3& Normalize() {
        float magnitude = MagnitudeSquared();
        if (CloseEnough(magnitude, 1.0f, Epsilon))
            return *this; // already normalized
        if (CloseEnough(magnitude, 0.0f, Epsilon))
            return *this; // zero vec
        magnitude = Sqrt(magnitude);
        return (*this) /= magnitude;
    }

    _XOINL 
    Vector4 Normalized() const {
        return Vector4(*this).Normalize();
    }

    _XOINL
    bool IsZero() const {
        return CloseEnough(MagnitudeSquared(), 0.0f, Epsilon);
    }

    _XOINL
    bool IsNormalized() const {
        return CloseEnough(MagnitudeSquared(), 1.0f, Epsilon);
    }

    // Note: comparisons against another vector will compare the magnitude.
    // comparisons against a number will also compare against magnitude (ignoring sign)
    VEC4_COMPARE_OP(<)
    VEC4_COMPARE_OP(<=)
    VEC4_COMPARE_OP(>)
    VEC4_COMPARE_OP(>=)
    // Note: '== and !=' against another Vector4 will check for the exact X,Y,Z match.
    // '== and !=' against a number will check against the magnitude (ignoring sign)
    VEC4_COMPARE_CLOSE_OP(==, &&)
    VEC4_COMPARE_CLOSE_OP(!=, ||)


    _XOINL static
    Vector4 Max(const Vector4& a, const Vector4& b) {
        return a >= b ? a : b;
    }

    _XOINL static
    Vector4 Min(const Vector4& a, const Vector4& b) {
        return a <= b ? a : b;
    }

    _XOINL static
    float Dot(const Vector4& a, const Vector4& b) {
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


    _XOINL static
    float DistanceSquared(const Vector4& a, const Vector4& b) {
        return (b - a).MagnitudeSquared();
    }

    _XOINL static
    float Distance(const Vector4&a, const Vector4&b) {
        return (b - a).Magnitude();
    }

    _XOINL static
    Vector4 Lerp(const Vector4& a, const Vector4& b, float t) {
        return a + ((b - a) * t);
    }

    _XOINL
    float Dot(const Vector4& v) const                                { return Dot(*this, v); }

    _XOINL
    float DistanceSquared(const Vector4& v) const                    { return DistanceSquared(*this, v); }

    _XOINL
    float Distance(const Vector4& v) const                           { return Distance(*this, v); }

    _XOINL 
    Vector4 Lerp(const Vector4& v, float t) const                    { return Lerp(*this, v, t); }

    friend std::ostream& operator <<(std::ostream& os, const Vector4& v) {
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", w:" << v.w << ", mag:" << v.Magnitude() << ")";
        return os;
    }

    union {
        struct {
            float x, y, z, w;
        };
        float f[4];
#if XO_SSE
        __m128 m;
#endif
    };
};

XOMATH_END_XO_NS

#endif // XOMATH_INTERNAL