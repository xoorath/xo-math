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

    _XOINL void Set(float x, float y, float z);

    _XOINL void Set(float f);
    _XOINL void Set(const Vector3& vec);

#if XO_SSE
    _XOINL void Set(const __m128& vec);
#endif

    _XOINL void Get(float& x, float& y, float &z) const;
    _XOINL void Get(float* f) const;

    _XOINL float& operator [](int i);
    _XOINL const float& operator [](int i) const;

    _XOINL Vector3 operator -() const;
    _XOINL Vector3 operator ~() const;

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

    _XOINL Vector3 ZYX() const;

    _XOINL float MagnitudeSquared() const;

    _XOINL float Magnitude() const;

    _XOINL const Vector3& Normalize();

    _XOINL Vector3 Normalized() const;

    _XOINL bool IsZero() const;

    _XOINL bool IsNormalized() const;

    _XOINL static Vector3 Max(const Vector3& a, const Vector3& b);

    _XOINL static Vector3 Min(const Vector3& a, const Vector3& b);

    _XOINL static float Dot(const Vector3& a, const Vector3& b);

    _XOINL static  Vector3 Cross(const Vector3& a, const Vector3& b);

    _XOINL static float AngleRadians(const Vector3& a, const Vector3& b);

    _XOINL static float AngleDegrees(const Vector3& a, const Vector3& b);

    _XOINL static float DistanceSquared(const Vector3& a, const Vector3& b);

    _XOINL static  float Distance(const Vector3&a, const Vector3&b);

    _XOINL static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

    _XOINL static Vector3 RotateRadians(const Vector3& v, const Vector3& axis, float angle);

    _XOINL static Vector3 RotateDegrees(const Vector3& v, const Vector3& axis, float angle);

    // A random vector on edge of a cone with a given angle relative to a given forward.
    _XOINL static Vector3 RandomOnConeRadians(const Vector3& forward, float angle);

    // A random vector inside a cone with a given angle relative to a given forward.
    // Note: this cone in this context will be rounded at the bottom, not flat. The
    //  forward vector magnitude will be the same as the returned vector.
    _XOINL static Vector3 RandomInConeRadians(const Vector3& forward, float angle);

    // A random vector on edge of a cone with a given angle relative to a given forward.
    // Note: this cone in this context will be rounded at the bottom, not flat. The
    //  forward vector magnitude will be the same as the returned vector.
    _XOINL static Vector3 RandomOnConeDegrees(const Vector3& forward, float angle);

    // A random vector inside a cone with a given angle relative to a given forward.
    // Note: this cone in this context will be rounded at the bottom, not flat. The
    //  forward vector magnitude will be the same as the returned vector.
    _XOINL static Vector3 RandomInConeDegrees(const Vector3& forward, float angle);

    // A random vector with a length of 1.0f
    _XOINL static Vector3 RandomOnSphere();

    // A random vector who's length does not exceed 1
    _XOINL static Vector3 RandomInSphere();

    // A random vector with length d
    _XOINL static Vector3 RandomAtDistance(float d);

    // A random vector who's length does not exceed d
    _XOINL static Vector3 RandomInDistance(float d);

    // A random vector with a magnitude between low and high
    _XOINL static Vector3 RandomInRange(float low, float high);

    // passthrough methods, will call the static counterpart.
    _XOINL float Dot(const Vector3& v) const;
    _XOINL  Vector3 Cross(const Vector3& v) const;
    _XOINL float AngleRadians(const Vector3& v) const;
    _XOINL float AngleDegrees(const Vector3& v) const;
    _XOINL float DistanceSquared(const Vector3& v) const;
    _XOINL float Distance(const Vector3& v) const;
    _XOINL Vector3 Lerp(const Vector3& v, float t) const;
    _XOINL Vector3 RotateRadians(const Vector3& axis, float angle) const;
    _XOINL Vector3 RotateDegrees(const Vector3& axis, float angle) const;
    _XOINL Vector3 RandomOnConeRadians(float angle) const;
    _XOINL Vector3 RandomInConeRadians(float angle) const;
    _XOINL Vector3 RandomOnConeDegrees(float angle) const;
    _XOINL Vector3 RandomInConeDegrees(float angle) const;

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