# =================NOTE================================
# | 这个文件是所有cmake结构的组织者, 不要随意改动      |
# =================NOTE================================

# 防止启动检测
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)



# 导入目标文件定义
include(${CMAKE_CURRENT_SOURCE_DIR}/project-settings.cmake)


project(${PROJECT_NAME} C CXX ASM)



# CMAKE版本检查
cmake_minimum_required(VERSION 3.0.0)
message(STATUS "CMake version: ${CMAKE_VERSION}")

# 设置编译环境
set(CROSS_COMPILE_PREFIX arm-none-eabi)
include(${CMAKE_SOURCE_DIR}/cmake/gcc-arm-none-eabi.cmake)

message(STATUS "Building project: ${PROJECT_NAME}")


## 设置编译选项
set(MCU_FLAGS "${CPU} -mthumb ${FPU} ${FLOAT_ABI}")
set(LINK_FLAGS "${MCU_FLAGS} -Wl,--gc-sections")
set(EXTRA_LINK_FLAGS "-Wl,-Map=${PROJECT_NAME}.map,--cref,--no-warn-mismatch -specs=nano.specs -specs=nosys.specs")
include(${CMAKE_SOURCE_DIR}/cmake/stm32-gcc-flags.cmake)


# 设置BUILD_TYPE
if(NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
  message(STATUS "Build type: Debug")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
  message(STATUS "Build type: Release")
endif()

# 打印配置信息
message(STATUS "MCU_LINE: ${MCU_LINE}")
message(STATUS "MCU_NAME: ${MCU_NAME}")
message(STATUS "Specified C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Linker script: ${LINKER_SCRIPT}")
message(STATUS "Use LL library: ${USE_LL_LIB}")
message(STATUS "Use HAL library: ${USE_HAL_LIB}")
message(STATUS "Use Segger SystemView library: ${USE_SYSTEM_VIEW}")

## 版本信息
set(VERSION_MAJOR 0 CACHE STRING "Project major version number.")
set(VERSION_MINOR 1 CACHE STRING "Project minor version number.")
set(VERSION_PATCH 0 CACHE STRING "Project patch version number.")


if(USE_SYSTEM_VIEW)
  add_definitions("-DENABLE_SYSTEMVIEW")
endif()

# 外部库
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Lib)

# 项目
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Src)

## post build
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32-gcc-postbuild.cmake)
# include(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_ROOT}/stm32-gcc-jlink.cmake)
