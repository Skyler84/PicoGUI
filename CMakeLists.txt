cmake_minimum_required(VERSION 3.12)

# Change your executable name to something creative!
set(PROJECT_NAME "LibGUI")

include(pimoroni_pico_import.cmake)
include(pico_sdk_import.cmake)

# Gooey boilerplate
project(${PROJECT_NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(examples)

set(NAME gui)

add_library(${NAME} STATIC
    ${CMAKE_CURRENT_LIST_DIR}/src/drawable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/view.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/view_controller.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/divider_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/list_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/text_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/qrcode_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/button.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/src/drivers/led.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/rp2040/sio_pin.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/rp2040/gpio_pin.cpp
)

# target_include_directories(${NAME}_headers INTERFACE
#     ${CMAKE_CURRENT_LIST_DIR}/include
# )

# Include required libraries
# This assumes `pimoroni-pico` is stored alongside your project
target_include_directories(${NAME} PUBLIC ${PIMORONI_PICO_PATH}/common/pimoroni_i2c)
target_include_directories(${NAME} PUBLIC ${PIMORONI_PICO_PATH}/common/pimoroni_bus)
target_include_directories(${NAME} PUBLIC ${PIMORONI_PICO_PATH}/libraries/bitmap_fonts/bitmap_fonts)
target_include_directories(${NAME} PUBLIC ${PIMORONI_PICO_PATH}/libraries/hershey_fonts/hershey_fonts)
target_include_directories(${NAME} PUBLIC ${PIMORONI_PICO_PATH}/libraries/pico_explorer/pico_explorer)
target_include_directories(${NAME} PUBLIC ${PIMORONI_PICO_PATH}/libraries/pico_display/pico_display)
target_include_directories(${NAME} PUBLIC ${PIMORONI_PICO_PATH}/drivers/rgbled/rgbled)
target_include_directories(${NAME} PUBLIC ${PIMORONI_PICO_PATH}/drivers/button/button)
# include(${PIMORONI_PICO_PATH}/drivers/st7789/st7789)

target_link_libraries(${NAME} INTERFACE
    pico_display
    pico_explorer
)