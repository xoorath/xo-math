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
static_assert(false, "Don't include Vector4.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

//! @brief A four dimensional euclidean vector, optimized for use in games.
//!
//! Most useful for three dimensional rotations. See Matrix4x4::Transform and Matrix4x4::operator*=.
//! @sa https://en.wikipedia.org/wiki/Euclidean_vector
class _MM_ALIGN16 Vector4 {
public:
    ////////////////////////////////////////////////////////////////////////// Constructors
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector47Vector4Ev
    //! @name Constructors
    //! @{
    _XOINL Vector4(); //!< Performs no initialization.
    _XOINL Vector4(float f); //!< All elements are set to f.
    _XOINL Vector4(float x, float y, float z, float w); //!< Assigns each named value accordingly.
    _XOINL Vector4(const Vector4& vec); //!< Copy constructor, trivial.
#if XO_SSE
    _XOINL Vector4(const __m128& vec); //!< Assigns m to vec, sets all elements.
#endif
    _XOINL Vector4(const class Vector2& v); //!< Assigns same-name values from v, zero to z and w. \f$\begin{pmatrix}v.x&v.y&0&0\end{pmatrix}\f$
    _XOINL Vector4(const class Vector3& v); //!< Assigns same-name values from v, zero to w.\f$\begin{pmatrix}v.x&v.y&v.z&0\end{pmatrix}\f$
    //! @}

    ////////////////////////////////////////////////////////////////////////// Set / Get Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector43SetEffff
    //! @name Set / Get Methods
    //! @{

    //! Set all. x, y, z and w will be assigned to the input params.
    _XOINL const Vector4& Set(float x, float y, float z, float w);
    //! Set each. x, y, z and w will be assigned to f.
    _XOINL const Vector4& Set(float f);
    //! Set each. Copies vec into this.
    _XOINL const Vector4& Set(const Vector4& vec);

#if XO_SSE
    //! Set each. Copies vec int m.
    _XOINL const Vector4& Set(const __m128& vec);
#endif
    //! Extract all getter. x, y, z and w will be assigned to those values of this vector.
    _XOINL void Get(float& x, float& y, float& z, float& w) const;
    //! Extract all getter. f[0], f[1], f[2] and f[3] will be assigned to x, y, z and w respectively. 
    _XOINL void Get(float* f) const;
    //! @}

    ////////////////////////////////////////////////////////////////////////// Special Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector423_XO_OVERLOAD_NEW_DELETEEv
    //! @name Special Operators
    //! @{

    //! Overloads the new and delete operators for Vector4 when memory alignment is required (such as with SSE).
    //! @sa XO_16ALIGNED_MALLOC, XO_16ALIGNED_FREE
    _XO_OVERLOAD_NEW_DELETE();
#if XO_SSE
    //! Type cast operator. Allows Vector4 to be used implicitly where ever __m128 can be.
    _XOINL operator const __m128&() const;
#endif
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
    //! Swizzle operator. Returns a vector with all elements in reverse order:
    //!
    //! \f$\begin{pmatrix}w,&z,&y,&x\end{pmatrix}\f$
    _XOINL Vector4 operator ~() const;
    //! @}

    ////////////////////////////////////////////////////////////////////////// Add Equals Operator
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector4pLERK7Vector4
    //! @name Add Equals Operator
    //! Adds all same-name vector elements with other vector types, or all elements to scalar/integer types.
    //! @{
    _XOINL const Vector4& operator += (const Vector4& v);
    _XOINL const Vector4& operator += (float v);
    _XOINL const Vector4& operator += (double v);
    _XOINL const Vector4& operator += (int v);
    _XOINL const Vector4& operator += (const class Vector2& v);
    _XOINL const Vector4& operator += (const class Vector3& v);
    //! @}
    ////////////////////////////////////////////////////////////////////////// Subtract Equals Operator
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector4mIERK7Vector4
    //! @name Subtract Equals Operator
    //! Subtracts all same-name vector elements with other vector types, or all elements to scalar/integer types.
    //! @{
    _XOINL const Vector4& operator -= (const Vector4& v);
    _XOINL const Vector4& operator -= (float v);
    _XOINL const Vector4& operator -= (double v);
    _XOINL const Vector4& operator -= (int v);
    _XOINL const Vector4& operator -= (const class Vector2& v);
    _XOINL const Vector4& operator -= (const class Vector3& v);
    //! @}
    ////////////////////////////////////////////////////////////////////////// Multiply Equals Operator
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector4mLERK7Vector4
    //! @name Multiply Equals Operator
    //! Multiplies all same-name vector elements with other vector types, or all elements to scalar/integer types.
    //! @{
    _XOINL const Vector4& operator *= (const Vector4& v);
    _XOINL const Vector4& operator *= (float v);
    _XOINL const Vector4& operator *= (double v);
    _XOINL const Vector4& operator *= (int v);
    _XOINL const Vector4& operator *= (const class Vector2& v);
    _XOINL const Vector4& operator *= (const class Vector3& v);
    //! @}
    ////////////////////////////////////////////////////////////////////////// Divide Equals Operator
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector4dVERK7Vector4
    //! @name Divide Equals Operator
    //! Divides all same-name vector elements with other vector types, or all elements to scalar/integer types.
    //! @sa XO_NO_INVERSE_DIVISION
    //! @{
    _XOINL const Vector4& operator /= (const Vector4& v);
    _XOINL const Vector4& operator /= (float v);
    _XOINL const Vector4& operator /= (double v);
    _XOINL const Vector4& operator /= (int v);
    _XOINL const Vector4& operator /= (const class Vector2& v);
    _XOINL const Vector4& operator /= (const class Vector3& v);
    //! @}
    ////////////////////////////////////////////////////////////////////////// Addition Operator
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector4plERK7Vector4
    //! @name Addition Operator
    //! Builds a vector by adding all same-name vector elements with other vector types, or all elements to scalar/integer types.
    //! @return A vector resulting from the equation.
    //! @{
    _XOINL Vector4 operator + (const Vector4& v) const;
    _XOINL Vector4 operator + (float v) const;
    _XOINL Vector4 operator + (double v) const;
    _XOINL Vector4 operator + (int v) const;
    _XOINL Vector4 operator + (const class Vector2& v) const;
    _XOINL Vector4 operator + (const class Vector3& v) const;
    //! @}
    ////////////////////////////////////////////////////////////////////////// Subtraction Operator
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector4miERK7Vector4
    //! @name Subtraction Operator
    //! Builds a vector by subtracting all same-name vector elements with other vector types, or all elements to scalar/integer types.
    //! @return A vector resulting from the equation.
    //! @{
    _XOINL Vector4 operator - (const Vector4& v) const;
    _XOINL Vector4 operator - (float v) const;
    _XOINL Vector4 operator - (double v) const;
    _XOINL Vector4 operator - (int v) const;
    _XOINL Vector4 operator - (const class Vector2& v) const;
    _XOINL Vector4 operator - (const class Vector3& v) const;
    //! @}
    ////////////////////////////////////////////////////////////////////////// Multiplication Operator
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector4mlERK7Vector4
    //! @name Multiplication Operator
    //! Builds a vector by multiplying all same-name vector elements with other vector types, or all elements to scalar/integer types.
    //! @return A vector resulting from the equation.
    //! @{
    _XOINL Vector4 operator * (const Vector4& v) const;
    _XOINL Vector4 operator * (float v) const;
    _XOINL Vector4 operator * (double v) const;
    _XOINL Vector4 operator * (int v) const;
    _XOINL Vector4 operator * (const class Vector2& v) const;
    _XOINL Vector4 operator * (const class Vector3& v) const;
    //! @}
    ////////////////////////////////////////////////////////////////////////// Division Operator
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector4dvERK7Vector4
    //! @name Division Operator
    //! Builds a vector by dividing all same-name vector elements with other vector types, or all elements to scalar/integer types.
    //! @return A vector resulting from the equation.
    //! @sa XO_NO_INVERSE_DIVISION
    //! @{
    _XOINL Vector4 operator / (const Vector4& v) const;
    _XOINL Vector4 operator / (float v) const;
    _XOINL Vector4 operator / (double v) const;
    _XOINL Vector4 operator / (int v) const;
    _XOINL Vector4 operator / (const class Vector2& v) const;
    _XOINL Vector4 operator / (const class Vector3& v) const;
    //! @}

    ////////////////////////////////////////////////////////////////////////// Comparison Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector4ltERK7Vector4
    //! @name Comparison Operators
    //! When comparing against other vectors, the square magnitude is compared.
    //! When comparing against scalars, the scalar is squared and compared to the square magnitude of this vector.
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
    //! @}
    ////////////////////////////////////////////////////////////////////////// Equality Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector4eqERK7Vector4
    //! @name Equality Operators
    //! When two vectors are compared, each element is compared; When all vector elements have a difference of <= Vector4::Epsilon, they are considered equal.
    //!
    //! When a scalar is compared, we square the scalar and check it against our square magnitude. If the difference is <= Vector4::Epsilon, they are considered equal.
    //! @{
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

    ////////////////////////////////////////////////////////////////////////// Public Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector43SumEv
    //! The sum of all vector elements.
    //!
    //! \f$x+y+z+w\f$
    _XOINL float Sum() const;
    //! The square length of this vector in 4 dimensional space.
    //! It's preferred to use Vector4::MagnitudeSquared when possible, as Vector4::Magnitude requires a call to Sqrt.
    //!
    //! \f$\lvert\rvert\boldsymbol{this}\lvert\rvert^2 = (x\times x)+(y\times y)+(z\times z)+(w\times w)\f$
    //! @sa https://en.wikipedia.org/wiki/Magnitude_(mathematics)#Euclidean_vector_space
    _XOINL float MagnitudeSquared() const;
    //! The length of this vector in 4 dimensional space.
    //! It's preferred to use Vector4::MagnitudeSquared when possible, as Vector4::Magnitude requires a call to Sqrt.
    //!
    //! \f$\lvert\rvert\boldsymbol{this}\lvert\rvert = \sqrt{(x\times x)+(y\times y)+(z\times z)+(w\times w)}\f$
    //! @sa https://en.wikipedia.org/wiki/Magnitude_(mathematics)#Euclidean_vector_space
    _XOINL float Magnitude() const;
    //! Normalizes this vector to a Magnitude of 1.
    //! @sa https://en.wikipedia.org/wiki/Unit_vector
    _XOINL const Vector4& Normalize();
    //! Produces a copy of this vector with a unit length of 1.
    //! @sa https://en.wikipedia.org/wiki/Unit_vector
    _XOINL Vector4 Normalized() const;
    //! Returns true when the Magnitude of this vector is <= Vector4::Epsilon
    _XOINL bool IsZero() const;
    //! Returns true when the Magnitude of this vector is within Vector4::Epsilon of being 1.0
    _XOINL bool IsNormalized() const;

    ////////////////////////////////////////////////////////////////////////// Static Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector43MaxERK7Vector4RK7Vector4R7Vector4
    //! Set outVec to have elements equal to the max of each element in a and b.
    //!
    //! \f$\begin{pmatrix}\max(a.x, b.x)&\max(a.y, b.y)&\max(a.z, b.z)&\max(a.w, b.w)\end{pmatrix}\f$
    _XOINL static void Max(const Vector4& a, const Vector4& b, Vector4& outVec);
    //! Set outVec to have elements equal to the min of each element in a and b.
    //!
    //! \f$\begin{pmatrix}\min(a.x, b.x)&\min(a.y, b.y)&\min(a.z, b.z)&\min(a.w, b.w)\end{pmatrix}\f$
    _XOINL static void Min(const Vector4& a, const Vector4& b, Vector4& outVec);
    //! Sets outVec to a vector interpolated between a and b by a scalar amount t.
    //! @sa https://en.wikipedia.org/wiki/Linear_interpolation
    _XOINL static void Lerp(const Vector4& a, const Vector4& b, float t, Vector4& outVec);
    
    ////////////////////////////////////////////////////////////////////////// Static Variants
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector43MaxERK7Vector4RK7Vector4
    //! @name Variants
    //! These static methods return the result of their overloads containing out outVec param.
    //! See the variant with an outVec param for more details.
    //! @{
    _XOINL static Vector4 Max(const Vector4& a, const Vector4& b);
    _XOINL static Vector4 Min(const Vector4& a, const Vector4& b);
    _XOINL static Vector4 Lerp(const Vector4& a, const Vector4& b, float t);
    //! @}

    ////////////////////////////////////////////////////////////////////////// Static Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector43MaxERK7Vector4RK7Vector4R7Vector4
    //! @name Static Methods
    //! @{

    //! Returns a single number representing a product of magnitudes. Commonly used with two normalized 
    //! vectors to determine if they are pointed the same way. In this case: 1.0 represents same-facing vectors
    //! 0 represents perpendicular vectors, and -1 will be facing away
    //!
    //! \f$a\cdot b =(a.x\times b.x) + (a.y\times b.y) + (a.z\times b.z) + (a.w\times  b.w)\f$
    //!
    //! @sa https://en.wikipedia.org/wiki/Dot_product
    _XOINL static float Dot(const Vector4& a, const Vector4& b);
    //! Returns the square distance between vectors a and b in 4 dimensional space.
    //! It's preferred to use the DistanceSquared when possible, as Distance requires a call to Sqrt.
    //!
    //! \f$distance^2 = \lvert\rvert\boldsymbol{b-a}\lvert\rvert^2\f$
    _XOINL static float DistanceSquared(const Vector4& a, const Vector4& b);
    //! Returns the distance between vectors a and b in 4 dimensional space.
    //! It's preferred to use the DistanceSquared when possible, as Distance requires a call to Sqrt.
    //!
    //! \f$distance = \lvert\rvert\boldsymbol{b-a}\lvert\rvert\f$
    _XOINL static float Distance(const Vector4&a, const Vector4&b);
    //! @}

    ////////////////////////////////////////////////////////////////////////// Non-static Variants
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector43DotERK7Vector4
    //! @name Variants
    //! Member variants of static functions. See the same-named static variant for more details on each method.
    //! @{
    _XOINL float Dot(const Vector4& v) const;
    _XOINL float DistanceSquared(const Vector4& v) const;
    _XOINL float Distance(const Vector4& v) const;
    _XOINL Vector4 Lerp(const Vector4& v, float t) const;
    //! @}

    ////////////////////////////////////////////////////////////////////////// Extras
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2lsRNSt7ostreamERK7Vector4
    //! @todo Make this optional with a define.
    //! Prints the contents of vector v and its magnitude to the provided ostream.
    friend std::ostream& operator <<(std::ostream& os, const Vector4& v) {
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", w:" << v.w << ", mag:" << v.Magnitude() << ")";
        return os;
    }

    ////////////////////////////////////////////////////////////////////////// Static Attributes
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector43OneE
    static const Vector4
        One, //!< \f$\begin{pmatrix}1&1&1&1\end{pmatrix}\f$
        Zero, //!< \f$\begin{pmatrix}0&0&0&0\end{pmatrix}\f$
        UnitX, //!< \f$\begin{pmatrix}1&0&0&0\end{pmatrix}\f$
        UnitY, //!< \f$\begin{pmatrix}0&1&0&0\end{pmatrix}\f$
        UnitZ, //!< \f$\begin{pmatrix}0&0&1&0\end{pmatrix}\f$
        UnitW; //!< \f$\begin{pmatrix}0&0&0&1\end{pmatrix}\f$

#if XO_SSE
    //! Smallest increment from zero that could be assigned to each element of this vector and would still be seen as equal to a zero vector.
    _XOCONSTEXPR static const float Epsilon = sse::SSEFloatEpsilon * 4.0f;
#else
    //! Smallest increment from zero that could be assigned to each element of this vector and would still be seen as equal to a zero vector.
    _XOCONSTEXPR static const float Epsilon = FloatEpsilon * 4.0f;
#endif

    ////////////////////////////////////////////////////////////////////////// Members
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#_CPPv2N7Vector41xE
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        float f[4]; //!< ordered as \f$\begin{pmatrix}x&y&z&w\end{pmatrix}\f$
#if XO_SSE
        //! Exists when SSE is in use, represents a 128 bit xmm register.
        //! @sa https://en.wikipedia.org/wiki/Streaming_SIMD_Extensions
        __m128 m;
#endif
    };
};

const Vector4 Vector4::One = {1.0f, 1.0f, 1.0f, 1.0f};
const Vector4 Vector4::Zero = {0.0f, 0.0f, 0.0f, 0.0f};

const Vector4 Vector4::UnitX = {1.0f, 0.0f, 0.0f, 0.0f};
const Vector4 Vector4::UnitY = {0.0f, 1.0f, 0.0f, 0.0f};
const Vector4 Vector4::UnitZ = {0.0f, 0.0f, 1.0f, 0.0f};
const Vector4 Vector4::UnitW = {0.0f, 0.0f, 0.0f, 1.0f};

XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL