#pragma once
#include <string>
#include "../flags.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

namespace sf
{
  class RenderWindow;
}

namespace bristol
{
  class VirtualWindowManager;

  struct WindowFlag
  {
    enum Enum
    {
      DrawBorder    =   1 << 1,
      DrawTitle     =   1 << 2,
      Movable       =   1 << 3,
      Resizable     =   1 << 4,

      DefaultFlags  =   DrawBorder | DrawTitle | Movable | Resizable
    };

    struct Bits { uint32_t drawBorder : 1; uint32_t drawTitle : 1; uint32_t movable : 1; uint32_t resizable : 1; };
  };

  typedef Flags<WindowFlag> WindowFlags;

  class VirtualWindow
  {
    friend class VirtualWindowManager;
  public:

    VirtualWindow(
        const std::string& title,
        const sf::Vector2f& pos,
        const sf::Vector2f& size,
        const WindowFlags& flags = WindowFlags(WindowFlag::DefaultFlags));
    virtual ~VirtualWindow() {}

    // TODO: update with fixed time step. allow windows to specify frequency
    virtual void Update() {}
    virtual bool Init();
    virtual void Draw();
    sf::RenderTexture* GetTexture() { return &_texture; }

    bool PointInside(const sf::Vector2f& pos, bool includeBorder);

    // TopBorderOuter is the outermost part of the top border (the part that is as thick
    // as the other side's borders).
    enum { LeftBorder = 1, RightBorder = 2, TopBorder = 4, TopBorderOuter = 8, BottomBorder = 16};
    uint32_t PointInsideBorder(const sf::Vector2f& pos);

    void SetPosition(const sf::Vector2f& pos);
    void SetSize(const sf::Vector2f& size);

  protected:

    void DrawBorder(sf::RenderWindow* window);

    VirtualWindowManager* _windowManager;

    sf::Sprite _sprite;
    sf::RenderTexture _texture;

    sf::Font _font;
    sf::Color _defaultColor;

    std::string _title;
    sf::Vector2f _pos;
    sf::Vector2f _size;
    int _depth;
    uint32_t _borderWidth;
    uint32_t _titleBarHeight;
    bool _focus;
    bool _moving;
    bool _resizing;

    WindowFlags _windowFlags;
  };

  class GridSplitter : public VirtualWindow
  {
  public:
    GridSplitter(
      VirtualWindow* topLeft,
      VirtualWindow* topRight,
      VirtualWindow* bottomLeft,
      VirtualWindow* bottomRight,
      const sf::Vector2f& size,
      const sf::Vector2f& ratio = sf::Vector2f(0.5f, 0.5f));

    virtual void Draw();
    virtual bool Init();

  private:
    enum Pos { TopLeft, TopRight, BottomLeft, BottomRight };
    union {
      VirtualWindow* _windows[4];
      struct {
        VirtualWindow*_topLeft;
        VirtualWindow*_topRight;
        VirtualWindow*_bottomLeft;
        VirtualWindow*_bottomRight;
      };
    };

    sf::Sprite _sprites[4];
    sf::Vector2f _ratio;
  };
}