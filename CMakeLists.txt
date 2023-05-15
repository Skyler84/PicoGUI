cmake_minimum_required(VERSION 3.12)

# Change your executable name to something creative!
set(PROJECT_NAME "LibGUI")

set(PICO_SDK_PATH /home/skyler/Software/pico/pico-sdk)
include(pimoroni_pico_import.cmake)
include(pico_sdk_import.cmake)

# Gooey boilerplate
project(${PROJECT_NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(examples)

set(NAME gui)

pico_add_library(${NAME})
target_sources(${NAME} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/drawable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/view.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/view_controller.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/divider_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/list_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/text_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/qrcode_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/button.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/rp2040/sio_pin.cpp
)

target_include_directories(${NAME}_headers INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# Include required libraries
# This assumes `pimoroni-pico` is stored alongside your project
include(common/pimoroni_i2c)
include(common/pimoroni_bus)
include(libraries/bitmap_fonts/bitmap_fonts)
include(libraries/hershey_fonts/hershey_fonts)
include(libraries/pico_explorer/pico_explorer)
include(libraries/pico_display/pico_display)
include(drivers/rgbled/rgbled)
include(drivers/button/button)
# include(drivers/st7789/st7789)

target_link_libraries(${NAME} INTERFACE
    pico_display
    pico_explorer
)