#include "PlayerInput.h"

#include <iostream>

#include <glm/glm/geometric.hpp>

#include "../../ResourceManager/ResourceHolder.h"

#include "../Components/Velocity.h"
#include "../Components/Player.h"
#include "../Components/Position.h"
#include "../Components/Bullet.h"
#include "../Components/Sprite.h"

PlayerInput::PlayerInput(entt::registry& reg, Game& game)
    : BaseSystem(reg, game)
{
    bulletTexture = ResourceHolder::get().textures.get("bullet");
}

void PlayerInput::handleInput()
{
    auto view = registry.view<Player, Velocity, Position>();
    auto firstPlayer = view.front();
    auto& player = registry.get<Player>(firstPlayer);
    auto& vel = registry.get<Velocity>(firstPlayer);
    auto& playerPos = registry.get<Position>(firstPlayer);

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
    if (isIdle) 
    {
        vel.value.x = 0;
        vel.value.y = 0;
    }
    else
    {
        const float mean =
            sqrtf(absVelocity.x * absVelocity.x + absVelocity.y * absVelocity.y);
        vel.value = (absVelocity / mean) * player.speed;
    }

    auto [x, y] = sf::Mouse::getPosition(game.getWindow());
    // TODO: Check cast
    const auto vec2Mouse = glm::vec2(x, y);
    auto target = vec2Mouse - playerPos.value;
    auto dist = glm::distance(playerPos.value, vec2Mouse);
    player.weaponTargetNormalize = target / dist;

    std::cout << x << "," << y << std::endl;


    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        
        const entt::entity bulletEntity = registry.create();

        registry.emplace<Bullet>(bulletEntity);

        auto& bulletSpr = registry.emplace<Sprite>(bulletEntity);
        auto& bulletVel = registry.emplace<Velocity>(bulletEntity);
        auto& bulletPos = registry.emplace<Position>(bulletEntity);

        bulletPos.value = playerPos.value;
        bulletVel.value = player.weaponTargetNormalize * 10.0f;

        bulletSpr.gfx.setTexture(bulletTexture);
    }
}