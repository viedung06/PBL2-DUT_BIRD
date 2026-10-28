#include "systems/PipeSpawn.hpp"
#include <random>

namespace systems {

namespace {
// Bộ sinh số ngẫu nhiên chuẩn C++17
std::random_device g_rd;
std::mt19937 g_rng(g_rd());
}

PipeSpawnerSystem::PipeSpawnerSystem(float spawnInterval)
    : m_spawnInterval(spawnInterval),
      m_spawnTimer(0.0f),
      m_isPaused(false) {
}

void PipeSpawnerSystem::update(float dt) {
    if (m_isPaused) {
        return;
    }

    // 1. Tích lũy thời gian và tự động sinh cặp ống mới
    m_spawnTimer += dt;
    if (m_spawnTimer >= m_spawnInterval) {
        m_spawnTimer = 0.0f;
        spawnPipe();
    }

    // 2. Di chuyển các cặp ống sang bên trái
    m_pipeQueue.forEach([dt](std::unique_ptr<entities::Pipe>& pipe) {
        if (pipe) {
            pipe->update(dt);
        }
    });

    // 3. Tự động thu hồi và xóa các ống đã trôi hoàn toàn ra khỏi màn hình bên trái
    // Ống trôi theo thứ tự FIFO: Ống nào vào trước sẽ ra khỏi màn hình trước
    while (!m_pipeQueue.empty() && m_pipeQueue.front() && m_pipeQueue.front()->isOffScreen()) {
        m_pipeQueue.pop(); // Tự động gọi destructor giải phóng Pipe
    }
}

void PipeSpawnerSystem::spawnPipe() {
    // Tọa độ xuất hiện bên ngoài mép phải màn hình
    const float startX = static_cast<float>(GameConfig::WINDOW_WIDTH) + 20.0f;

    // Sinh độ cao khe hở gapY ngẫu nhiên trong vùng an toàn bằng <random>
    std::uniform_real_distribution<float> dist(GameConfig::PIPE_MIN_GAP_Y, GameConfig::PIPE_MAX_GAP_Y);
    const float gapY = dist(g_rng);

    auto newPipe = std::make_unique<entities::Pipe>(startX, gapY, GameConfig::PIPE_GAP, GameConfig::PIPE_WIDTH);

    // Gán texture nếu đã nạp sẵn
    if (m_topTexture != nullptr && m_bottomTexture != nullptr) {
        newPipe->setTexture(*m_topTexture, *m_bottomTexture);
    }

    m_pipeQueue.push(std::move(newPipe));
}

void PipeSpawnerSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    m_pipeQueue.forEach([&target, states](const std::unique_ptr<entities::Pipe>& pipe) {
        if (pipe) {
            target.draw(*pipe, states);
        }
    });
}

bool PipeSpawnerSystem::checkPassedPipe(float birdX) {
    bool passedAny = false;
    m_pipeQueue.forEach([birdX, &passedAny](std::unique_ptr<entities::Pipe>& pipe) {
        if (pipe && !pipe->isPassed() && birdX > pipe->getX() + pipe->getWidth()) {
            pipe->setPassed(true);
            passedAny = true;
        }
    });
    return passedAny;
}

void PipeSpawnerSystem::setPaused(bool paused) {
    m_isPaused = paused;
}

bool PipeSpawnerSystem::isPaused() const {
    return m_isPaused;
}

void PipeSpawnerSystem::reset() {
    m_pipeQueue.clear();
    m_spawnTimer = 0.0f;
    m_isPaused = false;
}

void PipeSpawnerSystem::setTextures(const sf::Texture* topTex, const sf::Texture* bottomTex) {
    m_topTexture = topTex;
    m_bottomTexture = bottomTex;
    m_pipeQueue.forEach([this](std::unique_ptr<entities::Pipe>& pipe) {
        if (pipe && m_topTexture != nullptr && m_bottomTexture != nullptr) {
            pipe->setTexture(*m_topTexture, *m_bottomTexture);
        }
    });
}

size_t PipeSpawnerSystem::getPipeCount() const {
    return m_pipeQueue.size();
}

datastructures::my_queue<std::unique_ptr<entities::Pipe>>& PipeSpawnerSystem::getPipes() {
    return m_pipeQueue;
}

const datastructures::my_queue<std::unique_ptr<entities::Pipe>>& PipeSpawnerSystem::getPipes() const {
    return m_pipeQueue;
}

} // namespace systems
