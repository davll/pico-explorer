# ST7789 Driver

add_library(st7789 INTERFACE)

target_include_directories(st7789 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include)

target_sources(st7789 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/source/st7789.c)

target_link_libraries(st7789 INTERFACE
    pico_stdlib
    hardware_spi
    hardware_dma)
