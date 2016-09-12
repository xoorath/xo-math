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
static_assert(false, "Don't include Matrix4x4.h directly. Include GameMath.h, which fully implements this type.");
#elif defined(_XOMATH_INIT_MATRIX4X4) || defined(_XO_TRANSPOSE_SWAP)
_XOMATH_INTERNAL_MACRO_WARNING
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

class _MM_ALIGN16 Matrix4x4 {
public:

    //! @name Constructors
    //! @{
    _XOINL Matrix4x4(); //!< Performs no initialization.
    _XOINL explicit Matrix4x4(float m); //!< All elements are set to f.

    //! specify each element in row major form.
    /*!
        \f[
            \begin{bmatrix}
            m00&m01&m02&m03\\
            m10&m11&m12&m13\\
            m20&m21&m22&m23\\
            m30&m31&m32&m33
            \end{bmatrix}
        \f]
    */
    _XOINL Matrix4x4(float m00, float m01, float m02, float m03,
                     float m10, float m11, float m12, float m13,
                     float m20, float m21, float m22, float m23,
                     float m30, float m31, float m32, float m33);

    //! Copy constructor, trivial.
    _XOINL Matrix4x4(const Matrix4x4& m);

    //! Specifies each row.
    /*!
        \f[
            \begin{bmatrix}
            r0.x&r0.y&r0.z&r0.w\\
            r1.x&r1.y&r1.z&r1.w\\
            r2.x&r2.y&r2.z&r2.w\\
            r3.x&r3.y&r3.z&r3.w
            \end{bmatrix}
        \f]
    */
    _XOINL Matrix4x4(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3);

    //! Specify the upper left of the matrix as one Vector3 per row, leaving unset elements as 0 except the bottom right which will be set to 1.
    /*!
        \f[
            \begin{bmatrix}
            r0.x&r0.y&r0.z&0.0\\
            r1.x&r1.y&r1.z&0.0\\
            r2.x&r2.y&r2.z&0.0\\
            0.0&0.0&0.0&1.0
            \end{bmatrix}
        \f]
    */
    _XOINL Matrix4x4(const Vector3& r0, const Vector3& r1, const Vector3& r2);

    //! @}

    //! Sets this matrix as a transpose of itself
    /*!
        \f[
            \begin{bmatrix}
            m00&m01&m02&m03\\
            m10&m11&m12&m13\\
            m20&m21&m22&m23\\
            m30&m31&m32&m33
            \end{bmatrix}
            \Rrightarrow
            \begin{bmatrix}
            m00&m10&m20&m30\\
            m01&m11&m21&m31\\
            m02&m12&m22&m32\\
            m03&m13&m32&m33
            \end{bmatrix}
        \f]
    */
    //! @sa https://en.wikipedia.org/wiki/Transpose
    _XOINL const Matrix4x4& MakeTranspose();

    //! Returns a transpose of this matrix
    /*!
        \f[
            \begin{bmatrix}
            m00&m01&m02&m03\\
            m10&m11&m12&m13\\
            m20&m21&m22&m23\\
            m30&m31&m32&m33
            \end{bmatrix}
            \Rrightarrow
            \begin{bmatrix}
            m00&m10&m20&m30\\
            m01&m11&m21&m31\\
            m02&m12&m22&m32\\
            m03&m13&m32&m33
            \end{bmatrix}
        \f]
    */
    //! @sa https://en.wikipedia.org/wiki/Transpose
    _XOINL Matrix4x4 Transpose() const;

    //! @name Special Operators
    //! @{

    //! Overloads the new and delete operators for Matrix4x4 when memory alignment is required (such as with SSE).
    //! @sa XO_16ALIGNED_MALLOC, XO_16ALIGNED_FREE
    _XO_OVERLOAD_NEW_DELETE();

    //! Extracts a const reference of a row, useful for getting rows by index.
    _XOINL const Vector4& operator [](int i) const;

    //! Extracts a reference of a row, useful for setting rows by index.
    _XOINL Vector4& operator [](int i);

    //! Extracts a const reference of a value, useful for getting values by index.
    _XOINL const float& operator ()(int r, int c) const;
    
