#include "PlayerInput.h"

#include <iostream>

#include <glm/glm/geometric.hpp>

#include "../../ResourceManager/ResourceHolder.h"

#include "../Components/Velocity.h"
#include "../Components/Player.h"
#include "../Components/Position.h"
#include "../Components/Bullet.h"
#include "../Components/Sprite.h"
#include "../Components/PhysicBody.h"

#include "../../States/StatePlaying.h"

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
        physicBody.bodyDef->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    }
    else {
        const float mean =
            sqrtf(absVelocity.x * absVelocity.x + absVelocity.y * absVelocity.y);
        auto val = (absVelocity / mean) * player.speed;
        physicBody.bodyDef->SetLinearVelocity(b2Vec2(val.x, val.y));
    }

    auto [x, y] = sf::Mouse::getPosition(game.getWindow());

    // TODO: Check cast, ovverider operations
    auto playerPosBox2d = physicBody.bodyDef->GetPosition();
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

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(playerPos.x, playerPos.y);

        auto& spriteSize = bulletTexture.getSize();

        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(static_cast<float>(spriteSize.x) / 2.0f,
                            static_cast<float>(spriteSize.y) / 2.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;

        // TODO: Shit code!!!!
        auto playing = (StatePlaying*)(&state);

        physicBody.bodyDef = playing->physicWorld->CreateBody(&bodyDef);
        physicBody.bodyDef->CreateFixture(&fixtureDef);

        auto velocity = player.weaponTargetNormalize * 10.0f;

        physicBody.bodyDef->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));

        // bulletPos.value = playerPos;
        // bulletVel.value = player.weaponTargetNormalize * 10.0f;

        // TODO: Shit code, integration WIP
        // physicBody.bodyDef = playerState.physicWorld->CreateBody(new b2BodyDef());
    }
}