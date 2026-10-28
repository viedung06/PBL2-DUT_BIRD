#include "states/StateMachine.hpp"

namespace states {

StateMachine::~StateMachine() {
    clear();
}

void StateMachine::changeState(std::unique_ptr<GameState> newState) {
    m_pendingAction = PendingAction::Change;
    m_pendingState = std::move(newState);
}

void StateMachine::pushState(std::unique_ptr<GameState> newState) {
    m_pendingAction = PendingAction::Push;
    m_pendingState = std::move(newState);
}

void StateMachine::popState() {
    m_pendingAction = PendingAction::Pop;
}

void StateMachine::applyPendingChanges() {
    if (m_pendingAction == PendingAction::None) {
        return;
    }

    switch (m_pendingAction) {
        case PendingAction::Change:
            if (!m_states.empty()) {
                m_states.back()->exit();
                m_states.pop_back();
            }
            if (m_pendingState) {
                m_states.push_back(std::move(m_pendingState));
                m_states.back()->enter();
            }
            break;

        case PendingAction::Push:
            if (m_pendingState) {
                m_states.push_back(std::move(m_pendingState));
                m_states.back()->enter();
            }
            break;

        case PendingAction::Pop:
            if (!m_states.empty()) {
                m_states.back()->exit();
                m_states.pop_back();
            }
            break;

        default:
            break;
    }

    m_pendingAction = PendingAction::None;
    m_pendingState = nullptr;
}

void StateMachine::handleInput(const sf::Event& event) {
    if (!m_states.empty()) {
        m_states.back()->handleInput(event);
    }
}

void StateMachine::update(float dt) {
    if (!m_states.empty()) {
        m_states.back()->update(dt);
    }
}

void StateMachine::render(sf::RenderTarget& target) {
    // Nếu có nhiều state (ví dụ PlayState bên dưới PauseState), render từ đáy lên đỉnh
    for (const auto& state : m_states) {
        state->render(target);
    }
}

bool StateMachine::isEmpty() const noexcept {
    return m_states.empty();
}

void StateMachine::clear() {
    while (!m_states.empty()) {
        m_states.back()->exit();
        m_states.pop_back();
    }
    m_pendingAction = PendingAction::None;
    m_pendingState = nullptr;
}

} // namespace states

