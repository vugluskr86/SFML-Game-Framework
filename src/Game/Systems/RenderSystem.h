#pragma once

#include <SFML/Graphics.hpp>
#include <entt/single_include/entt/entt.hpp>

#include "../BaseSystem.h"

class RenderSystem : public BaseSystem {
  public:
    RenderSystem(entt::registry& reg)
        : BaseSystem(reg)
    {
    }

    void render(sf::RenderTarget& renderer);
};