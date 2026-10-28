#ifndef PLAY_STATE_HPP
#define PLAY_STATE_HPP

#include "states/GameState.hpp"
#include "entities/Bird.hpp"
#include "entities/Ground.hpp"
#include "systems/PipeSpawn.hpp"
#include "systems/PowerUp.hpp"
#include "systems/ScoreSystem.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

namespace states {

class StateMachine;

class PlayState : public GameState {
public:
    explicit PlayState(StateMachine& stateMachine);
    ~PlayState() override = default;

    void enter() override;
    void exit() override;

    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderTarget& target) override;

    void resetGame();

private:
    StateMachine& m_stateMachine;

    // Thực thể & hệ thống
    entities::Bird m_bird;
    entities::Ground m_ground;
    systems::PipeSpawnerSystem m_pipeSpawner;
    systems::PowerUpSystem m_powerUpSystem;
    systems::ScoreSystem m_scoreSystem;

    bool m_isGameOver{false};

    // UI Score
    std::optional<sf::Font> m_font;
    std::optional<sf::Text> m_scoreText;
};

} // namespace states

#endif // PLAY_STATE_HPP

