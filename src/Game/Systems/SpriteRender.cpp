#include "SpriteRender.h"

#include <iostream>

#include "../Components/Sprite.h"
#include "../Components/Position.h"
#include "../Components/Player.h"

void SpriteRender::render(sf::RenderTarget& renderer)
{
    auto view = registry.view<Sprite>();
    view.each([&renderer](Sprite& spr) { renderer.draw(spr.gfx); });
    debugDraw(renderer);
}

void SpriteRender::debugDraw(sf::RenderTarget& renderer)
{
    auto view = registry.view<Player, Position>();
    auto firstPlayer = view.front();
    auto& player = registry.get<Player>(firstPlayer);
    auto& pos = registry.get<Position>(firstPlayer);
    
    // TODO: Memory!!!
    sf::VertexArray lines(sf::LinesStrip, 2);
    auto playerPos = sf::Vector2f(pos.value.x, pos.value.y);
    lines[1].position = playerPos;
    lines[0].position = playerPos + ( sf::Vector2f(player.weaponTargetNormalize.x,
                                              player.weaponTargetNormalize.y) * 20.0f);
    renderer.draw(lines);
}