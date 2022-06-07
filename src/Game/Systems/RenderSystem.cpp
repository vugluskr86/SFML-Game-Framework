#include "RenderSystem.h"

#include "../Components/Sprite.h"


void RenderSystem::render(sf::RenderTarget& renderer)
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
}