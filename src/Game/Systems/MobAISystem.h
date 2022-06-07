#pragma once

#include <SFML/Graphics.hpp>
#include <entt/single_include/entt/entt.hpp>

#include "../BaseSystem.h"

class MobAISystem : public BaseSystem {
  public:
    MobAISystem(entt::registry& reg)
        : BaseSystem(reg)
    {
    }

    void update(sf::Time deltaTime);
};