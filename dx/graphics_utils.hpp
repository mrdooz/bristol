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

  //------------------------------------------------------------------------------
  struct Spherical
  {
    float r;
    // phi is angle around x axis, ccw, starting at 0 at the x-axis
    // theta is angle around the z axis
    float phi, theta;
  };

  //------------------------------------------------------------------------------
  DirectX::SimpleMath::Vector3 FromSpherical(const Spherical& s);
  Spherical ToSpherical(const DirectX::SimpleMath::Vector3& v);

  //------------------------------------------------------------------------------
  inline DirectX::SimpleMath::Vector3 Cross(const DirectX::SimpleMath::Vector3& a, const DirectX::SimpleMath::Vector3& b)
  {
    return DirectX::SimpleMath::Vector3(
      (a.y * b.z) - (a.z * b.y),
      (a.z * b.x) - (a.x * b.z),
      (a.x * b.y) - (a.y * b.x));
  }

  //------------------------------------------------------------------------------
  inline void Cross(
    float ax, float ay, float az,
    float bx, float by, float bz,
    float* nx, float* ny, float* nz)
  {
    *nx = (ay * bz) - (az * by);
    *ny = (az * bx) - (ax * bz);
    *nz = (ax * by) - (ay * bx);
  }

  //------------------------------------------------------------------------------
  inline void Normalize(float* x, float* y, float* z)
  {
    float xx = *x;
    float yy = *y;
    float zz = *z;

    float recipLen = 1.f / sqrtf(xx*xx + yy*yy + zz*zz);
    *x = xx * recipLen;
    *y = yy * recipLen;
    *z = zz * recipLen;
  }

  //------------------------------------------------------------------------------
  void ExtractPlanes(
    const DirectX::SimpleMath::Matrix& mtx,
    bool normalize,
    DirectX::SimpleMath::Plane* planes);

  //------------------------------------------------------------------------------
  inline float DistanceToPoint(const DirectX::SimpleMath::Plane& plane, const DirectX::SimpleMath::Vector3& pt)
  {
    return plane.x * pt.x + plane.y * pt.y + plane.z * pt.z + plane.w;

  }

  //------------------------------------------------------------------------------
  inline float Dot(const DirectX::SimpleMath::Plane& plane, const DirectX::SimpleMath::Vector3& pt)
  {
    // Note, this is just the dot product between the plane's normal and pt
    return plane.x * pt.x + plane.y * pt.y + plane.z * pt.z;
  }

  //------------------------------------------------------------------------------
  int ClipPolygonAgainstPlane(
    int vertexCount, const DirectX::SimpleMath::Vector3* vertex,
    const DirectX::SimpleMath::Plane& plane,
    DirectX::SimpleMath::Vector3* result);
}