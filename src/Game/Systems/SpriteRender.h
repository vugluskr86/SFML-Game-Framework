#pragma once

#include <SFML/Graphics.hpp>
#include <entt/single_include/entt/entt.hpp>

#include "../BaseSystem.h"

class SpriteRender : public BaseSystem {
  public:
    SpriteRender(entt::registry& reg, Game& game, StateBase& state)
        : BaseSystem(reg, game, state)
    {
    }

    void render(sf::RenderTarget& renderer);

  private:
    void debugDraw(sf::RenderTarget& renderer);
};