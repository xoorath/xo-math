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

//! @brief A four dimensional euclidean vector, optimized for use in games.
//!
//! Most useful for three dimensional rotations. See Matrix4x4::Transform and Matrix4x4::operator*=.
//! @sa https://en.wikipedia.org/wiki/Euclidean_vector
class _MM_ALIGN16 Vector4 {
public:
    //>See
    //! @name Constructors
    //! @{
    Vector4(); //!< Performs no initialization.
    Vector4(float f); //!< All elements are set to f.
    Vector4(float x, float y, float z, float w); //!< Assigns each named value accordingly.
    Vector4(const Vector4& vec); //!< Copy constructor, trivial.
#if defined(XO_SSE)
    Vector4(const __m128& vec); //!< Assigns m to vec, sets all elements.
#endif
    Vector4(const class Vector2& v); //!< Assigns same-name values from v, zero to z and w. \f$\begin{pmatrix}v.x&v.y&0&0\end{pmatrix}\f$
    Vector4(const class Vector2& v, float z, float w); //!< Assigns same-name values from v, then z and w. \f$\begin{pmatrix}v.x&v.y&0&0\end{pmatrix}\f$
    Vector4(const class Vector3& v); //!< Assigns same-name values from v, zero to w.\f$\begin{pmatrix}v.x&v.y&v.z&0\end{pmatrix}\f$
    Vector4(const class Vector3& v, float w); //!< Assigns same-name values from v, then w.\f$\begin{pmatrix}v.x&v.y&v.z&0\end{pmatrix}\f$
    Vector4(const class Quaternion& q);
    //! @}

    //>See
    //! @name Set / Get Methods
    //! @{

    //! Set all. x, y, z and w will be assigned to the input params.
    Vector4& Set(float x, float y, float z, float w);
    //! Set each. x, y, z and w will be assigned to f.
    Vector4& Set(float f);
    //! Set each. Copies vec into this.
    Vector4& Set(const Vector4& vec);
    //! Set each. Assigns same-name values from vec, zero to z and w.
    Vector4& Set(const Vector2& vec);
    //! Set each. Assigns same-name values from vec, then z and w.
    Vector4& Set(const Vector2& vec, float z, float w);
    //! Set each. Assigns same-name values from vec, zero to w.
    Vector4& Set(const Vector3& vec);
    //! Set each. Assigns same-name values from vec, then w.
    Vector4& Set(const Vector3& vec, float w);
#if defined(XO_SSE)
    //! Set each. Copies vec int m.
    Vector4& Set(const __m128& vec);
#endif
    //! Extract all getter. x, y, z and w will be assigned to those values of this vector.
    void Get(float& x, float& y, float& z, float& w) const;
    //! Extract all getter. f[0], f[1], f[2] and f[3] will be assigned to x, y, z and w respectively.
    //! @Note When using SSE f must be aligned. Example: _MM_ALIGN16 float f[4];
    void Get(float* f) const;
    //! @}

    //>See
    //! @name Special Operators
    //! @{

    //! Overloads the new and delete operators for Vector4 when memory alignment is required (such as with SSE).
    //! @sa XO_16ALIGNED_MALLOC, XO_16ALIGNED_FREE
    _XO_OVERLOAD_NEW_DELETE();
#if defined(XO_SSE)
    //! Type cast operator. Allows Vector4 to be used implicitly where ever __m128 can be.
    _XOINL operator const __m128&() const;
#endif
    //! Type cast operator. Allows Vector4 to be used implicitly wherever a float* can be.
    _XOINL operator float*() const { return (float*)f; }
    //! Extract reference operator, useful for setting values by index.
    //! \f[i\begin{cases}0 & return\ x;\\1 & return\ y;\\2 & return\ z;\\3 & return\ w;\\? & undefined\end{cases}\f]
    _XOINL float& operator [](int i);
    //! Extract const reference operator, useful for getting values by index.
    //! \f[i\begin{cases}0 & return\ x;\\1 & return\ y;\\2 & return\ z;\\3 & return\ w;\\? & undefined\end{cases}\f]
    _XOINL const float& operator [](int i) const;
    //! Negate operator. Returns a vector with all elements with a flipped sign:
    //!
    //! \f$\begin{pmatrix}-x,&-y,&-z,&-w\end{pmatrix}\f$
    _XOINL Vector4 operator -() const;
    //! Returns this vector swizzled so elements appear in reverse order. Vector4::WZYX provides the same functionality.
    //!
    //! \f$\begin{pmatrix}w,&z,&y,&x\end{pmatrix}\f$
    _XOINL Vector4 operator ~() const;
    //! @}