    //! Extracts a reference of a value, useful for setting values by index.
    _XOINL float& operator ()(int r, int c);

    // Return a copy of the transpose.
    // See: https://en.wikipedia.org/wiki/Transpose
    _XOINL Matrix4x4 operator ~() const;
    //! @}

    //! @name Set / Get Methods
    //! @{
    //! Get a const reference to a row in the matrix.
    _XOINL const Vector4& GetRow(int i) const;

    //! Return a column, copied out of the matrix
    _XOINL Vector4 GetColumn(int i) const;
    //! @}


    //! @name Operators
    //! @{
    _XOINL const Matrix4x4& operator += (const Matrix4x4& m);
    _XOINL const Matrix4x4& operator -= (const Matrix4x4& m);
    //! @sa https://en.wikipedia.org/wiki/Matrix_multiplication
    _XOINL const Matrix4x4& operator *= (const Matrix4x4& m);

    _XOINL Matrix4x4 operator + (const Matrix4x4& m) const;
    _XOINL Matrix4x4 operator - (const Matrix4x4& m) const;
    //! @sa https://en.wikipedia.org/wiki/Matrix_multiplication
    _XOINL Matrix4x4 operator * (const Matrix4x4& m) const;

    //! Vector transformation operator. Transforms vector v by this matrix.
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
    _XOINL Vector4 operator * (const Vector4& v) const;

