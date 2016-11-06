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

//! @todo why is this in sse? is there no fallback?
_XOINL Vector2 Abs(const Vector2& v) {
    return Vector2(Abs(v.x), Abs(v.y));
}

//! @todo why is this in sse? is there no fallback?
_XOINL Vector3 Abs(const Vector3& v) {
    return (sse::Abs(v.m));
}

//! @todo why is this in sse? is there no fallback?
_XOINL Vector4 Abs(const Vector4& v) {
    return Vector4(sse::Abs(v.m));
}

#if defined(XO_SSE)

namespace sse {

    // The control of MXCSR usage is inspired by Agner Fog's use of them in vectorclasses.
    // vectorclasses uses them to optionally speed up subnormal operations.
    // To achieve this in xomath, call the following once per thread where xo-math is used:
    //      sse::UpdateControlWord();       // updates the thread-local state.
    //      sse::SetDenormalsAreZero(true); // force all denormal values to 0
    //      sse::SetFlushToZero(true);      // underflowing operations produce 0
    // Note: this will only produce speed gains where subnormal values are likely to occur.
    // See http://wm.ite.pl/articles/sse-penalties-of-errors.html for more details.
    namespace mxcsr {
        // Flags that are set on the CPU if an exception had occured.
        // They will remain set until manually unset.
        enum class Flags {
            InvalidOperation                = (1 << 0),
            Denormal                        = (1 << 1),
            DivideByZero                    = (1 << 2),
            Overflow                        = (1 << 3),
            Underflow                       = (1 << 4),
            Precision                       = (1 << 5),
        };

        enum class DAZ {
            DenormalsAreZero                = (1 << 6),
        };

        enum class Masks {
            InvalidOperation                = (1 << 7),
            Denormal                        = (1 << 8),
            DivideByZero                    = (1 << 9),
            Overflow                        = (1 << 10),
            Underflow                       = (1 << 11),
            Precision                       = (1 << 12)
        };

        enum class Rounding {
            Nearest                         = 0,
            Negative                        = (1 << 13),
            Positive                        = (1 << 14),
            Zero                            = (1 << 13) | (1 << 14),
            
            Bits                            = (1 << 13) | (1 << 14)
        };

        enum class FZ {
            FlushToZero                     = (1 << 15)
        };
    }

    bool GetControlMask(mxcsr::Masks mask, bool withUpdate = false);
    bool GetControlMask(unsigned mask, bool withUpdate = false);
    bool GetDenormalExceptionMask(bool withUpdate = false);
    bool GetDivideByZeroExceptionMask(bool withUpdate = false);
    bool GetInvalidOperationExceptionMask(bool withUpdate = false);
    bool GetOverflowExceptionMask(bool withUpdate = false);
    bool GetPrecisionExceptionMask(bool withUpdate = false);
    bool GetUnderflowExceptionMask(bool withUpdate = false);

    bool HasControlFlagBeenSet(mxcsr::Flags flags, bool withUpdate = false, bool thenFlush = false);
    bool HasControlFlagBeenSet(unsigned flags, bool withUpdate = false, bool thenFlush = false);
    bool HasDenormalExceptionOccured(bool withUpdate = false, bool thenFlush = false);
    bool HasDenormalsAreZeroSet(bool withUpdate = false);
    bool HasDivideByZeroExceptionOccured(bool withUpdate = false, bool thenFlush = false);
    bool HasFlushToZeroSet(bool withUpdate = false);
    bool HasInvalidOperationExceptionOccured(bool withUpdate = false, bool thenFlush = false);
    bool HasOverflowExceptionOccured(bool withUpdate = false, bool thenFlush = false);
    bool HasPrecisionExceptionOccured(bool withUpdate = false, bool thenFlush = false);
    bool HasUnderflowExceptionOccured(bool withUpdate = false, bool thenFlush = false);

    mxcsr::Rounding GetRoundingMode(bool withUpdate = false);
    void GetAllMXCSRInfo(std::ostream& os, bool withUpdate = false);

    void RemoveControlWord(unsigned control);

    void SetControlMask(mxcsr::Masks mask, bool value, bool withUpdate = false);
    void SetControlMask(unsigned mask, bool value, bool withUpdate = false);
    void SetControlWord(unsigned control);
    void SetControlWordAddative(unsigned control);
    void SetDenormalExceptionMask(bool value, bool withUpdate = false);
    void SetDenormalsAreZero(bool value, bool withUpdate = false);
    void SetDivideByZeroExceptionMask(bool value, bool withUpdate = false);
    void SetFlushToZero(bool value, bool withUpdate = false);
    void SetInvalidOperationExceptionMask(bool value, bool withUpdate = false);
    void SetOverflowExceptionMask(bool value, bool withUpdate = false);
    void SetPrecisionExceptionMask(bool value, bool withUpdate = false);
    void SetRoundingMode(mxcsr::Rounding mode, bool withUpdate = false);
    void SetRoundingMode(unsigned mode, bool withUpdate = false);
    void SetUnderflowExceptionMask(bool value, bool withUpdate = false);

    void ThrowAllExceptions(bool withUpdate = false);
    void ThrowNoExceptions(bool withUpdate = false);
    void UpdateControlWord();
}
#endif

XOMATH_END_XO_NS();