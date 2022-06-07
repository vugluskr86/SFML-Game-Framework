#include "BulletCollision.h"

#include <iostream>

#include <glm/glm/vec2.hpp>
#include <glm/glm/geometric.hpp>

#include "../Components/Bullet.h"
#include "../Components/Position.h"
#include "../Components/Mob.h"

// TODO: Shit code, difficult, memory!!! Need object pool!!!
void BulletCollision::update(sf::Time deltaTime)
{
    auto& mobsView = registry.view<Mob, Position>();
    auto& bulletsView = registry.view<Bullet, Position>();

    bulletsView.each([&](auto bulletEntity, auto& bullet, auto& bulletPos) {
        mobsView.each([&](auto mobEntity, auto& mob, auto& mobPos) {
            const float distance = glm::distance(bulletPos.value, mobPos.value);
            if (distance < 20) {
                // TODO: Potential shit
                bullet._collided = true;
                mob._died = true;
            }
        });
    });

    bulletsView.each([&](auto bulletEntity, auto& bullet, auto& bulletPos) {
        if (bullet._collided) {
            registry.destroy(bulletEntity);
        }
    });

    mobsView.each([&](auto mobEntity, auto& mob, auto& mobPos) {
        if (mob._died) {
            registry.destroy(mobEntity);
        }
    });

    // TODO: Remove out of window
}