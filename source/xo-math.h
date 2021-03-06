/*
      :::    :::  ::::::::                  :::   :::       ::: ::::::::::: :::    ::: 
     :+:    :+: :+:    :+:                :+:+: :+:+:    :+: :+:   :+:     :+:    :+:  
     +:+  +:+  +:+    +:+               +:+ +:+:+ +:+  +:+   +:+  +:+     +:+    +:+   
     +#++:+   +#+    +:+ +#++:++#++:++ +#+  +:+  +#+ +#++:++#++: +#+     +#++:++#++    
   +#+  +#+  +#+    +#+               +#+       +#+ +#+     +#+ +#+     +#+    +#+     
 #+#    #+# #+#    #+#               #+#       #+# #+#     #+# #+#     #+#    #+#      
###    ###  ########                ###       ### ###     ### ###     ###    ###       

                                        v. alpha



               Fast math for game developers. No headaches, no cruft.



!! Warnings
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    - xo-math is in alpha. Contribute, test, but don't integrate yet.
    - Documentation does not exist yet.
    - Source code is untested.

// Contributing
//////////////////////////////////////////////////////////////////////////////////////////
    - There is no contributing guide yet.
    - Basically: keep it simple.
    - Repo: https://github.com/xoorath/xo-math

// About
//////////////////////////////////////////////////////////////////////////////////////////
    xo-math is being developed by Jared Thomson (better known as Xoorath) in an effort to
    make games a little faster and a little easier to make. Twitter: @xoorath

    xo-math is pronounced non-phonetically "ex oh math", and spelled without 
    capitalization where fonts permit.

// License
//////////////////////////////////////////////////////////////////////////////////////////
    See also: third-party-licenses.h

    Copyright 2018 Jared Thomson

    Permission is hereby granted, free of charge, to any person obtaining a copy of this
    software and associated documentation files (the "Software"), to deal in the Software
    without restriction, including without limitation the rights to use, copy, modify,
    merge, publish, distribute, sub license, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be included in all copies
    or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
    CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once
#include <inttypes.h>
#include <limits>
#include "xo-math-constants.h"
#include "xo-math-macros.h"
#include "xo-math-utilities.h"
#include "xo-math-detect-simd.h"

// As an end user you can configure these values
#define XO_CONFIG_LEFT_HANDED 1
#define XO_CONFIG_Y_UP 1
#define XO_CONFIG_DEFAULT_NEAR_PLANE 0.1f
#define XO_CONFIG_DEFAULT_FAR_PLANE 1000.f

// These configs can set themselves up based on the other configs above...
#define XO_CONFIG_RIGHT_HANDED (XO_CONFIG_LEFT_HANDED == 0 ? 1 : 0)
#define XO_CONFIG_Z_UP (XO_CONFIG_Y_UP == 0 ? 1 : 0)

#if XO_SSE_CURRENT >= XO_SSE4_1
#include "xo-math-sse4.h"
#else
#include "xo-math-reference.h"
#endif

#include "third-party-licenses.h"