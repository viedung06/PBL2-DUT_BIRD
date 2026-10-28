#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <memory>
#include <vector>
#include "states/GameState.hpp"

namespace states {

/**
 * @brief Quản lý chuyển đổi các GameState theo mô hình ngăn xếp (Stack-based)
 * Hỗ trợ chuyển đổi hoãn lại (Pending Actions) để an toàn tuyệt đối khi chuyển State giữa frame.
 */
class StateMachine {
public:
    StateMachine() = default;
    ~StateMachine();

    StateMachine(const StateMachine&) = delete;
    StateMachine& operator=(const StateMachine&) = delete;

    // Yêu cầu đổi sang State mới (xóa State cũ)
    void changeState(std::unique_ptr<GameState> newState);

    // Đẩy thêm State lên trên đỉnh stack (ví dụ PauseState)
    void pushState(std::unique_ptr<GameState> newState);

    // Gỡ bỏ State đỉnh stack (ví dụ thoát PauseState quay lại PlayState)
    void popState();

    // Áp dụng các thay đổi trạng thái hoãn lại ở cuối frame
    void applyPendingChanges();

    // Chuyển tiếp các lời gọi vòng đời
    void handleInput(const sf::Event& event);
    void update(float dt);
    void render(sf::RenderTarget& target);

    bool isEmpty() const noexcept;
    void clear();

private:
    enum class PendingAction {
        None,
        Change,
        Push,
        Pop
    };

    std::vector<std::unique_ptr<GameState>> m_states;
    PendingAction m_pendingAction{PendingAction::None};
    std::unique_ptr<GameState> m_pendingState{nullptr};
};

} // namespace states

#endif // STATE_MACHINE_HPP

