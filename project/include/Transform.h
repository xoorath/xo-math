//////////////////////////////////////////////////////////////////////
// Transform.h 
//
// AUTHOR
//   Jared Thomson <twitter: @xoorath> <email:jared@xoorath.com>
//
// LICENSE
//   See end of file for license information.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "xo-math-defines.h"
#include "Quaternion.h"
#include "Matrix4x4.h"

XO_NAMESPACE_BEGIN

class XO_ALIGN Transform {
public:
  XO_OVERLOAD_NEW_DELETE(); // for alignment

  Transform();

  void CalculateLocal();
  void CalculateGlobal();
  const Matrix4x4& GetGlobalWorldMatrix();

  const Vector3& GetScale() const;
  const Vector3& GetRotation() const;
  const Vector3& GetTranslation() const;

  void SetScale(const Vector3& scale);
  void SetRotation(const Vector3& rotation);
  void SetTranslation(const Vector3& translation);

  Transform* GetParent() const;
  void SetParent(Transform* parent);

private:
  Matrix4x4 LocalMatrix;
  Matrix4x4 GlobalMatrix;

  Matrix4x4 ScaleMatrix;
  Quaternion RotationQuat;
  Matrix4x4 RotationMatrix;
  Matrix4x4 TranslationMatrix; 
  
  Vector3 Scale;
  Vector3 Rotation;
  Vector3 Translation;

  Transform* Parent;

  bool ScaleDirty : 1;
  bool RotationDirty : 1;
  bool TranslationDirty : 1;
};

XO_NAMESPACE_END
//////////////////////////////////////////////////////////////////////
// This software is available under 2 licenses
// choose whichever you prefer.
//////////////////////////////////////////////////////////////////////

// MIT License
//////////////////////////////////////////////////////////////////////
/*
Copyright (c) 2017 Jared Thomson
Permission is hereby granted, free of charge, to any person obtaining 
a copy of this software and associated documentation files (the 
"Software"), to deal in the Software without restriction, including 
without limitation the rights to use, copy, modify, merge, publish, 
distribute, sublicense, and/or sell copies of the Software, and to 
permit persons to whom the Software is furnished to do so, subject to 
the following conditions:
The above copyright notice and this permission notice shall be 
included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////////////////////////////////

// The Unlicense
//////////////////////////////////////////////////////////////////////
/*
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.
In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
For more information, please refer to <http://unlicense.org/>
*/
//////////////////////////////////////////////////////////////////////