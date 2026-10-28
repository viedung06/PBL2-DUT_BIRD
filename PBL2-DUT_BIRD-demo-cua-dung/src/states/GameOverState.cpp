#include "states/GameOverState.hpp"
#include "states/PlayState.hpp"
#include "states/MenuState.hpp"
#include "states/StateMachine.hpp"
#include "core/GameConfig.hpp"

namespace states {

GameOverState::GameOverState(StateMachine& stateMachine, int score, int highScore)
    : m_stateMachine(stateMachine),
      m_score(score),
      m_highScore(highScore) {
    
    // Panel màu nền cho bảng điểm
    m_panel.setSize({300.0f, 220.0f});
    m_panel.setOrigin({150.0f, 110.0f});
    m_panel.setPosition({static_cast<float>(GameConfig::WINDOW_WIDTH) / 2.0f, 280.0f});
    m_panel.setFillColor(sf::Color(222, 216, 149)); // Màu cát vàng
    m_panel.setOutlineThickness(3.0f);
    m_panel.setOutlineColor(sf::Color(85, 140, 30));
}

void GameOverState::enter() {
    sf::Font font;
    if (font.openFromFile("assets/fonts/arial.ttf")) {
        m_font.emplace(std::move(font));

        m_gameOverText.emplace(*m_font, "GAME OVER", 38);
        m_gameOverText->setFillColor(sf::Color(230, 40, 40));
        m_gameOverText->setOutlineThickness(2.0f);
        m_gameOverText->setOutlineColor(sf::Color::Black);
        auto gob = m_gameOverText->getLocalBounds();
        m_gameOverText->setOrigin({gob.size.x / 2.0f, gob.size.y / 2.0f});
        m_gameOverText->setPosition({static_cast<float>(GameConfig::WINDOW_WIDTH) / 2.0f, 110.0f});

        m_scoreText.emplace(*m_font, "Score: " + std::to_string(m_score), 24);
        m_scoreText->setFillColor(sf::Color(50, 50, 50));
        auto sb = m_scoreText->getLocalBounds();
        m_scoreText->setOrigin({sb.size.x / 2.0f, sb.size.y / 2.0f});
        m_scoreText->setPosition({static_cast<float>(GameConfig::WINDOW_WIDTH) / 2.0f, 230.0f});

        m_highScoreText.emplace(*m_font, "Best: " + std::to_string(m_highScore), 24);
        m_highScoreText->setFillColor(sf::Color(210, 100, 0));
        auto hsb = m_highScoreText->getLocalBounds();
        m_highScoreText->setOrigin({hsb.size.x / 2.0f, hsb.size.y / 2.0f});
        m_highScoreText->setPosition({static_cast<float>(GameConfig::WINDOW_WIDTH) / 2.0f, 280.0f});

        m_instructionText.emplace(*m_font, "Press R to Replay\nPress M for Menu", 20);
        m_instructionText->setFillColor(sf::Color::White);
        m_instructionText->setOutlineThickness(1.5f);
        m_instructionText->setOutlineColor(sf::Color::Black);
        auto ib = m_instructionText->getLocalBounds();
        m_instructionText->setOrigin({ib.size.x / 2.0f, ib.size.y / 2.0f});
        m_instructionText->setPosition({static_cast<float>(GameConfig::WINDOW_WIDTH) / 2.0f, 440.0f});
    }
}

void GameOverState::exit() {
}

void GameOverState::handleInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::R || keyPressed->code == sf::Keyboard::Key::Space) {
            // Chơi lại ngay lập tức
            m_stateMachine.changeState(std::make_unique<PlayState>(m_stateMachine));
        } else if (keyPressed->code == sf::Keyboard::Key::M) {
            // Về Menu
            m_stateMachine.changeState(std::make_unique<MenuState>(m_stateMachine, m_highScore));
        }
    }
}

void GameOverState::update(float dt) {
    (void)dt;
}

void GameOverState::render(sf::RenderTarget& target) {
    target.clear(sf::Color(113, 197, 207));

    target.draw(m_panel);

    if (m_gameOverText.has_value()) {
        target.draw(*m_gameOverText);
    }
    if (m_scoreText.has_value()) {
        target.draw(*m_scoreText);
    }
    if (m_highScoreText.has_value()) {
        target.draw(*m_highScoreText);
    }
    if (m_instructionText.has_value()) {
        target.draw(*m_instructionText);
    }
}

} // namespace states

