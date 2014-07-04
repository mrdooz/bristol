#pragma once

// some code taken from various sources:
// https://github.com/LaurentGomila/SFML/wiki
// http://www.cs.rit.edu/~ncs/color/t_convert.html

// probably hacked to better fit my coding style
#include <stdint.h>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace bristol
{
  class RoundedRectangleShape : public sf::Shape
  {
  public:
    RoundedRectangleShape(
      const sf::Vector2f& size,
      float topLeftRadius,
      float topRightRadius,
      float bottomRightRadius,
      float bottomLeftRadius,
      int cornerPoints);

    virtual unsigned int getPointCount() const;
    virtual sf::Vector2f getPoint(unsigned int index) const;

  private:
    sf::Vector2f _size;
    float _topLeftRadius;
    float _topRightRadius;
    float _bottomRightRadius;
    float _bottomLeftRadius;
    int _cornerPoints;
  };

  //-----------------------------------------------------------------------------
  class ArcShape : public sf::Shape
  {
  public:
    ArcShape(const sf::Vector2f& center, float radius, float startAngle, float endAngle, uint32_t segments = 32);
    virtual uint32_t getPointCount() const;
    virtual sf::Vector2f getPoint(uint32_t index) const;

  private:
    sf::Vector2f _center;
    float _radius;
    float _startAngle, _endAngle;
    uint32_t _segments;
  };

  //-----------------------------------------------------------------------------
  class LineShape : public sf::Shape
  {
  public:

    LineShape(const sf::Vector2f& point1, const sf::Vector2f& point2, float thickness = 2);

    void setThickness(float thickness);
    float getThickness() const;
    float getLength() const;

    virtual unsigned int getPointCount() const;
    virtual sf::Vector2f getPoint(unsigned int index) const;

  private:
    sf::Vector2f m_direction; ///< Direction of the line
    float m_thickness;    ///< Thickness of the line
  };


  //-----------------------------------------------------------------------------
  struct HsvColor
  {
    HsvColor() : h(0), s(0), v(0) {}
    HsvColor(float h, float s, float v);
    operator sf::Color() const;
    float h, s, v;
  };
  //-----------------------------------------------------------------------------
  sf::Color ColorFromHsv(int hue, float sat, float val);
  HsvColor HsvFromColor(const sf::Color& color);

  inline uint32_t ColorToU32(const sf::Color& color)
  {
    return (color.r << 0) | (color.g << 8) | (color.b << 16) | (color.a << 24);
  }

  inline sf::Color ColorFromU32(uint32_t col)
  {
    return sf::Color(col & 0xff, (col >> 8) & 0xff, (col >> 16) & 0xff, (col >> 24) & 0xff);
  }

  void DrawRectOutline(
    sf::RenderTarget& texture,
    const sf::Vector2f& pos,
    const sf::Vector2f& size,
    const sf::Color& col,
    int horizBorder,
    int vertBorder = 1);

}
