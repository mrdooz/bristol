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

  //------------------------------------------------------------------------------
  // From http://gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf
  void ExtractPlanes(const Matrix& mtx, bool normalize, Plane* planes)
  {
    // Left clipping plane
    planes[0].x = mtx._14 + mtx._11;
    planes[0].y = mtx._24 + mtx._21;
    planes[0].z = mtx._34 + mtx._31;
    planes[0].w = mtx._44 + mtx._41;

    // Right clipping plane
    planes[1].x = mtx._14 - mtx._11;
    planes[1].y = mtx._24 - mtx._21;
    planes[1].z = mtx._34 - mtx._31;
    planes[1].w = mtx._44 - mtx._41;

    // Top clipping plane
    planes[2].x = mtx._14 - mtx._12;
    planes[2].y = mtx._24 - mtx._22;
    planes[2].z = mtx._34 - mtx._32;
    planes[2].w = mtx._44 - mtx._42;

    // Bottom clipping plane
    planes[3].x = mtx._14 + mtx._12;
    planes[3].y = mtx._24 + mtx._22;
    planes[3].z = mtx._34 + mtx._32;
    planes[3].w = mtx._44 + mtx._42;

    // Near clipping plane
    planes[4].x = mtx._13;
    planes[4].y = mtx._23;
    planes[4].z = mtx._33;
    planes[4].w = mtx._43;

    // Far clipping plane
    planes[5].x = mtx._14 - mtx._13;
    planes[5].y = mtx._24 - mtx._23;
    planes[5].z = mtx._34 - mtx._33;
    planes[5].w = mtx._44 - mtx._43;

    if (normalize)
    {
      for (int i = 0; i < 6; ++i)
        planes[i].Normalize();
    }
  }

  //------------------------------------------------------------------------------
  int ClipPolygonAgainstPlane(int vertexCount, const Vector3* vertex, const Plane& plane, Vector3* result)
  {
    // from http://www.terathon.com/code/clipping.html
    enum Side
    {
      polygonInterior = 1,
      polygonBoundary = 0,
      polygonExterior = -1
    };

    Side location[16];

    const float boundaryEpsilon = 1.0e-3F;

    int positive = 0;
    int negative = 0;

    for (int a = 0; a < vertexCount; a++)
    {
      float d = DistanceToPoint(plane, vertex[a]);
      if (d > boundaryEpsilon)
      {
        location[a] = polygonInterior;
        positive++;
      }
      else
      {
        if (d < -boundaryEpsilon)
        {
          location[a] = polygonExterior;
          negative++;
        }
        else
        {
          location[a] = polygonBoundary;
        }
      }
    }

    if (negative == 0)
    {
      for (int a = 0; a < vertexCount; a++) result[a] = vertex[a];
      return (vertexCount);
    }
    else if (positive == 0)
    {
      return (0);
    }

    int count = 0;
    int previous = vertexCount - 1;
    for (int index = 0; index < vertexCount; index++)
    {
      int loc = location[index];
      if (loc == polygonExterior)
      {
        if (location[previous] == polygonInterior)
        {
          const Vector3& v1 = vertex[previous];
          const Vector3& v2 = vertex[index];
          Vector3 dv = v2 - v1;

          float t = DistanceToPoint(plane, v2) / Dot(plane, dv);
          result[count++] = v2 - dv * t;
        }
      }
      else
      {
        const Vector3& v1 = vertex[index];
        if ((loc == polygonInterior) && (location[previous] == polygonExterior))
        {
          const Vector3& v2 = vertex[previous];
          Vector3 dv = v2 - v1;

          float t = DistanceToPoint(plane, v2) / Dot(plane, dv);
          result[count++] = v2 - dv * t;
        }

        result[count++] = v1;
      }

      previous = index;
    }

    return (count);
  }


}


