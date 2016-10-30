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

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector3.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

//! A three dimensional euclidean vector, optimized for use in games.
//! @sa https://en.wikipedia.org/wiki/Euclidean_vector
class _MM_ALIGN16 Vector3 {
public:
    //>See
    //! @name Constructors
    //! @{
    _XOINL Vector3(); //!< Performs no initialization.
    _XOINL Vector3(float f); //!< All elements are set to f.
    _XOINL Vector3(float x, float y, float z); //!< Assigns each named value accordingly.
    _XOINL Vector3(const Vector3& vec); //!< Copy constructor, trivial.
#if defined(XO_SSE)
    _XOINL Vector3(const __m128& vec); //!< Assigns m to vec, sets all elements.
#endif
    _XOINL Vector3(const class Vector2& v); //!< Assigns same-name values from v, zero to z. \f$\begin{pmatrix}v.x&v.y&0\end{pmatrix}\f$
    _XOINL Vector3(const class Vector4& v); //!< Assigns same-name values from v. \f$\begin{pmatrix}v.x&v.y&v.z\end{pmatrix}\f$
    //! @}

    //>See
    //! @name Set / Get Methods
    //! @{

    //! Set all. x, y, and z will be assigned to the input params.
    _XOINL const Vector3& Set(float x, float y, float z);
    //! Set each. x, y, and z will be assigned to f.
    _XOINL const Vector3& Set(float f);
    //! Set each. Copies vec into this.
    _XOINL const Vector3& Set(const Vector3& vec);
#if defined(XO_SSE)
    //! Set each. Copies vec int m.
    _XOINL const Vector3& Set(const __m128& vec);
#endif
    //! Extract all getter. x, y, and z will be assigned to those values of this vector.
    _XOINL void Get(float& x, float& y, float &z) const;
    //! Extract all getter. f[0], f[1], and f[2] will be assigned to x, y, and z respectively.
    //! @Note When using SSE f must be aligned and at least 4 floats wide. Example: _MM_ALIGN16 float f[4];
    _XOINL void Get(float* f) const;
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
    _XOINL bool IsNormalized() const;
    //! Returns true when the Magnitude of this vector is <= Vector3::Epsilon
    _XOINL bool IsZero() const;
    //! The length of this vector
    //! It's preferred to use Vector3::MagnitudeSquared when possible, as Vector3::Magnitude requires a call to Sqrt.
    //!
    //! \f$\lvert\rvert\boldsymbol{this}\lvert\rvert = \sqrt{(x\times x)+(y\times y)+(z\times z)}\f$
    //! @sa https://en.wikipedia.org/wiki/Magnitude_(mathematics)#Euclidean_vector_space
    _XOINL float Magnitude() const;
    //! The square length of this vector.
    //! It's preferred to use Vector3::MagnitudeSquared when possible, as Vector3::Magnitude requires a call to Sqrt.
    //!
    //! \f$\lvert\rvert\boldsymbol{this}\lvert\rvert^2 = (x\times x)+(y\times y)+(z\times z)\f$
    //! @sa https://en.wikipedia.org/wiki/Magnitude_(mathematics)#Euclidean_vector_space
    _XOINL float MagnitudeSquared() const;
    //! The sum of all vector elements.
    //!
    //! \f$x+y+z\f$
    _XOINL float Sum() const;

    //! Normalizes this vector to a Magnitude of 1.
    //! @sa https://en.wikipedia.org/wiki/Unit_vector
    _XOINL const Vector3& Normalize();

    //! Returns a copy of this vector with a Magnitude of 1.
    //! @sa https://en.wikipedia.org/wiki/Unit_vector
    _XOINL Vector3 Normalized() const;
    //! Returns this vector swizzled so elements appear in reverse order. Vector3::operator~() provides the same functionality.
    //!
    //! \f$\begin{pmatrix}z,&y,&x\end{pmatrix}\f$
    _XOINL Vector3 ZYX() const;
    
    //! @}

    //>See
    //! @name Static Methods
    //! @{

