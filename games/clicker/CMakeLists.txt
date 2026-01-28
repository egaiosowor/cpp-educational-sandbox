cmake_minimum_required(VERSION 3.11)
project(MonsterClicker)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch Raylib
include(FetchContent)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the provided examples
set(BUILD_GAMES OFF CACHE BOOL "" FORCE) # or games
FetchContent_MakeAvailable(raylib)

# Create executable
add_executable(MonsterClicker main.cpp)
target_link_libraries(MonsterClicker PRIVATE raylib)
