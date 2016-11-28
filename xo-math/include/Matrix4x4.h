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

//! @brief A 4x4 matrix type for building transformations.
//!
//! The Matrix4x4 is constructed with an array of four Vector4 rows.
//! @sa https://en.wikipedia.org/wiki/Matrix_(mathematics)
class _XOSIMDALIGN Matrix4x4 {
public:
    //> See
    //! @name Constructors
    //! @{
    Matrix4x4(); //!< Performs no initialization.
    explicit Matrix4x4(float m); //!< All elements are set to f.
    //! specify each element.
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
    Matrix4x4(float m00, float m01, float m02, float m03,
                     float m10, float m11, float m12, float m13,
                     float m20, float m21, float m22, float m23,
                     float m30, float m31, float m32, float m33);
    //! Copy constructor, trivial.
    Matrix4x4(const Matrix4x4& m);
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
    Matrix4x4(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3);
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
    Matrix4x4(const Vector3& r0, const Vector3& r1, const Vector3& r2);
    //! Creates a rotation matrix from quaternion q.
    Matrix4x4(const class Quaternion& q);
    //! @}

    //! @name Set / Get Methods
    //! @{

    //! Sets column i to vector r.
    Matrix4x4& SetRow(int i, const Vector4& r);
    //! Sets column i to vector r.
    Matrix4x4& SetColumn(int i, const Vector4& r);
    //! Get a const reference to a row in the matrix.
    const Vector4& GetRow(int i) const;
    //! Return a column, copied out of the matrix
    Vector4 GetColumn(int i) const;
    //! @}

    //>See
    //! @name Special Operators
    //! @{

    //! Overloads the new and delete operators for Matrix4x4 when memory alignment is required (such as with SSE).
    //! @sa XO_16ALIGNED_MALLOC, XO_16ALIGNED_FREE
    _XO_OVERLOAD_NEW_DELETE();
    //! Type cast operator. Allows Matrix4x4 to be used implicitly wherever a float* can be.
    _XOINL operator float*() const { return (float*)this; }
    //! Extracts a const reference of a row, useful for getting rows by index.
    _XOINL const Vector4& operator [](int i) const;
    //! Extracts a reference of a row, useful for setting rows by index.
    _XOINL Vector4& operator [](int i);
    //! Extracts a const reference of a value, useful for getting values by index.
    _XOINL const float& operator ()(int r, int c) const;
    //! Extracts a reference of a value, useful for setting values by index.
    _XOINL float& operator ()(int r, int c);
    //! See Matrix4x4::Transpose for details.
    //! @sa https://en.wikipedia.org/wiki/Transpose
    _XOINL Matrix4x4 operator ~() const;
    //! @}

    //! @name Operators
    //! @{
    _XOINL Matrix4x4& operator += (const Matrix4x4& m);
    _XOINL Matrix4x4& operator -= (const Matrix4x4& m);
    //! @sa https://en.wikipedia.org/wiki/Matrix_multiplication
    _XOINL Matrix4x4& operator *= (const Matrix4x4& m);

    _XOINL Matrix4x4 operator + (const Matrix4x4& m) const;
    _XOINL Matrix4x4 operator - (const Matrix4x4& m) const;
    //! @sa https://en.wikipedia.org/wiki/Matrix_multiplication
    _XOINL Matrix4x4 operator * (const Matrix4x4& m) const;

