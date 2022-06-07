#include "MobBehaviour.h"

#include <random>

#include <glm/glm/vec2.hpp>
#include <glm/glm/geometric.hpp>

#include "../../ResourceManager/ResourceHolder.h"

#include "../Components/Position.h"
#include "../Components/Velocity.h"
#include "../Components/Mob.h"
#include "../Components/Player.h"
#include "../Components/Bullet.h"
#include "../Components/Sprite.h"

MobBehaviour::MobBehaviour(entt::registry& reg, Game& game, StateBase& state)
    : BaseSystem(reg, game, state)
{
   mobTexture = ResourceHolder::get().textures.get("mob");
}

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


    auto mobsCount = mobsView.size_hint();

    if (mobsCount < 3) {
        std::random_device rnd;
        std::default_random_engine eng(rnd());
        std::uniform_real_distribution<float> randDistr(0.0f, 1.0f);

        const entt::entity e = registry.create();

        auto& mob = registry.emplace<Mob>(e);
        mob.speed = 1.2f;
        auto& pos = registry.emplace<Position>(e);
        registry.emplace<Velocity>(e);
        auto& spr = registry.emplace<Sprite>(e);
        spr.gfx.setTexture(mobTexture);
        const float randScale = 0.5f + randDistr(eng);
        spr.gfx.setScale(randScale, randScale);

        auto windowSize = game.getWindow().getSize();

        pos.value.x = static_cast<float>(randDistr(eng)) * windowSize.x;
        pos.value.y = static_cast<float>(randDistr(eng)) * windowSize.y;
    }
}
