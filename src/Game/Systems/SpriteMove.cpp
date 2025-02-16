#include "SpriteMove.h"

#include <glm/glm/geometric.hpp>
#include <glm/glm/vec2.hpp>

#include "../Components/PhysicBody.h"
#include "../Components/Sprite.h"


void SpriteMove::update(sf::Time deltaTime)
{
    auto view = registry.view<Sprite, PhysicBody>();
    
    view.each([](auto entity, auto& spr, PhysicBody& physicBody) {
        if (physicBody.body != nullptr) {
            auto& pos = physicBody.body->GetPosition();
            auto& sprSize = spr.gfx.getTextureRect();
            spr.gfx.setPosition(pos.x - sprSize.width / 2.0f,
                                pos.y - sprSize.height / 2.0f);
        }
    });
}