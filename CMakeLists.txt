cmake_minimum_required(VERSION 3.16)
project(FlappyBird LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Tránh dùng cờ -isystem cho imported targets trên MSYS2/MinGW để không làm gãy include_next
set(CMAKE_NO_SYSTEM_FROM_IMPORTED ON)

# Thêm cờ cảnh báo để đảm bảo chất lượng code
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include thư mục header
include_directories(include)

# Gom tất cả các file mã nguồn
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

# Tìm kiếm thư viện SFML
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# Tạo file thực thi
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio
)

# Tự động copy các DLL cần thiết (SFML, MinGW runtime) vào thư mục build khi biên dịch
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()
