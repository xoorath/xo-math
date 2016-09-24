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

#define _XO_MATH_OBJ
#include "xo-math.h"

XOMATH_BEGIN_XO_NS();


////////////////////////////////////////////////////////////////////////// Matrix4x4.cpp

const Matrix4x4 Matrix4x4::Identity(Vector4(1.0f, 0.0f, 0.0f, 0.0f),
                                    Vector4(0.0f, 1.0f, 0.0f, 0.0f),
                                    Vector4(0.0f, 0.0f, 1.0f, 0.0f),
                                    Vector4(0.0f, 0.0f, 0.0f, 1.0f));

const Matrix4x4 Matrix4x4::One(Vector4(1.0f, 1.0f, 1.0f, 1.0f),
                               Vector4(1.0f, 1.0f, 1.0f, 1.0f),
                               Vector4(1.0f, 1.0f, 1.0f, 1.0f),
                               Vector4(1.0f, 1.0f, 1.0f, 1.0f));

const Matrix4x4 Matrix4x4::Zero(Vector4(0.0f, 0.0f, 0.0f, 0.0f),
                                Vector4(0.0f, 0.0f, 0.0f, 0.0f),
                                Vector4(0.0f, 0.0f, 0.0f, 0.0f),
                                Vector4(0.0f, 0.0f, 0.0f, 0.0f));


////////////////////////////////////////////////////////////////////////// Quaternion.cpp

#if defined(_XONOCONSTEXPR)
#   if XO_SSE
const float Quaternion::Epsilon = sse::SSEFloatEpsilon * 4.0f;
#   else
const float Quaternion::Epsilon = FloatEpsilon * 4.0f;
#   endif
#endif

const Quaternion Quaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);
const Quaternion Quaternion::Zero(0.0f, 0.0f, 0.0f, 0.0f);


////////////////////////////////////////////////////////////////////////// SSE.cpp

#if XO_SSE
namespace sse {

    volatile _XOTLS unsigned LastKnownControlWord = 0;

    void UpdateControlWord() {
        LastKnownControlWord = _mm_getcsr();
    }

    void SetControlWord(unsigned control) {
        _mm_setcsr(LastKnownControlWord = control);
    }

    void SetControlWordAddative(unsigned control) {
        _mm_setcsr(LastKnownControlWord |= control);
    }

    void RemoveControlWord(unsigned control) {
        _mm_setcsr(LastKnownControlWord &= ~control);
    }

    bool HasControlFlagBeenSet(unsigned flags, bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        if((LastKnownControlWord & flags) == flags) {
            if(thenFlush) {
                RemoveControlWord(flags);
            }
            return true;
        }
        return false;
    }

    bool HasControlFlagBeenSet(mxcsr::Flags flags, bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        return HasControlFlagBeenSet((unsigned)flags, withUpdate, thenFlush);
    }

    bool HasInvalidOperationExceptionOccured(bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        return HasControlFlagBeenSet(mxcsr::Flags::InvalidOperation, withUpdate, thenFlush);
    }

    bool HasDenormalExceptionOccured(bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        return HasControlFlagBeenSet(mxcsr::Flags::Denormal, withUpdate, thenFlush);
    }

    bool HasDivideByZeroExceptionOccured(bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        return HasControlFlagBeenSet(mxcsr::Flags::DivideByZero, withUpdate, thenFlush);
    }

    bool HasOverflowExceptionOccured(bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        return HasControlFlagBeenSet(mxcsr::Flags::Overflow, withUpdate, thenFlush);
    }

    bool HasUnderflowExceptionOccured(bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        return HasControlFlagBeenSet(mxcsr::Flags::Underflow, withUpdate, thenFlush);
    }

    bool HasPrecisionExceptionOccured(bool withUpdate/*= false*/, bool thenFlush/*= false*/) {
        return HasControlFlagBeenSet(mxcsr::Flags::Precision, withUpdate, thenFlush);
    }

    void SetControlMask(unsigned mask, bool value, bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        if(value) {
            SetControlWordAddative(mask);
        }
        else {
            RemoveControlWord(mask);
        }
    }

    void SetControlMask(mxcsr::Masks mask, bool value, bool withUpdate/*= false*/) {
        SetControlMask((unsigned)mask, value, withUpdate);
    }

    bool GetControlMask(unsigned mask, bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        return (LastKnownControlWord & mask) == mask;
    }

    bool GetControlMask(mxcsr::Masks mask, bool withUpdate/*= false*/) {
        return GetControlMask((unsigned)mask, withUpdate);
    }

    void SetInvalidOperationExceptionMask(bool value, bool withUpdate/*= false*/) {
        SetControlMask(mxcsr::Masks::InvalidOperation, value, withUpdate);
    }

    void SetDenormalExceptionMask(bool value, bool withUpdate/*= false*/) {
        SetControlMask(mxcsr::Masks::Denormal, value, withUpdate);
    }

