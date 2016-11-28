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

//! A three dimensional euclidean vector, optimized for use in games.
//! @sa https://en.wikipedia.org/wiki/Euclidean_vector
class _XOSIMDALIGN Vector3 {
public:
    //>See
    //! @name Constructors
    //! @{
    Vector3() { } //!< Performs no initialization.
    Vector3(float f); //!< All elements are set to f.
    Vector3(float x, float y, float z); //!< Assigns each named value accordingly.
    Vector3(const Vector3& vec); //!< Copy constructor, trivial.
#if defined(XO_SSE)
    Vector3(const __m128& vec); //!< Assigns xmm to vec, sets all elements.
#elif defined(XO_NEON)
    Vector3(const float32x4_t& vec); //!< Assigns n to vec, sets all elements.
#endif
    Vector3(const class Vector2& v); //!< Assigns same-name values from v, zero to z. \f$\begin{pmatrix}v.x&v.y&0\end{pmatrix}\f$
    Vector3(const class Vector4& v); //!< Assigns same-name values from v. \f$\begin{pmatrix}v.x&v.y&v.z\end{pmatrix}\f$
    //! @}

    //>See
    //! @name Set / Get Methods
    //! @{

    //! Set all. x, y, and z will be assigned to the input params.
    Vector3& Set(float x, float y, float z);
    //! Set each. x, y, and z will be assigned to f.
    Vector3& Set(float f);
    //! Set each. Copies vec into this.
    Vector3& Set(const Vector3& vec);
#if defined(XO_SSE)
    //! Set each. Copies vec int xmm.
    Vector3& Set(const __m128& vec);
#elif defined(XO_NEON)
    //! Set each. Copies vec int n.
    Vector3& Set(const float32x4_t& vec);
#endif
    //! Extract all getter. x, y, and z will be assigned to those values of this vector.
    void Get(float& x, float& y, float &z) const;
    //! Extract all getter. f[0], f[1], and f[2] will be assigned to x, y, and z respectively.
    //! @Note When using SSE f must be aligned and at least 4 floats wide. Example: _MM_ALIGN16 float f[4];
    void Get(float* f) const;
    //! @}

    //>See
    //! @name Special Operators
    //! @{

    //! Overloads the new and delete operators for Vector3 when memory alignment is required (such as with SSE).
    //! @sa XO_16ALIGNED_MALLOC, XO_16ALIGNED_FREE
    _XO_OVERLOAD_NEW_DELETE();
#if defined(XO_SSE)
    //! Type cast operator. Allows Vector3 to be used implicitly where ever __m128 can be.
    _XOINL operator __m128() const;
#elif defined(XO_NEON)
    //! Type cast operator. Allows Vector3 to be used implicitly where ever float32x4_t can be.
    _XOINL operator float32x4_t() const;
#endif
    //! Extract reference operator, useful for setting values by index.
    //! \f[i\begin{cases}0 & return\ x;\\1 & return\ y;\\2 & return\ z;\\? & undefined\end{cases}\f]
    _XOINL float& operator [](int i);
    //! Extract const reference operator, useful for getting values by index.
    //! \f[i\begin{cases}0 & return\ x;\\1 & return\ y;\\2 & return\ z;\\? & undefined\end{cases}\f]
    _XOINL const float& operator [](int i) const;
    //! Negate operator. Returns a vector with all elements with a flipped sign:
    //!
    //! \f$\begin{pmatrix}-x,&-y,&-z\end{pmatrix}\f$
    _XOINL Vector3 operator -() const;
    //! Returns this vector swizzled so elements appear in reverse order. Vector3::ZYX provides the same functionality.
    //!
    //! \f$\begin{pmatrix}z,&y,&x\end{pmatrix}\f$
    _XOINL Vector3 operator ~() const;
    //! @}

