#include "graphics_utils.hpp"
#include "../utils.hpp"

using namespace DirectX::SimpleMath;
namespace bristol
{
  //------------------------------------------------------------------------------
  uint32_t VertexSizeFromFlags(uint32_t flags)
  {
    uint32_t size = 0;
    if (flags & VF_POS) size += 12;
    if (flags & VF_POS_XY) size += 8;
    if (flags & VF_NORMAL) size += 12;
    if (flags & VF_TEX2_0) size += 8;
    if (flags & VF_TEX3_0) size += 12;
    if (flags & VF_COLOR) size += 16;
    if (flags & VF_COLOR_U32) size += 4;

    return size;
  }

  //------------------------------------------------------------------------------
  uint32_t IndexSizeFromFormat(DXGI_FORMAT format)
  {
    if (format == DXGI_FORMAT_R16_UINT) return 2;
    if (format == DXGI_FORMAT_R32_UINT) return 4;
    assert(!"Unsupported index buffer format");
    return 0;
  }

  //------------------------------------------------------------------------------
  Vector3 ScreenToViewSpace(const Matrix& proj, uint32_t x, uint32_t y, int w, int h)
  {
    float xv = (2.0f * x / w - 1) / proj(0, 0);
    float yv = (-2.0f * y / h + 1) / proj(1, 1);

    return Vector3(xv, yv, 1);
  }

  //------------------------------------------------------------------------------
  float Raycast(const Vector3& center, float radius, const Vector3& o, const Vector3& d)
  {
    // ray -> sphere intersection
    // Compute A, B and C coefficients
    float a = Dot(d, d);
    float b = 2 * Dot(d, o - center);
    float c = Dot(o - center, o - center) - (radius * radius);

    // Find discriminant
    float disc = b * b - 4 * a * c;

    // if discriminant is negative there are no real roots, so ray misses sphere
    if (disc < 0)
      return -1;

    float t0 = (-b - sqrt(disc)) / (2 * a);
    float t1 = (-b + sqrt(disc)) / (2 * a);

    // return smallest non-negative 
    float ta = min(t0, t1);
    float tb = max(t0, t1);
    return ta < 0 ? tb : ta;
  }

  //------------------------------------------------------------------------------
  uint32_t ColorToU32(float r, float g, float b, float a)
  {
    return 
      ((uint32_t)(255 * Clamp(0.0f, 1.0f, r)) << 0) +
      ((uint32_t)(255 * Clamp(0.0f, 1.0f, g)) << 8) +
      ((uint32_t)(255 * Clamp(0.0f, 1.0f, b)) << 16) +
      ((uint32_t)(255 * Clamp(0.0f, 1.0f, a)) << 24);
  }

  //------------------------------------------------------------------------------
  Vector3 FromSpherical(const Spherical& s)
  {
    // phi is angle around x axis, ccw, starting at 0 at the x-axis
    // theta is angle around the z axis

    float yProj = s.r * sinf(s.theta);
    return Vector3(yProj * cosf(s.phi), s.r * cosf(s.theta), yProj * sinf(s.phi));
  }

  //------------------------------------------------------------------------------
  Spherical ToSpherical(const Vector3& v)
  {
    float r = v.Length();
    if (r == 0.f)
      return{ 0, 0, 0 };

    return{ r, atan2f(v.z, v.x), acosf(v.y/r) };
  }
}
