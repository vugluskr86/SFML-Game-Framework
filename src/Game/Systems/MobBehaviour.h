#pragma once

#include <SFML/Graphics.hpp>
#include <entt/single_include/entt/entt.hpp>

#include "../BaseSystem.h"

class MobBehaviour : public BaseSystem {
  public:
    MobBehaviour(entt::registry& reg, Game& game)
        : BaseSystem(reg, game)
    {
    }

    void update(sf::Time deltaTime);
};