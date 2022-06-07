#include "PlayerMovementInput.h"

#include <SFML/Graphics.hpp>

#include "../Components/Velocity.h"
#include "../Components/Player.h"

void PlayerMovementInput::handleInput()
{
    auto view = registry.view<Player, Velocity>();

    view.each([](Player& player, Velocity& vel) {
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
            vel.value.x = 0;
            vel.value.y = 0;
        }
        else {
            const float mean =
                sqrtf(absVelocity.x * absVelocity.x + absVelocity.y * absVelocity.y);
            vel.value = (absVelocity / mean) * player.speed;
        }
    });
}