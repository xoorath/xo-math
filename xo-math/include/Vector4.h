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
static_assert(false, "Don't include Vector4.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

//! @brief A four dimensional euclidean vector, optimized for use in games.
//!
//! Most useful for three dimensional rotations. See Matrix4x4::Transform and Vector4::operator*=.
//! @sa https://en.wikipedia.org/wiki/Euclidean_vector
class _MM_ALIGN16 Vector4 {
public:
#if XO_SSE
    //! Smallest increment from zero that could be assigned to each element of this vector and would still be seen as equal to a zero vector.
    _XOCONSTEXPR static const float Epsilon = sse::SSEFloatEpsilon * 4.0f;
#else
    //! Smallest increment from zero that could be assigned to each element of this vector and would still be seen as equal to a zero vector.
    _XOCONSTEXPR static const float Epsilon = FloatEpsilon * 4.0f;
#endif

    //! @name Constructors
    //! @{
    _XOINL Vector4(); //!< @note Performs no initialization.
    _XOINL Vector4(float f);
    _XOINL Vector4(float x, float y, float z, float w);
    _XOINL Vector4(const Vector4& vec);
#if XO_SSE
    _XOINL Vector4(const __m128& vec);
#endif
    _XOINL Vector4(const class Vector2& v); //!< @note w and z will be zero after values from v are assigned.
    _XOINL Vector4(const class Vector3& v); //!< @note w will be zero after values from v are assigned.
    //! @}


    //! @name Set / Get Methods
    //! Set and get methods
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

    //! Overloads the new and delete operators for vector4 when memory alignment is required (such as with SSE).
    //! @sa XO_16ALIGNED_MALLOC, XO_16ALIGNED_FREE
    _XO_OVERLOAD_NEW_DELETE();

#if XO_SSE
    //! type cast operator. Allows Vector4 to be used implicitly where ever __m128 can be.
    _XOINL operator const __m128&() const;
#endif

    //! extract reference operator, useful for setting values by index.
    //! \f[i\begin{cases}0 & return\ x;\\1 & return\ y;\\2 & return\ z;\\3 & return\ w;\\? & undefined\end{cases}\f]
    _XOINL float& operator [](int i);

    //! extract const reference operator, useful for getting values by index.
    //! \f[i\begin{cases}0 & return\ x;\\1 & return\ y;\\2 & return\ z;\\3 & return\ w;\\? & undefined\end{cases}\f]
    _XOINL const float& operator [](int i) const;

    //! negate operator. Returns a vector with all elements with a flipped sign: \f$\begin{pmatrix}-x,&-y,&-z,&-w\end{pmatrix}\f$
    _XOINL Vector4 operator -() const;

    //! swizzle operator. Returns a vector with all elements in reverse order: \f$\begin{pmatrix}w,&z,&y,&x\end{pmatrix}\f$
    _XOINL Vector4 operator ~() const;

    //! @name Add Equals Operator
    //! Adds all same-name vector elements with other vector types, or all elements to scalar/integer types.
    //! @return A const reference to this vector.
    //! @{
    _XOINL const Vector4& operator += (const Vector4& v);
    _XOINL const Vector4& operator += (float v);
    _XOINL const Vector4& operator += (double v);
    _XOINL const Vector4& operator += (int v);
    _XOINL const Vector4& operator += (const class Vector2& v);
    _XOINL const Vector4& operator += (const class Vector3& v);
    //! @}

    //! @name Subtract Equals Operator
    //! Subtracts all same-name vector elements with other vector types, or all elements to scalar/integer types.
    //! @return A const reference to this vector.
    //! @{
    _XOINL const Vector4& operator -= (const Vector4& v);
    _XOINL const Vector4& operator -= (float v);
    _XOINL const Vector4& operator -= (double v);
    _XOINL const Vector4& operator -= (int v);
    _XOINL const Vector4& operator -= (const class Vector2& v);
    _XOINL const Vector4& operator -= (const class Vector3& v);
    //! @}

    //! @name Multiply Equals Operator
    //! Multiplies all same-name vector elements with other vector types, or all elements to scalar/integer types.
    //! @return A const reference to this vector.
    //! @{
    _XOINL const Vector4& operator *= (const Vector4& v);
    _XOINL const Vector4& operator *= (float v);
    _XOINL const Vector4& operator *= (double v);
    _XOINL const Vector4& operator *= (int v);
    _XOINL const Vector4& operator *= (const class Vector2& v);
    _XOINL const Vector4& operator *= (const class Vector3& v);

    //! Matrix multiplication operator. Transforms this vector by matrix m.
    //!
    // The following is latex, renders nicely in the docs.
    // See this online editor to preview equations: http://www.hostmath.com/
    /*!
    \f[
        \begin{bmatrix}
            m00&m01&m02&m03\\
            m10&m11&m12&m13\\
            m20&m21&m22&m23\\
            m30&m31&m32&m33\\
        \end{bmatrix}
        \times
        \begin{bmatrix}
            x\\
            y\\
            z\\
            w\\
        \end{bmatrix}
    \f]
    \f[
            =
            \begin{bmatrix}
                (m00\times x) + (m01\times x) + (m02\times x) + (m03\times x)\\
                (m10\times y) + (m11\times y) + (m12\times y) + (m13\times y)\\
                (m20\times z) + (m21\times z) + (m22\times z) + (m23\times z)\\
                (m30\times w) + (m31\times w) + (m32\times w) + (m33\times w)
            \end{bmatrix}
        \f]
    */
    //! @sa https://en.wikipedia.org/wiki/Matrix_multiplication
    //! @sa https://youtu.be/Awcj447pYuk?t=1m28s
    _XOINL const Vector4& operator *= (const class Matrix4x4& M);
    //! @}

    //! @name Divide Equals Operator
    //! Divides all same-name vector elements with other vector types, or all elements to scalar/integer types.
    //! @return A const reference to this vector.
    //! @sa XO_NO_INVERSE_DIVISION
    //! @{
    _XOINL const Vector4& operator /= (const Vector4& v);
    _XOINL const Vector4& operator /= (float v);
    _XOINL const Vector4& operator /= (double v);
    _XOINL const Vector4& operator /= (int v);
    _XOINL const Vector4& operator /= (const class Vector2& v);
    _XOINL const Vector4& operator /= (const class Vector3& v);
    //! @}

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
    _XOINL Vector4 operator * (const class Matrix4x4& M) const;
    //! @}

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

    //! @name Equal Operators
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

    //! The sum of all vector elements.
    //! @returns x+y+z+w
    _XOINL float Sum() const;

    //! The square length of this vector in 4 dimensional space.
    //! It's prefered to use the MagnitudeSquared when possible, as Magnitude requires a call to Sqrt.
    //! @sa https://en.wikipedia.org/wiki/Magnitude_(mathematics)#Euclidean_vector_space
    _XOINL float MagnitudeSquared() const;

    //! The length of this vector in 4 dimensional space.
    //! It's preferred to use the MagnitudeSquared when possible, as Magnitude requires a call to Sqrt.
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

    
    _XOINL static void Max(const Vector4& a, const Vector4& b, Vector4& outVec);
    _XOINL static void Min(const Vector4& a, const Vector4& b, Vector4& outVec);
    _XOINL static void Lerp(const Vector4& a, const Vector4& b, float t, Vector4& outVec);

    _XOINL static Vector4 Max(const Vector4& a, const Vector4& b);
    _XOINL static Vector4 Min(const Vector4& a, const Vector4& b);
    _XOINL static Vector4 Lerp(const Vector4& a, const Vector4& b, float t);

    _XOINL static float Dot(const Vector4& a, const Vector4& b);

    _XOINL static float DistanceSquared(const Vector4& a, const Vector4& b);
    _XOINL static float Distance(const Vector4&a, const Vector4&b);

    _XOINL float Dot(const Vector4& v) const;
    _XOINL float DistanceSquared(const Vector4& v) const;
    _XOINL float Distance(const Vector4& v) const;
    _XOINL Vector4 Lerp(const Vector4& v, float t) const;

    friend std::ostream& operator <<(std::ostream& os, const Vector4& v) {
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", w:" << v.w << ", mag:" << v.Magnitude() << ")";
        return os;
    }

    static const Vector4
        One, //!< \f$\begin{pmatrix}1&1&1&1\end{pmatrix}\f$
        Zero, //!< \f$\begin{pmatrix}0&0&0&0\end{pmatrix}\f$
        UnitX, //!< \f$\begin{pmatrix}1&0&0&0\end{pmatrix}\f$
        UnitY, //!< \f$\begin{pmatrix}0&1&0&0\end{pmatrix}\f$
        UnitZ, //!< \f$\begin{pmatrix}0&0&1&0\end{pmatrix}\f$
        UnitW; //!< \f$\begin{pmatrix}0&0&0&1\end{pmatrix}\f$

    //! @union Data
    union {
        //! @struct Float data
        struct {
            float x, y, z, w;
        };
        float f[4];
#if XO_SSE
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