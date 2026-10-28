#include "core/Game.hpp"
#include "states/MenuState.hpp"
#include <algorithm>
#include <string>

namespace core {

Game::Game()
    : m_window(sf::VideoMode({GameConfig::WINDOW_WIDTH, GameConfig::WINDOW_HEIGHT}),
               std::string(GameConfig::WINDOW_TITLE),
               sf::Style::Close | sf::Style::Titlebar) {
    
    if (GameConfig::VSYNC_ENABLED) {
        m_window.setVerticalSyncEnabled(true);
    } else {
        m_window.setFramerateLimit(GameConfig::FPS);
    }

    // Khởi tạo và kích hoạt ngay MenuState ban đầu
    m_stateMachine.changeState(std::make_unique<states::MenuState>(m_stateMachine));
    m_stateMachine.applyPendingChanges(); // Áp dụng ngay để m_states không bị rỗng
}

void Game::run() {
    while (m_window.isOpen() && m_isRunning && !m_stateMachine.isEmpty()) {
        float dt = m_clock.restart().asSeconds();
        
        // Giới hạn delta time bằng std::clamp từ <algorithm>
        dt = std::clamp(dt, 0.0f, 0.1f);

        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    // SFML 3: pollEvent trả về std::optional<sf::Event>
    while (const auto event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
            m_isRunning = false;
            return;
        }
        
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                m_window.close();
                m_isRunning = false;
                return;
            }
        }

        // Chuyển sự kiện cho State hiện tại xử lý
        m_stateMachine.handleInput(*event);
    }
}

void Game::update(float dt) {
    m_stateMachine.update(dt);

    // Áp dụng các thay đổi State hoãn lại (Pending Actions) an toàn ở cuối frame
    m_stateMachine.applyPendingChanges();
}

void Game::render() {
    m_stateMachine.render(m_window);
    m_window.display();
}

} // namespace core
