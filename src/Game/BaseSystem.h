#pragma once

#include <SFML/Graphics.hpp>

class BaseSystem {
  public:
    BaseSystem(entt::registry& registry)
        : registry(registry)
    {

    }

    virtual void handleEvent(sf::Event e) {};
    virtual void handleInput() {};
    virtual void update(sf::Time deltaTime) {};
    virtual void fixedUpdate(sf::Time deltaTime) {};
    virtual void render(sf::RenderTarget& renderer) {};

protected:
    entt::registry& registry;
};