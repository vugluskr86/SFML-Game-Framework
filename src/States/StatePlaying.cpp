#include "StatePlaying.h"

#include <random>
#include <iostream>
#include <entt/single_include/entt/entt.hpp>

#include "../ResourceManager/ResourceHolder.h"

#include "../Game/Components/Bullet.h"
#include "../Game/Components/Player.h"
#include "../Game/Components/Position.h"
#include "../Game/Components/Sprite.h"
#include "../Game/Components/Velocity.h"
#include "../Game/Components/Mob.h"
#include "../Game/Components/PhysicBody.h"

#include "../Game/Systems/SpriteRender.h"
#include "../Game/Systems/SpriteMove.h"
#include "../Game/Systems/PlayerInput.h"
#include "../Game/Systems/MobBehaviour.h"
#include "../Game/Systems/BulletCollision.h"

entt::entity initPlayer(entt::registry& reg, const sf::Texture& texture,
                        const sf::RenderWindow& window, b2World* physicWorld)
{
    const entt::entity e = reg.create();
    auto& player = reg.emplace<Player>(e);
    player.speed = 1.8f;

    auto& spr = reg.emplace<Sprite>(e);
    spr.gfx.setTexture(texture);

    auto windowSize = window.getSize();
    auto spriteSize = texture.getSize();

    auto posX = windowSize.x / 2.0f - spriteSize.x / 2.0f;
    auto posY = windowSize.y / 2.0f - spriteSize.y / 2.0f;

    auto& physicBody = reg.emplace<PhysicBody>(e);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(posX, posY);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(static_cast<float>(spriteSize.x) / 2.0f,
                        static_cast<float>(spriteSize.y) / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    physicBody.bodyDef = physicWorld->CreateBody(&bodyDef);
    physicBody.bodyDef->CreateFixture(&fixtureDef);

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
{
    const sf::RenderWindow& window = m_pGame->getWindow();

    physicWorld = new b2World(b2Vec2(0.0f, 0.0f));
    debugDrawBox2d = new Box2dDebugDraw(window, 1.0f);
    debugDrawBox2d->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit |
                             b2Draw::e_centerOfMassBit);
    physicWorld->SetDebugDraw(debugDrawBox2d);

    auto& catTexture = ResourceHolder::get().textures.get("cat");
    auto& mobtTexture = ResourceHolder::get().textures.get("mob");

    initPlayer(registry, catTexture, window, physicWorld);

    /*
    for (int i = 0; i < 5; i++) {
        initMob(registry, mobtTexture, window);
    }
    */

    systems.emplace_back(new PlayerInput(registry, game, *this));
    systems.emplace_back(new SpriteRender(registry, game, *this));
    systems.emplace_back(new SpriteMove(registry, game, *this));
    

    // 
    // systems.emplace_back(new BulletCollision(registry, game, *this));
    // 
    // systems.emplace_back(new MobBehaviour(registry, game, *this));
    // systems.emplace_back(new SpriteRender(registry, game, *this));


    /*
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 0.0f);

    b2Body* groundBody = physicWorld->CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(100.0f, 100.0f);


    groundBody->CreateFixture(&groundBox, 0.0f);
    */
}

StatePlaying::~StatePlaying()
{
    delete physicWorld;
    delete debugDrawBox2d;
}

void StatePlaying::handleEvent(sf::Event e)
{
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

    physicWorld->Step(static_cast<float>(dt.asMilliseconds()), 6, 2);
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

    physicWorld->DebugDraw();
}
