#pragma once
#include "DirectXTK/SimpleMath.h"
#include <dxgiformat.h>

namespace bristol
{
  enum VertexFlags
  {
    VF_POS            = 1 << 0,
    VF_POS_XY         = 1 << 1,
    VF_NORMAL         = 1 << 4,
    VF_TEX2_0         = 1 << 7,
    VF_TEX3_0         = 1 << 8,
    VF_COLOR          = 1 << 10,
    VF_COLOR_U32      = 1 << 11,

    // flags specifying order
    VF_ORDER_TEX_COL   = 1 << 16,
  };

  uint32_t VertexSizeFromFlags(uint32_t flags);
  uint32_t IndexSizeFromFormat(DXGI_FORMAT format);

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

  // (w,h) = backbuffer size
  DirectX::SimpleMath::Vector3 ScreenToViewSpace(
    const DirectX::SimpleMath::Matrix& proj,
    uint32_t x,
    uint32_t y,
    int backbufferWidth,
    int backbufferHeight);

}