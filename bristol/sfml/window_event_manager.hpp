#pragma once
#include <functional>
#include <stdint.h>
#include <map>
#include <vector>
#include <unordered_map>
#include <SFML/Window/Event.hpp>

namespace sf
{
  class RenderWindow;
}

namespace bristol
{
  class WindowEventManager
  {
  public:
    typedef std::function<bool(const sf::Event&)> fnEventHandler;

    WindowEventManager(sf::RenderWindow* window);

    uint32_t RegisterHandler(sf::Event::EventType event, const fnEventHandler& handler);
    void UnregisterHandler(uint32_t handle);

    void Poll();

  private:
    typedef std::pair<uint32_t, fnEventHandler> HandlerPair;
    std::map<sf::Event::EventType, std::vector<HandlerPair> > _handlersByWindow;
    std::unordered_map<uint32_t, sf::Event::EventType> _idToEvent;
    uint32_t _nextId;

    sf::RenderWindow* _window;
  };
}