    void SetDivideByZeroExceptionMask(bool value, bool withUpdate/*= false*/) {
        SetControlMask(mxcsr::Masks::DivideByZero, value, withUpdate);
    }

    void SetOverflowExceptionMask(bool value, bool withUpdate/*= false*/) {
        SetControlMask(mxcsr::Masks::Overflow, value, withUpdate);
    }

    void SetUnderflowExceptionMask(bool value, bool withUpdate/*= false*/) {
        SetControlMask(mxcsr::Masks::Underflow, value, withUpdate);
    }

    void SetPrecisionExceptionMask(bool value, bool withUpdate/*= false*/) {
        SetControlMask(mxcsr::Masks::Precision, value, withUpdate);
    }

    void ThrowAllExceptions(bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        SetInvalidOperationExceptionMask(false);
        SetDenormalExceptionMask(false);
        SetDivideByZeroExceptionMask(false);
        SetOverflowExceptionMask(false);
        SetUnderflowExceptionMask(false);
        SetPrecisionExceptionMask(false);
    }

    void ThrowNoExceptions(bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        SetInvalidOperationExceptionMask(true);
        SetDenormalExceptionMask(true);
        SetDivideByZeroExceptionMask(true);
        SetOverflowExceptionMask(true);
        SetUnderflowExceptionMask(true);
        SetPrecisionExceptionMask(true);
    }

    bool GetInvalidOperationExceptionMask(bool withUpdate/*= false*/) {
        return GetControlMask(mxcsr::Masks::InvalidOperation, withUpdate);
    }

    bool GetDenormalExceptionMask(bool withUpdate/*= false*/) {
        return GetControlMask(mxcsr::Masks::Denormal, withUpdate);
    }

    bool GetDivideByZeroExceptionMask(bool withUpdate/*= false*/) {
        return GetControlMask(mxcsr::Masks::DivideByZero, withUpdate);
    }

    bool GetOverflowExceptionMask(bool withUpdate/*= false*/) {
        return GetControlMask(mxcsr::Masks::Overflow, withUpdate);
    }

    bool GetUnderflowExceptionMask(bool withUpdate/*= false*/) {
        return GetControlMask(mxcsr::Masks::Underflow, withUpdate);
    }

    bool GetPrecisionExceptionMask(bool withUpdate/*= false*/) {
        return GetControlMask(mxcsr::Masks::Precision, withUpdate);
    }

    mxcsr::Rounding GetRoundingMode(bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        return (mxcsr::Rounding)(LastKnownControlWord & (unsigned)mxcsr::Rounding::Bits);
    }

    void SetRoundingMode(unsigned mode, bool withUpdate/*= false*/) {
        mode &= (unsigned)mxcsr::Rounding::Bits;
        if(withUpdate) {
            UpdateControlWord();
        }
        SetControlWordAddative(mode);
    }

    void SetRoundingMode(mxcsr::Rounding mode, bool withUpdate/*= false*/) {
        SetRoundingMode((unsigned)mode, withUpdate);
    }

    bool HasDenormalsAreZeroSet(bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        return (LastKnownControlWord & (unsigned)mxcsr::DAZ::DenormalsAreZero) == (unsigned)mxcsr::DAZ::DenormalsAreZero;
    }

    void SetDenormalsAreZero(bool value, bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        if(value) {
            SetControlWordAddative((unsigned)mxcsr::DAZ::DenormalsAreZero);
        }
        else {
            RemoveControlWord((unsigned)mxcsr::DAZ::DenormalsAreZero);
        }
    }

    bool HasFlushToZeroSet(bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        return (LastKnownControlWord & (unsigned)mxcsr::FZ::FlushToZero) == (unsigned)mxcsr::FZ::FlushToZero;
    }

    void SetFlushToZero(bool value, bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        if(value) {
            SetControlWordAddative((unsigned)mxcsr::FZ::FlushToZero);
        }
        else {
            RemoveControlWord((unsigned)mxcsr::FZ::FlushToZero);
        }
    }

