
# If the user provided an override, copy it to the custom directory
if (NOT STM32_USER_INCLUDE_DIR)
    message(FATAL_ERROR "Error: 请在project-settings.cmake中指定 STM32_USER_INCLUDE_DIR. 这个目录必须包含stm32_assert.h和stm32XXXX_hal_conf.h")
endif()    



file(GLOB_RECURSE STM32F1_HAL_SRC Src/*_hal.c Src/*_hal_*.c)
file(GLOB_RECURSE STM32F1_LL_SRC Src/*_ll_*.c)

if(USE_HAL_LIB)
    set(STM32F1_LIB_SRC ${STM32F1_HAL_SRC} ${STM32F1_LL_SRC})
else()
    set(STM32F1_LIB_SRC ${STM32F1_LL_SRC})
endif()

add_library(STM32F1xx_HAL_Driver ${STM32F1_LIB_SRC})
if(USE_LL_LIB)
    target_compile_definitions(STM32F1xx_HAL_Driver
        PUBLIC -DUSE_FULL_LL_DRIVER)
endif()
if(USE_HAL_LIB)
    target_compile_definitions(STM32F1xx_HAL_Driver
        PUBLIC -DUSE_HAL_DRIVER)
endif()
target_link_libraries(STM32F1xx_HAL_Driver
    PUBLIC
        CMSIS)
target_include_directories(STM32F1xx_HAL_Driver
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
        ${STM32_USER_INCLUDE_DIR}
    PRIVATE
        Src)
