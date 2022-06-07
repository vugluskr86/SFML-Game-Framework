#pragma once

#include <entt/single_include/entt/entt.hpp>

#include "../BaseSystem.h"

class PlayerInput : public BaseSystem {
  public:
    PlayerInput(entt::registry& reg)
        : BaseSystem(reg)
    {

    }

    void handleInput();
};