//////////////////////////////////////////////////////////////////////
// Transform.cpp
//
// AUTHOR
//   Jared Thomson <twitter: @xoorath> <email:jared@xoorath.com>
//
// LICENSE
//   See end of file for license information.
//
//////////////////////////////////////////////////////////////////////
#include "../include/Transform.h"

XO_NAMESPACE_BEGIN

Transform::Transform()
  : LocalMatrix(Matrix4x4::Identity)
  , GlobalMatrix(Matrix4x4::Identity)
  , ScaleMatrix(Matrix4x4::Identity)
  , RotationQuat(Quaternion::Identity)
  , RotationMatrix(Matrix4x4::Identity)
  , TranslationMatrix(Matrix4x4::Identity)
  , Scale(Vector3::One)
  , Rotation(Vector3::Zero)
  , Translation(Vector3::Zero)
  , Parent(nullptr)
  , ScaleDirty(false)
  , RotationDirty(false)
  , TranslationDirty(false)
{
}

void Transform::CalculateLocal() {
  bool anyDirty = false;
  if(ScaleDirty) {
    ScaleMatrix = Matrix4x4::Scale(Scale);
    ScaleDirty = false;
    anyDirty = true;
  }
  if(RotationDirty) {
    RotationQuat = Quaternion::RotationEuler(Rotation);
    RotationMatrix = RotationQuat;
    RotationDirty = false;
    anyDirty = true;
  }
  if(TranslationDirty) {
    TranslationMatrix = Matrix4x4::Translation(Translation);
    TranslationDirty = false;
    anyDirty = true;
  }
  if(anyDirty) {
    LocalMatrix = ScaleMatrix * RotationMatrix * TranslationMatrix;
  }
}

void Transform::CalculateGlobal() {
  if(!Parent) {
    GlobalMatrix = LocalMatrix;
  } 
  else {
    GlobalMatrix = Parent->GetGlobalWorldMatrix();
    GlobalMatrix *= LocalMatrix;
  }
}

const Matrix4x4& Transform::GetGlobalWorldMatrix() {
  return GlobalMatrix;
}

const Vector3& Transform::GetScale() const {
  return Scale;
}

const Vector3& Transform::GetRotation() const {
  return Rotation;
}

const Vector3& Transform::GetTranslation() const {
  return Translation;
}

void Transform::SetScale(const Vector3& scale) {
  ScaleDirty = true;
  Scale = scale;
}

void Transform::SetRotation(const Vector3& rotation) {
  RotationDirty = true;
  Rotation = rotation;
}

void Transform::SetTranslation(const Vector3& translation) {
  TranslationDirty = true;
  Translation = Translation;
}

Transform* Transform::GetParent() const {
  return Parent;
}

void Transform::SetParent(Transform* parent) {
  Parent = parent;
  while(parent) {
    // Detect circular parenting, and detatch
    if(parent == this) {
      Parent = nullptr;
      return;
    }
    parent = parent->Parent;
  }
}

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