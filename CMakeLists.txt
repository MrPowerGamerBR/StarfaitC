cmake_minimum_required(VERSION 4.2)
project(Starfait C)

set(CMAKE_C_STANDARD 23)

file(GLOB SOURCES "src/*.c" "src/*.h" "src/wad/*.c" "src/wad/*.h")

set(ENABLE_ASAN_DEFAULT ON)

add_executable(Starfait ${SOURCES})
