#include "SpriteRender.h"

#include <iostream>

#include "../Components/Sprite.h"
#include "../Components/Player.h"

void SpriteRender::render(sf::RenderTarget& renderer)
{
    auto view = registry.view<Sprite>();
    view.each([&renderer](Sprite& spr) { renderer.draw(spr.gfx); });
}