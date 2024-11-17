cmake_minimum_required(VERSION 3.26)
project(backtest)

set(CMAKE_CXX_STANDARD 17)
# 查找操作系统类型并设置不同的路径
if (APPLE)
    # macOS 设置
    set(BOOST_ROOT "/usr/local/opt/boost")  # Homebrew 安装的 Boost 默认路径
    set(Boost_USE_STATIC_LIBS ON)  # 是否使用静态库
elseif(WIN32)
    # Windows 设置
    set(BOOST_ROOT "C:/boost")
    set(Boost_DEBUG ON)
    set(Boost_USE_STATIC_LIBS ON)
endif()


# 设置 TA-Lib 的路径
set(TA_LIB_ROOT "${CMAKE_SOURCE_DIR}/external/ta-lib")

# 查找并引入 Boost 库
find_package(Boost 1.86 REQUIRED COMPONENTS system filesystem)

# 检查 Boost 是否找到
if (Boost_FOUND)
    message(STATUS "Boost found: ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost not found!")
endif()

# 添加 TA-Lib 子目录
add_subdirectory(${TA_LIB_ROOT} ${CMAKE_BINARY_DIR}/ta-lib)

# 添加项目 include 目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 自动匹配源文件和头文件
file(GLOB_RECURSE TRADERBACKTEST_SRC "${PROJECT_SOURCE_DIR}/src/*.cpp")

# 创建静态库 backtest
add_library(backtest STATIC ${TRADERBACKTEST_SRC})

# 链接 Boost 库
target_link_libraries(backtest PRIVATE ${Boost_LIBRARIES})

# 链接 TA-Lib 库
target_link_libraries(backtest PUBLIC ta_lib)

# 包含 TA-Lib 的头文件
target_include_directories(backtest PRIVATE ${TA_LIB_ROOT}/include)

# 将项目的 include 目录公开给其他目标
target_include_directories(backtest PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 添加 src/examples 子目录
add_subdirectory(src/examples)
