#pragma once

#include <stdint.h>

#define XO_ALIGN
#define XO_ALIGN_TYPE()

namespace xo {
  class Vector2 {
    public:
    float x, y;
  };

  class Vector3 {
    public:
      float x, y, z;
  };

  class Vector4 {
    public:
    float x, y, z, w;
  };

  class Matrix4x4 {
    public:
    float m[4][4];
  };

  class Quaternion {
    public:
    float i, j, k, r;
  };

  class Transform {
    public:
    Vector3 Scale;
    Vector3 Rotation;
    Vector3 Translation;

    struct {
      union {
        struct {
          bool ScaleDirty : 1;
          bool RotationDirty : 1;
          bool TranslationDirty : 1;
        };
        uint32_t FlagData;
      };
    } Flags;
    Matrix4x4 ScaleMatrix;
    Quaternion RotationQuaternion;
    Matrix4x4 TranslationMatrix;
    Matrix4x4 World;
  };

}