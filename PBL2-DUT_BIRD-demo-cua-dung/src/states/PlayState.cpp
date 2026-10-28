#include "states/PlayState.hpp"
#include "states/PauseState.hpp"
#include "states/GameOverState.hpp"
#include "states/StateMachine.hpp"
#include "systems/collisionmanage.hpp"
#include "core/GameConfig.hpp"

namespace states {

PlayState::PlayState(StateMachine& stateMachine)
    : m_stateMachine(stateMachine) {
}

void PlayState::enter() {
    sf::Font font;
    if (font.openFromFile("assets/fonts/arial.ttf")) {
        m_font.emplace(std::move(font));

        m_scoreText.emplace(*m_font, "0", 36);
        m_scoreText->setFillColor(sf::Color::White);
        m_scoreText->setOutlineThickness(2.0f);
        m_scoreText->setOutlineColor(sf::Color::Black);
        m_scoreText->setPosition({static_cast<float>(GameConfig::WINDOW_WIDTH) / 2.0f - 10.0f, 30.0f});
    }

    resetGame();
}

void PlayState::exit() {
}

void PlayState::resetGame() {
    m_bird.reset();
    m_ground.reset();
    m_pipeSpawner.reset();
    m_powerUpSystem.reset();
    m_scoreSystem.resetCurrentScore();
    m_isGameOver = false;

    if (m_scoreText.has_value()) {
        m_scoreText->setString("0");
    }
}

void PlayState::handleInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::P) {
            // Tạm dừng game
            m_stateMachine.pushState(std::make_unique<PauseState>(m_stateMachine));
            return;
        }
    }

    // Chuyển sự kiện cho chim
    m_bird.handleEvent(event);
}

void PlayState::update(float dt) {
    // 1. Cập nhật các thực thể
    m_bird.update(dt);
    m_ground.update(dt);
    m_pipeSpawner.update(dt);
    m_powerUpSystem.update(dt, m_bird, m_pipeSpawner.getPipes());

    // 2. Kiểm tra va chạm hộp quà
    m_powerUpSystem.checkCollisions(m_bird);

    // 3. Kiểm tra tính điểm khi chim bay qua ống
    if (m_pipeSpawner.checkPassedPipe(m_bird.getPosition().x)) {
        m_scoreSystem.addScore(1);
        if (m_scoreText.has_value()) {
            m_scoreText->setString(std::to_string(m_scoreSystem.getCurrentScore()));
        }
    }

    // 4. Kiểm tra va chạm ống & đất
    bool isDead = systems::CollisionManager::checkCollisions(
        m_bird,
        m_pipeSpawner.getPipes(),
        m_ground
    );

    if (isDead || m_bird.isDead()) {
        m_isGameOver = true;
        m_ground.setPaused(true);
        m_pipeSpawner.setPaused(true);

        // Chuyển sang GameOverState
        m_stateMachine.changeState(std::make_unique<GameOverState>(
            m_stateMachine,
            m_scoreSystem.getCurrentScore(),
            m_scoreSystem.getHighScore()
        ));
    }
}

void PlayState::render(sf::RenderTarget& target) {
    target.clear(sf::Color(113, 197, 207));

    // Vẽ ống
    m_pipeSpawner.draw(target);

    // Vẽ hộp quà
    m_powerUpSystem.draw(target);

    // Vẽ đất
    target.draw(m_ground);

    // Vẽ chim
    target.draw(m_bird);

    // Vẽ UI Score trên cùng
    if (m_scoreText.has_value()) {
        target.draw(*m_scoreText);
    }
}

} // namespace states

