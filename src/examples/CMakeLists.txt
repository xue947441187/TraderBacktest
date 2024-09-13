cmake_minimum_required(VERSION 3.16)
project(TraderBacktestTest)

# 设置使用 UTF-8 编码（仅适用于 MSVC 编译器）
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置 Boost 环境
set(BOOST_INSTALL_DIR ${CMAKE_BINARY_DIR}/boost)  # 替换为实际的 Boost 安装路径
set(Boost_INCLUDE_DIR ${BOOST_INSTALL_DIR}/include)
set(Boost_LIBRARY_DIR ${BOOST_INSTALL_DIR}/lib)

if(NOT EXISTS ${BOOST_LIBRARY_DIR})
    message(STATUS "Boost not found, building Boost...")
else()
    find_package(Boost REQUIRED COMPONENTS system filesystem HINTS ${BOOST_INSTALL_DIR})
    include_directories(${Boost_INCLUDE_DIR})
    link_directories(${Boost_LIBRARY_DIR})
endif()

# 设置 TraderBacktestLib 的路径和库链接
set(TRADERBACKTESTLIB_DIR ${CMAKE_BINARY_DIR})  # 设置 TraderBacktestLib 输出目录
link_directories(${TRADERBACKTESTLIB_DIR})  # 将构建目录添加到库路径中

# 设置 TA-Lib 环境
set(TA_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/path/to/ta-lib)  # 替换为实际的 TA-Lib 路径

# 查找所有的 .cpp 文件
file(GLOB_RECURSE CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# 添加数据文件到输出目录
configure_file(AAPL.csv aapl.csv COPYONLY)

# 为每个 .cpp 文件创建一个可执行程序并链接库
foreach(CPP_FILE ${CPP_FILES})
    # 获取文件名（不包括路径和扩展名）
    get_filename_component(FILE_NAME ${CPP_FILE} NAME_WE)

    # 添加一个可执行目标
    add_executable(${FILE_NAME} ${CPP_FILE})

    # 链接 TraderBacktestLib 库到每个可执行程序
    target_link_libraries(${FILE_NAME} TraderBacktestLib ${Boost_LIBRARIES} ta_lib)

    # 设置包含目录
    target_include_directories(${FILE_NAME} PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS} ${TA_LIB_DIR}/include)

    # 将 ta-lib.dll 复制到每个可执行文件的输出目录
    add_custom_command(TARGET ${FILE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:ta_lib>
            $<TARGET_FILE_DIR:${FILE_NAME}>
    )
endforeach()

message(STATUS "Subproject setup finished.")
