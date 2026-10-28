#include "systems/ScoreSystem.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

namespace systems {

ScoreSystem::ScoreSystem(std::string highScoreFilePath)
    : m_filePath(std::move(highScoreFilePath)) {
    loadHighScore();
}

void ScoreSystem::addScore(int points) {
    m_currentScore += points;
    if (m_currentScore > m_highScore) {
        m_highScore = m_currentScore;
        saveHighScore();
    }
}

int ScoreSystem::getCurrentScore() const noexcept {
    return m_currentScore;
}

int ScoreSystem::getHighScore() const noexcept {
    return m_highScore;
}

void ScoreSystem::resetCurrentScore() noexcept {
    m_currentScore = 0;
}

void ScoreSystem::loadHighScore() {
    std::ifstream file(m_filePath);
    if (file.is_open()) {
        int score = 0;
        if (file >> score) {
            m_highScore = std::max(0, score);
        }
        file.close();
    } else {
        m_highScore = 0;
        saveHighScore(); // Tạo file nếu chưa có
    }
}

void ScoreSystem::saveHighScore() const {
    std::ofstream file(m_filePath, std::ios::trunc);
    if (file.is_open()) {
        file << m_highScore << "\n";
        file.close();
    }
}

} // namespace systems

