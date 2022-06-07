#include "SpriteRender.h"

#include <iostream>

#include "../Components/Sprite.h"
#include "../Components/Position.h"
#include "../Components/Player.h"

void SpriteRender::render(sf::RenderTarget& renderer)
{
    auto view = registry.view<Sprite>();
    view.each([&renderer](Sprite& spr) { renderer.draw(spr.gfx); });

    /*
    auto bulletView = reg.view<Bullet, Position>();
    for (auto ent : bulletView) {
        auto pos = bulletView.get<Position>(ent);
        bulletGfx.setPosition(pos.value.x, pos.value.y);
        window.draw(bulletGfx);
    }
    */

    debugDraw(renderer);
}

void SpriteRender::debugDraw(sf::RenderTarget& renderer)
{
    auto view = registry.view<Player, Position>();
    auto firstPlayer = view.front();
    auto& player = registry.get<Player>(firstPlayer);
    auto& pos = registry.get<Position>(firstPlayer);

    /* std::cout << pos.value.x << "," << pos.value.y
              << std::endl;*/

    sf::VertexArray lines(sf::LinesStrip, 2);

    auto playerPos = sf::Vector2f(pos.value.x, pos.value.y);

    lines[1].position = playerPos;
    lines[0].position = playerPos + ( sf::Vector2f(player.weaponTargetNormalize.x,
                                              player.weaponTargetNormalize.y) * 20.0f);

    renderer.draw(lines);
}