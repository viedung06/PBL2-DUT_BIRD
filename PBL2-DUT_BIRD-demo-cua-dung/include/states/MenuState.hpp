#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include "states/GameState.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

namespace states {

class StateMachine;

class MenuState : public GameState {
public:
    explicit MenuState(StateMachine& stateMachine, int highScore = 0);
    ~MenuState() override = default;

    void enter() override;
    void exit() override;

    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderTarget& target) override;

private:
    StateMachine& m_stateMachine;
    int m_highScore{0};

    std::optional<sf::Font> m_font;
    std::optional<sf::Text> m_titleText;
    std::optional<sf::Text> m_instructionText;
    std::optional<sf::Text> m_highScoreText;

    float m_blinkTimer{0.0f};
};

} // namespace states

#endif // MENU_STATE_HPP

