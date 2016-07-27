#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector3.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS
class Vector3 {
    static const unsigned IDX_X = 0;
    static const unsigned IDX_Y = 1;
    static const unsigned IDX_Z = 2;
    static const unsigned IDX_W = 3;
public:
    constexpr static const float Epsilon = FloatEpsilon * 3.0f;

    Vector3() : m(_mm_setzero_ps()) {
    }
    Vector3(float f) : m(_mm_set1_ps(f)) {
    }
    Vector3(float x, float y, float z) : m(_mm_set_ps(0.0f, z, y, x)) {
        m.m128_f32[IDX_W] = 0.0f;
    }
    Vector3(const Vector3& vec) : m(vec.m) {
    }
    Vector3(const __m128& vec) : m(vec) {
        m.m128_f32[IDX_W] = 0.0f;
    }
    //Vector3(const class Vector2& v);

    float& operator [](int i) {
        return m.m128_f32[i];
    }

    const float& operator [](int i) const {
        return m.m128_f32[i];
    }

    void Set(float x, float y, float z) {
        m = _mm_set_ps(0.0f, z, y, x);
    }

    void Set(float f) {
        m = _mm_set1_ps(f);
    }

    void Set(const Vector3& vec) {
        m = vec.m;
    }

    void Set(const __m128& vec) {
        m = vec;
    }

    void Get(float& x, float& y, float &z) {
        __declspec(align(16)) float f[4];
        _mm_store_ps(f, m);
        x = f[IDX_X];
        y = f[IDX_Y];
        z = f[IDX_Z];
    }

    // note: we zero out the z component during anything but a multiply (which would maintain 0 by itself)
    // this keeps things like horizontal addition, and comparing __m128's simple.
    VEC3D_SIMPLE_OP(+, _mm_add_ps, { ret.m.m128_f32[IDX_W] = 0.0f; })
    VEC3D_SIMPLE_OP(-, _mm_sub_ps, { ret.m.m128_f32[IDX_W] = 0.0f; })
    VEC3D_SIMPLE_OP(*, _mm_mul_ps, {})
    VEC3D_SIMPLE_OP(/ , _mm_div_ps, { ret.m.m128_f32[IDX_W] = 0.0f; })

    VEC3D_SIMPLE_OP_ADD(+=, _mm_add_ps, { m.m128_f32[IDX_W] = 0.0f; })
    VEC3D_SIMPLE_OP_ADD(-=, _mm_sub_ps, { m.m128_f32[IDX_W] = 0.0f; })
    VEC3D_SIMPLE_OP_ADD(*=, _mm_mul_ps, {})
    VEC3D_SIMPLE_OP_ADD(/=, _mm_div_ps, { m.m128_f32[IDX_W] = 0.0f; })

    Vector3 operator - () const {
        static const __m128 anti = _mm_set1_ps(-1.0f);
        return Vector3(_mm_mul_ps(m, anti));
    }

