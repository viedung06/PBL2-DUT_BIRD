#ifndef PIPE_SPAWNER_SYSTEM_HPP
#define PIPE_SPAWNER_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "entities/Pipe.hpp"
#include "core/GameConfig.hpp"
#include "datastructures/my_queue.hpp"

namespace systems {

/**
 * @brief Hệ thống quản lý vòng đời của các chướng ngại vật Pipe (SFML 3.x / 3.1).
 * 
 * - Sử dụng cấu trúc dữ liệu my_queue tự viết để quản lý các cặp ống vào/ra theo chuẩn FIFO.
 * - Tự động tạo cặp ống sau mỗi khoảng thời gian m_spawnInterval.
 * - Tự động giải phóng các ống đã trôi hoàn toàn ra khỏi màn hình bên trái (Zero Memory Leak).
 */
class PipeSpawnerSystem {
public:
    explicit PipeSpawnerSystem(float spawnInterval = GameConfig::PIPE_SPAWN_INTERVAL);
    ~PipeSpawnerSystem() = default;

    PipeSpawnerSystem(const PipeSpawnerSystem&) = delete;
    PipeSpawnerSystem& operator=(const PipeSpawnerSystem&) = delete;

    PipeSpawnerSystem(PipeSpawnerSystem&&) noexcept = default;
    PipeSpawnerSystem& operator=(PipeSpawnerSystem&&) noexcept = default;

    // Cập nhật timer, sinh ống mới, di chuyển ống và xóa ống ra khỏi màn hình
    void update(float dt);

    // Vẽ toàn bộ các cặp ống lên target
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

    // Kiểm tra nếu chim vừa vượt qua một ống chưa được tính điểm (trả về true để cộng điểm)
    bool checkPassedPipe(float birdX);

    // Điều khiển trạng thái
    void setPaused(bool paused);
    bool isPaused() const;
    void reset();

    // Thiết lập Texture cho các ống sinh ra trong tương lai (nếu có asset ảnh)
    void setTextures(const sf::Texture* topTex, const sf::Texture* bottomTex);

    // Truy xuất số lượng ống đang hoạt động
    size_t getPipeCount() const;

    // Truy xuất hàng đợi ống để CollisionManager / PowerUp kiểm tra va chạm
    datastructures::my_queue<std::unique_ptr<entities::Pipe>>& getPipes();
    const datastructures::my_queue<std::unique_ptr<entities::Pipe>>& getPipes() const;

private:
    void spawnPipe();

private:
    datastructures::my_queue<std::unique_ptr<entities::Pipe>> m_pipeQueue;
    float m_spawnInterval{GameConfig::PIPE_SPAWN_INTERVAL};
    float m_spawnTimer{0.0f};
    bool m_isPaused{false};

    const sf::Texture* m_topTexture{nullptr};
    const sf::Texture* m_bottomTexture{nullptr};
};

} // namespace systems

#endif // PIPE_SPAWNER_SYSTEM_HPP
