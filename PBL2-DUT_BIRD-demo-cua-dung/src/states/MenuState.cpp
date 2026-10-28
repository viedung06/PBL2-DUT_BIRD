#include "states/MenuState.hpp"
#include "states/PlayState.hpp"
#include "states/StateMachine.hpp"
#include "core/GameConfig.hpp"

namespace states {

MenuState::MenuState(StateMachine& stateMachine, int highScore)
    : m_stateMachine(stateMachine),
      m_highScore(highScore) {
}

void MenuState::enter() {
    sf::Font font;
    if (font.openFromFile("assets/fonts/arial.ttf")) {
        m_font.emplace(std::move(font));

        m_titleText.emplace(*m_font, "DUT BIRD", 42);
        m_titleText->setFillColor(sf::Color(255, 204, 0));
        m_titleText->setOutlineThickness(2.5f);
        m_titleText->setOutlineColor(sf::Color(40, 40, 40));
        auto titleBounds = m_titleText->getLocalBounds();
        m_titleText->setOrigin({titleBounds.size.x / 2.0f, titleBounds.size.y / 2.0f});
        m_titleText->setPosition({static_cast<float>(GameConfig::WINDOW_WIDTH) / 2.0f, 180.0f});

        m_instructionText.emplace(*m_font, "Press SPACE to Play", 22);
        m_instructionText->setFillColor(sf::Color::White);
        m_instructionText->setOutlineThickness(1.5f);
        m_instructionText->setOutlineColor(sf::Color::Black);
        auto instBounds = m_instructionText->getLocalBounds();
        m_instructionText->setOrigin({instBounds.size.x / 2.0f, instBounds.size.y / 2.0f});
        m_instructionText->setPosition({static_cast<float>(GameConfig::WINDOW_WIDTH) / 2.0f, 320.0f});

        m_highScoreText.emplace(*m_font, "High Score: " + std::to_string(m_highScore), 18);
        m_highScoreText->setFillColor(sf::Color(240, 240, 240));
        m_highScoreText->setOutlineThickness(1.0f);
        m_highScoreText->setOutlineColor(sf::Color::Black);
        auto hsBounds = m_highScoreText->getLocalBounds();
        m_highScoreText->setOrigin({hsBounds.size.x / 2.0f, hsBounds.size.y / 2.0f});
        m_highScoreText->setPosition({static_cast<float>(GameConfig::WINDOW_WIDTH) / 2.0f, 400.0f});
    }
}

void MenuState::exit() {
}

void MenuState::handleInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Space || keyPressed->code == sf::Keyboard::Key::Enter) {
            m_stateMachine.changeState(std::make_unique<PlayState>(m_stateMachine));
        }
    } else if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            m_stateMachine.changeState(std::make_unique<PlayState>(m_stateMachine));
        }
    }
}

void MenuState::update(float dt) {
    m_blinkTimer += dt * 3.0f;
    if (m_instructionText.has_value()) {
        // Nhấp nháy dòng chữ hướng dẫn
        float alpha = (static_cast<int>(m_blinkTimer) % 2 == 0) ? 255.0f : 80.0f;
        sf::Color col = m_instructionText->getFillColor();
        col.a = static_cast<std::uint8_t>(alpha);
        m_instructionText->setFillColor(col);
    }
}

void MenuState::render(sf::RenderTarget& target) {
    target.clear(sf::Color(113, 197, 207));

    if (m_titleText.has_value()) {
        target.draw(*m_titleText);
    }
    if (m_instructionText.has_value()) {
        target.draw(*m_instructionText);
    }
    if (m_highScoreText.has_value()) {
        target.draw(*m_highScoreText);
    }
}

} // namespace states