    Vector3 operator ~ () const {
        return Vector3(_mm_shuffle_ps(m, m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Y, IDX_Z)));
    }

    Vector3 operator + (const class Vector2& v) const;

    float Magnitude() const {
      // get all the elements multiplied by themselves.
        auto square = _mm_mul_ps(m, m);
        // add horizontally. Now the first two floats are [z+y, x+w]
        square = _mm_hadd_ps(square, square);
        // add horizontally. now the first float is [z+y+x+w]
        square = _mm_hadd_ps(square, square);
        return Sqrt(_mm_cvtss_f32(square));
    }

    float MagnitudeSquared() const {
        // get all the elements multiplied by themselves.
        auto square = _mm_mul_ps(m, m);
        // add horizontally. Now the first two floats are [z+y, x+w]
        square = _mm_hadd_ps(square, square);

        // add horizontally. now the first float is [z+y+x+w]
        square = _mm_hadd_ps(square, square);
        return _mm_cvtss_f32(square);
    }

    void Normalize() {
        float magnitude = MagnitudeSquared();
        if (magnitude - 1.0f <= Epsilon)
            return; // already normalized
        magnitude = Sqrt(magnitude);
        if (magnitude < Epsilon)
            return; // zero vec
        magnitude = 1.0f / magnitude;
        (*this) *= magnitude;
    }

    Vector3 Normalized() const {
        float magnitude = MagnitudeSquared();
        if (magnitude - 1.0f <= Epsilon)
            return *this; // already normalized
        magnitude = Sqrt(magnitude);
        if (magnitude < Epsilon)
            return *this; // zero vec
        magnitude = 1.0f / magnitude;
        return (*this) * magnitude;
    }

    bool IsZero() const {
        return MagnitudeSquared() <= Epsilon;
    }

    bool IsNormalized() const {
      // todo: check closeness
        return MagnitudeSquared() - 1.0f <= Epsilon;
    }

    VEC3_COMPARE_OP(<)
    VEC3_COMPARE_OP(<=)
    VEC3_COMPARE_OP(>)
    VEC3_COMPARE_OP(>=)
    VEC3_COMPARE_CLOSE_OP(==)
    VEC3_COMPARE_CLOSE_OP(!=)

    static Vector3 Max(const Vector3& a, const Vector3& b) {
        if (a.MagnitudeSquared() >= b.MagnitudeSquared())
            return a;
        return b;
    }

    static Vector3 Min(const Vector3& a, const Vector3& b) {
        if (a.MagnitudeSquared() <= b.MagnitudeSquared())
            return a;
        return b;
    }

    static float Dot(const Vector3& a, const Vector3& b) {
        auto d = _mm_mul_ps(a.m, b.m);
        // add horizontally. Now the first two floats are [z+y, x+w]
        d = _mm_hadd_ps(d, d);
        // add horizontally. now the first float is [z+y+x+w]
        d = _mm_hadd_ps(d, d);
        return _mm_cvtss_f32(d);
    }

    static Vector3 Cross(const Vector3& a, const Vector3& b) {
      // for reference:
      // float x = (a[1] * b[2]) - (a[2] * b[1]);
      // float y = (a[2] * b[0]) - (a[0] * b[2]);
      // float z = (a[0] * b[1]) - (a[1] * b[0]);
        auto l = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)));
        auto r = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)));
        return Vector3(_mm_sub_ps(l, r));
    }

    static float AngleRadians(const Vector3& a, const Vector3& b) {
        auto l = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)));
        auto r = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)));
        auto s = _mm_sub_ps(l, r);
        s = _mm_mul_ps(s, s);
        s = _mm_hadd_ps(s, s);
        s = _mm_hadd_ps(s, s);

        return Atan2(Sqrt(_mm_cvtss_f32(s)) + Epsilon, Dot(a, b));
    }

    static float AngleDegrees(const Vector3& a, const Vector3& b) {
        return AngleRadians(a, b) * Rad2Deg;
    }

    static float DistanceSquared(const Vector3& a, const Vector3& b) {
        return (a - b).MagnitudeSquared();
    }

    static float Distance(const Vector3&a, const Vector3&b) {
        return (a - b).Magnitude();
    }

    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
        return a + ((b - a) * t);
    }

    float Dot(const Vector3& v) { return Dot(*this, v); }
    Vector3 Cross(const Vector3& v) { return Cross(*this, v); }
    float AngleRadians(const Vector3& v) { return AngleRadians(*this, v); }
    float AngleDegrees(const Vector3& v) { return AngleDegrees(*this, v); }
    float DistanceSquared(const Vector3& v) { return DistanceSquared(*this, v); }
    float Distance(const Vector3& v) { return Distance(*this, v); }

    friend std::ostream& operator <<(std::ostream& os, const Vector3& v) {
        __declspec(align(16)) float f[4];
        _mm_store_ps(f, v.m);
        os << '(' << f[IDX_X] << ',' << f[IDX_Y] << ',' << f[IDX_Z] << ')';
        return os;
    }

    static const Vector3
        UnitX, UnitY, UnitZ,
        Up, Down, Left, Right, Forward, Backward,
        One, Zero;

    union {
        struct {
            float x;
            float y;
            float z;
        };
        __m128 m;
    };
};

const Vector3 Vector3::UnitX(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UnitY(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UnitZ(0.0f, 0.0f, 1.0f);

const Vector3 Vector3::Up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Down(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::Left(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Forward(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Backward(0.0f, 0.0f, -1.0f);

const Vector3 Vector3::One(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);

XOMATH_END_XO_NS

#endif // XOMATH_INTERNAL