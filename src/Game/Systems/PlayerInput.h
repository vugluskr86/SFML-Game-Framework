#pragma once

#include <entt/single_include/entt/entt.hpp>

#include <SFML/Graphics.hpp>

#include "../BaseSystem.h"

class PlayerInput : public BaseSystem {
  public:
    PlayerInput(entt::registry& reg, Game& game);

    void handleInput();

  private:
    sf::Texture bulletTexture;
};