    void GetAllMXCSRInfo(std::ostream& os, bool withUpdate/*= false*/) {
        if(withUpdate) {
            UpdateControlWord();
        }
        os << "MXCSR rounding:\n";
        os << "\t";
        switch(GetRoundingMode()) {
            case mxcsr::Rounding::Nearest:
                os << "Nearest";
                break;
            case mxcsr::Rounding::Positive:
                os << "Positive";
                break;
            case mxcsr::Rounding::Negative:
                os << "Negative";
                break;
            case mxcsr::Rounding::Zero:
                os << "Zero";
                break;
        }
        os << "\n";
        os << "MXCSR masks:\n";
        os << "\t" << "InvalidOperationException: " << GetInvalidOperationExceptionMask() << "\n"; 
        os << "\t" << "DenormalException: " << GetDenormalExceptionMask() << "\n"; 
        os << "\t" << "DivideByZeroException: " << GetDivideByZeroExceptionMask() << "\n"; 
        os << "\t" << "OverflowException: " << GetOverflowExceptionMask() << "\n"; 
        os << "\t" << "UnderflowException: " << GetUnderflowExceptionMask() << "\n"; 
        os << "\t" << "PrecisionException: " << GetPrecisionExceptionMask() << "\n"; 
        os << "\t" << "DenormalsAreZero: " << HasDenormalsAreZeroSet() << "\n";
        os << "\t" << "FlushToZero: " << HasFlushToZeroSet() << "\n";
        os << "MXCSR flags:\n";
        os << "\t" << "InvalidOperationException: " << HasInvalidOperationExceptionOccured() << "\n"; 
        os << "\t" << "DenormalException: " << HasDenormalExceptionOccured() << "\n"; 
        os << "\t" << "DivideByZeroException: " << HasDivideByZeroExceptionOccured() << "\n"; 
        os << "\t" << "OverflowException: " << HasOverflowExceptionOccured() << "\n"; 
        os << "\t" << "UnderflowException: " << HasUnderflowExceptionOccured() << "\n"; 
        os << "\t" << "PrecisionException: " << HasPrecisionExceptionOccured() << "\n";
    }
}
#endif


////////////////////////////////////////////////////////////////////////// Vector2.cpp

#if defined(_XONOCONSTEXPR)
const float Vector2::Epsilon = FloatEpsilon * 2.0f;
#endif

const Vector2 Vector2::UnitX(1.0f, 0.0f);
const Vector2 Vector2::UnitY(0.0f, 1.0f);

const Vector2 Vector2::Up(0.0f, 1.0f);
const Vector2 Vector2::Down(0.0f, -1.0f);
const Vector2 Vector2::Left(-1.0f, 0.0f);
const Vector2 Vector2::Right(1.0f, 0.0f);

const Vector2 Vector2::One(1.0f, 1.0f);
const Vector2 Vector2::Zero(0.0f, 0.0f);


////////////////////////////////////////////////////////////////////////// Vector3.cpp

#if defined(_XONOCONSTEXPR)
#   if XO_SSE
const float Vector3::Epsilon = sse::SSEFloatEpsilon * 3.0f;
#   else
const float Vector3::Epsilon = FloatEpsilon * 3.0f;
#   endif
#endif

#if XO_SSE2
const __m128 Vector3::MASK = _mm_castsi128_ps(_mm_set_epi32(0, 0xffffffff, 0xffffffff, 0xffffffff));
#elif XO_SSE
const __m128 Vector3::MASK = {-1, -1, -1, 0};
#endif

const Vector3 Vector3::Origin(0.0f, 0.0f, 0.0f);

const Vector3 Vector3::UnitX(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UnitY(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UnitZ(0.0f, 0.0f, 1.0f);

const Vector3 Vector3::Left(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);

#if defined(XO_SPACE_ZUP)
const Vector3 Vector3::Up(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Down(0.0f, 0.0f, -1.0f);
#elif defined(XO_SPACE_YUP)
const Vector3 Vector3::Up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Down(0.0f, -1.0f, 0.0f);
#endif

#if defined(XO_SPACE_LEFTHAND)
#   if defined(XO_SPACE_ZUP)
const Vector3 Vector3::Forward(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::Backward(0.0f, 1.0f, 0.0f);
#   elif defined(XO_SPACE_YUP)
const Vector3 Vector3::Forward(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Backward(0.0f, 0.0f, -1.0f);
#   endif
#elif defined(XO_SPACE_RIGHTHAND)
#   if defined(XO_SPACE_ZUP)
const Vector3 Vector3::Forward(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Backward(0.0f, -1.0f, 0.0f);
#   elif defined(XO_SPACE_YUP)
const Vector3 Vector3::Forward(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::Backward(0.0f, 0.0f, 1.0f);
#   endif
#endif

const Vector3 Vector3::One(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);


////////////////////////////////////////////////////////////////////////// Vector4.cpp

#if defined(_XONOCONSTEXPR)
#   if XO_SSE
const float Vector4::Epsilon = sse::SSEFloatEpsilon * 4.0f;
#   else
const float Vector4::Epsilon = FloatEpsilon * 4.0f;
#   endif
#endif

const Vector4 Vector4::One(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 Vector4::Zero(0.0f, 0.0f, 0.0f, 0.0f);

const Vector4 Vector4::UnitX(1.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::UnitY(0.0f, 1.0f, 0.0f, 0.0f);
const Vector4 Vector4::UnitZ(0.0f, 0.0f, 1.0f, 0.0f);
const Vector4 Vector4::UnitW(0.0f, 0.0f, 0.0f, 1.0f);


XOMATH_END_XO_NS();
#undef _XO_MATH_OBJ