    //>See
    //! @name Math Operators
    //! Operates on all same-name vector elements with other vector types, or all elements to scalar/integer types.
    //! @sa XO_NO_INVERSE_DIVISION
    //! @{
    _XOINL Vector4& operator += (const Vector4& v);
    _XOINL Vector4& operator += (float v);
    _XOINL Vector4& operator += (double v);
    _XOINL Vector4& operator += (int v);
    _XOINL Vector4& operator += (const class Vector2& v);
    _XOINL Vector4& operator += (const class Vector3& v);
    _XOINL Vector4& operator -= (const Vector4& v);
    _XOINL Vector4& operator -= (float v);
    _XOINL Vector4& operator -= (double v);
    _XOINL Vector4& operator -= (int v);
    _XOINL Vector4& operator -= (const class Vector2& v);
    _XOINL Vector4& operator -= (const class Vector3& v);
    _XOINL Vector4& operator *= (const Vector4& v);
    _XOINL Vector4& operator *= (float v);
    _XOINL Vector4& operator *= (double v);
    _XOINL Vector4& operator *= (int v);
    _XOINL Vector4& operator *= (const class Vector2& v);
    _XOINL Vector4& operator *= (const class Vector3& v);
    // See: XO_NO_INVERSE_DIVISION
    _XOINL Vector4& operator /= (const Vector4& v);
    _XOINL Vector4& operator /= (float v);
    _XOINL Vector4& operator /= (double v);
    _XOINL Vector4& operator /= (int v);
    _XOINL Vector4& operator /= (const class Vector2& v);
    _XOINL Vector4& operator /= (const class Vector3& v);
    _XOINL Vector4 operator + (const Vector4& v) const;
    _XOINL Vector4 operator + (float v) const;
    _XOINL Vector4 operator + (double v) const;
    _XOINL Vector4 operator + (int v) const;
    _XOINL Vector4 operator + (const class Vector2& v) const;
    _XOINL Vector4 operator + (const class Vector3& v) const;
    _XOINL Vector4 operator - (const Vector4& v) const;
    _XOINL Vector4 operator - (float v) const;
    _XOINL Vector4 operator - (double v) const;
    _XOINL Vector4 operator - (int v) const;
    _XOINL Vector4 operator - (const class Vector2& v) const;
    _XOINL Vector4 operator - (const class Vector3& v) const;
    _XOINL Vector4 operator * (const Vector4& v) const;
    _XOINL Vector4 operator * (float v) const;
    _XOINL Vector4 operator * (double v) const;
    _XOINL Vector4 operator * (int v) const;
    _XOINL Vector4 operator * (const class Vector2& v) const;
    _XOINL Vector4 operator * (const class Vector3& v) const;
    // See: XO_NO_INVERSE_DIVISION
    _XOINL Vector4 operator / (const Vector4& v) const;
    _XOINL Vector4 operator / (float v) const;
    _XOINL Vector4 operator / (double v) const;
    _XOINL Vector4 operator / (int v) const;
    _XOINL Vector4 operator / (const class Vector2& v) const;
    _XOINL Vector4 operator / (const class Vector3& v) const;
    //! @}