    //! Vector transformation operator. Transforms vector v by this matrix.
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
            0\\
        \end{bmatrix}
    \f]
    \f[
        =
        \begin{bmatrix}
            (m00\times x) + (m01\times x) + (m02\times x) + (m03\times x)\\
            (m10\times y) + (m11\times y) + (m12\times y) + (m13\times y)\\
            (m20\times z) + (m21\times z) + (m22\times z) + (m23\times z)
        \end{bmatrix}
    \f]
    */
    //! @sa https://en.wikipedia.org/wiki/Matrix_multiplication
    //! @sa https://youtu.be/Awcj447pYuk?t=1m28s
    _XOINL Vector3 operator * (const Vector3& v) const;

    //! @}
    
    _XOINL const Matrix4x4& Transform(Vector3& v) const;
    _XOINL const Matrix4x4& Transform(Vector4& v) const;

    _XOINL static void Scale(float xyz, Matrix4x4& outMatrix);
    _XOINL static void Scale(float x, float y, float z, Matrix4x4& outMatrix);
    _XOINL static void Scale(const Vector3& v, Matrix4x4& outMatrix);

    _XOINL static void Translation(float x, float y, float z, Matrix4x4& outMatrix);
    _XOINL static void Translation(const Vector3& v, Matrix4x4& outMatrix);
    
    _XOINL static void RotationXRadians(float radians, Matrix4x4& outMatrix);
    _XOINL static void RotationYRadians(float radians, Matrix4x4& outMatrix);
    _XOINL static void RotationZRadians(float radians, Matrix4x4& outMatrix);
    _XOINL static void RotationRadians(float x, float y, float z, Matrix4x4& outMatrix);
    _XOINL static void RotationRadians(const Vector3& v, Matrix4x4& outMatrix);
    _XOINL static void AxisAngleRadians(const Vector3& axis, float radians, Matrix4x4& outMatrix);

    _XOINL static void RotationXDegrees(float degrees, Matrix4x4& outMatrix);
    _XOINL static void RotationYDegrees(float degrees, Matrix4x4& outMatrix);
    _XOINL static void RotationZDegrees(float degrees, Matrix4x4& outMatrix);
    _XOINL static void RotationDegrees(float x, float y, float z, Matrix4x4& outMatrix);
    _XOINL static void RotationDegrees(const Vector3& v, Matrix4x4& outMatrix);
    _XOINL static void AxisAngleDegrees(const Vector3& axis, float degrees, Matrix4x4& outMatrix);

    _XOINL static void OrthographicProjection(float width, float height, float near, float far, Matrix4x4& outMatrix);
    _XOINL static void Projection(float fovx, float fovy, float near, float far, Matrix4x4& outMatrix);

    _XOINL static void LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up, Matrix4x4& outMatrix);
    _XOINL static void LookAtFromPosition(const Vector3& from, const Vector3& to, Matrix4x4& outMatrix);
    _XOINL static void LookAtFromDirection(const Vector3& direction, const Vector3& up, Matrix4x4& outMatrix);
    _XOINL static void LookAtFromDirection(const Vector3& direction, Matrix4x4& outMatrix);

    _XOINL static Matrix4x4 Scale(float xyz);
    _XOINL static Matrix4x4 Scale(float x, float y, float z);
    _XOINL static Matrix4x4 Scale(const Vector3& v);

    _XOINL static Matrix4x4 Translation(float x, float y, float z);
    _XOINL static Matrix4x4 Translation(const Vector3& v);
    
    _XOINL static Matrix4x4 RotationXRadians(float radians);
    _XOINL static Matrix4x4 RotationYRadians(float radians);
    _XOINL static Matrix4x4 RotationZRadians(float radians);
    _XOINL static Matrix4x4 RotationRadians(float x, float y, float z);
    _XOINL static Matrix4x4 RotationRadians(const Vector3& v);
    _XOINL static Matrix4x4 AxisAngleRadians(const Vector3& axis, float radians);

    _XOINL static Matrix4x4 RotationXDegrees(float degrees);
    _XOINL static Matrix4x4 RotationYDegrees(float degrees);
    _XOINL static Matrix4x4 RotationZDegrees(float degrees);
    _XOINL static Matrix4x4 RotationDegrees(float x, float y, float z);
    _XOINL static Matrix4x4 RotationDegrees(const Vector3& v);
    _XOINL static Matrix4x4 AxisAngleDegrees(const Vector3& axis, float degrees);

    _XOINL static Matrix4x4 OrthographicProjection(float width, float height, float near, float far);
    _XOINL static Matrix4x4 Projection(float fovx, float fovy, float near, float far);

    _XOINL static Matrix4x4 LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up);
    _XOINL static Matrix4x4 LookAtFromPosition(const Vector3& from, const Vector3& to);
    _XOINL static Matrix4x4 LookAtFromDirection(const Vector3& direction, const Vector3& up);
    _XOINL static Matrix4x4 LookAtFromDirection(const Vector3& direction);

    Vector4 r[4];

    static const Matrix4x4
        /*!
        \f[
            \begin{bmatrix}
            1&0&0&0\\
            0&1&0&0\\
            0&0&1&0\\
            0&0&0&1
            \end{bmatrix}
        \f]
        */
        Identity,
        /*!
        \f[
            \begin{bmatrix}
            1&1&1&1\\
            1&1&1&1\\
            1&1&1&1\\
            1&1&1&1
            \end{bmatrix}
        \f]
        */
        One,
        /*!
        \f[
            \begin{bmatrix}
            0&0&0&0\\
            0&0&0&0\\
            0&0&0&0\\
            0&0&0&0
            \end{bmatrix}
        \f]
        */
        Zero;
};

const Matrix4x4 Matrix4x4::Identity = {
                                        {1.0f, 0.0f, 0.0f, 0.0f},
                                        {0.0f, 1.0f, 0.0f, 0.0f},
                                        {0.0f, 0.0f, 1.0f, 0.0f},
                                        {0.0f, 0.0f, 0.0f, 1.0f}
                                    };

const Matrix4x4 Matrix4x4::One = {
                                    {1.0f, 1.0f, 1.0f, 1.0f},
                                    {1.0f, 1.0f, 1.0f, 1.0f},
                                    {1.0f, 1.0f, 1.0f, 1.0f},
                                    {1.0f, 1.0f, 1.0f, 1.0f}
                                };

const Matrix4x4 Matrix4x4::Zero = {
                                    {0.0f, 0.0f, 0.0f, 0.0f},
                                    {0.0f, 0.0f, 0.0f, 0.0f},
                                    {0.0f, 0.0f, 0.0f, 0.0f},
                                    {0.0f, 0.0f, 0.0f, 0.0f}
                                };

XOMATH_END_XO_NS();

#undef _XOMATH_INIT_MATRIX4X4

#endif // XOMATH_INTERNAL