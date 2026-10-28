#pragma once

#include <SFML/Graphics.hpp>
#include "GameConfig.hpp"
#include "states/StateMachine.hpp"

namespace core {

class Game {
public:
    Game();
    ~Game() = default;

    // Ngăn chặn copy để bảo toàn tài nguyên window và context
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    // Vòng lặp chính của trò chơi
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

private:
    sf::RenderWindow m_window;
    sf::Clock m_clock;
    bool m_isRunning{true};

    // StateMachine quản lý toàn bộ các màn chơi theo Kiến trúc phân lớp
    states::StateMachine m_stateMachine;
};

} // namespace core
