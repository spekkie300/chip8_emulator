cmake_minimum_required(VERSION 3.16)

project(Chip8_emulator)

find_package(SDL3 REQUIRED CONFIG)

add_executable(chip8 main.c)

target_link_libraries(chip8 PRIVATE SDL3::SDL3)
