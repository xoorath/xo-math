#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector3.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS
// TODO: align new/delete
class _MM_ALIGN16 Vector3 {
#if XO_SSE
        static const unsigned IDX_X = 0;
        static const unsigned IDX_Y = 1;
        static const unsigned IDX_Z = 2;
        static const unsigned IDX_W = 3;
        static const __m128 MASK;
#endif

public:
    constexpr static const float Epsilon = FloatEpsilon*3.0f;

    _XOINL Vector3();

    _XOINL Vector3(float f);

    _XOINL Vector3(float x, float y, float z);

    _XOINL Vector3(const Vector3& vec);

#if XO_SSE
    _XOINL Vector3(const __m128& vec);
#endif

    _XOINL Vector3(const class Vector2& v);
    _XOINL Vector3(const class Vector4& v);

    _XOINL float& operator [](int i);
    _XOINL const float& operator [](int i) const;

    _XOINL void Set(float x, float y, float z);

    _XOINL void Set(float f);
    _XOINL void Set(const Vector3& vec);

#if XO_SSE
    _XOINL void Set(const __m128& vec);
#endif

    _XOINL void Get(float& x, float& y, float &z) const;
    _XOINL void Get(float* f) const;

    _XOINL Vector3 operator + (const Vector3& v) const;
    _XOINL Vector3 operator + (float v) const;
    _XOINL Vector3 operator + (double v) const;
    _XOINL Vector3 operator + (int v) const;
    _XOINL Vector3 operator + (const class Vector2& v) const;
    _XOINL Vector3 operator + (const class Vector4& v) const;

    _XOINL Vector3 operator - (const Vector3& v) const;
    _XOINL Vector3 operator - (float v) const;
    _XOINL Vector3 operator - (double v) const;
    _XOINL Vector3 operator - (int v) const;
    _XOINL Vector3 operator - (const class Vector2& v) const;
    _XOINL Vector3 operator - (const class Vector4& v) const;

    _XOINL Vector3 operator * (const Vector3& v) const;
    _XOINL Vector3 operator * (float v) const;
    _XOINL Vector3 operator * (double v) const;
    _XOINL Vector3 operator * (int v) const;
    _XOINL Vector3 operator * (const class Vector2& v) const;
    _XOINL Vector3 operator * (const class Vector4& v) const;

    _XOINL Vector3 operator / (const Vector3& v) const;
    _XOINL Vector3 operator / (float v) const;
    _XOINL Vector3 operator / (double v) const;
    _XOINL Vector3 operator / (int v) const;
    _XOINL Vector3 operator / (const class Vector2& v) const;
    _XOINL Vector3 operator / (const class Vector4& v) const;

    _XOINL const Vector3& operator += (const Vector3& v);
    _XOINL const Vector3& operator += (float v);
    _XOINL const Vector3& operator += (double v);
    _XOINL const Vector3& operator += (int v);
    _XOINL const Vector3& operator += (const class Vector2& v);
    _XOINL const Vector3& operator += (const class Vector4& v);

    _XOINL const Vector3& operator -= (const Vector3& v);
    _XOINL const Vector3& operator -= (float v);
    _XOINL const Vector3& operator -= (double v);
    _XOINL const Vector3& operator -= (int v);
    _XOINL const Vector3& operator -= (const class Vector2& v);
    _XOINL const Vector3& operator -= (const class Vector4& v);

    _XOINL const Vector3& operator *= (const Vector3& v);
    _XOINL const Vector3& operator *= (float v);
    _XOINL const Vector3& operator *= (double v);
    _XOINL const Vector3& operator *= (int v);
    _XOINL const Vector3& operator *= (const class Vector2& v);
    _XOINL const Vector3& operator *= (const class Vector4& v);

    _XOINL const Vector3& operator /= (const Vector3& v);
    _XOINL const Vector3& operator /= (float v);
    _XOINL const Vector3& operator /= (double v);
    _XOINL const Vector3& operator /= (int v);
    _XOINL const Vector3& operator /= (const class Vector2& v);
    _XOINL const Vector3& operator /= (const class Vector4& v);

