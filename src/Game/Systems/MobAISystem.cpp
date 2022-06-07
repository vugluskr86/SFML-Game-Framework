#include "MobAISystem.h"

#include <glm/glm/vec2.hpp>
#include <glm/glm/geometric.hpp>

#include "../Components/Position.h"
#include "../Components/Velocity.h"
#include "../Components/Mob.h"
#include "../Components/Player.h"
#include "../Components/Bullet.h"

void MobAISystem::update(sf::Time deltaTime)
{
    auto mobsView = registry.view<Mob, Velocity, Position>();
    auto playerView = registry.view<Player, Position>();

    mobsView.each([&playerView](Mob& mob, Velocity& vel, Position& mobPos) {
        // TODO: Select first entity
        playerView.each([&mobPos, &vel, &mob](Player& player, Position& playerPos) {
            const float distance = glm::distance(mobPos.value, playerPos.value);
            if (distance > 1.0f) {
                const glm::vec2 targetHeading = playerPos.value - mobPos.value;
                const glm::vec2 targetDirection = targetHeading / distance;
                vel.value = targetDirection * mob.speed;
            }
        });
    });


    mobsView.each([&](Mob& mob, Velocity& vel, Position& mobPos) {
        // TODO: Select first entity
        playerView.each([&](Player& player, Position& playerPos) {
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
        });
    });
}
