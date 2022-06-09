#include "BulletCollision.h"

#include <iostream>

#include <glm/glm/vec2.hpp>
#include <glm/glm/geometric.hpp>

#include "../Components/Bullet.h"
#include "../Components/Position.h"
#include "../Components/Mob.h"
#include "../Components/PhysicBody.h"

// TODO: SOLID
#include "../../States/StatePlaying.h"

// TODO: Shit code, difficult, memory!!! Need object pool!!!
void BulletCollision::update(sf::Time deltaTime)
{
    auto& mobsView = registry.view<Mob, PhysicBody>();
    auto& bulletsView = registry.view<Bullet, PhysicBody>();

    bulletsView.each([&](auto bulletEntity, auto& bullet, auto& bulletPhysicBody) {
        mobsView.each([&](auto mobEntity, auto& mob, auto& mobPhysicBody) {
            auto bulletPos = bulletPhysicBody.bodyDef->GetPosition();
            auto mobPos = mobPhysicBody.bodyDef->GetPosition();
            const float distance = glm::distance(glm::vec2(bulletPos.x, bulletPos.y),
                                                 glm::vec2(mobPos.x, mobPos.y));
            if (distance < 20) {
                // TODO: Potential shit
                bullet._collided = true;
                mob._died = true;
            }
        });
    });

    bulletsView.each([&](auto bulletEntity, auto& bullet, auto& bulletPhysicBody) {
        if (bullet._collided) {
            registry.destroy(bulletEntity);
        }
    });

    mobsView.each([&](auto mobEntity, auto& mob, auto& mobPhysicBody) {
        if (mob._died) {
            registry.destroy(mobEntity);
        }
    });

    // TODO: Remove out of window
}