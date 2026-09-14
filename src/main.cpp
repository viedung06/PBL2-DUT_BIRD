#include <SFML/Graphics.hpp>
#include <string>
#include "core/GameConfig.hpp"
#include "entities/Bird.hpp"
#include "entities/Ground.hpp"
#include "entities/Pipe.hpp"

int main() {
    // Khởi tạo cửa sổ game theo chuẩn SFML 3 (VideoMode nhận sf::Vector2u)
    sf::RenderWindow window(
        sf::VideoMode({GameConfig::WINDOW_WIDTH, GameConfig::WINDOW_HEIGHT}),
        std::string(GameConfig::WINDOW_TITLE),
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(GameConfig::FPS);

    entities::Bird bird;
    entities::Ground ground;
    entities::Pipe pipe(300.0f, 250.0f);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        // Xử lý sự kiện theo chuẩn SFML 3 (pollEvent trả về std::optional<sf::Event>)
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            bird.handleEvent(*event);
        }

        // Cập nhật các thực thể
        bird.update(dt);
        ground.update(dt);
        pipe.update(dt);

        // Nếu ống trôi khỏi màn hình thì lặp lại từ bên phải
        if (pipe.isOffScreen()) {
            pipe.setX(static_cast<float>(GameConfig::WINDOW_WIDTH) + 50.0f);
        }

        // Kiểm tra va chạm mặt đất (SFML 3: findIntersection trả về std::optional)
        if (bird.getHitbox().findIntersection(ground.getHitbox()).has_value()) {
            bird.setDead(true);
        }

        // Kiểm tra va chạm với cặp ống
        if (bird.getHitbox().findIntersection(pipe.getTopHitbox()).has_value() ||
            bird.getHitbox().findIntersection(pipe.getBottomHitbox()).has_value()) {
            bird.setDead(true);
        }

        // Vẽ màn hình
        window.clear(sf::Color(113, 197, 207)); // Màu nền trời Flappy Bird
        window.draw(pipe);
        window.draw(ground);
        window.draw(bird);
        window.display();
    }

    return 0;
}