    _XOINL bool operator < (const Vector3& v) const;
    _XOINL bool operator < (float v) const;
    _XOINL bool operator < (double v) const;
    _XOINL bool operator < (int v) const;
    _XOINL bool operator < (const class Vector2& v) const;
    _XOINL bool operator < (const class Vector4& v) const;

    _XOINL bool operator <= (const Vector3& v) const;
    _XOINL bool operator <= (float v) const;
    _XOINL bool operator <= (double v) const;
    _XOINL bool operator <= (int v) const;
    _XOINL bool operator <= (const class Vector2& v) const;
    _XOINL bool operator <= (const class Vector4& v) const;

    _XOINL bool operator > (const Vector3& v) const;
    _XOINL bool operator > (float v) const;
    _XOINL bool operator > (double v) const;
    _XOINL bool operator > (int v) const;
    _XOINL bool operator > (const class Vector2& v) const;
    _XOINL bool operator > (const class Vector4& v) const;

    _XOINL bool operator >= (const Vector3& v) const;
    _XOINL bool operator >= (float v) const;
    _XOINL bool operator >= (double v) const;
    _XOINL bool operator >= (int v) const;
    _XOINL bool operator >= (const class Vector2& v) const;
    _XOINL bool operator >= (const class Vector4& v) const;

    _XOINL bool operator == (const Vector3& v) const;
    _XOINL bool operator == (float v) const;
    _XOINL bool operator == (double v) const;
    _XOINL bool operator == (int v) const;
    _XOINL bool operator == (const class Vector2& v) const;
    _XOINL bool operator == (const class Vector4& v) const;

    _XOINL bool operator != (const Vector3& v) const;
    _XOINL bool operator != (float v) const;
    _XOINL bool operator != (double v) const;
    _XOINL bool operator != (int v) const;
    _XOINL bool operator != (const class Vector2& v) const;
    _XOINL bool operator != (const class Vector4& v) const;

    _XOINL 
    Vector3 operator - () const {
#if XO_SSE
        static const __m128 anti = _mm_set1_ps(-1.0f);
        return Vector3(_mm_mul_ps(m, anti));
#else
        return Vector3(-x, -y, -z);
#endif
    }

    _XOINL
    Vector3 operator ~ () const {
#if XO_SSE
        return Vector3(_mm_shuffle_ps(m, m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Y, IDX_Z)));
#else
        return Vector3(z, y, x);
#endif
    }

    _XOINL 
    Vector3 ZYX() const {
        return ~(*this);
    }

    _XOINL 
    float MagnitudeSquared() const {
#if XO_SSE3
        auto square = _mm_and_ps(_mm_mul_ps(m, m), MASK);
        square = _mm_hadd_ps(square, square);
        square = _mm_hadd_ps(square, square);
        return _mm_cvtss_f32(square);
#elif XO_SSE
        auto square = _mm_mul_ps(m, m);
        return square.m128_f32[IDX_X] + square.m128_f32[IDX_Y] + square.m128_f32[IDX_Z];
#else
        return (x*x) + (y*y) + (z*z);
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
    Vector3 Normalized() const {
        return Vector3(*this).Normalize();
    }

    _XOINL 
    bool IsZero() const {
        return CloseEnough(MagnitudeSquared(), 0.0f, Epsilon);
    }

    _XOINL 
    bool IsNormalized() const {
        return CloseEnough(MagnitudeSquared(), 1.0f, Epsilon);
    }

    _XOINL static 
    Vector3 Max(const Vector3& a, const Vector3& b) {
        return a >= b ? a : b;
    }

    _XOINL static 
    Vector3 Min(const Vector3& a, const Vector3& b) {
        return a <= b ? a : b;
    }

    _XOINL static 
    float Dot(const Vector3& a, const Vector3& b) {
#if XO_SSE3
        auto d = _mm_and_ps(_mm_mul_ps(a.m, b.m), MASK);
        d = _mm_hadd_ps(d, d);
        d = _mm_hadd_ps(d, d);
        return _mm_cvtss_f32(d);
#elif XO_SSE
        auto d = _mm_mul_ps(a.m, b.m);
        return _mm_cvtss_f32(x+y+z);
#else
        return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
#endif
    }

    _XOINL static 
    Vector3 Cross(const Vector3& a, const Vector3& b) {
#if XO_SSE
            // Todo: There's a trick to do this with three shuffles. Look into that.
            auto l = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)));
            auto r = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)));
            return Vector3(_mm_sub_ps(l, r));
