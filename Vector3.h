#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector3.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS
// TODO: align new/delete
class _MM_ALIGN16 Vector3 {
    XO_IF_SIMD (
        static const unsigned IDX_X = 0;
        static const unsigned IDX_Y = 1;
        static const unsigned IDX_Z = 2;
        static const unsigned IDX_W = 3;
        static const __m128 MASK;
    )

public:
    constexpr static const float Epsilon = FloatEpsilon+FloatEpsilon+FloatEpsilon;

    XOMATH_INLINE Vector3() :
        XO_IF_SIMD (
            m(_mm_setzero_ps())
        ) XO_IFN_SIMD (
            x(0.0f), y(0.0f), z(0.0f)
        ) { }
    XOMATH_INLINE Vector3(float f) :
        XO_IF_SIMD (
            m(_mm_set1_ps(f))
        ) XO_IFN_SIMD (
            x(f), y(f), z(f)
        ) { }
    XOMATH_INLINE Vector3(float x, float y, float z) :
        XO_IF_SIMD (
            m(_mm_set_ps(0.0f, z, y, x))
        ) XO_IFN_SIMD (
            x(x), y(y), z(z)
        ) { }
    XOMATH_INLINE Vector3(const Vector3& vec) :
        XO_IF_SIMD (
            m(vec.m)
        ) XO_IFN_SIMD (
            x(vec.x), y(vec.y), z(vec.z)
        ) { }

#if !defined(XO_NO_SIMD)
    XOMATH_INLINE Vector3(const __m128& vec) : m(vec) { }
#endif

    Vector3(const class Vector2& v);

    XOMATH_INLINE float& XOMATH_FAST(operator [](int i)) {
        return f[i];
    }

    XOMATH_INLINE const float& XOMATH_FAST(operator [](int i) const) {
        return f[i];
    }

    XOMATH_INLINE void XOMATH_FAST(Set(float x, float y, float z)) {
        XO_IF_SIMD (
            m = _mm_set_ps(0.0f, z, y, x);
        ) XO_IFN_SIMD (
            this->x = x;
            this->y = y;
            this->z = z;
        )
    }

    XOMATH_INLINE void XOMATH_FAST(Set(float f)) {
        XO_IF_SIMD (
            m = _mm_set1_ps(f);
        ) XO_IFN_SIMD (
            this->x = f;
            this->y = f;
            this->z = f;
        )
    }

    XOMATH_INLINE void XOMATH_FAST(Set(const Vector3& vec)) {
        XO_IF_SIMD (
            m = vec.m;
        ) XO_IFN_SIMD (
            this->x = vec.x;
            this->y = vec.y;
            this->z = vec.z;
        )
    }

#if !defined(XO_NO_SIMD)
    XOMATH_INLINE void XOMATH_FAST(Set(const __m128& vec)) {
        m = vec;
    }
#endif

    XOMATH_INLINE void XOMATH_FAST(Get(float& x, float& y, float &z) const) {
        x = this->x;
        y = this->y;
        z = this->z;
    }

    XOMATH_INLINE void XOMATH_FAST(Get(float* f) const) {
        XO_IF_SIMD (
            _mm_store_ps(f, m);
        ) XO_IFN_SIMD (
            f[0] = this->x;
            f[1] = this->y;
            f[2] = this->z;
        )
    }

    VEC3D_SIMPLE_OP(+, _mm_add_ps)
    VEC3D_SIMPLE_OP(-, _mm_sub_ps)
    VEC3D_SIMPLE_OP(*, _mm_mul_ps)