    //>See
    //! @name Comparison Operators
    //! When comparing (<, >, <=, >=) against other vectors, the square magnitude is compared.
    //! 
    //! When comparing (<, >, <=, >=) against scalars, the scalar is squared and compared to the SquareMagnitude() of this vector.
    //!
    //! When comparing equality (==, !=) against other vectors, each same name element is compared. If each element has a 
    //! difference of <= Vector4::Epsilon the vector is considered equal.
    //!
    //! When comparing equality (==, !=) against scalars, the scalar is squared and compared to the SquareMagnitude() of this vector.
    //! @{
    _XOINL bool operator < (const Vector4& v) const;
    _XOINL bool operator < (float v) const;
    _XOINL bool operator < (double v) const;
    _XOINL bool operator < (int v) const;
    _XOINL bool operator < (const class Vector2& v) const;
    _XOINL bool operator < (const class Vector3& v) const;
    _XOINL bool operator <= (const Vector4& v) const;
    _XOINL bool operator <= (float v) const;
    _XOINL bool operator <= (double v) const;
    _XOINL bool operator <= (int v) const;
    _XOINL bool operator <= (const class Vector2& v) const;
    _XOINL bool operator <= (const class Vector3& v) const;
    _XOINL bool operator > (const Vector4& v) const;
    _XOINL bool operator > (float v) const;
    _XOINL bool operator > (double v) const;
    _XOINL bool operator > (int v) const;
    _XOINL bool operator > (const class Vector2& v) const;
    _XOINL bool operator > (const class Vector3& v) const;
    _XOINL bool operator >= (const Vector4& v) const;
    _XOINL bool operator >= (float v) const;
    _XOINL bool operator >= (double v) const;
    _XOINL bool operator >= (int v) const;
    _XOINL bool operator >= (const class Vector2& v) const;
    _XOINL bool operator >= (const class Vector3& v) const;

    _XOINL bool operator == (const Vector4& v) const;
    _XOINL bool operator == (float v) const;
    _XOINL bool operator == (double v) const;
    _XOINL bool operator == (int v) const;
    _XOINL bool operator == (const class Vector2& v) const;
    _XOINL bool operator == (const class Vector3& v) const;
    _XOINL bool operator != (const Vector4& v) const;
    _XOINL bool operator != (float v) const;
    _XOINL bool operator != (double v) const;
    _XOINL bool operator != (int v) const;
    _XOINL bool operator != (const class Vector2& v) const;
    _XOINL bool operator != (const class Vector3& v) const;
    //! @}

    //!> See
    //! @name Methods
    //! @{

    //! Returns true when the Magnitude of this vector is within Vector4::Epsilon of being 1.0
    bool IsNormalized() const {
        return CloseEnough(MagnitudeSquared(), 1.0f, Epsilon);
    }
    //! Returns true when the Magnitude of this vector is <= Vector4::Epsilon
    bool IsZero() const {
        return CloseEnough(MagnitudeSquared(), 0.0f, Epsilon);
    }
    //! The length of this vector in 4 dimensional space.
    //! It's preferred to use Vector4::MagnitudeSquared when possible, as Vector4::Magnitude requires a call to Sqrt.
    //!
    //! \f$\lvert\rvert\boldsymbol{this}\lvert\rvert = \sqrt{(x\times x)+(y\times y)+(z\times z)+(w\times w)}\f$
    //! @sa https://en.wikipedia.org/wiki/Magnitude_(mathematics)#Euclidean_vector_space
    float Magnitude() const {
        return Sqrt(MagnitudeSquared());
    }
    //! The square length of this vector in 4 dimensional space.
    //! It's preferred to use Vector4::MagnitudeSquared when possible, as Vector4::Magnitude requires a call to Sqrt.
    //!
    //! \f$\lvert\rvert\boldsymbol{this}\lvert\rvert^2 = (x\times x)+(y\times y)+(z\times z)+(w\times w)\f$
    //! @sa https://en.wikipedia.org/wiki/Magnitude_(mathematics)#Euclidean_vector_space
    float MagnitudeSquared() const {
        return ((*this) * (*this)).Sum();
    }
    //! The sum of all vector elements.
    //!
    //! \f$x+y+z+w\f$
    float Sum() const;