    //! Returns the cross product (vector product) of vectors a and b.
    //! @sa https://en.wikipedia.org/wiki/Cross_product
    _XOINL static void Cross(const Vector3& a, const Vector3& b, Vector3& outVec);
    //! Sets outVec to a vector interpolated between a and b by a scalar amount t.
    //! @sa https://en.wikipedia.org/wiki/Linear_interpolation
    _XOINL static void Lerp(const Vector3& a, const Vector3& b, float t, Vector3& outVec);
    //! Set outVec to have elements equal to the max of each element in a and b.
    //!
    //! \f$\begin{pmatrix}\max(a.x, b.x)&\max(a.y, b.y)&\max(a.z, b.z)\end{pmatrix}\f$
    _XOINL static void Max(const Vector3& a, const Vector3& b, Vector3& outVec);
    //! Set outVec to have elements equal to the min of each element in a and b.
    //!
    //! \f$\begin{pmatrix}\min(a.x, b.x)&\min(a.y, b.y)&\min(a.z, b.z)\end{pmatrix}\f$
    _XOINL static void Min(const Vector3& a, const Vector3& b, Vector3& outVec);
    //! Return a random vector with a magnitude of d.
    _XOINL static void RandomAtDistance(float d, Vector3& outVec);
    //! Calls Vector3::RandomInConeRadians, converting the input angle in degrees to radians.
    _XOINL static void RandomInConeDegrees(const Vector3& forward, float angle, Vector3& outVec);
    //! Return a random vector inside a cone with an angle relative to the input forward.
    //! @note this 'cone' does not have a flat bottom, it is a rotation of the forward vector.
    //! angles at and beyond PI radians will no longer represent a meaningful cone.
    _XOINL static void RandomInConeRadians(const Vector3& forward, float angle, Vector3& outVec);
    //! Return a random vector with a magnitude that does not exceed d.
    _XOINL static void RandomInDistance(float d, Vector3& outVec);
    //! Assigns outVec to a random vector with a magnitude between low and high, inclusively.
    _XOINL static void RandomInRange(float low, float high, Vector3& outVec);
    //! Return a random vector with a magnitude that does not exceed 1.
    _XOINL static void RandomInSphere(Vector3& outVec);
    //! Calls Vector3::RandomOnConeRadians, converting the input angle in degrees to radians.
    _XOINL static void RandomOnConeDegrees(const Vector3& forward, float angle, Vector3& outVec);
    //! Return a random vector on edge of a cone with an angle relative to the input forward.
    //! @note this 'cone' does not have a flat bottom, it is a rotation of the forward vector.
    //! angles at and beyond PI radians will no longer represent a meaningful cone.
    _XOINL static void RandomOnConeRadians(const Vector3& forward, float angle, Vector3& outVec);
    //! Return a random vector with a length of 1.0f.
    _XOINL static void RandomOnSphere(Vector3& outVec);
    //! Calls Vector3::RotateRadians, converting the input angle in degrees to radians.
    _XOINL static void RotateDegrees(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec);
    //! Assign outVec to v rotated along an axis by a specified angle in radians.
    _XOINL static void RotateRadians(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec);
    // Calls Vector3::AngleRadians, converting the return value to degrees.
    _XOINL static float AngleDegrees(const Vector3& a, const Vector3& b);
    //! Returns the angle in radians between vector a and b.
    _XOINL static float AngleRadians(const Vector3& a, const Vector3& b);
    //! Returns the distance between vectors a and b in 3 dimensional space.
    //! It's preferred to use the DistanceSquared when possible, as Distance requires a call to Sqrt.
    //!
    //! \f$distance = \lvert\rvert\boldsymbol{b-a}\lvert\rvert\f$
    _XOINL static float Distance(const Vector3&a, const Vector3&b);
    //! Returns the square distance between vectors a and b in 3 dimensional space.
    //! It's preferred to use the DistanceSquared when possible, as Distance requires a call to Sqrt.
    //!
    //! \f$distance^2 = \lvert\rvert\boldsymbol{b-a}\lvert\rvert^2\f$
    _XOINL static float DistanceSquared(const Vector3& a, const Vector3& b);
    //! Returns a single number representing a product of magnitudes. Commonly used with two normalized 
    //! vectors to determine if they are pointed the same way. In this case: 1.0 represents same-facing vectors
    //! 0 represents perpendicular vectors, and -1 will be facing away
    //!
    //! \f$a\cdot b =(a.x\times b.x) + (a.y\times b.y) + (a.z\times b.z)\f$
    //!
    //! @sa https://en.wikipedia.org/wiki/Dot_product
    _XOINL static float Dot(const Vector3& a, const Vector3& b);
    
    //! @}

    //>See
    //! @name Variants
    //! Variants of other same-name static methods. See their documentation for more details under the 
    //! Static Methods heading.
    //!
    //! Non static variants replace the first Vector3 parameter by 'this' vector.
    //! Static variants return what would have been the outVec param.
    //! @{
    _XOINL static Vector3 Cross(const Vector3& a, const Vector3& b);
    _XOINL static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);
    _XOINL static Vector3 Max(const Vector3& a, const Vector3& b);
    _XOINL static Vector3 Min(const Vector3& a, const Vector3& b);
    _XOINL static Vector3 RandomAtDistance(float d);
    _XOINL static Vector3 RandomInConeDegrees(const Vector3& forward, float angle);
    _XOINL static Vector3 RandomInConeRadians(const Vector3& forward, float angle);
    _XOINL static Vector3 RandomInDistance(float d);
    _XOINL static Vector3 RandomInRange(float low, float high);
    _XOINL static Vector3 RandomInSphere();
    _XOINL static Vector3 RandomOnConeDegrees(const Vector3& forward, float angle);
    _XOINL static Vector3 RandomOnConeRadians(const Vector3& forward, float angle);
    _XOINL static Vector3 RandomOnSphere();
    _XOINL static Vector3 RotateDegrees(const Vector3& v, const Vector3& axis, float angle);
    _XOINL static Vector3 RotateRadians(const Vector3& v, const Vector3& axis, float angle);

    _XOINL float AngleDegrees(const Vector3& v) const;
    _XOINL float AngleRadians(const Vector3& v) const;
    _XOINL float Distance(const Vector3& v) const;
    _XOINL float DistanceSquared(const Vector3& v) const;
    _XOINL float Dot(const Vector3& v) const;
    _XOINL Vector3 Cross(const Vector3& v) const;
    _XOINL Vector3 Lerp(const Vector3& v, float t) const;
    _XOINL Vector3 RandomInConeDegrees(float angle) const;
    _XOINL Vector3 RandomInConeRadians(float angle) const;
    _XOINL Vector3 RandomOnConeDegrees(float angle) const;
    _XOINL Vector3 RandomOnConeRadians(float angle) const;
    _XOINL Vector3 RotateDegrees(const Vector3& axis, float angle) const;
    _XOINL Vector3 RotateRadians(const Vector3& axis, float angle) const;
    //! @}

    //>See
    //! @name Extras
    //! @{

    //! @todo Make this optional with a define.
    //! Prints the contents of vector v and its magnitude to the provided ostream.
    friend std::ostream& operator <<(std::ostream& os, const Vector3& v) {
#if defined(XO_SSE)
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", w:" << v.w << ", mag:" << v.Magnitude() << ")";
#else
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", mag:" << v.Magnitude() << ")";
#endif
        return os;
    }
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

private:
#if defined(XO_SSE)
    static const __m128 MASK;
#endif
};

XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL