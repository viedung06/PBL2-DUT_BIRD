#ifndef GAME_OVER_STATE_HPP
#define GAME_OVER_STATE_HPP

#include "states/GameState.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

namespace states {

class StateMachine;

class GameOverState : public GameState {
public:
    GameOverState(StateMachine& stateMachine, int score, int highScore);
    ~GameOverState() override = default;

    void enter() override;
    void exit() override;

    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderTarget& target) override;

private:
    StateMachine& m_stateMachine;
    int m_score{0};
    int m_highScore{0};

    sf::RectangleShape m_panel;
    std::optional<sf::Font> m_font;
    std::optional<sf::Text> m_gameOverText;
    std::optional<sf::Text> m_scoreText;
    std::optional<sf::Text> m_highScoreText;
    std::optional<sf::Text> m_instructionText;
};

} // namespace states

#endif // GAME_OVER_STATE_HPP

