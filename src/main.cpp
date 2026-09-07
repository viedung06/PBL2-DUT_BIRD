#include <SFML/Graphics.hpp>
#include "../include/core/Game.hpp"
#include <iostream>
#include <exception>

int main() {
    try {
        core::Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal Exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown Fatal Error!" << std::endl;
        return 1;
    }

    return 0;
}