    //! Normalizes this vector to a Magnitude of 1.
    //! @sa https://en.wikipedia.org/wiki/Unit_vector
    Vector4& Normalize() {
        return (*this) /= Magnitude();
    }

    Vector4& NormalizeSafe();

    //! Returns a copy of this vector with a Magnitude of 1.
    //! @sa https://en.wikipedia.org/wiki/Unit_vector
    Vector4 Normalized() const {
        return Vector4(*this).Normalize();
    }

    Vector3 NormalizedSafe() const {
        return Vector3(*this).NormalizeSafe();
    }
    //! @}

    //>See
    //! @name Static Methods
    //! @{

    //! Sets outVec to a vector interpolated between a and b by a scalar amount t.
    //! @sa https://en.wikipedia.org/wiki/Linear_interpolation
    static void Lerp(const Vector4& a, const Vector4& b, float t, Vector4& outVec) {
        outVec = a + ((b - a) * t);
    }
    //! Set outVec to have elements equal to the max of each element in a and b.
    //!
    //! \f$\begin{pmatrix}\max(a.x, b.x)&\max(a.y, b.y)&\max(a.z, b.z)&\max(a.w, b.w)\end{pmatrix}\f$
    static void Max(const Vector4& a, const Vector4& b, Vector4& outVec) {
        outVec.Set(_XO_MAX(a.x, b.x), _XO_MAX(a.y, b.y), _XO_MAX(a.z, b.z), _XO_MAX(a.w, b.w));
    }
    //! Set outVec to have elements equal to the min of each element in a and b.
    //!
    //! \f$\begin{pmatrix}\min(a.x, b.x)&\min(a.y, b.y)&\min(a.z, b.z)&\min(a.w, b.w)\end{pmatrix}\f$
    static void Min(const Vector4& a, const Vector4& b, Vector4& outVec) {
        outVec.Set(_XO_MIN(a.x, b.x), _XO_MIN(a.y, b.y), _XO_MIN(a.z, b.z), _XO_MIN(a.w, b.w));
    }
    //! Returns the distance between vectors a and b in 4 dimensional space.
    //! It's preferred to use the DistanceSquared when possible, as Distance requires a call to Sqrt.
    //!
    //! \f$distance = \lvert\rvert\boldsymbol{b-a}\lvert\rvert\f$
    static float Distance(const Vector4&a, const Vector4&b) {
        return (b - a).Magnitude();
    }
    //! Returns the square distance between vectors a and b in 4 dimensional space.
    //! It's preferred to use the DistanceSquared when possible, as Distance requires a call to Sqrt.
    //!
    //! \f$distance^2 = \lvert\rvert\boldsymbol{b-a}\lvert\rvert^2\f$
    static float DistanceSquared(const Vector4& a, const Vector4& b) {
        return (b - a).MagnitudeSquared();
    }
    //! Returns a single number representing a product of magnitudes. Commonly used with two normalized 
    //! vectors to determine if they are pointed the same way. In this case: 1.0 represents same-facing vectors
    //! 0 represents perpendicular vectors, and -1 will be facing away
    //!
    //! \f$a\cdot b =(a.x\times b.x) + (a.y\times b.y) + (a.z\times b.z) + (a.w\times  b.w)\f$
    //!
    //! @sa https://en.wikipedia.org/wiki/Dot_product
    static float Dot(const Vector4& a, const Vector4& b);
    //! @}
    
#define _RET_VARIANT(name) { Vector4 tempV; name(
#define _RET_VARIANT_END() tempV); return tempV; }
#define _RET_VARIANT_0(name)                                 _RET_VARIANT(name)                               _RET_VARIANT_END()
#define _RET_VARIANT_1(name, first)                          _RET_VARIANT(name) first,                        _RET_VARIANT_END()
#define _RET_VARIANT_2(name, first, second)                  _RET_VARIANT(name) first, second,                _RET_VARIANT_END()
#define _RET_VARIANT_3(name, first, second, third)           _RET_VARIANT(name) first, second, third,         _RET_VARIANT_END()
#define _RET_VARIANT_4(name, first, second, third, fourth)   _RET_VARIANT(name) first, second, third, fourth, _RET_VARIANT_END()
#define _THIS_VARIANT1(name, first)                         { return name(*this, first); }
#define _THIS_VARIANT2(name, first, second)                 { return name(*this, first, second); }

