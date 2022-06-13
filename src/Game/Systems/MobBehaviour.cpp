#include "MobBehaviour.h"

#include <random>

#include <glm/glm/vec2.hpp>
#include <glm/glm/geometric.hpp>

#include "../../ResourceManager/ResourceHolder.h"

#include "../Components/Mob.h"
#include "../Components/Player.h"
#include "../Components/Bullet.h"
#include "../Components/Sprite.h"
#include "../Components/PhysicBody.h"

#include "../../States/StatePlaying.h"

#include "../eCollideObjectGroups.h"

MobBehaviour::MobBehaviour(entt::registry& reg, Game& game, StateBase& state)
    : BaseSystem(reg, game, state)
{
   mobTexture = ResourceHolder::get().textures.get("mob");
}

void MobBehaviour::update(sf::Time deltaTime)
{

    auto mobsView = registry.view<Mob, PhysicBody>();
    auto playerView = registry.view<Player, PhysicBody>();

    auto firstPlayer = playerView.front();

    auto& playerPhysicBody = registry.get<PhysicBody>(firstPlayer);
    auto playerPosBox2d = playerPhysicBody.body->GetPosition();
    auto playerPos = glm::vec2(playerPosBox2d.x, playerPosBox2d.y);
    
    mobsView.each([&](Mob& mob, PhysicBody& mobPhysicBody) {
        auto mobPosBox2d = mobPhysicBody.body->GetPosition();
        auto mobPos = glm::vec2(mobPosBox2d.x, mobPosBox2d.y);
        const float distance = glm::distance(mobPos, playerPos);
        if (distance > 1.0f) {
            const glm::vec2 targetHeading = playerPos - mobPos;
            const glm::vec2 targetDirection = targetHeading / distance;
            
            mobPhysicBody.body->SetLinearVelocity(
                b2Vec2(targetDirection.x * mob.speed, targetDirection.y * mob.speed));
        }
    });

    auto mobsCount = mobsView.size_hint();

    if (mobsCount < 3) {
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

        b2BodyDef body;
        body.type = b2_dynamicBody;
        body.position.Set(static_cast<float>(randDistr(eng)) * windowSize.x,
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

        fixtureDef.filter.groupIndex = static_cast<int16>(eCollideObjectGroups::MOB);
        fixtureDef.userData.pointer = static_cast<uintptr_t>(e);

        physicBody.body = playerState.physicWorld->CreateBody(&body);
        physicBody.body->CreateFixture(&fixtureDef);
    }
}
