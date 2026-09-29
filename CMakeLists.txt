# For reference:
# "target_include_directories" tells where the compiler to look
# "target_sources" tells that file must exist

cmake_minimum_required(VERSION 4.2)
project(Starfait C)

set(CMAKE_C_STANDARD 23)

set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(
        GENERATED_OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/generated/arraylist_uint32.h
        ${CMAKE_CURRENT_BINARY_DIR}/generated/arraylist_builtinfunction.h
        ${CMAKE_CURRENT_BINARY_DIR}/generated/arraylist_builtinvariable.h
        ${CMAKE_CURRENT_BINARY_DIR}/generated/arraylist_variable.h
        ${CMAKE_CURRENT_BINARY_DIR}/generated/arraylist_script.h
        ${CMAKE_CURRENT_BINARY_DIR}/generated/arraylist_function.h
        ${CMAKE_CURRENT_BINARY_DIR}/generated/arraylist_codeentry.h
        ${CMAKE_CURRENT_BINARY_DIR}/generated/arraylist_rvalue.h
        ${CMAKE_CURRENT_BINARY_DIR}/generated/arraylist_callframe.h
        ${CMAKE_CURRENT_BINARY_DIR}/generated/arraylist_string.h
)

file(GLOB SOURCES "src/*.c" "src/*.h" "src/wad/*.c" "src/wad/*.h" "src/vm/*.c" "src/vm/*.h")

set(ENABLE_ASAN ON)

add_executable(Starfait)
target_sources(Starfait PRIVATE ${SOURCES} ${GENERATED_OUTPUT})

target_compile_options(Starfait PRIVATE -Wall -Wextra -Werror -Wno-error=unused-parameter -Wno-error=unused-variable)
target_include_directories(Starfait PRIVATE ${CMAKE_SOURCE_DIR}/src ${GENERATED_DIR})

add_custom_command(OUTPUT ${GENERATED_OUTPUT} COMMAND python3 ${CMAKE_SOURCE_DIR}/scripts/gen.py ${CMAKE_SOURCE_DIR}/src/templates ${GENERATED_DIR} DEPENDS ${CMAKE_SOURCE_DIR}/scripts/gen.py ${CMAKE_SOURCE_DIR}/src/templates/arraylist.h)

if(ENABLE_ASAN)
    target_compile_options(Starfait PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(Starfait PRIVATE -fsanitize=address)
endif()