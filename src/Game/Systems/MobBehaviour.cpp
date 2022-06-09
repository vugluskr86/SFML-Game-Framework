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
#include "../Components/PhysicBody.h"

#include "../../States/StatePlaying.h"

MobBehaviour::MobBehaviour(entt::registry& reg, Game& game, StateBase& state)
    : BaseSystem(reg, game, state)
{
   mobTexture = ResourceHolder::get().textures.get("mob");
}

void MobBehaviour::update(sf::Time deltaTime)
{
    auto mobsView = registry.view<Mob, PhysicBody>();

    auto view = registry.view<Player, PhysicBody>();
    auto firstPlayer = view.front();

    auto& playerPhysicBody = registry.get<PhysicBody>(firstPlayer);
    auto playerPosBox2d = playerPhysicBody.bodyDef->GetPosition();
    auto playerPos = glm::vec2(playerPosBox2d.x, playerPosBox2d.y);

   // auto& playerPos = registry.get<Position>(firstPlayer);

    mobsView.each([&](Mob& mob, PhysicBody& mobPhysicBody) {
        auto mobPosBox2d = mobPhysicBody.bodyDef->GetPosition();
        auto mobPos = glm::vec2(mobPosBox2d.x, mobPosBox2d.y);
        const float distance = glm::distance(mobPos, playerPos);
        if (distance > 1.0f) {
            const glm::vec2 targetHeading = playerPos - mobPos;
            const glm::vec2 targetDirection = targetHeading / distance;
            
            mobPhysicBody.bodyDef->SetLinearVelocity(
                b2Vec2(targetDirection.x * mob.speed, targetDirection.y * mob.speed));

            // vel.value = targetDirection * mob.speed;
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

    if (mobsCount < 5) {
        std::random_device rnd;
        std::default_random_engine eng(rnd());
        std::uniform_real_distribution<float> randDistr(0.0f, 1.0f);

        const entt::entity e = registry.create();

        auto& mob = registry.emplace<Mob>(e);
        mob.speed = 1.2f;

        auto& spr = registry.emplace<Sprite>(e);
        spr.gfx.setTexture(mobTexture);

        const float randScale = 0.5f + randDistr(eng);
        spr.gfx.setScale(randScale, randScale);

        auto& spriteSize = mobTexture.getSize();

        auto windowSize = game.getWindow().getSize();

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(static_cast<float>(randDistr(eng)) * windowSize.x,
                             static_cast<float>(randDistr(eng)) * windowSize.y);

        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(static_cast<float>(spriteSize.x) / 2.0f * randScale,
                            static_cast<float>(spriteSize.y) / 2.0f * randScale);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        // TODO: Shit code!!!
        StatePlaying& playerState = static_cast<StatePlaying&>(state);

        auto& physicBody = registry.emplace<PhysicBody>(e);

        physicBody.bodyDef = playerState.physicWorld->CreateBody(&bodyDef);
        physicBody.bodyDef->CreateFixture(&fixtureDef);
    }
}