    //>See
    //! @name Math Operators
    //! Operates on all same-name vector elements with other vector types, or all elements to scalar/integer types.
    //! @sa XO_NO_INVERSE_DIVISION
    //! @{
    _XOINL Vector3& operator += (const Vector3& v);
    _XOINL Vector3& operator += (float v);
    _XOINL Vector3& operator += (double v);
    _XOINL Vector3& operator += (int v);
    _XOINL Vector3& operator += (const class Vector2& v);
    _XOINL Vector3& operator += (const class Vector4& v);
    _XOINL Vector3& operator -= (const Vector3& v);
    _XOINL Vector3& operator -= (float v);
    _XOINL Vector3& operator -= (double v);
    _XOINL Vector3& operator -= (int v);
    _XOINL Vector3& operator -= (const class Vector2& v);
    _XOINL Vector3& operator -= (const class Vector4& v);
    _XOINL Vector3& operator *= (const Vector3& v);
    _XOINL Vector3& operator *= (float v);
    _XOINL Vector3& operator *= (double v);
    _XOINL Vector3& operator *= (int v);
    _XOINL Vector3& operator *= (const class Vector2& v);
    _XOINL Vector3& operator *= (const class Vector4& v);
    _XOINL Vector3& operator /= (const Vector3& v);
    _XOINL Vector3& operator /= (float v);
    _XOINL Vector3& operator /= (double v);
    _XOINL Vector3& operator /= (int v);
    _XOINL Vector3& operator /= (const class Vector2& v);
    _XOINL Vector3& operator /= (const class Vector4& v);
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
    //! @}

    //>See
    //! @name Comparison Operators
    //! When comparing (<, >, <=, >=) against other vectors, the square magnitude is compared.
    //! 
    //! When comparing (<, >, <=, >=) against scalars, the scalar is squared and compared to the SquareMagnitude() of this vector.
    //!
    //! When comparing equality (==, !=) against other vectors, each same name element is compared. If each element has a 
    //! difference of <= Vector2::Epsilon the vector is considered equal.
    //!
    //! When comparing equality (==, !=) against scalars, the scalar is squared and compared to the SquareMagnitude() of this vector.
    //! @{
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
    //! @}

    //>See
    //! @name Methods
    //! @{

    //! Returns true when the Magnitude of this vector is within Vector3::Epsilon of being 1.0
    bool IsNormalized() const {
        return CloseEnough(MagnitudeSquared(), 1.0f, Epsilon);
    }
    //! Returns true when the Magnitude of this vector is <= Vector3::Epsilon
    bool IsZero() const {
        return CloseEnough(MagnitudeSquared(), 0.0f, Epsilon);
    }
    //! The length of this vector
    //! It's preferred to use Vector3::MagnitudeSquared when possible, as Vector3::Magnitude requires a call to Sqrt.
    //!
    //! \f$\lvert\rvert\boldsymbol{this}\lvert\rvert = \sqrt{(x\times x)+(y\times y)+(z\times z)}\f$
    //! @sa https://en.wikipedia.org/wiki/Magnitude_(mathematics)#Euclidean_vector_space
    float Magnitude() const {
        return Sqrt(MagnitudeSquared());
    }
    //! The square length of this vector.
    //! It's preferred to use Vector3::MagnitudeSquared when possible, as Vector3::Magnitude requires a call to Sqrt.
    //!
    //! \f$\lvert\rvert\boldsymbol{this}\lvert\rvert^2 = (x\times x)+(y\times y)+(z\times z)\f$
    //! @sa https://en.wikipedia.org/wiki/Magnitude_(mathematics)#Euclidean_vector_space
    float MagnitudeSquared() const {
        return ((*this) * (*this)).Sum();
    }
    //! The sum of all vector elements.
    //!
    //! \f$x+y+z\f$
    float Sum() const;

    //! Normalizes this vector to a Magnitude of 1.
    //! @sa https://en.wikipedia.org/wiki/Unit_vector
    Vector3& Normalize();

    Vector3& NormalizeSafe();

    //! Returns a copy of this vector with a Magnitude of 1.
    //! @sa https://en.wikipedia.org/wiki/Unit_vector
    Vector3 Normalized() const {
        return Vector3(*this).Normalize();
    }

    Vector3 NormalizedSafe() const {
        return Vector3(*this).NormalizeSafe();
    }
    //! Returns this vector swizzled so elements appear in reverse order. Vector3::operator~() provides the same functionality.
    //!
    //! \f$\begin{pmatrix}z,&y,&x\end{pmatrix}\f$
    Vector3 ZYX() const;
    
    //! @}

    //>See
    //! @name Static Methods
    //! @{

