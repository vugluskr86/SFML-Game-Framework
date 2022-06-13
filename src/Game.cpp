#include "Game.h"

#include <iostream>

// #include <optick/src/optick.h>

#include "States/StatePlaying.h"

/*
#if OPTICK_MSVC
#pragma warning(push)

// C4250. inherits 'std::basic_ostream'
#pragma warning(disable : 4250)

// C4127. Conditional expression is constant
#pragma warning(disable : 4127)
#endif
*/

Game::Game()
    : m_window({1280, 720}, "GAME_NAME")
{
    m_window.setPosition({m_window.getPosition().x, 0});
    m_window.setFramerateLimit(120);
    pushState<StatePlaying>(*this);
}

// Runs the main loop
void Game::run()
{
    constexpr unsigned TPS = 30; // ticks per seconds
    const sf::Time timePerUpdate = sf::seconds(1.0f / float(TPS));
    unsigned ticks = 0;

    sf::Clock timer;
    auto lastTime = sf::Time::Zero;
    auto lag = sf::Time::Zero;

    // OPTICK_APP("ConsoleApp");

    // Main loop of the game
    while (m_window.isOpen() && !m_states.empty()) {
        // OPTICK_FRAME("MainThread");
        auto& state = getCurrentState();

        // Get times
        auto time = timer.getElapsedTime();
        auto elapsed = time - lastTime;
        lastTime = time;
        lag += elapsed;

        // Real time update
        state.handleInput();
        state.update(elapsed);
        counter.update();

        // Fixed time update
        while (lag >= timePerUpdate) {
            ticks++;
            lag -= timePerUpdate;
            state.fixedUpdate(elapsed);
        }

        // Render
        m_window.clear();
        state.render(m_window);
        counter.draw(m_window);
        m_window.display();

        // Handle window events
        handleEvent();
        tryPop();
    }

    // OPTICK_SHUTDOWN();
}

// Tries to pop the current game state
void Game::tryPop()
{
    if (m_shouldPop) {
        m_shouldPop = false;
        if (m_shouldExit) {
            m_states.clear();
            return;
        }
        else if (m_shouldChageState) {
            m_shouldChageState = false;
            m_states.pop_back();
            pushState(std::move(m_change));
            return;
        }

        m_states.pop_back();
    }
}

// Handles window events, called every frame
void Game::handleEvent()
{
    sf::Event e;

    while (m_window.pollEvent(e)) {
        getCurrentState().handleEvent(e);
        switch (e.type) {
            case sf::Event::Closed:
                m_window.close();
                break;

            default:
                break;
        }
    }
}

// Returns a reference to the current game state
StateBase& Game::getCurrentState()
{
    return *m_states.back();
}

void Game::pushState(std::unique_ptr<StateBase> state)
{
    m_states.push_back(std::move(state));
}

// Flags a boolean for the game to pop state
void Game::popState()
{
    m_shouldPop = true;
}

void Game::exitGame()
{
    m_shouldPop = true;
    m_shouldExit = true;
}

// on tin
const sf::RenderWindow& Game::getWindow() const
{
    return m_window;
}

//#if OPTICK_MSVC
//#pragma warning(pop)
//#endif