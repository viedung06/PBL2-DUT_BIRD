#ifndef PAUSE_STATE_HPP
#define PAUSE_STATE_HPP

#include "states/GameState.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

namespace states {

class StateMachine;

class PauseState : public GameState {
public:
    explicit PauseState(StateMachine& stateMachine);
    ~PauseState() override = default;

    void enter() override;
    void exit() override;

    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderTarget& target) override;

private:
    StateMachine& m_stateMachine;

    sf::RectangleShape m_overlay;
    std::optional<sf::Font> m_font;
    std::optional<sf::Text> m_pauseText;
    std::optional<sf::Text> m_instructionText;
};

} // namespace states

#endif // PAUSE_STATE_HPP