    //! Returns the cross product (vector product) of vectors a and b.
    //! @sa https://en.wikipedia.org/wiki/Cross_product
    static void Cross(const Vector3& a, const Vector3& b, Vector3& outVec);
    //! Sets outVec to a vector interpolated between a and b by a scalar amount t.
    //! @sa https://en.wikipedia.org/wiki/Linear_interpolation
    static void Lerp(const Vector3& a, const Vector3& b, float t, Vector3& outVec) {
        outVec = a + ((b - a) * t);
    }
    //! Set outVec to have elements equal to the max of each element in a and b.
    //!
    //! \f$\begin{pmatrix}\max(a.x, b.x)&\max(a.y, b.y)&\max(a.z, b.z)\end{pmatrix}\f$
    static void Max(const Vector3& a, const Vector3& b, Vector3& outVec) {
        outVec.Set(_XO_MAX(a.x, b.x), _XO_MAX(a.y, b.y), _XO_MAX(a.z, b.z));
    }
    //! Set outVec to have elements equal to the min of each element in a and b.
    //!
    //! \f$\begin{pmatrix}\min(a.x, b.x)&\min(a.y, b.y)&\min(a.z, b.z)\end{pmatrix}\f$
    static void Min(const Vector3& a, const Vector3& b, Vector3& outVec) {
        outVec.Set(_XO_MIN(a.x, b.x), _XO_MIN(a.y, b.y), _XO_MIN(a.z, b.z));
    }
    //! Calls Vector3::RotateRadians, converting the input angle in degrees to radians.
    static void RotateDegrees(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec) {
        RotateRadians(v, axis, angle * Deg2Rad, outVec);
    }
    //! Assign outVec to v rotated along an axis by a specified angle in radians.
    static void RotateRadians(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec);
    // Calls Vector3::AngleRadians, converting the return value to degrees.
    static float AngleDegrees(const Vector3& a, const Vector3& b) {
        return AngleRadians(a, b) * Rad2Deg;
    }
    //! Returns the angle in radians between vector a and b.
    static float AngleRadians(const Vector3& a, const Vector3& b);
    //! Returns the distance between vectors a and b in 3 dimensional space.
    //! It's preferred to use the DistanceSquared when possible, as Distance requires a call to Sqrt.
    //!
    //! \f$distance = \lvert\rvert\boldsymbol{b-a}\lvert\rvert\f$
    static float Distance(const Vector3&a, const Vector3&b) {
        return (b - a).Magnitude();
    }
    //! Returns the square distance between vectors a and b in 3 dimensional space.
    //! It's preferred to use the DistanceSquared when possible, as Distance requires a call to Sqrt.
    //!
    //! \f$distance^2 = \lvert\rvert\boldsymbol{b-a}\lvert\rvert^2\f$
    static float DistanceSquared(const Vector3& a, const Vector3& b) {
        return (b - a).MagnitudeSquared();
    }
    //! Returns a single number representing a product of magnitudes. Commonly used with two normalized 
    //! vectors to determine if they are pointed the same way. In this case: 1.0 represents same-facing vectors
    //! 0 represents perpendicular vectors, and -1 will be facing away
    //!
    //! \f$a\cdot b =(a.x\times b.x) + (a.y\times b.y) + (a.z\times b.z)\f$
    //!
    //! @sa https://en.wikipedia.org/wiki/Dot_product
    static float Dot(const Vector3& a, const Vector3& b);
    
    //! @}

    //>See
    //! @name Random Methods
    //! @{
 
    static void RandomInCircle(const Vector3& up, float radius, Vector3& outVec);
    static void RandomInConeRadians(const Vector3& forward, float angle, Vector3& outVec);
    static void RandomOnCircle(const Vector3& up, float radius, Vector3& outVec);
    static void RandomOnConeRadians(const Vector3& forward, float angle, Vector3& outVec);
    static void RandomOnCube(float size, Vector3& outVec);
    static void RandomOnSphere(float radius, Vector3& outVec);

    static void RandomOnCircle(Vector3& outVec) {
        RandomOnCircle(Vector3::Up, 1.0f, outVec);
    }
    static void RandomInFanRadians(const Vector3& forward, const Vector3& up, float angle, Vector3& outVec) {
        Vector3::RotateRadians(forward, up, RandomRange(-angle*0.5f, angle*0.5f), outVec);
    }
    static void RandomInCube(float size, Vector3& outVec) {
        outVec.Set(RandomRange(-size, size), RandomRange(-size, size), RandomRange(-size, size));
    }
    static void RandomInSphere(float minRadius, float maxRadius, Vector3& outVec) {
        RandomOnSphere(Sqrt(RandomRange(minRadius, maxRadius)), outVec);
    }
    static void RandomInSphere(float radius, Vector3& outVec) {
        RandomInSphere(0.0f, radius, outVec);
    }
    static void RandomInSphere(Vector3& outVec) {
        RandomInSphere(1.0f, outVec);
    }
    static void RandomInCircle(Vector3& outVec) {
        RandomInCircle(Vector3::Up, 1.0f, outVec);
    }
    static void RandomInCube(Vector3& outVec) {
        RandomInCube(0.5f, outVec);
    }
    static void RandomInConeDegrees(const Vector3& forward, float angle, Vector3& outVec) { 
        RandomInConeRadians(forward, angle * Deg2Rad, outVec); 
    }
    static void RandomOnCube(Vector3& outVec) {
        RandomOnCube(0.5f, outVec);
    }
    static void RandomInFanDegrees(const Vector3& forward, const Vector3& up, float angle, Vector3& outVec) { 
        RandomInFanRadians(forward, up, angle * Deg2Rad, outVec); 
    }
    static void RandomOnConeDegrees(const Vector3& forward, float angle, Vector3& outVec) { 
        RandomOnConeRadians(forward, angle * Deg2Rad, outVec); 
    }
    static void RandomOnSphere(Vector3& outVec) {
        RandomOnSphere(1.0f, outVec);
    }

