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

    _XOINL Matrix4x4();

    // initialize all elements to m
    _XOINL explicit Matrix4x4(float m);

    // specify each element in row major form.
    // [a0, b0, c0, d0]
    // [a1, b1, c1, d1]
    // [a2, b2, c2, d2]
    // [a3, b3, c3, d3]
    _XOINL Matrix4x4(float a0, float b0, float c0, float d0, float a1, float b1, float c1, float d1, float a2, float b2, float c2, float d2, float a3, float b3, float c3, float d3);

    // specify this matrix as a copy of m
    _XOINL Matrix4x4(const Matrix4x4& m);

    // initialize each row as the Vector4's 
    // [r0.x, r0.y, r0.z, r0.w]
    // [r1.x, r1.y, r1.z, r1.w]
    // [r2.x, r2.y, r2.z, r2.w]
    // [r3.x, r3.y, r3.z, r3.w]
    _XOINL Matrix4x4(const class Vector4& r0, const class Vector4& r1, const class Vector4& r2, const class Vector4& r3);

    // Specify the upper left of the matrix as one Vector3 per row, leaving the rightmost column as 0, except it's bottommost element.
    // [r0.x, r0.y, r0.z, 0.0]
    // [r1.x, r1.y, r1.z, 0.0]
    // [r2.x, r2.y, r2.z, 0.0]
    // [0.0,  0.0,  0.0,  1.0]
    _XOINL Matrix4x4(const class Vector3& r0, const class Vector3& r1, const class Vector3& r2);

    // Set this matrix as a transpose of itself, then return a ref to itself.
    // See: https://en.wikipedia.org/wiki/Transpose
    _XOINL const Matrix4x4& MakeTranspose();

    // Return a transpose of this matrix
    // See: https://en.wikipedia.org/wiki/Transpose
    _XOINL Matrix4x4 Transpose() const;

    _XO_OVERLOAD_NEW_DELETE();

    // Get a constant reference to a row in the matrix
    _XOINL const Vector4& operator [](int i) const;

    // Get a mutable reference to a row in the matrix
    _XOINL Vector4& operator [](int i);

    // Get a constant reference to a float in the matrix
    _XOINL const float& operator ()(int r, int c) const;
    
    // Get a mutable reference to a float in the matrix
    _XOINL float& operator ()(int r, int c);

    // Get a reference to a row in the matrix.
    _XOINL const Vector4& GetRow(int i) const;

    // Return a column, copied out of the matrix
    _XOINL Vector4 GetColumn(int i) const;

    // Return a copy of the transpose.
    // See: https://en.wikipedia.org/wiki/Transpose
    _XOINL Matrix4x4 operator ~() const;

    _XOINL const Matrix4x4& operator += (const Matrix4x4& m);
    _XOINL const Matrix4x4& operator -= (const Matrix4x4& m);
    _XOINL const Matrix4x4& operator *= (const Matrix4x4& m);
    
    _XOINL Matrix4x4 operator + (const Matrix4x4& m) const;
    _XOINL Matrix4x4 operator - (const Matrix4x4& m) const;
    _XOINL Matrix4x4 operator * (const Matrix4x4& m) const;
    
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
        Identity, One, Zero;
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

XOMATH_END_XO_NS()

#undef _XOMATH_INIT_MATRIX4X4

#endif // XOMATH_INTERNAL