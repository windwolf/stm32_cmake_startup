

file(GLOB_RECURSE CIRCLE_BUFFER_SRC Src/*.c)


add_library(CircleBuffer ${CIRCLE_BUFFER_SRC})
add_library("windwolf::CircleBuffer" ALIAS CircleBuffer)


target_link_libraries(CircleBuffer
    PUBLIC
        azrtos::threadx)

target_include_directories(CircleBuffer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Src)
