#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <SFML/Graphics.hpp>

namespace states {

class StateMachine;

/**
 * @brief Interface trừu tượng cho mọi trạng thái trong trò chơi
 */
class GameState {
public:
    virtual ~GameState() = default;

    virtual void enter() {}
    virtual void exit() {}

    virtual void handleInput(const sf::Event& event) = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderTarget& target) = 0;
};

} // namespace states

#endif // GAME_STATE_HPP

