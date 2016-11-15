// The MIT License (MIT)
//
// Copyright (c) 2016 Jared Thomson
//
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included 
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT 
// OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
// THE USE OR OTHER DEALINGS IN THE SOFTWARE.

XOMATH_BEGIN_XO_NS();

class _MM_ALIGN16 Quaternion {
public:
    Quaternion();
    Quaternion(const Matrix4x4& m);
    Quaternion(float x, float y, float z, float w);

    _XO_OVERLOAD_NEW_DELETE();

    _XOINL float& operator [](int i);
    _XOINL const float& operator [](int i) const;

    _XOINL Quaternion& operator *= (const Quaternion& q);
    _XOINL Quaternion operator * (const Quaternion& q) const;

    _XOINL bool operator == (const Quaternion& q) const;
    _XOINL bool operator != (const Quaternion& q) const;

    Quaternion& MakeConjugate();
    Quaternion& MakeInverse();
    Quaternion& Normalize();
    Quaternion Conjugate() const;
    Quaternion Inverse() const;
    Quaternion Normalized() const;
    void GetAxisAngleRadians(Vector3& axis, float& radians) const;

    static void AxisAngleRadians(const Vector3& axis, float radians, Quaternion& outQuat);
    static void Lerp(const Quaternion& a, const Quaternion& b, float t, Quaternion& outQuat);
    static void LookAtFromDirection(const Vector3& direction, const Vector3& up, Quaternion& outQuat);
    static void LookAtFromDirection(const Vector3& direction, Quaternion& outQuat);
    static void LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up, Quaternion& outQuat);
    static void LookAtFromPosition(const Vector3& from, const Vector3& to, Quaternion& outQuat);
    static void RotationRadians(const Vector3& v, Quaternion& outQuat);
    static void RotationRadians(float x, float y, float z, Quaternion& outQuat);
    static void Slerp(const Quaternion& a, const Quaternion& b, float t, Quaternion& outQuat);

#define _RET_VARIANT(name) { Quaternion tempV; name(
#define _RET_VARIANT_END() tempV); return tempV; }
#define _RET_VARIANT_0(name)                                 _RET_VARIANT(name)                               _RET_VARIANT_END()
#define _RET_VARIANT_1(name, first)                          _RET_VARIANT(name) first,                        _RET_VARIANT_END()
#define _RET_VARIANT_2(name, first, second)                  _RET_VARIANT(name) first, second,                _RET_VARIANT_END()
#define _RET_VARIANT_3(name, first, second, third)           _RET_VARIANT(name) first, second, third,         _RET_VARIANT_END()
#define _RET_VARIANT_4(name, first, second, third, fourth)   _RET_VARIANT(name) first, second, third, fourth, _RET_VARIANT_END()
#define _THIS_VARIANT1(name, first)                         { return name(*this, first); }
#define _THIS_VARIANT2(name, first, second)                 { return name(*this, first, second); }

    static Quaternion AxisAngleRadians(const Vector3& axis, float radians)                          _RET_VARIANT_2(AxisAngleRadians, axis, radians)
    static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t)                       _RET_VARIANT_3(Lerp, a, b, t)
    static Quaternion LookAtFromDirection(const Vector3& direction)                                 _RET_VARIANT_1(LookAtFromDirection, direction)
    static Quaternion LookAtFromDirection(const Vector3& direction, const Vector3& up)              _RET_VARIANT_2(LookAtFromDirection, direction, up)
    static Quaternion LookAtFromPosition(const Vector3& from, const Vector3& to)                    _RET_VARIANT_2(LookAtFromPosition, from, to)
    static Quaternion LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up) _RET_VARIANT_3(LookAtFromPosition, from, to, up)
    static Quaternion RotationRadians(const Vector3& v)                                             _RET_VARIANT_1(RotationRadians, v)
    static Quaternion RotationRadians(float x, float y, float z)                                    _RET_VARIANT_3(RotationRadians, x, y, z)
    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t)                      _RET_VARIANT_3(Slerp, a, b, t)

#undef _RET_VARIANT
#undef _RET_VARIANT_END
#undef _RET_VARIANT_0
#undef _RET_VARIANT_1
#undef _RET_VARIANT_2
#undef _RET_VARIANT_3
#undef _RET_VARIANT_4
#undef _THIS_VARIANT1
#undef _THIS_VARIANT2

    static const Quaternion
        Identity,
        Zero;

#if defined(_XONOCONSTEXPR)
    static const float Epsilon;
#else
#   if defined(XO_SSE)
    _XOCONSTEXPR static const float Epsilon = sse::SSEFloatEpsilon * 4.0f;
#   else
    _XOCONSTEXPR static const float Epsilon = FloatEpsilon * 4.0f;
#   endif
#endif

    union {
        struct {
            float x, y, z, w;
        };
        float f[4];
#if defined(XO_SSE)
        __m128 xmm;
#endif
    };

private:
#if defined(XO_SSE)
    static const unsigned IDX_X = 0;
    static const unsigned IDX_Y = 1;
    static const unsigned IDX_Z = 2;
    static const unsigned IDX_W = 3;
#endif
};

XOMATH_END_XO_NS();