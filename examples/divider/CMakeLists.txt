set(NAME divider)

add_executable(${NAME}
    main.cpp
)

target_link_libraries(${NAME} PRIVATE
    gui 
    pico_stdio_usb
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})

pico_enable_stdio_uart(${NAME} 0)
pico_enable_stdio_semihosting(${NAME} 0)
message(${CMAKE_BUILD_TYPE})
string(TOLOWER ${CMAKE_BUILD_TYPE} build_type)
if (build_type STREQUAL debug)
  pico_enable_stdio_usb(${NAME} 1)
else()
  pico_enable_stdio_usb(${NAME} 0)
endif()
