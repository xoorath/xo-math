#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Quaternion.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

class _MM_ALIGN16 Quaternion {
#if XO_SSE
    static const unsigned IDX_X = 0;
    static const unsigned IDX_Y = 1;
    static const unsigned IDX_Z = 2;
    static const unsigned IDX_W = 3;
#endif
public:
    _XOINL Quaternion(){};
    _XOINL Quaternion(const Matrix4x4& m){};
    _XOINL Quaternion(float x, float y, float z, float w){};

    _XOINL float& operator [](int i) { return f[i]; };
    _XOINL const float& operator [](int i) const { return f[i]; };

    _XOINL static Quaternion Inverse(const Quaternion& q) { return Identity; };
    _XOINL static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) { return Identity; };
    _XOINL static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t) { return Identity; };
    _XOINL static Quaternion AxisAngleRadians(const Vector3& axis, float angle) { return Identity; };
    _XOINL static Quaternion LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up) { return Identity; };
    _XOINL static Quaternion LookAtFromPosition(const Vector3& from, const Vector3& to) { return Identity; };
    _XOINL static Quaternion LookAtFromDirection(const Vector3& direction, const Vector3& up) { return Identity; };
    _XOINL static Quaternion LookAtFromDirection(const Vector3& direction) { return Identity; };

    _XOINL static float AngleRadians(const Quaternion& a, const Quaternion& b) { return 0.0f; }
    _XOINL static float Dot(const Quaternion& a, const Quaternion& b) { return 0.0f; }

    _XOINL void GetAxisAngleRadians(Vector3& axis, float& radians) const { }
    _XOINL Vector3 GetEulerAngles() const { return Vector3::Zero; }

    union {
        struct {
            float x, y, z, w;
        };
        float f[4];
    #if XO_SSE
        __m128 m;
    #endif
    };

    static const Quaternion
        Identity;
};

const Quaternion Quaternion::Identity = { 0.0f, 0.0f, 0.0f, 1.0f };

XOMATH_END_XO_NS

#endif // XOMATH_INTERNAL