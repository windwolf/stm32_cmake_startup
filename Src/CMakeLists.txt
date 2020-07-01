

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Drivers)



file(GLOB_RECURSE SRC_SOURCES_PATH ${PROJECT_ROOT}/Core/Src/*.* ${PROJECT_ROOT}/Startup/*.*)

set(ELF_TARGET ${PROJECT_NAME}.elf)



add_executable(${ELF_TARGET} ${SRC_SOURCES_PATH})

target_compile_definitions(${ELF_TARGET} PUBLIC -D${MCU_LINE})
target_link_libraries(${ELF_TARGET} PUBLIC CMSIS STM32F1xx_HAL_Driver -lc -lm -lnosys)
target_link_libraries(${ELF_TARGET} PRIVATE -T${LINKER_SCRIPT} ${EXTRA_LINK_FLAGS})

target_include_directories(${ELF_TARGET}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
    ${PROJECT_SOURCE_DIR}/LibConf
)