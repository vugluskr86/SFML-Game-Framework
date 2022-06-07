#pragma once

#include <SFML/Graphics.hpp>
#include <entt/single_include/entt/entt.hpp>

#include "../BaseSystem.h"

class SpriteMove : public BaseSystem {
  public:
    SpriteMove(entt::registry& reg, Game& game, StateBase& state)
        : BaseSystem(reg, game, state)
    {
    }

    void update(sf::Time deltaTime);
};