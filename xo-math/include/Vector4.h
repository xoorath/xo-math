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
//!  Most useful for three dimensional rotations. See Matrix4x4::Transform and Vector4::operator*=.
//! @sa https://en.wikipedia.org/wiki/Euclidean_vector
class _MM_ALIGN16 Vector4 {
public:
#if XO_SSE
    _XOCONSTEXPR static const float Epsilon = sse::SSEFloatEpsilon * 4.0f;
#else
    _XOCONSTEXPR static const float Epsilon = FloatEpsilon * 4.0f;
#endif
    //! Trivial constructor. No initialization is done.
    _XOINL Vector4();

    //! set all constrctor, x, y, z, and w will assigned to f.
    _XOINL Vector4(float f);

    //! set each constructor. x, y, z, and w will be assigned to the input params.
    _XOINL Vector4(float x, float y, float z, float w);

    //! trivial copy constructor.
    _XOINL Vector4(const Vector4& vec);

#if XO_SSE
    //! converting constructor for sse.
    _XOINL Vector4(const __m128& vec);
#endif

    //! converting constructor. x and y will be assigned to v.x and v.y; z and w will be assigned to 0.
    _XOINL Vector4(const class Vector2& v);

    //! converting constructor. x, y and z will be assigned to v.x, v.y and v.z. w will be assigned to 0
    _XOINL Vector4(const class Vector3& v);

    //! set all. x, y, z and w will be assigned to the input params.
    _XOINL const Vector4& Set(float x, float y, float z, float w);

    //! set each. x, y, z and w will be assigned to f.
    _XOINL const Vector4& Set(float f);

    //! trivial copy setter. x, y, z and w will be assigned to those values of vec.
    _XOINL const Vector4& Set(const Vector4& vec);

#if XO_SSE
    //! trivial copy setter. m will be assigned to vec.
    _XOINL const Vector4& Set(const __m128& vec);
#endif

    //! extract all getter. x, y, z and w will be assigned to those values of this vector.
    _XOINL void Get(float& x, float& y, float& z, float& w) const;

    //! extract all getter. f[0], f[1], f[2] and f[3] will be assigned to x, y, z and w respectively. 
    _XOINL void Get(float* f) const;

    //! overloads the new and delete operators for vector4 when memory alignment is required (such as with SSE).
    //!
    //! @sa XO_16ALIGNED_MALLOC \n XO_16ALIGNED_FREE
    _XO_OVERLOAD_NEW_DELETE();

#if XO_SSE
    //! type cast operator. Allows Vector4 to be used implicitly where ever __m128 can be.
    _XOINL operator const __m128&() const;
#endif

    //! extract reference operator, useful for setting values by index.
    //!
    //! \f[i\begin{cases}0 & return\ x;\\1 & return\ y;\\2 & return\ z;\\3 & return\ w;\\? & undefined\end{cases}\f]
    _XOINL float& operator [](int i);

    //! extract const reference operator, useful for getting values by index.
    //!
    //! \f[i\begin{cases}0 & return\ x;\\1 & return\ y;\\2 & return\ z;\\3 & return\ w;\\? & undefined\end{cases}\f]
    _XOINL const float& operator [](int i) const;

    //! negate operator. Returns a vector with all elements with a flipped sign: 
    //! \f$ \begin{pmatrix}-x&-y&-z&-w\end{pmatrix} \f$
    _XOINL Vector4 operator -() const;

    //! swizzle operator. Returns a vector with all elements in reverse order:
    //!
    //! \f$\begin{pmatrix}w&z&y&x\end{pmatrix}\f$
    _XOINL Vector4 operator ~() const;

    //! Trivial add equal operator. Modifies this vector to be equal to
    //! \f$\begin{pmatrix}x+v.x&y+v.y&z+v.z&w+v.w\end{pmatrix}\f$
    //! @returns const reference to this vector.
    _XOINL const Vector4& operator += (const Vector4& v);

    //! Trivial add equal operator. Modifies this vector to be equal to \n\f$\begin{pmatrix}x+v&y+v&z+v&w+v\end{pmatrix}\f$
    _XOINL const Vector4& operator += (float v);

    //! Trivial add equal operator. Modifies this vector to be equal to \n\f$\begin{pmatrix}x+v&y+v&z+v&w+v\end{pmatrix}\f$
    _XOINL const Vector4& operator += (double v);

    //! Trivial add equal operator. Modifies this vector to be equal to \n\f$\begin{pmatrix}x+v&y+v&z+v&w+v\end{pmatrix}\f$
    _XOINL const Vector4& operator += (int v);

