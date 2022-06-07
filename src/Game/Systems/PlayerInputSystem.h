#pragma once

#include <entt/single_include/entt/entt.hpp>

#include "../BaseSystem.h"

class PlayerInputSystem : public BaseSystem {
  public:
    PlayerInputSystem(entt::registry& reg)
        : BaseSystem(reg)
    {

    }

    void handleInput();
};