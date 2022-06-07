#pragma once

#include <SFML/Graphics.hpp>

#include "../Game.h"

class BaseSystem {
  public:
    BaseSystem(entt::registry& registry, Game& game)
        : registry(registry)
        , game(game)
    {

    }

    virtual void handleEvent(sf::Event e) {};
    virtual void handleInput() {};
    virtual void update(sf::Time deltaTime) {};
    virtual void fixedUpdate(sf::Time deltaTime) {};
    virtual void render(sf::RenderTarget& renderer) {};

protected:
    entt::registry& registry;
    Game& game;
};