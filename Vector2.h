#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector2.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS
class Vector2 {
public:
    constexpr static const float Epsilon = FloatEpsilon+FloatEpsilon;

    Vector2() : x(0.0f), y(0.0f) {
    }
    Vector2(float v) : x(v), y(v) {
    }
    Vector2(float x, float y) : x(x), y(y) {
    }
    Vector2(const Vector2& v) : x(v.x), y(v.y) {
    }
    Vector2(const class Vector3& v);

    float& operator [](int i) { return f[i]; }
    const float& operator [](int i) const { return f[i]; }

    void Set(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void Set(float v) {
        this->x = v;
        this->y = v;
    }

    void Set(const Vector2& v) {
        this->x = v[0];
        this->y = v[1];
    }
    
    VEC2D_SIMPLE_OP(+)
    VEC2D_SIMPLE_OP(-)
    VEC2D_SIMPLE_OP(*)
    VEC2D_SIMPLE_OP(/ )
    VEC2D_SIMPLE_OP_ADD(+=)
    VEC2D_SIMPLE_OP_ADD(-=)
    VEC2D_SIMPLE_OP_ADD(*=)
    VEC2D_SIMPLE_OP_ADD(/=)

    Vector2 operator - () const { return Vector2(-x, -y); }
    Vector2 operator ~ () const { return Vector2(y, x); }
    
    float Magnitude() const {
        return sqrtf(x*x + y*y);
    }

    float MagnitudeSquared() const {
        return x*x + y*y;
    }

    void Normalize() {
        float magnitude = MagnitudeSquared();
        if (CloseEnough(magnitude, 1.0f, Epsilon))
            return; // already normalized
        if (CloseEnough(magnitude, 0.0f, Epsilon))
            return; // zero vec
        magnitude = sqrtf(magnitude);
        magnitude = 1.0f / magnitude;
        x *= magnitude;
        y *= magnitude;
    }

    Vector2 Normalized() const {
        float magnitude = MagnitudeSquared();
        if (CloseEnough(magnitude, 1.0f, Epsilon))
            return *this; // already normalized
        if (CloseEnough(magnitude, 0.0f, Epsilon))
            return *this; // zero vec
        magnitude = sqrtf(magnitude);
        magnitude = 1.0f / magnitude;
        return { x * magnitude, y * magnitude };
    }

    bool IsZero() const {
        return MagnitudeSquared() <= Epsilon;
    }

    bool IsNormalized() const {
      // todo: check closeness
        return MagnitudeSquared() - 1.0f <= Epsilon;
    }

    VEC2_COMPARE_OP(<)
    VEC2_COMPARE_OP(<= )
    VEC2_COMPARE_OP(>)
    VEC2_COMPARE_OP(>= )
    VEC2_COMPARE_CLOSE_OP(==)
    VEC2_COMPARE_CLOSE_OP(!=)

    static Vector2 Max(const Vector2& a, const Vector2& b) {
        if (a.MagnitudeSquared() >= b.MagnitudeSquared())
            return a;
        return b;
    }

    static Vector2 Min(const Vector2& a, const Vector2& b) {
        if (a.MagnitudeSquared() <= b.MagnitudeSquared())
            return a;
        return b;
    }

    static float Dot(const Vector2& a, const Vector2& b) {
        return (a.x * b.x) + (a.y * b.y);
    }

    static float Cross(const Vector2& a, const Vector2& b) {
        return (a.x * b.y) - (a.y * b.x);
    }

    // input vector rotated 90 degrees
    static Vector2 OrthogonalCCW(const Vector2& v) {
        return Vector2(-v.y, v.x);
    }

    // input vector rotated -90 degrees
    static Vector2 OrthogonalCW(const Vector2& v) {
        return Vector2(v.y, -v.x);
    }

    static float AngleRadians(const Vector2& a, const Vector2& b) {
        return atan2(Cross(a, b)+Epsilon, Dot(a, b));
    }

    static float AngleDegrees(const Vector2& a, const Vector2& b) {
        return AngleRadians(a, b) * Rad2Deg;
    }

    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
        return a + ((b - a) * t);
    }

    
    float Dot(const Vector2& v) const { return Dot(*this, v); }
    Vector2 Cross(const Vector2& v) const { return Cross(*this, v); }
    Vector2 OrthogonalCCW() const { return OrthogonalCCW(*this); }
    Vector2 OrthogonalCW() const { return OrthogonalCW(*this); }
    float AngleRadians(const Vector2& v) const { return AngleRadians(*this, v); }
    float AngleDegrees(const Vector2& v) const { return AngleDegrees(*this, v); }
    Vector2 Lerp(const Vector2& v, float t) const { return Lerp(*this, v, t); }

    friend std::ostream& operator <<(std::ostream& os, const Vector2& v) {
        os << '(' << v[0] << ',' << v[1] << ')';
        return os;
    }

    static const Vector2
        UnitX, UnitY,
        Up, Down, Left, Right,
        One, Zero;

    union {
        struct { float x, y; };
        float f[2];
    };
};

const Vector2 Vector2::UnitX(1.0f, 0.0f);
const Vector2 Vector2::UnitY(0.0f, 1.0f);

const Vector2 Vector2::Up(0.0f, 1.0f);
const Vector2 Vector2::Down(0.0f, -1.0f);
const Vector2 Vector2::Left(-1.0f, 0.0f);
const Vector2 Vector2::Right(1.0f, 0.0f);

const Vector2 Vector2::One(1.0f, 1.0f);
const Vector2 Vector2::Zero(0.0f, 0.0f);
XOMATH_END_XO_NS

#endif // XOMATH_INTERNAL