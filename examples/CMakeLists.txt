cmake_minimum_required(VERSION 3.16)
project(TraderBacktestTest)  # 修改项目名称为 TraderBacktestTest

# 设置使用 UTF-8 编码
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加测试可执行文件
add_executable(test main.cpp)

# 链接 TraderBacktestLib 库到测试可执行文件
target_link_libraries(test TraderBacktestLib)

# 设置包含目录，包括主项目库的包含目录
target_include_directories(test PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)

# 将 ta-lib.dll 复制到可执行文件的输出目录
add_custom_command(TARGET test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:ta_lib>
        $<TARGET_FILE_DIR:test>
)
