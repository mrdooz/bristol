#include <algorithm>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "sfml_helpers.hpp"
#include "../utils.hpp"

using namespace sf;
using namespace bristol;
using namespace std;

namespace
{
  Vector2f Normalize(const Vector2f& v)
  {
    float len = sqrtf(v.x*v.x + v.y*v.y);
    return 1/len * v;
  }
}

//-----------------------------------------------------------------------------
void bristol::DrawRectOutline(
    RenderTarget& texture,
    const Vector2f& pos,
    const Vector2f& size,
    const Color& col,
    int horizBorder,
    int vertBorder)
{
  if (vertBorder == -1)
    vertBorder = horizBorder;

  RectangleShape rect;
  rect.setFillColor(col);
  rect.setPosition(pos);
  rect.setSize(size);
  texture.draw(rect);

  Vector2f v((float)vertBorder, (float)horizBorder);
  rect.setPosition(pos + v);
  rect.setSize(size - 2.0f * v);
  rect.setFillColor(Color(col.r / 2, col.g / 2, col.b / 2));
  texture.draw(rect);
}

//-----------------------------------------------------------------------------
RoundedRectangleShape::RoundedRectangleShape(
    const Vector2f& size,
    float topLeftRadius,
    float topRightRadius,
    float bottomRightRadius,
    float bottomLeftRadius,
    int cornerPoints)
  : _size(size)
  , _topLeftRadius(topLeftRadius)
  , _topRightRadius(topRightRadius)
  , _bottomRightRadius(bottomRightRadius)
  , _bottomLeftRadius(bottomLeftRadius)
  , _cornerPoints(max(1, cornerPoints))
{
  update();
}

//-----------------------------------------------------------------------------
unsigned int RoundedRectangleShape::getPointCount() const
{
  return _cornerPoints * 4;
}

//-----------------------------------------------------------------------------
Vector2f RoundedRectangleShape::getPoint(unsigned int index) const
{
  static const float PI = 3.141592654f;

  Vector2f center;
  int idx = index / _cornerPoints;
  float r;
  switch (idx)
  {
    // top right
  case 0:
    r = _topRightRadius;
    center.x = _size.x - r;
    center.y = r;
    break;

    // top left
  case 1:
    r = _topLeftRadius;
    center.x = r;
    center.y = r;
    break;

    // bottom left
  case 2:
    r = _bottomLeftRadius;
    center.x = r;
    center.y = _size.y - r;
    break;

    // bottom right
  case 3:
    r = _bottomRightRadius;
    center.x = _size.x - r;
    center.y = _size.y - r;
    break;
  }

  // calc starting angle
  float start = idx * PI / 2;
  float angle = PI / 2 * (index - idx * _cornerPoints) / _cornerPoints;
  return center + Vector2f(r * cos(start + angle), -r * sin(start + angle));
}

//-----------------------------------------------------------------------------
LineStrip::LineStrip(float thickness, const sf::Color& color)
    : _thickness(thickness),
      _color(color)
{
}

//-----------------------------------------------------------------------------
void LineStrip::addPoint(const sf::Vector2f& v)
{
  _points.push_back(sf::Vertex(v, _color));
}

//-----------------------------------------------------------------------------
void LineStrip::addPoint(const sf::Vector2f& v, const sf::Color& col)
{
  _points.push_back(sf::Vertex(v, col));
}

//-----------------------------------------------------------------------------
void LineStrip::draw(RenderTarget& target, sf::RenderStates states) const
{
  if (_points.size() <= 1)
    return;

  VertexArray verts(sf::Triangles);

  for (size_t i = 0; i < _points.size() - 1; ++i)
  {
    const sf::Vector2f& p0  = _points[i+0].position;
    const sf::Vector2f& p1  = _points[i+1].position;
    const sf::Color& c0     = _points[i+0].color;
    const sf::Color& c1     = _points[i+1].color;

    const sf::Vector2f& dir = Normalize(p1-p0);
    sf::Vector2f normal(-dir.y, dir.x);
    sf::Vector2f ofs = _thickness / 2 * normal;

    sf::Vector2f v0 = p0 - ofs;
    sf::Vector2f v1 = p1 - ofs;
    sf::Vector2f v2 = p1 + ofs;
    sf::Vector2f v3 = p0 + ofs;

    verts.append(Vertex(v0, c0));
    verts.append(Vertex(v1, c1));
    verts.append(Vertex(v2, c1));

    verts.append(Vertex(v0, c0));
    verts.append(Vertex(v2, c1));
    verts.append(Vertex(v3, c0));
  }

  target.draw(verts, states);
}


