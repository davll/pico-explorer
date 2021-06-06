cmake_minimum_required(VERSION 3.16)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pico-explorer C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(st7789)
add_subdirectory(explorer)

add_executable(firmware main.c)
target_link_libraries(firmware
    pico_stdlib
    explorer)
pico_add_extra_outputs(firmware)
