#pragma once

#include <vector>

#include <entt/single_include/entt/entt.hpp>

#include "StateBase.h"

#include "../GUI/StackMenu.h"
#include "../Game/BaseSystem.h"

#include <box2d/box2d.h>

/**
    Game state for the main part of the game
*/
class StatePlaying : public StateBase {
  public:
    StatePlaying(Game& game);

    void handleEvent(sf::Event e) override;
    void handleInput() override;
    void update(sf::Time deltaTime) override;
    void fixedUpdate(sf::Time deltaTime) override;
    void render(sf::RenderTarget& renderer) override;

  private:
    gui::StackMenu m_TestMenu;

    entt::registry registry;
    std::vector <std::unique_ptr<BaseSystem>> systems;

  public: // TODO: Shit code
    b2World* physicWorld;
};
