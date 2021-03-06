#include "virtual_window.hpp"
#include "../utils.hpp"
#include "sfml_helpers.hpp"
#include "virtual_window_manager.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>


using namespace bristol;
using namespace std;
using namespace sf;

//-----------------------------------------------------------------------------
VirtualWindow::VirtualWindow(
    const string& title,
    const Vector2f& pos,
    const Vector2f& size,
    const WindowFlags& flags)
  : _windowManager(nullptr)
  , _title(title)
  , _pos(pos)
  , _size(size)
  , _depth(0)
  , _borderWidth(4)
  , _titleBarHeight(10)
  , _focus(false)
  , _moving(false)
  , _resizing(false)
  , _windowFlags(flags)
{
  _texture.create((int)size.x, (int)size.y);
  _sprite.setTexture(_texture.getTexture());
  _sprite.setPosition(pos);

  _defaultColor = Color(rand() % 255, rand() % 255, rand() % 255);
}

//-----------------------------------------------------------------------------
bool VirtualWindow::Init()
{
  if (!_font.loadFromFile("gfx/04b_03b_.ttf"))
    return false;

  return true;
}

//-----------------------------------------------------------------------------
void VirtualWindow::Draw()
{
  _texture.clear();

  RectangleShape rect;
  rect.setPosition(_pos);
  rect.setSize(_size);
  rect.setFillColor(_defaultColor);
  _texture.draw(rect);

  _texture.display();
}

//-----------------------------------------------------------------------------
void VirtualWindow::SetSize(const Vector2f& size)
{
  _size = size;
  _texture.create((int)size.x, (int)size.y);
  _sprite.setTexture(_texture.getTexture(), true);
}

//-----------------------------------------------------------------------------
void VirtualWindow::SetPosition(const Vector2f& pos)
{
  _pos = pos;
  _sprite.setPosition(pos);
}

//-----------------------------------------------------------------------------
void VirtualWindow::DrawBorder(RenderWindow* window)
{
  bool drawBorder = _windowFlags.IsSet(WindowFlag::DrawBorder);

  float w = drawBorder ? (float)_borderWidth : 0;

  if (drawBorder)
  {
    RoundedRectangleShape rect(_size + Vector2f(2*w, w+_titleBarHeight), 10, 10, 0, 0, 40);
    rect.setPosition(_pos - Vector2f(w, 10));
    rect.setFillColor(_focus ? Color(50, 50, 50) : Color(30, 30, 30));
    window->draw(rect);
  }

  // center the text (taking border into account)
  if (_windowFlags.IsSet(WindowFlag::DrawTitle))
  {
    Text text(_title, _font, _titleBarHeight - 2);
    float tw = text.getLocalBounds().width;
    text.setPosition(_pos + Vector2f((float)(w + (_size.x - tw) / 2), (float)_titleBarHeight));
    text.setColor(Color::White);
    window->draw(text);
  }
}

//-----------------------------------------------------------------------------
bool VirtualWindow::PointInside(const Vector2f& pos, bool includeBorder)
{
  float w = includeBorder ? (float)_borderWidth : 0;
  float h = includeBorder ? (float)_titleBarHeight : 0;

  FloatRect rect(_pos - Vector2f(w, h),
    Vector2f(2*w + _size.x, h + _size.y + w));

  return rect.contains(pos);
}

//-----------------------------------------------------------------------------
uint32_t VirtualWindow::PointInsideBorder(const Vector2f& pos)
{
  //enum { LeftBorder = 1, RightBorder = 2, TopBorder = 4, BottomBorder = 8};

  float w = (float)_borderWidth;
  float h = (float)_titleBarHeight;

  uint32_t mask = 0;

  FloatRect rectLeft(_pos - Vector2f(w, h), Vector2f(w, h+_size.y+w));
  if (rectLeft.contains(pos))
    mask |= LeftBorder;

  FloatRect rectRight(_pos + Vector2f(_size.x, -h), Vector2f(w, h+_size.y+w));
  if (rectRight.contains(pos))
    mask |= RightBorder;

  FloatRect rectTop(_pos - Vector2f(w, h), Vector2f(2*w + _size.x, h));
  if (rectTop.contains(pos))
    mask |= TopBorder;

  FloatRect rectTopOuter(_pos - Vector2f(w, h), Vector2f(2*w + _size.x, w));
  if (rectTopOuter.contains(pos))
    mask |= TopBorderOuter;

  FloatRect rectBottom(_pos + Vector2f(-w, _size.y), Vector2f(2*w + _size.x, w));
  if (rectBottom.contains(pos))
    mask |= BottomBorder;

  return mask;
}