    //! Vector transformation operator. Transforms vector v by this matrix.
    //!
    //? The following is latex, renders nicely in the docs.
    //? See this online editor to preview equations: http://www.hostmath.com/
    /*!
    \f[
        \begin{bmatrix}
            m00&m01&m02&m03\\
            m10&m11&m12&m13\\
            m20&m21&m22&m23\\
            m30&m31&m32&m33
        \end{bmatrix}
        \times
        \begin{bmatrix}
            x\\
            y\\
            z\\
            w
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
    //? The following is latex, renders nicely in the docs.
    //? See this online editor to preview equations: http://www.hostmath.com/
    /*!
    \f[
        \begin{bmatrix}
            m00&m01&m02&m03\\
            m10&m11&m12&m13\\
            m20&m21&m22&m23\\
            m30&m31&m32&m33
        \end{bmatrix}
        \times
        \begin{bmatrix}
            x\\
            y\\
            z\\
            0
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

    //>See
    //! @name Methods
    //! @{

    //! Returns true if the dot between any two columns is 1 or 0.
    bool HasOrthonormalBasis() const;
    //! Returns true if this matrix is unitary. A matrix is unitary when the following is true: (m.Transposed() * m) == Identity
    bool IsUnitary() const;
    //! Gets the determinant of this matrix.
    float Determinant() const;


    void MakeInverse();
    void GetInverse(Matrix4x4& o) const { o = *this; o.MakeInverse(); }
    bool TryMakeInverse();
    bool TryGetInverse(Matrix4x4& o) const { o = *this; return o.TryMakeInverse(); }

    //! Sets this matrix as a transpose of itself
    /*!
        \f[
            \begin{bmatrix}
            m00&m01&m02&m03\\
            m10&m11&m12&m13\\
            m20&m21&m22&m23\\
            m30&m31&m32&m33
            \end{bmatrix}
            \Rightarrow
            \begin{bmatrix}
            m00&m10&m20&m30\\
            m01&m11&m21&m31\\
            m02&m12&m22&m32\\
            m03&m13&m32&m33
            \end{bmatrix}
        \f]
    */
    //! @sa https://en.wikipedia.org/wiki/Transpose
    Matrix4x4& Transpose();
    //! Transforms vector v in place by this matrix.
    const Matrix4x4& Transform(Vector3& v) const;
    //! Transforms vector v in place by this matrix.
    const Matrix4x4& Transform(Vector4& v) const;

    //! Returns a copy of this matrix transposed. See Matrix4x4::Transposed
    //! @sa https://en.wikipedia.org/wiki/Transpose
    Matrix4x4 Transposed() const;
    //! @}

    //>See
    //! @name Static Methods
    //! @{