    //>See
    //! @name Variants
    //! Variants of other same-name static methods. See their documentation for more details under the 
    //! Static Methods heading.
    //!
    //! Non static variants replace the first Vector4 parameter by 'this' vector.
    //! Static variants return what would have been the outVec param.
    //! @{
    static Vector4 Lerp(const Vector4& a, const Vector4& b, float t)    _RET_VARIANT_3(Lerp, a, b, t)
    static Vector4 Max(const Vector4& a, const Vector4& b)              _RET_VARIANT_2(Max, a, b)
    static Vector4 Min(const Vector4& a, const Vector4& b)              _RET_VARIANT_2(Min, a, b)

    float Distance(const Vector4& v) const                              _THIS_VARIANT1(Distance, v)
    float DistanceSquared(const Vector4& v) const                       _THIS_VARIANT1(DistanceSquared, v)
    float Dot(const Vector4& v) const                                   _THIS_VARIANT1(Dot, v)
    Vector4 Lerp(const Vector4& v, float t) const                       _THIS_VARIANT2(Lerp, v, t)
    //! @}

#undef _RET_VARIANT
#undef _RET_VARIANT_END
#undef _RET_VARIANT_0
#undef _RET_VARIANT_1
#undef _RET_VARIANT_2
#undef _RET_VARIANT_3
#undef _RET_VARIANT_4
#undef _THIS_VARIANT1
#undef _THIS_VARIANT2

    //>See
    //! @name Extras
    //! @{

    //! @todo Make this optional with a define.
    //! Prints the contents of vector v and its magnitude to the provided ostream.
#ifndef XO_NO_OSTREAM
    friend std::ostream& operator <<(std::ostream& os, const Vector4& v) {
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", w:" << v.w << ", mag:" << v.Magnitude() << ")";
        return os;
    }
#endif
    //! @}

    ////////////////////////////////////////////////////////////////////////// Static Attributes
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#public_static_attributes
    static const Vector4
        One, //!< \f$\begin{pmatrix}1&1&1&1\end{pmatrix}\f$
        Zero, //!< \f$\begin{pmatrix}0&0&0&0\end{pmatrix}\f$
        UnitX, //!< \f$\begin{pmatrix}1&0&0&0\end{pmatrix}\f$
        UnitY, //!< \f$\begin{pmatrix}0&1&0&0\end{pmatrix}\f$
        UnitZ, //!< \f$\begin{pmatrix}0&0&1&0\end{pmatrix}\f$
        UnitW; //!< \f$\begin{pmatrix}0&0&0&1\end{pmatrix}\f$

#if defined(_XONOCONSTEXPR)
    static const float Epsilon;
#else
#   if defined(XO_SSE)
    //! Smallest increment from zero that could be assigned to each element of this vector and would still be seen as equal to a zero vector.
    _XOCONSTEXPR static const float Epsilon = sse::SSEFloatEpsilon * 4.0f;
#   else
    _XOCONSTEXPR static const float Epsilon = FloatEpsilon * 4.0f;
#   endif
#endif

    ////////////////////////////////////////////////////////////////////////// Members
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#public_members
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        float f[4]; //!< ordered as \f$\begin{pmatrix}x&y&z&w\end{pmatrix}\f$
#if defined(XO_SSE)
        //! Exists when SSE is in use, represents a 128 bit xmm register.
        //! @sa https://en.wikipedia.org/wiki/Streaming_SIMD_Extensions
        __m128 m;
#endif
    };
};

XOMATH_END_XO_NS();