//-----------------------------------------------------------------------------
ArcShape::ArcShape(
  const Vector2f& center,
  float radius,
  float startAngle,
  float endAngle,
  uint32_t segments)
  : _center(center)
  , _radius(radius)
  , _startAngle(startAngle)
  , _endAngle(endAngle)
  , _segments(segments)
{
  update();
}

//-----------------------------------------------------------------------------
uint32_t ArcShape::getPointCount() const
{
  return _segments + 1;
}

//-----------------------------------------------------------------------------
Vector2f ArcShape::getPoint(uint32_t idx) const
{
  if (idx == 0)
    return _center;

  // 0 degrees is up, ie (0,-1)
  float angle = lerp(_startAngle, _endAngle, (float)(idx-1) / (_segments-1));
  return _center + _radius * Vector2f(sinf(angle), -cosf(angle));
}

//-----------------------------------------------------------------------------
// hue: 0-360�; sat: 0.f-1.f; val: 0.f-1.f
Color bristol::ColorFromHsv(int hue, float sat, float val)
{
  hue %= 360;
  while(hue<0) hue += 360;

  sat = Clamp(sat, 0.f, 1.f);
  val = Clamp(val, 0.f, 1.f);

  int h = hue/60;
  float f = float(hue)/60-h;
  float p = val*(1.f-sat);
  float q = val*(1.f-sat*f);
  float t = val*(1.f-sat*(1-f));

#define U8(x) ((sf::Uint8)x)
  switch(h)
  {
  default:
  case 0:
  case 6: return Color(U8(val*255), U8(t*255),    U8(p*255));
  case 1: return Color(U8(q*255),   U8(val*255),  U8(p*255));
  case 2: return Color(U8(p*255),   U8(val*255),  U8(t*255));
  case 3: return Color(U8(p*255),   U8(q*255),    U8(val*255));
  case 4: return Color(U8(t*255),   U8(p*255),    U8(val*255));
  case 5: return Color(U8(val*255), U8(p*255),    U8(q*255));
  }
#undef U8
}

//-----------------------------------------------------------------------------
HsvColor bristol::HsvFromColor(const Color& color)
{
  // r,g,b values are from 0 to 1
  // h = [0,360], s = [0,1], v = [0,1]
  //		if s == 0, then h = -1 (undefined)

  HsvColor res;
  float r = color.r / 255.0f;
  float g = color.g / 255.0f;
  float b = color.b / 255.0f;

  float min, max, delta;
  min = min3( r, g, b );
  max = max3( r, g, b );
  // v
  res.v = max;
  delta = max - min;
  if( max != 0 )
  {
    // s
    res.s = delta / max;
  }
  else
  {
    // r = g = b = 0
    // s = 0, v is undefined
    res.s = 0;
    res.h = -1;
    return res;
  }

  if( r == max )
    // between yellow & magenta
    res.h = ( g - b ) / delta;
  else if( g == max )
    // between cyan & yellow
    res.h = 2 + ( b - r ) / delta;
  else
    // between magenta & cyan
    res.h = 4 + ( r - g ) / delta;

  // degrees
  res.h *= 60;
  if( res.h < 0 )
    res.h += 360;

  return res;
}

//-----------------------------------------------------------------------------
HsvColor::HsvColor(float h, float s, float v)
  : h(h), s(s), v(v)
{
}

//-----------------------------------------------------------------------------
HsvColor::operator Color() const
{
  return ColorFromHsv((int)h, s, v);
}