    //! Assigns outMatrix to a scale matrix, where each of x y and z scale values are equal to xyz.
    /*!
    \f[
        \begin{bmatrix}
            xyz&0&0&0\\
            0&xyz&0&0\\
            0&0&xyz&0\\
            0&0&0&1
        \end{bmatrix}
    \f]
    */
    static void Scale(float xyz, Matrix4x4& outMatrix);
    //! Assigns outMatrix to a scale matrix, where each of x y and z scale values are their same-named parameters.
    /*!
    \f[
        \begin{bmatrix}
            x&0&0&0\\
            0&y&0&0\\
            0&0&z&0\\
            0&0&0&1
        \end{bmatrix}
    \f]
    */
    static void Scale(float x, float y, float z, Matrix4x4& outMatrix);
    //! Assigns outMatrix to a scale matrix, where each of x y and z scale values are provided by v.
    /*!
    \f[
        \begin{bmatrix}
            v.x&0&0&0\\
            0&v.y&0&0\\
            0&0&v.z&0\\
            0&0&0&1
        \end{bmatrix}
    \f]
    */
    static void Scale(const Vector3& v, Matrix4x4& outMatrix);
    //! Assigns outMatrix to a translation matrix, where each of x y and z are their same-named parameters
    /*!
    \f[
        \begin{bmatrix}
            1&0&0&x\\
            0&1&0&y\\
            0&0&1&z\\
            0&0&0&1
        \end{bmatrix}
    \f]
    */
    static void Translation(float x, float y, float z, Matrix4x4& outMatrix);
    //! Assigns outMatrix to a translation matrix, where each of x y and z are provided by v.
    /*!
    \f[
        \begin{bmatrix}
            1&0&0&v.x\\
            0&1&0&v.y\\
            0&0&1&v.z\\
            0&0&0&1
        \end{bmatrix}
    \f]
    */
    static void Translation(const Vector3& v, Matrix4x4& outMatrix);
    //! Assigns outMatrix to a pitch matrix, where theta \f$\theta\f$ is the parameter radians.
    /*!
    \f[
        \begin{bmatrix}
            1&0&0&0\\
            0&\cos\theta&-\sin\theta&0\\
            0&\sin\theta&\cos\theta&0\\
            0&0&0&1
        \end{bmatrix}
    \f]
    */
    //! @sa https://en.wikipedia.org/wiki/Rotation_matrix
    static void RotationXRadians(float radians, Matrix4x4& outMatrix);
    //! Assigns outMatrix to a yaw matrix, where theta \f$\theta\f$ is the parameter radians.
    /*!
    \f[
        \begin{bmatrix}
            \cos\theta&0&\sin\theta&0\\
            0&1&0&0\\
            -\sin\theta&0&\cos\theta&0\\
            0&0&0&1
        \end{bmatrix}
    \f]
    */
    //! @sa https://en.wikipedia.org/wiki/Rotation_matrix
    static void RotationYRadians(float radians, Matrix4x4& outMatrix);
    //! Assigns outMatrix to a roll matrix, where theta \f$\theta\f$ is the parameter radians.
    /*!
    \f[
        \begin{bmatrix}
            \cos\theta&-\sin\theta&0&0\\
            \sin\theta&\cos\theta&0&0\\
            0&0&1&0\\
            0&0&0&1
        \end{bmatrix}
    \f]
    */
    //! @sa https://en.wikipedia.org/wiki/Rotation_matrix
    static void RotationZRadians(float radians, Matrix4x4& outMatrix);
    //! Assigns outMatrix to a rotation matrix. Rotation matricies are build by multiplying a yaw a pitch and a roll matrix. 
    //! See Matrix4x4::RotationXRadians, Matrix4x4::RotationYRadians and Matrix4x4::RotationZRadians for details.
    //! @sa https://en.wikipedia.org/wiki/Rotation_matrix
    static void RotationRadians(float x, float y, float z, Matrix4x4& outMatrix);
    //! Assigns outMatrix to a rotation matrix. Rotation matricies are build by multiplying a yaw a pitch and a roll matrix. 
    //! See Matrix4x4::RotationXRadians, Matrix4x4::RotationYRadians and Matrix4x4::RotationZRadians for details.
    //! @sa https://en.wikipedia.org/wiki/Rotation_matrix
    static void RotationRadians(const Vector3& v, Matrix4x4& outMatrix);
    //! Assigns outMatrix to an axis-angle rotation matrix with \f$\theta\f$ radians along axis a.    
    //!
    //! \f$let\ c = \cos\theta\f$
    //!
    //! \f$let\ s = \sin\theta\f$
    //!
    //! \f$let\ t = 1 - c\f$
    /*!
    \f[
        \begin{bmatrix}
            txx+c&txy-zs&txz+ys&0\\
            txy+zs&tyy+c&tyz-xs&0\\
            txz-ys&tyz+xs&tzz+c&0\\
            0&0&0&1
        \end{bmatrix}
    \f]
    */
    static void AxisAngleRadians(const Vector3& axis, float radians, Matrix4x4& outMatrix);
    //! The length of this vector.
    //! It's preferred to use Vector3::MagnitudeSquared when possible, as Vector3::Magnitude requires a call to Sqrt.
    //!
    //! \f$\lvert\rvert\boldsymbol{this}\lvert\rvert = \sqrt{(x\times x)+(y\times y)+(z\times z)}\f$
    //! @sa https://en.wikipedia.org/wiki/Magnitude_(mathematics)#Euclidean_vector_space
    static void RotationXDegrees(float degrees, Matrix4x4& outMatrix);
    //! Calls Matrix4x4::RotationYRadians, converting the input degrees to radians.
    static void RotationYDegrees(float degrees, Matrix4x4& outMatrix);
    //! Calls Matrix4x4::RotationZRadians, converting the input degrees to radians.
    static void RotationZDegrees(float degrees, Matrix4x4& outMatrix);
    //! Calls Matrix4x4::RotationDegrees, converting the input degrees to radians.
    static void RotationDegrees(float x, float y, float z, Matrix4x4& outMatrix);
    //! Calls Matrix4x4::RotationDegrees, converting the input degrees to radians.
    static void RotationDegrees(const Vector3& v, Matrix4x4& outMatrix);
    //! Calls Matrix4x4::AxisAngleDegrees, converting the input degrees to radians.
    static void AxisAngleDegrees(const Vector3& axis, float degrees, Matrix4x4& outMatrix);
    //! Assigns outMatrix to an orthographic projection matrix.
    //!
    //! \f$let\ w = width\f$
    //!
    //! \f$let\ h = height\f$
    //!
    //! \f$let\ n = near\f$
    //!
    //! \f$let\ f = far\f$
    /*!
    \f[
        \begin{bmatrix}
            1/w&0&0&0\\
            0&1/h&0&0\\
            0&0&f-n&0\\
            0&0&n\times (f-n)&1
        \end{bmatrix}
    \f]
    */
    //! @sa http://scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix
    static void OrthographicProjection(float width, float height, float near, float far, Matrix4x4& outMatrix);
    //! Assigns outMatrix to a perspective projection matrix. Parameters fovx and fovy are angles in radians.
    //! If you know the desired fovx and aren't sure what to use for fovy, multiply the fovx you have by the aspect
    //! ratio of your screen.
    //!
    //! \f$let\ n = near\f$
    //!
    //! \f$let\ f = far\f$
    /*!
    \f[
        \begin{bmatrix}
            atan (fovx/2)&0&0&0\\
            0&atan (fovy/2)&0&0\\
            0&0&f/(f-n)&1\\
            0&0&-n\times (f/-n)&1
        \end{bmatrix}
    \f]
    */
    //! @sa http://scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix
    static void PerspectiveProjectionRadians(float fovx, float fovy, float near, float far, Matrix4x4& outMatrix);
    //! Calls Matrix4x4::PerspectiveProjectionRadians , converting the inputs fovx and fovy to radians.
    static void PerspectiveProjectionDegrees(float fovx, float fovy, float near, float far, Matrix4x4& outMatrix);
    //! Assigns outMatrix to a look at matrix.
    static void LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up, Matrix4x4& outMatrix);
    //! Assigns outMatrix to a look at matrix, with up being Vector3::Up
    static void LookAtFromPosition(const Vector3& from, const Vector3& to, Matrix4x4& outMatrix);
    //! Assigns outMatrix to a look at matrix, from Vector3::Zero towards direction.
    static void LookAtFromDirection(const Vector3& direction, const Vector3& up, Matrix4x4& outMatrix);
    //! Assigns outMatrix to a look at matrix, from Vector3::Zero towards direction with up being Vector3::Up.
    static void LookAtFromDirection(const Vector3& direction, Matrix4x4& outMatrix);
    //! @}

    //!> See
    //! @name Variants
    //! Variants of other same-name static methods. See their documentation for more details under the 
    //! Static Methods heading.
    //! @{
    static Matrix4x4 Scale(float xyz);
    static Matrix4x4 Scale(float x, float y, float z);
    static Matrix4x4 Scale(const Vector3& v);

    static Matrix4x4 Translation(float x, float y, float z);
    static Matrix4x4 Translation(const Vector3& v);
    
    static Matrix4x4 RotationXRadians(float radians);
    static Matrix4x4 RotationYRadians(float radians);
    static Matrix4x4 RotationZRadians(float radians);
    static Matrix4x4 RotationRadians(float x, float y, float z);
    static Matrix4x4 RotationRadians(const Vector3& v);
    static Matrix4x4 AxisAngleRadians(const Vector3& axis, float radians);

    static Matrix4x4 RotationXDegrees(float degrees);
    static Matrix4x4 RotationYDegrees(float degrees);
    static Matrix4x4 RotationZDegrees(float degrees);
    static Matrix4x4 RotationDegrees(float x, float y, float z);
    static Matrix4x4 RotationDegrees(const Vector3& v);
    static Matrix4x4 AxisAngleDegrees(const Vector3& axis, float degrees);

    static Matrix4x4 OrthographicProjection(float width, float height, float near, float far);
    static Matrix4x4 PerspectiveProjectionRadians(float fovx, float fovy, float near, float far);
    static Matrix4x4 PerspectiveProjectionDegrees(float fovx, float fovy, float near, float far);

    static Matrix4x4 LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up);
    static Matrix4x4 LookAtFromPosition(const Vector3& from, const Vector3& to);
    static Matrix4x4 LookAtFromDirection(const Vector3& direction, const Vector3& up);
    static Matrix4x4 LookAtFromDirection(const Vector3& direction);
    //! @}

    //>See
    //! @name Extras
    //! @{

    //! @todo Make this optional with a define.
    //! Prints the contents of matrix m to the provided ostream in the form of its four row vectors.
#ifndef XO_NO_OSTREAM
    friend std::ostream& operator <<(std::ostream& os, const Matrix4x4& m) {
        os << "\nrow 0: " << m.r[0] << "\nrow 1: " << m.r[1] << "\nrow 2: " << m.r[2] << "\nrow 3: " << m.r[3] << "\n";
        return os;
    }
#endif
    //! @}

    //! Matrix rows
    union {
        struct {
            Vector4 r[4];
        };
        float m[16];
        struct {
            float   m00, m01, m02, m03,
                    m10, m11, m12, m13,
                    m20, m21, m22, m23,
                    m30, m31, m32, m33;
        };
    };

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


XOMATH_END_XO_NS();