#else
            return Vector3((a.y*b.z)-(a.z*b.y), (a.z*b.x)-(a.x*b.z), (a.x*b.y)-(a.y*b.x));
#endif
    }

    _XOINL static
    float AngleRadians(const Vector3& a, const Vector3& b) {
#if XO_SSE3
        auto cross = Cross(a, b).m;
        cross = _mm_and_ps(_mm_mul_ps(cross, cross), MASK);
        cross = _mm_hadd_ps(cross, cross);
        cross = _mm_hadd_ps(cross, cross);
        return Atan2(Sqrt(_mm_cvtss_f32(cross)), Dot(a, b));
#elif XO_SSE
        auto cross = Cross(a, b).m;
        cross = _mm_mul_ps(cross, cross);
        return Atan2(Sqrt(cross.m128_f32[IDX_X] + cross.m128_f32[IDX_Y] + cross.m128_f32[IDX_Z]), Dot(a, b));
#else
        auto cross = Cross(a, b);
        cross *= cross;
        return Atan2(Sqrt(cross.x + cross.y + cross.z), Dot(a, b));
#endif
    }

    _XOINL static
    float AngleDegrees(const Vector3& a, const Vector3& b) {
        return AngleRadians(a, b) * Rad2Deg;
    }

    _XOINL static
    float DistanceSquared(const Vector3& a, const Vector3& b) {
        return (b - a).MagnitudeSquared();
    }

    _XOINL static 
    float Distance(const Vector3&a, const Vector3&b) {
        return (b - a).Magnitude();
    }

    _XOINL static
    Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
        return a + ((b - a) * t);
    }

    _XOINL static
    Vector3 RotateRadians(const Vector3& v, const Vector3& axis, float angle) {
        // Rodrigues' rotation formula
        // https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
        auto sinAng = Sin(angle);
        auto cosAng = Cos(angle);
        return v * cosAng + axis.Cross(v) * sinAng + axis * axis.Dot(v) * (1.0f - cosAng);
    }

    _XOINL static
    Vector3 RotateDegrees(const Vector3& v, const Vector3& axis, float angle) {
        return RotateRadians(v, axis, angle * Deg2Rad);
    }

    // A random vector on edge of a cone with a given angle relative to a given forward.
    _XOINL static
    Vector3 RandomOnConeRadians(const Vector3& forward, float angle) {
        auto cross = forward.Cross(forward == Up ? Left : Up); // cross anything but itself will do. We just need an orthogonal vector
        auto tilted = forward.RotateRadians(cross, angle);
        return tilted.RotateRadians(forward, RandomRange(-PI, PI));
    }

    // A random vector inside a cone with a given angle relative to a given forward.
    _XOINL static
    Vector3 RandomInConeRadians(const Vector3& forward, float angle) {
        auto cross = forward.Cross(forward == Up ? Left : Up); // cross anything but itself will do. We just need an orthogonal vector
        auto tilted = forward.RotateRadians(cross, RandomRange(0.0f, angle));
        return tilted.RotateRadians(forward, RandomRange(-PI, PI));
    }

    // A random vector on edge of a cone with a given angle relative to a given forward.
    _XOINL static
    Vector3 RandomOnConeDegrees(const Vector3& forward, float angle) {
        return RandomOnConeRadians(forward, angle * Deg2Rad);
    }

    // A random vector inside a cone with a given angle relative to a given forward.
    _XOINL static
    Vector3 RandomInConeDegrees(const Vector3& forward, float angle) {
        return RandomInConeRadians(forward, angle * Deg2Rad);
    }

    // A random vector with a length of 1.0f
    _XOINL static
    Vector3 RandomOnSphere() {
        return Vector3(
            RandomRange(-1.0f, 1.0f), 
            RandomRange(-1.0f, 1.0f), 
            RandomRange(-1.0f, 1.0f)).Normalized();
    }

    // A random vector who's length does not exceed 1
    _XOINL static
    Vector3 RandomInSphere() {
        return RandomOnSphere() * RandomRange(0.0f, 1.0f);
    }

    // A random vector with length d
    _XOINL static
    Vector3 RandomAtDistance(float d) {
        return Vector3(
            RandomRange(-1.0f, 1.0f),
            RandomRange(-1.0f, 1.0f),
            RandomRange(-1.0f, 1.0f)).Normalized() * d;
    }

    // A random vector who's length does not exceed d
    _XOINL static
    Vector3 RandomInDistance(float d) {
        return Vector3(
            RandomRange(-1.0f, 1.0f),
            RandomRange(-1.0f, 1.0f),
            RandomRange(-1.0f, 1.0f)).Normalized() * RandomRange(0.0f, d);
    }

    // A random vector with a magnitude between low and high
    _XOINL static
    Vector3 RandomInRange(float low, float high) {
        return Vector3(
            RandomRange(-1.0f, 1.0f),
            RandomRange(-1.0f, 1.0f),
            RandomRange(-1.0f, 1.0f)).Normalized() * RandomRange(low, high);
    }

    _XOINL
    float Dot(const Vector3& v) const                                { return Dot(*this, v); }

    _XOINL 
    Vector3 Cross(const Vector3& v) const                            { return Cross(*this, v); }

    _XOINL
    float AngleRadians(const Vector3& v) const                       { return AngleRadians(*this, v); }

    _XOINL
    float AngleDegrees(const Vector3& v) const                       { return AngleDegrees(*this, v); }

    _XOINL
    float DistanceSquared(const Vector3& v) const                    { return DistanceSquared(*this, v); }

    _XOINL
    float Distance(const Vector3& v) const                           { return Distance(*this, v); }

    _XOINL
    Vector3 Lerp(const Vector3& v, float t) const                    { return Lerp(*this, v, t); }

    _XOINL
    Vector3 RotateRadians(const Vector3& axis, float angle) const    { return RotateRadians(*this, axis, angle); }

    _XOINL
    Vector3 RotateDegrees(const Vector3& axis, float angle) const    { return RotateDegrees(*this, axis, angle); }

    _XOINL
    Vector3 RandomOnConeRadians(float angle) const                   { return RandomOnConeRadians(*this, angle); }

    _XOINL
    Vector3 RandomInConeRadians(float angle) const                   { return RandomInConeRadians(*this, angle); }

    _XOINL
    Vector3 RandomOnConeDegrees(float angle) const                   { return RandomOnConeDegrees(*this, angle); }

    _XOINL
    Vector3 RandomInConeDegrees(float angle) const                   { return RandomInConeDegrees(*this, angle); }

    friend std::ostream& operator <<(std::ostream& os, const Vector3& v) {
#if XO_SSE
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", w:" << v.w << ", mag:" << v.Magnitude() << ")";
#else
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", mag:" << v.Magnitude() << ")";
#endif
        return os;
    }

    static const Vector3
        Origin,
        UnitX, UnitY, UnitZ,
        Up, Down, Left, Right, Forward, Backward,
        One, Zero;

#if XO_SSE
    union {
        struct {
            float x, y, z, w;
        };
        float f[4];
        __m128 m;
    };
#else
    union {
        struct {
            float x, y, z;
        };
        float f[3];
    };
#endif
};

#if XO_SSE
const __m128 Vector3::MASK = _mm_castsi128_ps(_mm_set_epi32(0, 0xffffffff, 0xffffffff, 0xffffffff));
#endif

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