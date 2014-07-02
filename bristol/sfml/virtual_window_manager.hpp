#pragma once
#include <functional>
#include <stdint.h>
#include <map>
#include <vector>
#include <deque>
#include <SFML/Window/Event.hpp>

namespace sf
{
  class RenderWindow;
}

namespace bristol
{
  class VirtualWindow;
  class WindowEventManager;

  class VirtualWindowManager
  {
  public:
    typedef std::function<bool(const sf::Event&)> fnEventHandler;

    VirtualWindowManager(
        sf::RenderWindow* window,
        WindowEventManager* eventManager);
    ~VirtualWindowManager();

    bool AddWindow(VirtualWindow* window);
    // passing a null parent is valid
    void SetParent(VirtualWindow* parent, VirtualWindow* child);
    bool Init();

    void Update();
    void Draw();

    // The WindowManager will send the events to the appropriate Virtual Window
    uint32_t RegisterHandler(sf::Event::EventType event, VirtualWindow* window, const fnEventHandler& handler);
    void UnregisterHandler(uint32_t handle);

  private:

    bool OnLostFocus(const sf::Event& event);
    bool OnResize(const sf::Event& event);
    bool OnMouseButtonPressed(const sf::Event& event);
    bool OnMouseButtonReleased(const sf::Event& event);
    bool OnMouseMove(const sf::Event& event);
    bool OnKeyReleased(const sf::Event& event);

    bool GenericHandler(const sf::Event& event);
    bool HandlerForFocusWindow(const sf::Event& event);
    void SetFocus(VirtualWindow* window);

    bool ResetMovingAndResizing();

    typedef std::pair<uint32_t, fnEventHandler> HandlerPair;
    typedef std::map<VirtualWindow*, HandlerPair> HandlersByWindow;
    std::map<sf::Event::EventType, HandlersByWindow > _handlersByWindow;
    std::map<sf::Event::EventType, std::vector<HandlerPair> > _genericHandlers;

    sf::RenderWindow* _renderWindow;
    WindowEventManager* _eventManager;
    std::vector<VirtualWindow*> _windows;
    uint32_t _nextId;
    uint32_t _nextDepth;

    // Moving data
    sf::Vector2f _startOperationPos;
    sf::Vector2f _windowOrgPos;
    sf::Vector2f _windowOrgSize;
    VirtualWindow* _focusWindow;

    VirtualWindow* _movingWindow;
    VirtualWindow* _resizingWindow;
    uint8_t _resizeFlags;
  };

}