    //! @}

#define _RET_VARIANT(name) { Vector3 tempV; name(
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
    //! Non static variants replace the first Vector3 parameter by 'this' vector.
    //! Static variants return what would have been the outVec param.
    //! @{

    float AngleDegrees(const Vector3& v) const                                                  _THIS_VARIANT1(AngleDegrees, v)
    float AngleRadians(const Vector3& v) const                                                  _THIS_VARIANT1(AngleRadians, v)
    float Distance(const Vector3& v) const                                                      _THIS_VARIANT1(Distance, v)
    float DistanceSquared(const Vector3& v) const                                               _THIS_VARIANT1(DistanceSquared, v)
    float Dot(const Vector3& v) const                                                           _THIS_VARIANT1(Dot, v)
    Vector3 Cross(const Vector3& v) const                                                       _THIS_VARIANT1(Cross, v)
    Vector3 Lerp(const Vector3& v, float t) const                                               _THIS_VARIANT2(Lerp, v, t)
    Vector3 RotateDegrees(const Vector3& axis, float angle) const                               _THIS_VARIANT2(RotateDegrees, axis, angle)
    Vector3 RotateRadians(const Vector3& axis, float angle) const                               _THIS_VARIANT2(RotateRadians, axis, angle)

    static Vector3 Cross(const Vector3& a, const Vector3& b)                                    _RET_VARIANT_2(Cross, a, b)
    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t)                            _RET_VARIANT_3(Lerp, a, b, t)
    static Vector3 Max(const Vector3& a, const Vector3& b)                                      _RET_VARIANT_2(Max, a, b)
    static Vector3 Min(const Vector3& a, const Vector3& b)                                      _RET_VARIANT_2(Min, a, b)
    static Vector3 RandomInCircle()                                                             _RET_VARIANT_0(RandomInCircle)
    static Vector3 RandomInCircle(const Vector3& up, float radius)                              _RET_VARIANT_2(RandomInCircle, up, radius)
    static Vector3 RandomInConeDegrees(const Vector3& forward, float angle)                     _RET_VARIANT_2(RandomInConeDegrees, forward, angle)
    static Vector3 RandomInConeRadians(const Vector3& forward, float angle)                     _RET_VARIANT_2(RandomInConeRadians, forward, angle)
    static Vector3 RandomInCube()                                                               _RET_VARIANT_0(RandomInCube)
    static Vector3 RandomInCube(float size)                                                     _RET_VARIANT_1(RandomInCube, size)
    static Vector3 RandomInFanDegrees(const Vector3& forward, const Vector3& up, float angle)   _RET_VARIANT_3(RandomInFanDegrees, forward, up, angle)
    static Vector3 RandomInFanRadians(const Vector3& forward, const Vector3& up, float angle)   _RET_VARIANT_3(RandomInFanRadians, forward, up, angle)
    static Vector3 RandomInSphere()                                                             _RET_VARIANT_0(RandomInSphere)
    static Vector3 RandomInSphere(float minRadius, float maxRadius)                             _RET_VARIANT_2(RandomInSphere, minRadius, maxRadius)
    static Vector3 RandomInSphere(float radius)                                                 _RET_VARIANT_1(RandomInSphere, radius)
    static Vector3 RandomOnCircle()                                                             _RET_VARIANT_0(RandomOnCircle)
    static Vector3 RandomOnCircle(const Vector3& up, float radius)                              _RET_VARIANT_2(RandomOnCircle, up, radius)
    static Vector3 RandomOnConeDegrees(const Vector3& forward, float angle)                     _RET_VARIANT_2(RandomOnConeDegrees, forward, angle)
    static Vector3 RandomOnConeRadians(const Vector3& forward, float angle)                     _RET_VARIANT_2(RandomOnConeRadians, forward, angle)
    static Vector3 RandomOnCube()                                                               _RET_VARIANT_0(RandomOnCube)
    static Vector3 RandomOnCube(float size)                                                     _RET_VARIANT_1(RandomOnCube, size)
    static Vector3 RandomOnSphere()                                                             _RET_VARIANT_0(RandomOnSphere)
    static Vector3 RandomOnSphere(float radius)                                                 _RET_VARIANT_1(RandomOnSphere, radius)
    static Vector3 RotateDegrees(const Vector3& v, const Vector3& axis, float angle)            _RET_VARIANT_3(RotateDegrees, v, axis, angle)
    static Vector3 RotateRadians(const Vector3& v, const Vector3& axis, float angle)            _RET_VARIANT_3(RotateRadians, v, axis, angle)
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

    //! Prints the contents of vector v and its magnitude to the provided ostream.
