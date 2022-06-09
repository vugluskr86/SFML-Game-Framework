#pragma once

#include <vector>

#include <box2d/box2d.h>
#include <entt/single_include/entt/entt.hpp>

#include "StateBase.h"

#include "../Game/BaseSystem.h"
#include "../Util/Box2dDebugDraw.h"


/**
    Game state for the main part of the game
*/
class StatePlaying : public StateBase {
  public:
    StatePlaying(Game& game);

    virtual ~StatePlaying();

    void handleEvent(sf::Event e) override;
    void handleInput() override;
    void update(sf::Time deltaTime) override;
    void fixedUpdate(sf::Time deltaTime) override;
    void render(sf::RenderTarget& renderer) override;

  private:
    entt::registry registry;
    std::vector <std::unique_ptr<BaseSystem>> systems;

  public: // TODO: Shit code
    b2World* physicWorld;
    Box2dDebugDraw* debugDrawBox2d;
};
