

file(GLOB_RECURSE CIRCLE_QUEUE_SRC Src/*.c)


add_library(CircleQueue ${CIRCLE_QUEUE_SRC})
add_library("windwolf::CircleQueue" ALIAS CircleQueue)


target_link_libraries(CircleQueue
    PUBLIC
        azrtos::threadx)

target_include_directories(CircleQueue
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Src)
