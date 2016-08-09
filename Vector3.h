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

    // Return a copy of this vector swizzled so that x=z, y=y and z=x.
    _XOINL Vector3 ZYX() const;

    // Return the magnitude (length) of this vector squared. This is faster than Magnitude().
    _XOINL float MagnitudeSquared() const;

    // Return the magnitude (length) of this vector.
    _XOINL float Magnitude() const;

    // Normalize this vector so that the magnitude (length) is 1, then return a reference to this vector.
    // Note: This method has no effect if the length of the vector is already 1.
    _XOINL const Vector3& Normalize();

    // Return a copy of this vector normalized so that the magnitude (length) is 1.
    // Note: This method has no effect if the length of the vector is already 1.
    _XOINL Vector3 Normalized() const;

    // Return true if the magnitude (length) of this vector is zero.
    _XOINL bool IsZero() const;

    // Return true if the magnitude (length) of this vector is 1.
    _XOINL bool IsNormalized() const;

    // Return a vector with each element equal to the max of that element in and b.
    // Example: Max({1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f}) == {1.0f, 1.0f, -1.0f}
    _XOINL static Vector3 Max(const Vector3& a, const Vector3& b);

    // Return a vector with each element equal to the min of that element in and b.
    // Example: Max({1.0f, -1.0f, 1.0f}, {-1.0f, 1.0f, 1.0f}) == {-1.0f, -1.0f, 1.0f}
    _XOINL static Vector3 Min(const Vector3& a, const Vector3& b);

    // Returns the dot product (scalar product) of vectors a and b.
    // See: https://en.wikipedia.org/wiki/Dot_product
    _XOINL static float Dot(const Vector3& a, const Vector3& b);

    // Returns the cross product (vector product) of vectors a and b.
    // See: https://en.wikipedia.org/wiki/Cross_product
    _XOINL static  Vector3 Cross(const Vector3& a, const Vector3& b);

    // Returns the angle in radians between vectors a and b.
    _XOINL static float AngleRadians(const Vector3& a, const Vector3& b);

    // Returns the angle in degrees between vectors a and b.
    _XOINL static float AngleDegrees(const Vector3& a, const Vector3& b);

    // Returns the distance between vectors a and b. This is faster than Distance().
    _XOINL static float DistanceSquared(const Vector3& a, const Vector3& b);

    // Returns the distance between vectors a and b.
    _XOINL static  float Distance(const Vector3&a, const Vector3&b);

    // Returns a point linearly interpolated between a and b by a factor of t. 
    // Where t is 0, a is returned. Where t is 1, b is returned.
    // See: https://en.wikipedia.org/wiki/Linear_interpolation
    _XOINL static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

    // Rotate the input vector v by 'angle' radians on the specified axis, and return the result.
    // Example: RotateRadians(Vector3::Up, Vector3::Right, Pi/2.0f) == Vector3::Forward
    _XOINL static Vector3 RotateRadians(const Vector3& v, const Vector3& axis, float angle);

    // Rotate the input vector v by 'angle' degrees on the specified axis, and return the result.
    // Example: RotateDegrees(Vector3::Up, Vector3::Right, 90.0f) == Vector3::Forward
    _XOINL static Vector3 RotateDegrees(const Vector3& v, const Vector3& axis, float angle);

    // Return a random vector on edge of a cone with an angle relative to forward.
    // Note this 'cone' does not have a flat bottom, it is a rotation of the forward vector.
    // angles at and beyond PI radians will no longer represent a meaningful cone.
    _XOINL static Vector3 RandomOnConeRadians(const Vector3& forward, float angle);

    // Return a random vector inside a cone with an angle relative to forward.
    // Note this 'cone' does not have a flat bottom, it is a rotation of the forward vector.
    // angles at and beyond PI radians will no longer represent a meaningful cone.
    _XOINL static Vector3 RandomInConeRadians(const Vector3& forward, float angle);

    // Return a random vector on edge of a cone with an angle relative forward.
    // Note this 'cone' does not have a flat bottom, it is a rotation of the forward vector.
    // angles at and beyond 180.0f degrees will no longer represent a meaningful cone.
    _XOINL static Vector3 RandomOnConeDegrees(const Vector3& forward, float angle);

    // Return a random vector inside a cone with an angle relative to forward.
    // Note this 'cone' does not have a flat bottom, it is a rotation of the forward vector.
    // angles at and beyond 180.0f degrees will no longer represent a meaningful cone.
    _XOINL static Vector3 RandomInConeDegrees(const Vector3& forward, float angle);

    // Return a random vector with a length of 1.0f
    _XOINL static Vector3 RandomOnSphere();

    // Return a random vector who's length does not exceed 1
    _XOINL static Vector3 RandomInSphere();

    // Return a random vector with length d
    _XOINL static Vector3 RandomAtDistance(float d);

    // Return a random vector who's length does not exceed d
    _XOINL static Vector3 RandomInDistance(float d);

    // Return a random vector with a magnitude between low and high
    _XOINL static Vector3 RandomInRange(float low, float high);

    // See static Vector3::Dot
    _XOINL float Dot(const Vector3& v) const;

    // See static Vector3::Cross
    _XOINL Vector3 Cross(const Vector3& v) const;

    // See static Vector3::Dot
    _XOINL float AngleRadians(const Vector3& v) const;

    // See static Vector3::AngleDegrees
    _XOINL float AngleDegrees(const Vector3& v) const;

    // See static Vector3::DistanceSquared
    _XOINL float DistanceSquared(const Vector3& v) const;

    // See static Vector3::Distance
    _XOINL float Distance(const Vector3& v) const;

    // See static Vector3::Lerp
    _XOINL Vector3 Lerp(const Vector3& v, float t) const;

    // See static Vector3::RotateRadians
    _XOINL Vector3 RotateRadians(const Vector3& axis, float angle) const;

    // See static Vector3::RotateDegrees
    _XOINL Vector3 RotateDegrees(const Vector3& axis, float angle) const;

    // See static Vector3::RandomOnConeRadians
    _XOINL Vector3 RandomOnConeRadians(float angle) const;

    // See static Vector3::RandomInConeRadians
    _XOINL Vector3 RandomInConeRadians(float angle) const;

    // See static Vector3::RandomOnConeDegrees
    _XOINL Vector3 RandomOnConeDegrees(float angle) const;

    // See static Vector3::RandomInConeDegrees
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

#if XO_SSE2
const __m128 Vector3::MASK = _mm_castsi128_ps(_mm_set_epi32(0, 0xffffffff, 0xffffffff, 0xffffffff));
#elif XO_SSE
const __m128 Vector3::MASK = {-1, -1, -1, 0};
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