    //! Trivial add equal operator. Modifies this vector to be equal to \n\f$\begin{pmatrix}x+v.x&y+v.y&z&w\end{pmatrix}\f$
    _XOINL const Vector4& operator += (const class Vector2& v);

    //! Trivial add equal operator. Modifies this vector to be equal to \n\f$\begin{pmatrix}x+v.x&y+v.y&z+v.z&w\end{pmatrix}\f$
    _XOINL const Vector4& operator += (const class Vector3& v);

    //! Trivial subtract equal operator. Modifies this vector to be equal to (x-v.x, y-v.y, z-v.z, w-v.w).
    _XOINL const Vector4& operator -= (const Vector4& v);

    //! Trivial subtract equal operator. Modifies this vector to be equal to (x-v, y-v, z-v, w-v).
    _XOINL const Vector4& operator -= (float v);

    //! Trivial subtract equal operator. Modifies this vector to be equal to (x-v, y-v, z-v, w-v).
    _XOINL const Vector4& operator -= (double v);

    //! Trivial subtract equal operator. Modifies this vector to be equal to (x-v, y-v, z-v, w-v).
    _XOINL const Vector4& operator -= (int v);

    //! Trivial subtract equal operator. Modifies this vector to be equal to (x-v.x, y-v.y, z, w).
    _XOINL const Vector4& operator -= (const class Vector2& v);

    //! Trivial subtract equal operator. Modifies this vector to be equal to (x-v.x, y-v.y, z-v.z, w).
    _XOINL const Vector4& operator -= (const class Vector3& v);


    //! Trivial multiply equal operator. Modifies this vector to be equal to (x*v.x, y*v.y, z*v.z, w*v.w).
    _XOINL const Vector4& operator *= (const Vector4& v);

    //! Trivial multiply equal operator. Modifies this vector to be equal to (x*v, y*v, z*v, w*v).
    _XOINL const Vector4& operator *= (float v);

    //! Trivial multiply equal operator. Modifies this vector to be equal to (x*v, y*v, z*v, w*v).
    _XOINL const Vector4& operator *= (double v);

    //! Trivial multiply equal operator. Modifies this vector to be equal to (x*v, y*v, z*v, w*v).
    _XOINL const Vector4& operator *= (int v);

    //! Trivial multiply equal operator. Modifies this vector to be equal to (x*v.x, y*v.y, z, w).
    _XOINL const Vector4& operator *= (const class Vector2& v);

    //! Trivial multiply equal operator. Modifies this vector to be equal to (x*v.x, y*v.y, z*v.z, w).
    _XOINL const Vector4& operator *= (const class Vector3& v);

    //! Matrix multiplication operator. Transforms this vector by matrix m.
    //!
    // The following is latex, renders nicely in the docs.
    // See this online editor to preview equations: http://www.hostmath.com/
    /*!
    \f[
        \overrightarrow{this}
        \begin{pmatrix}
            x\\
            y\\
            z\\
            w\\
        \end{pmatrix}
        *=
        M
        \begin{pmatrix}
            m&m&m&m\\
            m&m&m&m\\
            m&m&m&m\\
            m&m&m&m\\
        \end{pmatrix}
        =
        \overrightarrow{this} 
        \begin{pmatrix}
            x\\
            y\\
            z\\
            w\\
        \end{pmatrix}
    \f]
    */
    _XOINL const Vector4& operator *= (const class Matrix4x4& M);

    _XOINL const Vector4& operator /= (const Vector4& v);
    _XOINL const Vector4& operator /= (float v);
    _XOINL const Vector4& operator /= (double v);
    _XOINL const Vector4& operator /= (int v);
    _XOINL const Vector4& operator /= (const class Vector2& v);
    _XOINL const Vector4& operator /= (const class Vector3& v);

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
    _XOINL Vector4 operator * (const class Matrix4x4& M) const;

    _XOINL Vector4 operator / (const Vector4& v) const;
    _XOINL Vector4 operator / (float v) const;
    _XOINL Vector4 operator / (double v) const;
    _XOINL Vector4 operator / (int v) const;
    _XOINL Vector4 operator / (const class Vector2& v) const;
    _XOINL Vector4 operator / (const class Vector3& v) const;

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

    _XOINL float Sum() const;
    _XOINL float MagnitudeSquared() const;
    _XOINL float Magnitude() const;
    _XOINL const Vector4& Normalize();
    _XOINL Vector4 Normalized() const;
    _XOINL bool IsZero() const;
    _XOINL bool IsNormalized() const;

    // todo: reflect glsl behaviour on max and min.
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
        One, Zero,
        UnitX, UnitY, UnitZ, UnitW;

    union {
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