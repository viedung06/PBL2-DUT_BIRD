#include "states/PauseState.hpp"
#include "states/MenuState.hpp"
#include "states/StateMachine.hpp"
#include "core/GameConfig.hpp"

namespace states {

PauseState::PauseState(StateMachine& stateMachine)
    : m_stateMachine(stateMachine) {
    m_overlay.setSize({static_cast<float>(GameConfig::WINDOW_WIDTH), static_cast<float>(GameConfig::WINDOW_HEIGHT)});
    m_overlay.setFillColor(sf::Color(0, 0, 0, 120)); // Lớp phủ mờ bán trong suốt
}

void PauseState::enter() {
    sf::Font font;
    if (font.openFromFile("assets/fonts/arial.ttf")) {
        m_font.emplace(std::move(font));

        m_pauseText.emplace(*m_font, "PAUSED", 38);
        m_pauseText->setFillColor(sf::Color::White);
        m_pauseText->setOutlineThickness(2.0f);
        m_pauseText->setOutlineColor(sf::Color::Black);
        auto pb = m_pauseText->getLocalBounds();
        m_pauseText->setOrigin({pb.size.x / 2.0f, pb.size.y / 2.0f});
        m_pauseText->setPosition({static_cast<float>(GameConfig::WINDOW_WIDTH) / 2.0f, 240.0f});

        m_instructionText.emplace(*m_font, "Press P to Resume\nPress M for Menu", 20);
        m_instructionText->setFillColor(sf::Color(230, 230, 230));
        m_instructionText->setOutlineThickness(1.5f);
        m_instructionText->setOutlineColor(sf::Color::Black);
        auto ib = m_instructionText->getLocalBounds();
        m_instructionText->setOrigin({ib.size.x / 2.0f, ib.size.y / 2.0f});
        m_instructionText->setPosition({static_cast<float>(GameConfig::WINDOW_WIDTH) / 2.0f, 340.0f});
    }
}

void PauseState::exit() {
}

void PauseState::handleInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::P || keyPressed->code == sf::Keyboard::Key::Space) {
            // Tiếp tục chơi (Pop PauseState khỏi stack)
            m_stateMachine.popState();
        } else if (keyPressed->code == sf::Keyboard::Key::M) {
            // Về Menu
            m_stateMachine.changeState(std::make_unique<MenuState>(m_stateMachine));
        }
    }
}

void PauseState::update(float dt) {
    (void)dt;
    // PauseState không cập nhật vật lý
}

void PauseState::render(sf::RenderTarget& target) {
    // Vẽ lớp phủ bóng tối mờ lên trên PlayState
    target.draw(m_overlay);

    if (m_pauseText.has_value()) {
        target.draw(*m_pauseText);
    }
    if (m_instructionText.has_value()) {
        target.draw(*m_instructionText);
    }
}

} // namespace states

