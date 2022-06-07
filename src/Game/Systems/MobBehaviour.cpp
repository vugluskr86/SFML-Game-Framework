#include "MobBehaviour.h"

#include <glm/glm/vec2.hpp>
#include <glm/glm/geometric.hpp>

#include "../Components/Position.h"
#include "../Components/Velocity.h"
#include "../Components/Mob.h"
#include "../Components/Player.h"
#include "../Components/Bullet.h"

void MobBehaviour::update(sf::Time deltaTime)
{
    auto mobsView = registry.view<Mob, Velocity, Position>();

    auto view = registry.view<Player, Position>();
    auto firstPlayer = view.front();
    auto& playerPos = registry.get<Position>(firstPlayer);

    mobsView.each([&](Mob& mob, Velocity& vel, Position& mobPos) {
        const float distance = glm::distance(mobPos.value, playerPos.value);
        if (distance > 1.0f) {
            const glm::vec2 targetHeading = playerPos.value - mobPos.value;
            const glm::vec2 targetDirection = targetHeading / distance;
            vel.value = targetDirection * mob.speed;
        }

        /*
        const float distance = glm::distance(mobPos.value, playerPos.value);
        const float DISTANCE_TO_SHOTING = 200.0f;
        if (distance <= DISTANCE_TO_SHOTING) {
            glm::vec2 targetHeading = playerPos.value - mobPos.value;
            glm::vec2 targetDirection = targetHeading / distance;

            const entt::entity e = registry.create();

            registry.emplace<Bullet>(e);
            auto& bulletVel = registry.emplace<Velocity>(e);
            auto& bulletPos = registry.emplace<Position>(e);

            bulletPos.value = mobPos.value;
            bulletVel.value = targetDirection * 10.0f;
        }
        */
    });
}
