#include "BulletCollision.h"

#include <iostream>

#include <box2d/box2d.h>

#include <glm/glm/vec2.hpp>
#include <glm/glm/geometric.hpp>

#include "../Components/Bullet.h"
#include "../Components/Mob.h"
#include "../Components/PhysicBody.h"
#include "../Components/Sprite.h"

// TODO: SOLID
#include "../../States/StatePlaying.h"

#include "../eCollideObjectGroups.h"


BulletCollision::BulletCollision(entt::registry& reg, Game& game, StateBase& state)
    : BaseSystem(reg, game, state)
    , b2ContactListener()
{
    // TODO: Shit code!!!
    StatePlaying& playerState = static_cast<StatePlaying&>(state);
    playerState.physicWorld->SetContactListener(this);
}

BulletCollision::~BulletCollision()
{

}

void BulletCollision::update(sf::Time deltaTime)
{   
    StatePlaying& playingState = static_cast<StatePlaying&>(state);

    for (auto entity : toRemove) {
        auto& physicBody = registry.get<PhysicBody>(entity);
        playingState.physicWorld->DestroyBody(physicBody.body);
        physicBody.body = nullptr;
        registry.destroy(entity);
    }

    toRemove.clear();
}

void BulletCollision::BeginContact(b2Contact* contact)
{
    auto fixtureA = contact->GetFixtureA();
    auto fixtureB = contact->GetFixtureB();

    auto groupIndexA =
        static_cast<eCollideObjectGroups>(fixtureA->GetFilterData().groupIndex);

    auto groupIndexB =
        static_cast<eCollideObjectGroups>(fixtureB->GetFilterData().groupIndex);

    if ((groupIndexA == eCollideObjectGroups::BULLET &&
         groupIndexB == eCollideObjectGroups::MOB) || 
        (groupIndexB == eCollideObjectGroups::BULLET &&
         groupIndexA == eCollideObjectGroups::MOB)) {

        auto userDataA = fixtureA->GetUserData();
        auto userDataB = fixtureB->GetUserData();

        auto entA = static_cast<entt::entity>(userDataA.pointer);
        auto entB = static_cast<entt::entity>(userDataB.pointer);

        toRemove.insert(entA);
        toRemove.insert(entB);
    }
}

void BulletCollision::EndContact(b2Contact* contact)
{

}

void BulletCollision::PreSolve(b2Contact* contact,
                                       const b2Manifold* oldManifold)
{

}

void BulletCollision::PostSolve(b2Contact* contact,
                                        const b2ContactImpulse* impulse)
{

}