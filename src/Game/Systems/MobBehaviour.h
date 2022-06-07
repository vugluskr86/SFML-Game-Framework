#pragma once

#include <SFML/Graphics.hpp>
#include <entt/single_include/entt/entt.hpp>

#include "../BaseSystem.h"

class MobBehaviour : public BaseSystem {
  public:
    MobBehaviour(entt::registry& reg, Game& game, StateBase& state);

    void update(sf::Time deltaTime);

  private:
    sf::Texture mobTexture;
};