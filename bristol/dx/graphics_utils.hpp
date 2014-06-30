#pragma once
#include "DirectXTK/SimpleMath.h"

namespace bristol
{
  enum VertexFlags
  {
    VF_POS    = 1 << 0,
    VF_NORMAL = 1 << 1,
    VF_UV     = 1 << 2,
    VF_COLOR  = 1 << 3,
  };

  DirectX::SimpleMath::Vector3 GetRow(const DirectX::SimpleMath::Matrix& m, uint32_t row);

  uint32_t ColorToU32(float r, float g, float b, float a);

  float Raycast(
      const DirectX::SimpleMath::Vector3& center,
      float radius,
      const DirectX::SimpleMath::Vector3& o,
      const DirectX::SimpleMath::Vector3& d);

  template <typename T>
  float Dot(const T& a, const T& b)
  {
    return a.Dot(b);
  }
}