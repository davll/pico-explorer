# Pimoroni Pico Explorer

add_library(explorer INTERFACE)

target_include_directories(explorer INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include)

target_sources(explorer INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/source/explorer.c)

target_link_libraries(explorer INTERFACE
    pico_stdlib
    hardware_adc
    hardware_pwm
    hardware_i2c
    hardware_spi
    st7789)
