#include "PlayerInput.h"

#include <iostream>

#include <glm/glm/geometric.hpp>

#include "../../ResourceManager/ResourceHolder.h"

#include "../Components/Player.h"
#include "../Components/Bullet.h"
#include "../Components/Sprite.h"
#include "../Components/PhysicBody.h"

#include "../../States/StatePlaying.h"

#include "../eCollideObjectGroups.h"

PlayerInput::PlayerInput(entt::registry& reg, Game& game, StateBase& state)
    : BaseSystem(reg, game, state)
{
    bulletTexture = ResourceHolder::get().textures.get("bullet");
}

void PlayerInput::handleInput()
{
    auto view = registry.view<Player, PhysicBody>();
    auto firstPlayer = view.front();
    auto& player = registry.get<Player>(firstPlayer);
    auto& physicBody = registry.get<PhysicBody>(firstPlayer);

    glm::vec2 absVelocity(0.0f, 0.0f);
    bool isIdle = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        absVelocity.y = -1;
        isIdle = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        absVelocity.y = 1;
        isIdle = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        absVelocity.x = -1;
        isIdle = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        absVelocity.x = 1;
        isIdle = false;
    }
    if (isIdle) {
        physicBody.body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    }
    else {
        const float mean =
            sqrtf(absVelocity.x * absVelocity.x + absVelocity.y * absVelocity.y);
        auto val = (absVelocity / mean) * player.speed;
        physicBody.body->SetLinearVelocity(b2Vec2(val.x, val.y));
    }

    auto [x, y] = sf::Mouse::getPosition(game.getWindow());

    // TODO: Check cast, ovverider operations
    auto playerPosBox2d = physicBody.body->GetPosition();
    auto playerPos = glm::vec2(playerPosBox2d.x, playerPosBox2d.y);
    const auto vec2Mouse = glm::vec2(x, y);
    auto target = vec2Mouse - playerPos;
    auto dist = glm::distance(playerPos, vec2Mouse);
    player.weaponTargetNormalize = target / dist;

    // TODO: Shit code!!!
    StatePlaying& playerState = static_cast<StatePlaying&>(state);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        
        const entt::entity bulletEntity = registry.create();

        registry.emplace<Bullet>(bulletEntity);

        auto& bulletSpr = registry.emplace<Sprite>(bulletEntity);
        bulletSpr.gfx.setTexture(bulletTexture);

        auto& physicBody = registry.emplace<PhysicBody>(bulletEntity);

        b2BodyDef body;
        body.type = b2_dynamicBody;
        body.position.Set(playerPos.x, playerPos.y);

        auto& spriteSize = bulletTexture.getSize();

        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(static_cast<float>(spriteSize.x) / 2.0f,
                            static_cast<float>(spriteSize.y) / 2.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.filter.groupIndex = static_cast<int16>(eCollideObjectGroups::BULLET);
        fixtureDef.userData.pointer = static_cast<uintptr_t>(bulletEntity);

        // TODO: Shit code!!!!
        auto playing = (StatePlaying*)(&state);

        physicBody.body = playing->physicWorld->CreateBody(&body);
        physicBody.body->CreateFixture(&fixtureDef);

        auto velocity = player.weaponTargetNormalize * 10.0f;

        physicBody.body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
    }
}