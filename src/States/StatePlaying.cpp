#include "StatePlaying.h"

#include <random>
#include <iostream>
#include <entt/single_include/entt/entt.hpp>

#include "../GUI/Button.h"
#include "../GUI/Textbox.h"
#include "../Game.h"

#include "../Game/Components/Bullet.h"
#include "../Game/Components/Player.h"
#include "../Game/Components/Position.h"
#include "../Game/Components/Sprite.h"
#include "../Game/Components/Velocity.h"
#include "../Game/Components/Mob.h"

#include "../Game/Systems/SpriteRender.h"
#include "../Game/Systems/SpriteMove.h"
#include "../Game/Systems/PlayerInput.h"
#include "../Game/Systems/MobBehaviour.h"
#include "../Game/Systems/BulletCollision.h"


entt::entity initPlayer(entt::registry& reg, const sf::Texture& texture,
                        const sf::RenderWindow& window)
{
    const entt::entity e = reg.create();

    auto& player = reg.emplace<Player>(e);

    player.speed = 1.8f;

    auto& pos = reg.emplace<Position>(e);
    reg.emplace<Velocity>(e);

    auto& spr = reg.emplace<Sprite>(e);
    spr.gfx.setTexture(texture);

    auto windowSize = window.getSize();
    auto spriteSize = texture.getSize();

    pos.value.x = windowSize.x / 2.0f - spriteSize.x / 2.0f;
    pos.value.y = windowSize.y / 2.0f - spriteSize.y / 2.0f;

    return e;
}

entt::entity initMob(entt::registry& reg, const sf::Texture& texture,
                     const sf::RenderWindow& window)
{
    std::random_device rnd;
    std::default_random_engine eng(rnd());
    std::uniform_real_distribution<float> randDistr(0.0f, 1.0f);

    const entt::entity e = reg.create();

    auto& mob = reg.emplace<Mob>(e);
    mob.speed = 1.2f;
    auto& pos = reg.emplace<Position>(e);
    reg.emplace<Velocity>(e);
    auto& spr = reg.emplace<Sprite>(e);
    spr.gfx.setTexture(texture);
    const float randScale = 0.5f + randDistr(eng);
    spr.gfx.setScale(randScale, randScale);

    auto windowSize = window.getSize();

    pos.value.x = static_cast<float>(randDistr(eng)) * windowSize.x;
    pos.value.y = static_cast<float>(randDistr(eng)) * windowSize.y;
    return e;
}


std::string test;

StatePlaying::StatePlaying(Game& game)
    : StateBase(game)
    , m_TestMenu(game.getWindow(), 50)
{
    auto b = std::make_unique<gui::Button>();
    b->setText("Button 1");
    b->setFunction([]() { std::cout << "Button 1 clicked!" << '\n'; });

    m_TestMenu.addWidget(std::move(b));

    auto& window = m_pGame->getWindow();

    auto& catTexture = ResourceHolder::get().textures.get("cat");
    auto& mobtTexture = ResourceHolder::get().textures.get("mob");

    initPlayer(registry, catTexture, window);

    for (int i = 0; i < 5; i++) {
        initMob(registry, mobtTexture, window);
    }

    systems.emplace_back(new BulletCollision(registry, game, *this));
    systems.emplace_back(new SpriteMove(registry, game, *this));
    systems.emplace_back(new PlayerInput(registry, game, *this));
    systems.emplace_back(new MobBehaviour(registry, game, *this));
    systems.emplace_back(new SpriteRender(registry, game, *this));

    // physicWorld = new b2World(b2Vec2(0.0f, 0.0f));
}

void StatePlaying::handleEvent(sf::Event e)
{
    m_TestMenu.handleEvent(e, m_pGame->getWindow());

    for (auto& system : systems) {
        system->handleEvent(e);
    }
}

void StatePlaying::handleInput()
{
    for (auto& system : systems) {
        system->handleInput();
    }
}

void StatePlaying::update(sf::Time dt)
{
    for (auto& system : systems) {
        system->update(dt);
    }
}

void StatePlaying::fixedUpdate(sf::Time dt)
{
    for (auto& system : systems) {
        system->fixedUpdate(dt);
    }
}

void StatePlaying::render(sf::RenderTarget& renderer)
{
    for (auto& system : systems) {
        system->render(renderer);
    }

    m_TestMenu.render(renderer);
}
