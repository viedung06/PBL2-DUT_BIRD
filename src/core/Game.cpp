#include "../../include/core/Game.hpp"
#include <iostream>

namespace core {

Game::Game()
    : m_window(sf::VideoMode(GameConfig::WINDOW_WIDTH, GameConfig::WINDOW_HEIGHT),
               std::string(GameConfig::WINDOW_TITLE),
               sf::Style::Close | sf::Style::Titlebar) {
    
    if (GameConfig::VSYNC_ENABLED) {
        m_window.setVerticalSyncEnabled(true);
    } else {
        m_window.setFramerateLimit(GameConfig::TARGET_FPS);
    }
}

void Game::run() {
    while (m_window.isOpen() && m_isRunning) {
        float dt = m_clock.restart().asSeconds();
        
        // Giới hạn delta time (clamp) để tránh "spiral of death" hoặc nhảy xuyên vật thể
        // khi cửa sổ bị kéo di chuyển hoặc lag đột xuất.
        if (dt > 0.1f) {
            dt = 0.1f;
        }

        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
            m_isRunning = false;
        }
        
        // Nhấn phím Escape để thoát game
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            m_window.close();
            m_isRunning = false;
        }
    }
}

void Game::update(float dt) {
    // Phase 2: Khung sườn game loop đã sẵn sàng nhận dt
    // Các State, EntityManager, Physics và Systems sẽ được kết nối ở các phase tiếp theo
    (void)dt;
}

void Game::render() {
    // Màu xanh da trời đặc trưng của Flappy Bird (RGB: 113, 197, 207)
    m_window.clear(sf::Color(113, 197, 207));

    // Phase 2: Render buffer (sẽ render các Entity & UI ở các phase tiếp theo)

    m_window.display();
}

} // namespace core
