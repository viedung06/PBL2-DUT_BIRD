#include "systems/PowerUp.hpp"
#include <algorithm>
#include <random>

namespace systems {

namespace {
std::random_device g_rdPower;
std::mt19937 g_rngPower(g_rdPower());
std::uniform_real_distribution<float> g_dist01(0.0f, 1.0f);
}

// 1. Cập nhật vị trí hộp quà di chuyển trôi sang trái (cùng tốc độ với ống nước)
void GiftBox::update(float dt, float speed) {
    position.x -= speed * dt;
    hitbox.position = position;
}

// 2. Vẽ hộp quà lên màn hình
void GiftBox::draw(sf::RenderTarget& target) const {
    sf::RectangleShape rect(sf::Vector2f(30.0f, 30.0f));
    rect.setPosition(position);
    rect.setFillColor(sf::Color::Yellow);
    target.draw(rect);
}

// --- Khởi tạo PowerUpSystem ---
PowerUpSystem::PowerUpSystem() {
}

void PowerUpSystem::update(float dt, entities::Bird& bird, datastructures::my_queue<std::unique_ptr<entities::Pipe>>& pipes) {
    // --- 1. TỰ ĐỘNG SINH HỘP QUÀ KHI CÓ ỐNG MỚI ---
    pipes.forEach([this](std::unique_ptr<entities::Pipe>& pipePtr) {
        if (pipePtr && !pipePtr->hasSpawnedGift && pipePtr->getX() >= GameConfig::WINDOW_WIDTH - 5.0f) {
            pipePtr->hasSpawnedGift = true;

            // Tỉ lệ ngẫu nhiên xuất hiện hộp quà dùng <random>
            if (g_dist01(g_rngPower) < GameConfig::Rate_spawn_giftbox) {
                spawnGiftBox(pipePtr->getX(), pipePtr->getGapY());
            }
        }
    });

    // --- Cập nhật vị trí các hộp quà đang có ---
    for (auto& box : m_giftBoxes) {
        box.update(dt, GameConfig::PIPE_SPEED); 
    }

    // Xóa hộp quà khi bay ra khỏi màn hình hoặc đã bị nhặt
    m_giftBoxes.erase(
        std::remove_if(m_giftBoxes.begin(), m_giftBoxes.end(), [](const GiftBox& box) {
            return box.position.x + 30.0f < 0.0f || box.collected;
        }),
        m_giftBoxes.end()
    );

    // --- Quản lý thời gian khiên ---
    if (m_shieldTimer > 0.0f) {
        m_shieldTimer -= dt;
        if (m_shieldTimer <= 0.0f) {
            m_shieldTimer = 0.0f;
            bird.setHasShield(false);
        }
    }
}

void PowerUpSystem::draw(sf::RenderTarget& target) const {
    for (const auto& box : m_giftBoxes) {
        if (!box.collected) {
            box.draw(target);
        }
    }
}

void PowerUpSystem::spawnGiftBox(float x, float y) {
    GiftBox box;
    box.position = sf::Vector2f(x, y);
    box.hitbox = sf::FloatRect({x, y}, {30.0f, 30.0f});
    box.type = PowerUpType::Shield;
    box.collected = false;
    
    m_giftBoxes.push_back(box);
}

void PowerUpSystem::checkCollisions(entities::Bird& bird) {
    sf::FloatRect birdHitbox = bird.getHitbox();

    for (auto& box : m_giftBoxes) {
        if (!box.collected && birdHitbox.findIntersection(box.hitbox).has_value()) {
            box.collected = true;
            bird.setHasShield(true);
            m_shieldTimer = 10.0f;
        }
    }
}

void PowerUpSystem::reset() {
    m_giftBoxes.clear();
    m_shieldTimer = 0.0f;
    m_speedBoostTimer = 0.0f;
    m_enlargeTimer = 0.0f;
}

} // namespace systems
