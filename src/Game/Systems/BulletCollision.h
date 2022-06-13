#pragma once

#include <SFML/Graphics.hpp>
#include <entt/single_include/entt/entt.hpp>
#include <box2d/box2d.h>

#include "../BaseSystem.h"

class BulletCollision : public BaseSystem, b2ContactListener {
  public:
    BulletCollision(entt::registry& reg, Game& game, StateBase& state);
    virtual ~BulletCollision();

    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

    void update(sf::Time deltaTime);

  private:
    std::set<entt::entity> toRemove;
};