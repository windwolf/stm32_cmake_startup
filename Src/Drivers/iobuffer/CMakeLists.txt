

file(GLOB_RECURSE IOBUFFER_SRC Src/*.c)


add_library(IoBuffer ${IOBUFFER_SRC})
add_library("windwolf::IoBuffer" ALIAS IoBuffer)


target_link_libraries(IoBuffer
    PUBLIC
        azrtos::threadx)

target_include_directories(IoBuffer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Src)
