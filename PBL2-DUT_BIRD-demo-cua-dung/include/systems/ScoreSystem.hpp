#ifndef SCORE_SYSTEM_HPP
#define SCORE_SYSTEM_HPP

#include <string>

namespace systems {

/**
 * @brief Hệ thống quản lý điểm số và lưu trữ kỷ lục (High Score) ra file
 */
class ScoreSystem {
public:
    explicit ScoreSystem(std::string highScoreFilePath = "assets/highscores.txt");
    ~ScoreSystem() = default;

    void addScore(int points = 1);
    int getCurrentScore() const noexcept;
    int getHighScore() const noexcept;
    void resetCurrentScore() noexcept;

    void loadHighScore();
    void saveHighScore() const;

private:
    int m_currentScore{0};
    int m_highScore{0};
    std::string m_filePath;
};

} // namespace systems

#endif // SCORE_SYSTEM_HPP

