#pragma once

#include <entt/single_include/entt/entt.hpp>

#include "../BaseSystem.h"

class PlayerMovementInput : public BaseSystem {
  public:
    PlayerMovementInput(entt::registry& reg)
        : BaseSystem(reg)
    {

    }

    void handleInput();
};