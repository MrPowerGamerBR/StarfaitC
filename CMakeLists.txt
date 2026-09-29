cmake_minimum_required(VERSION 4.2)
project(Starfait C)

set(CMAKE_C_STANDARD 23)

file(GLOB SOURCES "src/*.c" "src/*.h" "src/wad/*.c" "src/wad/*.h" "src/vm/*.c" "src/vm/*.h")

set(ENABLE_ASAN_DEFAULT ON)

add_executable(Starfait ${SOURCES})
target_compile_options(Starfait PRIVATE -Wall -Wextra -Werror=switch -Werror=switch-enum)