    VEC3D_SIMPLE_OP_ADD(+=, +, _mm_add_ps)
    VEC3D_SIMPLE_OP_ADD(-=, -,  _mm_sub_ps)
    VEC3D_SIMPLE_OP_ADD(*=, *, _mm_mul_ps)

#if !defined(XO_NO_INVERSE_DIVISION)
    XOMATH_INLINE Vector3 XOMATH_FAST(operator / (const Vector3& v) const) {
        XO_IF_SIMD (
            return Vector3(_mm_div_ps(m, v.m));
        ) XO_IFN_SIMD (
            return Vector3(x/v.x, y/v.y, z/v.z);
        )
    } // no choice here, just divide each
    XOMATH_INLINE Vector3 XOMATH_FAST(operator / (float v) const)       { return (*this) * (1.0f/v); }
    XOMATH_INLINE Vector3 XOMATH_FAST(operator / (double v) const)      { return (*this) / (float)v; }
    XOMATH_INLINE Vector3 XOMATH_FAST(operator / (int v) const)         { return (*this) / (float)v; }
    XOMATH_INLINE const Vector3& XOMATH_FAST(operator /= (const Vector3& v)) {
        XO_IF_SIMD (
            m = _mm_div_ps(m, v.m);
        ) XO_IFN_SIMD (
            x /= v.x; 
            y /= v.y; 
            z /= v.z;
        )
        return *this;
    }
    XOMATH_INLINE const Vector3& XOMATH_FAST(operator /= (float v))     { return (*this) *= 1.0f / v; }
    XOMATH_INLINE const Vector3& XOMATH_FAST(operator /= (double v))    { return (*this) /= (float)v; }
    XOMATH_INLINE const Vector3& XOMATH_FAST(operator /= (int v))       { return (*this) /= (float)v; }
#else
    // scalar division is slower, so we try to avoid it unless XO_NO_INVERSE_DIVISION is defined.
    VEC3D_SIMPLE_OP(/ , _mm_div_ps)
    VEC3D_SIMPLE_OP_ADD(/=, / , _mm_div_ps)
#endif
    
    XOMATH_INLINE Vector3 operator - () const {
        XO_IF_SIMD (
            static const __m128 anti = _mm_set1_ps(-1.0f);
            return Vector3(_mm_mul_ps(m, anti));
        ) XO_IFN_SIMD (
            return Vector3(-x, -y, -z);
        )
    }

