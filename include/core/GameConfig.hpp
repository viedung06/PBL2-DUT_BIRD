#pragma once
#include <string_view>

namespace GameConfig {
    // Window settings
    inline constexpr unsigned int WINDOW_WIDTH = 480;
    inline constexpr unsigned int WINDOW_HEIGHT = 640;
    inline constexpr std::string_view WINDOW_TITLE = "Flappy Bird SFML";
    inline constexpr unsigned int TARGET_FPS = 60;
    inline constexpr bool VSYNC_ENABLED = false;

    // Physics settings (chuẩn bị cho Phase 3)
    inline constexpr float GRAVITY = 980.0f;
    inline constexpr float JUMP_VELOCITY = -320.0f;

    // Pipe settings (chuẩn bị cho Phase 4)
    inline constexpr float PIPE_SPEED = 180.0f;
    inline constexpr float PIPE_SPAWN_INTERVAL = 1.6f;
    inline constexpr float PIPE_GAP = 140.0f;
}
