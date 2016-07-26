#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector2.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS
class Vector2 {
public:
    constexpr static const float Epsilon = FloatEpsilon * 2.0f;

    Vector2() : x(0.0f), y(0.0f) {
    }
    Vector2(float v) : x(v), y(v) {
    }
    Vector2(float x, float y) : x(x), y(y) {
    }
    Vector2(const Vector2& v) : x(v[0]), y(v[1]) {
    }
    //Vector2(const class Vector3& v);

    float& operator [](int i) { switch (i) { default: case 0: return x; case 1: return y; } }
    const float& operator [](int i) const { switch (i) { default: case 0: return x; case 1: return y; } }

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

    Vector2 operator + (const class Vector3& v) const;

    float Magnitude() const {
        return sqrtf(x*x + y*y);
    }

    float MagnitudeSquared() const {
        return x*x + y*y;
    }

    void Normalize() {
        float magnitude = MagnitudeSquared();
        if (magnitude - 1.0f <= Epsilon)
            return; // already normalized
        magnitude = sqrtf(magnitude);
        if (magnitude < Epsilon)
            return; // zero vec
        magnitude = 1.0f / magnitude;
        x *= magnitude;
        y *= magnitude;
    }

    Vector2 Normalized() const {
        float magnitude = MagnitudeSquared();
        if (magnitude - 1.0f <= Epsilon)
            return *this; // already normalized
        magnitude = sqrtf(magnitude);
        if (magnitude < Epsilon)
            return *this; // zero vec
        magnitude = 1.0f / magnitude;
        return{ x * magnitude, y * magnitude };
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

    static float AngleRadians(const Vector2& a, const Vector2& b) {
        return 0.0f; // todo
    }

    static float AngleDegrees(const Vector2& a, const Vector2& b) {
        return AngleRadians(a, b) * Rad2Deg;
    }

    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
        return a + ((b - a) * t);
    }

    float AngleRadians(const Vector2& v) { return AngleRadians(*this, v); }
    float AngleDegrees(const Vector2& v) { return AngleDegrees(*this, v); }

    friend std::ostream& operator <<(std::ostream& os, const Vector2& v) {
        os << '(' << v[0] << ',' << v[1] << ')';
        return os;
    }

    static const Vector2
        UnitX, UnitY,
        Up, Down, Left, Right,
        One, Zero;

private:
    float x, y;
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