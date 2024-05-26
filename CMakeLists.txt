cmake_minimum_required(VERSION 3.16)
project(TraderBacktest)

# 设置使用 UTF-8 编码
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置TALib子模块路径
set(TA_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/ta-lib)

# 添加TALib的子目录
add_subdirectory(${TA_LIB_DIR} ${CMAKE_BINARY_DIR}/ta-lib)
# 添加头文件目录
include_directories(${TA_LIB_DIR}/include)

# 自动匹配源文件和头文件
file(GLOB_RECURSE TRADERBACKTEST_SRC
        "library.cpp"
        "trading/*.cpp"
        "backtest/*.cpp"
        "indicators/*.cpp"
        "stocks/*.cpp"
        "include/*.h"
)

# 添加库
add_library(TraderBacktestLib ${TRADERBACKTEST_SRC})

# 添加包含目录
target_include_directories(TraderBacktestLib PRIVATE ${TA_LIB_DIR}/include)

# 链接TALib库到TraderBacktestLib
target_link_libraries(TraderBacktestLib ta_lib)

# 添加数据文件到输出目录
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/aapl.csv ${CMAKE_CURRENT_BINARY_DIR}/aapl.csv COPYONLY)

# 将 ta-lib.dll 复制到可执行文件的输出目录
add_custom_command(TARGET TraderBacktestLib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:ta_lib>
        $<TARGET_FILE_DIR:TraderBacktestLib>
)

# 添加examples子目录
add_subdirectory(examples)
