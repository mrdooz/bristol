#include "graphics_utils.hpp"
#include "../utils.hpp"

using namespace DirectX::SimpleMath;
namespace bristol
{
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

}