cmake_minimum_required(VERSION 4.2)
project(Starfait C)

set(CMAKE_C_STANDARD 23)

file(GLOB SOURCES "src/*.c" "src/*.h" "src/wad/*.c" "src/wad/*.h" "src/vm/*.c" "src/vm/*.h")

set(ENABLE_ASAN ON)

add_executable(Starfait ${SOURCES})

target_compile_options(Starfait PRIVATE -Wall -Wextra -Werror -Wno-error=unused-parameter -Wno-error=unused-variable)

if(ENABLE_ASAN)
    target_compile_options(Starfait PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(Starfait PRIVATE -fsanitize=address)
endif()