    XOMATH_INLINE Vector3 operator ~ () const {
        XO_IF_SIMD (
            return Vector3(_mm_shuffle_ps(m, m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Y, IDX_Z)));
        ) XO_IFN_SIMD (
            return Vector3(z, y, x);
        )
    }

    XOMATH_INLINE Vector3 ZYX() const {
        return ~(*this);
    }

    XOMATH_INLINE float MagnitudeSquared() const {
        XO_IF_SIMD (
            auto square = _mm_and_ps(_mm_mul_ps(m, m), MASK);
            square = _mm_hadd_ps(square, square);
            square = _mm_hadd_ps(square, square);
            return _mm_cvtss_f32(square);
        ) XO_IFN_SIMD (
            return (x*x) + (y*y) + (z*z);
        )
    }

    XOMATH_INLINE float Magnitude() const {
        return Sqrt(MagnitudeSquared());
    }

    XOMATH_INLINE Vector3 Normalized() const {
        float magnitude = MagnitudeSquared();
        if (CloseEnough(magnitude, 1.0f, Epsilon))
            return *this; // already normalized
        if (CloseEnough(magnitude, 0.0f, Epsilon))
            return *this; // zero vec
        magnitude = Sqrt(magnitude);
        return (*this) / magnitude;
    }

    XOMATH_INLINE void Normalize() {
        (*this) = Normalized();
    }

    XOMATH_INLINE bool IsZero() const {
        return MagnitudeSquared() <= Epsilon;
    }

    XOMATH_INLINE bool IsNormalized() const {
      // todo: check closeness
        return MagnitudeSquared() - 1.0f <= Epsilon;
    }

    // Note: comparisons against another vector will compare the magnitude.
    // comparisons against a number will also compare against magnitude (ignoring sign)
    VEC3_COMPARE_OP(<)
    VEC3_COMPARE_OP(<=)
    VEC3_COMPARE_OP(>)
    VEC3_COMPARE_OP(>=)
    // Note: '== and !=' against another Vector3 will check for the exact X,Y,Z match.
    // '== and !=' against a number will check against the magnitude (ignoring sign)
    VEC3_COMPARE_CLOSE_OP(==, &&)
    VEC3_COMPARE_CLOSE_OP(!=, ||)


    XOMATH_INLINE static Vector3 XOMATH_FAST(Max(const Vector3& a, const Vector3& b)) {
        return a >= b ? a : b;
    }

    XOMATH_INLINE static Vector3 XOMATH_FAST(Min(const Vector3& a, const Vector3& b)) {
        return a <= b ? a : b;
    }

    XOMATH_INLINE static float XOMATH_FAST(Dot(const Vector3& a, const Vector3& b)) {
        XO_IF_SIMD (
            auto d = _mm_and_ps(_mm_mul_ps(a.m, b.m), MASK);
            d = _mm_hadd_ps(d, d);
            d = _mm_hadd_ps(d, d);
            return _mm_cvtss_f32(d);
        ) XO_IFN_SIMD (
            return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
        )
    }

    XOMATH_INLINE static Vector3 XOMATH_FAST(Cross(const Vector3& a, const Vector3& b)) {
        XO_IF_SIMD (
            // Todo: There's a trick to do this with three shuffles. Look into that.
            auto l = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)));
            auto r = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)));
            return Vector3(_mm_sub_ps(l, r));
        ) XO_IFN_SIMD (
            return Vector3((a.y*b.z)-(a.z*b.y), (a.z*b.x)-(a.x*b.z), (a.x*b.y)-(a.y*b.x));
        )
    }

    XOMATH_INLINE static float XOMATH_FAST(AngleRadians(const Vector3& a, const Vector3& b)) {
        XO_IF_SIMD (
            auto cross = Cross(a, b).m;
            cross = _mm_and_ps(_mm_mul_ps(cross, cross), MASK);
            cross = _mm_hadd_ps(cross, cross);
            cross = _mm_hadd_ps(cross, cross);
            return Atan2(Sqrt(_mm_cvtss_f32(cross)), Dot(a, b));
        ) XO_IFN_SIMD (
            auto cross = Cross(a, b);
            cross *= cross;
            return Atan2(Sqrt(cross.x + cross.y + cross.z), Dot(a, b));
        )
    }

    XOMATH_INLINE static float XOMATH_FAST(AngleDegrees(const Vector3& a, const Vector3& b)) {
        return AngleRadians(a, b) * Rad2Deg;
    }

    XOMATH_INLINE static float XOMATH_FAST(DistanceSquared(const Vector3& a, const Vector3& b)) {
        return (b - a).MagnitudeSquared();
    }

    XOMATH_INLINE static float XOMATH_FAST(Distance(const Vector3&a, const Vector3&b)) {
        return (b - a).Magnitude();
    }

    XOMATH_INLINE static Vector3 XOMATH_FAST(Lerp(const Vector3& a, const Vector3& b, float t)) {
        return a + ((b - a) * t);
    }

    XOMATH_INLINE static Vector3 XOMATH_FAST(RotateRadians(const Vector3& v, const Vector3& axis, float angle)) {
        // Rodrigues' rotation formula
        // https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
        auto sinAng = Sin(angle);
        auto cosAng = Cos(angle);
        return v * cosAng + axis.Cross(v) * sinAng + axis * axis.Dot(v) * (1.0f - cosAng);
    }

    XOMATH_INLINE static Vector3 XOMATH_FAST(RotateDegrees(const Vector3& v, const Vector3& axis, float angle)) {
        return RotateRadians(v, axis, angle * Deg2Rad);
    }

    // A random vector on edge of a cone with a given angle relative to a given forward.
    XOMATH_INLINE static Vector3 XOMATH_FAST(RandomOnConeRadians(const Vector3& forward, float angle)) {
        auto cross = forward.Cross(forward == Up ? Left : Up); // cross anything but itself will do. We just need an orthogonal vector
        auto tilted = forward.RotateRadians(cross, angle);
        return tilted.RotateRadians(forward, RandomRange(-PI, PI));
    }

    // A random vector inside a cone with a given angle relative to a given forward.
    XOMATH_INLINE static Vector3 XOMATH_FAST(RandomInConeRadians(const Vector3& forward, float angle)) {
        auto cross = forward.Cross(forward == Up ? Left : Up); // cross anything but itself will do. We just need an orthogonal vector
        auto tilted = forward.RotateRadians(cross, RandomRange(0.0f, angle));
        return tilted.RotateRadians(forward, RandomRange(-PI, PI));
    }

    // A random vector on edge of a cone with a given angle relative to a given forward.
    XOMATH_INLINE static Vector3 XOMATH_FAST(RandomOnConeDegrees(const Vector3& forward, float angle)) {
        return RandomOnConeRadians(forward, angle * Deg2Rad);
    }

    // A random vector inside a cone with a given angle relative to a given forward.
    XOMATH_INLINE static Vector3 XOMATH_FAST(RandomInConeDegrees(const Vector3& forward, float angle)) {
        return RandomInConeRadians(forward, angle * Deg2Rad);
    }

    // A random vector with a length of 1.0f
    XOMATH_INLINE static Vector3 XOMATH_FAST(RandomOnSphere()) {
        return Vector3(
            RandomRange(-1.0f, 1.0f), 
            RandomRange(-1.0f, 1.0f), 
            RandomRange(-1.0f, 1.0f)).Normalized();
    }

    // A random vector who's length does not exceed 1
    XOMATH_INLINE static Vector3 XOMATH_FAST(RandomInSphere()) {
        return RandomOnSphere() * RandomRange(0.0f, 1.0f);
    }

    // A random vector with length d
    XOMATH_INLINE static Vector3 XOMATH_FAST(RandomAtDistance(float d)) {
        return Vector3(
            RandomRange(-1.0f, 1.0f),
            RandomRange(-1.0f, 1.0f),
            RandomRange(-1.0f, 1.0f)).Normalized() * d;
    }

    // A random vector who's length does not exceed d
    XOMATH_INLINE static Vector3 XOMATH_FAST(RandomInDistance(float d)) {
        return Vector3(
            RandomRange(-1.0f, 1.0f),
            RandomRange(-1.0f, 1.0f),
            RandomRange(-1.0f, 1.0f)).Normalized() * RandomRange(0.0f, d);
    }

    // A random vector with a magnitude between low and high
    XOMATH_INLINE static Vector3 XOMATH_FAST(RandomInRange(float low, float high)) {
        return Vector3(
            RandomRange(-1.0f, 1.0f),
            RandomRange(-1.0f, 1.0f),
            RandomRange(-1.0f, 1.0f)).Normalized() * RandomRange(low, high);
    }

    XOMATH_INLINE float XOMATH_FAST(Dot(const Vector3& v) const)                                { return Dot(*this, v); }
    XOMATH_INLINE Vector3 XOMATH_FAST(Cross(const Vector3& v) const)                            { return Cross(*this, v); }
    XOMATH_INLINE float XOMATH_FAST(AngleRadians(const Vector3& v) const)                       { return AngleRadians(*this, v); }
    XOMATH_INLINE float XOMATH_FAST(AngleDegrees(const Vector3& v) const)                       { return AngleDegrees(*this, v); }
    XOMATH_INLINE float XOMATH_FAST(DistanceSquared(const Vector3& v) const)                    { return DistanceSquared(*this, v); }
    XOMATH_INLINE float XOMATH_FAST(Distance(const Vector3& v) const)                           { return Distance(*this, v); }
    XOMATH_INLINE Vector3 XOMATH_FAST(Lerp(const Vector3& v, float t) const)                    { return Lerp(*this, v, t); }
    XOMATH_INLINE Vector3 XOMATH_FAST(RotateRadians(const Vector3& axis, float angle) const)    { return RotateRadians(*this, axis, angle); }
    XOMATH_INLINE Vector3 XOMATH_FAST(RotateDegrees(const Vector3& axis, float angle) const)    { return RotateDegrees(*this, axis, angle); }
    XOMATH_INLINE Vector3 XOMATH_FAST(RandomOnConeRadians(float angle) const)                   { return RandomOnConeRadians(*this, angle); }
    XOMATH_INLINE Vector3 XOMATH_FAST(RandomInConeRadians(float angle) const)                   { return RandomInConeRadians(*this, angle); }
    XOMATH_INLINE Vector3 XOMATH_FAST(RandomOnConeDegrees(float angle) const)                   { return RandomOnConeDegrees(*this, angle); }
    XOMATH_INLINE Vector3 XOMATH_FAST(RandomInConeDegrees(float angle) const)                   { return RandomInConeDegrees(*this, angle); }

    friend std::ostream& operator <<(std::ostream& os, const Vector3& v) {
        XO_IF_SIMD (
            os << "( x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", w:" << v.w << ", mag:" << v.Magnitude() << ")";
        ) XO_IFN_SIMD(
            os << "( x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", mag:" << v.Magnitude() << ")";
        )
        return os;
    }

    static const Vector3
        Origin,
        UnitX, UnitY, UnitZ,
        Up, Down, Left, Right, Forward, Backward,
        One, Zero;

#if defined(XO_NO_SIMD)
    union {
        struct {
            float x, y, z;
        };
        float f[3];
    };
#else
    union {
        struct {
            float x, y, z, w;
        };
        float f[4];
        __m128 m;
    };
#endif
};

XO_IF_SIMD (
    const __m128 Vector3::MASK = _mm_castsi128_ps(_mm_set_epi32(0, 0xffffffff, 0xffffffff, 0xffffffff));
)

const Vector3 Vector3::Origin(0.0f, 0.0f, 0.0f);

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