#ifndef XO_NO_OSTREAM
    friend std::ostream& operator <<(std::ostream& os, const Vector3& v) {
#   if defined(XO_SSE)
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", w:" << v.w << ", mag:" << v.Magnitude() << ")";
#   else
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", mag:" << v.Magnitude() << ")";
#   endif
        return os;
    }
#endif
    //! @}

    ////////////////////////////////////////////////////////////////////////// Static Attributes
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#public_static_attributes
    static const Vector3
        Origin, //!< \f$\begin{pmatrix}0&0&0\end{pmatrix}\f$
        UnitX, //!< \f$\begin{pmatrix}1&0&0\end{pmatrix}\f$
        UnitY, //!< \f$\begin{pmatrix}0&1&0\end{pmatrix}\f$
        UnitZ, //!< \f$\begin{pmatrix}0&0&1\end{pmatrix}\f$
        //! @sa XO_SPACE_ZUP, XO_SPACE_YUP
        Up,
        //! @sa XO_SPACE_ZUP, XO_SPACE_YUP
        Down,
        Left, //!< \f$\begin{pmatrix}-1&0&0\end{pmatrix}\f$
        Right, //!< \f$\begin{pmatrix}1&0&0\end{pmatrix}\f$
        //! @sa XO_SPACE_LEFTHAND, XO_SPACE_RIGHTHAND, XO_SPACE_ZUP, XO_SPACE_YUP
        Forward,
        //! @sa XO_SPACE_LEFTHAND, XO_SPACE_RIGHTHAND, XO_SPACE_ZUP, XO_SPACE_YUP
        Backward,
        One, //!< \f$\begin{pmatrix}1&1&1\end{pmatrix}\f$
        Zero; //!< \f$\begin{pmatrix}0&0&0\end{pmatrix}\f$

#if defined(_XONOCONSTEXPR)
    static const float Epsilon;
#else
#   if defined(XO_SSE)
    //! Smallest increment from zero that could be assigned to each element of this vector and would still be seen as equal to a zero vector.
    _XOCONSTEXPR static const float Epsilon = sse::SSEFloatEpsilon * 3.0f;
#   else
    _XOCONSTEXPR static const float Epsilon = FloatEpsilon * 3.0f;
#   endif
#endif

    ////////////////////////////////////////////////////////////////////////// Members
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#public_members
#if defined(XO_SSE)
    union {
        struct {
            float x;
            float y;
            float z;
            //! Unused in math, used to pad the vector to 128 bits for SSE.
            //! Exists only when SSE is in use, represents a 128 bit xmm register.
            //! @sa https://en.wikipedia.org/wiki/Streaming_SIMD_Extensions
            float w;
        };
        float f[4]; //!< ordered as \f$\begin{pmatrix}x&y&z&w\end{pmatrix}\f$
        //! Exists when SSE is in use, represents a 128 bit xmm register.
        //! @sa https://en.wikipedia.org/wiki/Streaming_SIMD_Extensions
        __m128 xmm;
    };
#elif defined(XO_NEON)
    union {
        struct {
            float x, y, z, w;
        };
        float f[4];
        float32x4_t n;
    };
#else
    union {
        struct {
            float x, y, z;
        };
        float f[3];
    };
#endif

private:
#if defined(XO_SSE)
    static const __m128 MASK;
#endif
};

XOMATH_END_XO_NS();