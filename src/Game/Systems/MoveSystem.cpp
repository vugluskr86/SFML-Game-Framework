#include "MoveSystem.h"

#include <glm/glm/geometric.hpp>
#include <glm/glm/vec2.hpp>

#include "../Components/Position.h"
#include "../Components/Velocity.h"
#include "../Components/Sprite.h"

void MoveSystem::update(sf::Time deltaTime)
{
    auto view = registry.view<Sprite, Position, Velocity>();

    view.each([](auto& spr, auto& pos, auto vel) {
        pos.value += vel.value;
        spr.gfx.setPosition(pos.value.x, pos.value.y);
    });
}