

file(GLOB_RECURSE SERIAL_PORT_SRC Src/*.c)


add_library(SerialPort ${SERIAL_PORT_SRC})
add_library("windwolf::SerialPort" ALIAS SerialPort)


target_link_libraries(SerialPort
    PUBLIC
        azrtos::threadx
        windwolf::IoBuffer
        windwolf::CircleQueue)

target_include_directories(SerialPort
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Src)
