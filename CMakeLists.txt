cmake_minimum_required(VERSION 3.16)
project(TraderBacktest)
cmake_policy(SET CMP0144 NEW)
# 设置使用 UTF-8 编码
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
message(STATUS "Runtime output directory: ${CMAKE_CURRENT_SOURCE_DIR}")
# 设置examples目录
set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/examples)

# 设置 Boost 源代码路径和安装路径
set(BOOST_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/boost)  # Boost 源码路径
set(BOOST_INSTALL_DIR ${CMAKE_BINARY_DIR}/boost)  # 安装路径

# 设置 Boost 环境
set(BOOST_BUILD_OPTIONS_DEBUG "link=static variant=debug  install --prefix=${BOOST_INSTALL_DIR} --without-python")
set(BOOST_BUILD_OPTIONS_RELEASE "link=static variant=release install --prefix=${BOOST_INSTALL_DIR} --without-python")

# 构建 Boost
message(STATUS "Building Boost...")

# 检查是否已经存在构建结果
function(check_file_exists file_path)
    if(EXISTS ${file_path})
        message(STATUS "File ${file_path} already exists. Skipping build.")
        set(RESULT TRUE PARENT_SCOPE)
    else()
        set(RESULT FALSE PARENT_SCOPE)
    endif()
endfunction()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(BOOST_BOOTSTRAP_CMD "${BOOST_SRC_DIR}/bootstrap.bat")
    # Windows 环境下的 b2 命令
    set(BOOST_BUILD_CMD_RELEASE "${BOOST_SRC_DIR}/b2.exe ${BOOST_BUILD_OPTIONS_RELEASE}")
    set(BOOST_BUILD_CMD_DEBUG "${BOOST_SRC_DIR}/b2.exe ${BOOST_BUILD_OPTIONS_DEBUG}")

    # 检查 Release 构建文件是否存在
    check_file_exists(${BOOST_INSTALL_DIR}/lib/release)
    if(NOT RESULT)
        # 创建 Release 构建的自定义命令
        add_custom_command(
                OUTPUT ${BOOST_INSTALL_DIR}/lib/release
                COMMAND cmd.exe /c "cd ${BOOST_SRC_DIR} && ${BOOST_BUILD_CMD_RELEASE}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Building Boost for Release"
                VERBATIM
        )
    endif()
    # 创建 bootstrap 的自定义命令
    add_custom_command(
            OUTPUT ${BOOST_SRC_DIR}/b2.exe
            COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Running Boost bootstrap.bat"
            COMMAND ${BOOST_BOOTSTRAP_CMD}
            WORKING_DIRECTORY ${BOOST_SRC_DIR}
            COMMENT "Bootstrapping Boost for Windows"
            VERBATIM
    )

    # 创建 Release 构建的自定义命令
    add_custom_command(
            OUTPUT ${BOOST_INSTALL_DIR}/lib/release
            COMMAND cmd.exe /c "cd ${BOOST_SRC_DIR} && ${BOOST_BUILD_CMD_RELEASE}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Building Boost for Release"
            VERBATIM
    )

    # 检查 Debug 构建文件是否存在
    check_file_exists(${BOOST_INSTALL_DIR}/lib/debug)
    if(NOT RESULT)
        # 创建 Debug 构建的自定义命令
        add_custom_command(
                OUTPUT ${BOOST_INSTALL_DIR}/lib/debug
                COMMAND cmd.exe /c "cd ${BOOST_SRC_DIR} && ${BOOST_BUILD_CMD_DEBUG}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Building Boost for Debug"
                VERBATIM
        )
    endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(BOOST_BOOTSTRAP_CMD "${BOOST_SRC_DIR}/bootstrap.sh")
    # 创建 bootstrap 的自定义命令
    add_custom_command(
            OUTPUT ${BOOST_SRC_DIR}/b2
            COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Running Boost bootstrap.sh"
            COMMAND bash ${BOOST_BOOTSTRAP_CMD}
            WORKING_DIRECTORY ${BOOST_SRC_DIR}
            COMMENT "Bootstrapping Boost for macOS/Linux"
            VERBATIM
    )
    # macOS 和 Linux 环境下的 b2 命令
    set(BOOST_BUILD_CMD_RELEASE "${BOOST_SRC_DIR}/b2 ${BOOST_BUILD_OPTIONS_RELEASE}")
    set(BOOST_BUILD_CMD_DEBUG "${BOOST_SRC_DIR}/b2 ${BOOST_BUILD_OPTIONS_DEBUG}")

    # 检查 Release 构建文件是否存在
    check_file_exists(${BOOST_INSTALL_DIR}/lib/release)
    if(NOT RESULT)
        # 创建 Release 构建的自定义命令
        add_custom_command(
                OUTPUT ${BOOST_INSTALL_DIR}/lib/release
                COMMAND bash -c "cd ${BOOST_SRC_DIR} && ${BOOST_BUILD_CMD_RELEASE}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Building Boost for Release"
                VERBATIM
        )
    endif()

    # 检查 Debug 构建文件是否存在
    check_file_exists(${BOOST_INSTALL_DIR}/lib/debug)
    if(NOT RESULT)
        # 创建 Debug 构建的自定义命令
        add_custom_command(
                OUTPUT ${BOOST_INSTALL_DIR}/lib/debug
                COMMAND bash -c "cd ${BOOST_SRC_DIR} && ${BOOST_BUILD_CMD_DEBUG}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Building Boost for Debug"
                VERBATIM
        )
    endif()

else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

# 添加自定义目标以确保 Boost 被构建
add_custom_target(build_boost_release ALL
        DEPENDS ${BOOST_INSTALL_DIR}/lib/release
)

add_custom_target(build_boost_debug ALL
        DEPENDS ${BOOST_INSTALL_DIR}/lib/debug
)

# 添加构建 Boost 的目标
add_custom_target(build_boost ALL
        DEPENDS build_boost_release build_boost_debug
)

message(STATUS "Boost setup finished.")




# 检查 Boost 是否已经构建
if(NOT EXISTS ${BOOST_INSTALL_DIR}/lib)
    message(STATUS "Boost not found, building Boost...")
else()
    message(STATUS "Boost found at ${BOOST_INSTALL_DIR}")
    # 设置 Boost 环境
    set(BOOST_ROOT ${BOOST_INSTALL_DIR})
    set(Boost_INCLUDE_DIR ${BOOST_INSTALL_DIR}/include)
    set(Boost_LIBRARY_DIR ${BOOST_INSTALL_DIR}/lib)

    # 查找 Boost 库
    find_package(Boost REQUIRED COMPONENTS system filesystem)

    # 添加 Boost 头文件目录
    include_directories(${Boost_INCLUDE_DIR})

    # 设置 Boost 的库目录
    link_directories(${Boost_LIBRARY_DIR})

endif()

# 设置yaml-cpp
set(YAML_CPP ${CMAKE_CURRENT_SOURCE_DIR}/external/yaml-cpp)
add_subdirectory(${YAML_CPP} ${CMAKE_BINARY_DIR}/yaml-cpp)
include_directories(${YAML_CPP}/include)

# 设置TALib子模块路径
set(TA_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/ta-lib)
# 添加 TA-Lib 子目录
add_subdirectory(${TA_LIB_DIR} ${CMAKE_BINARY_DIR}/ta-lib)
# 添加头文件目录
include_directories(${TA_LIB_DIR}/include)


# 添加项目文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 自动匹配源文件和头文件
file(GLOB_RECURSE TRADERBACKTEST_SRC "${PROJECT_SOURCE_DIR}/src/*.cpp" )
# 添加 YAML 文件
file(GLOB_RECURSE YAML_FILES "${PROJECT_SOURCE_DIR}/src/*.yaml")

# 添加库
add_library(TraderBacktestLib ${TRADERBACKTEST_SRC})

# 添加包含目录
target_include_directories(TraderBacktestLib PRIVATE ${TA_LIB_DIR}/include)

foreach(YAML_FILE ${YAML_FILES})
    get_filename_component(FILENAME ${YAML_FILE} NAME)
    configure_file(${YAML_FILE} ${CMAKE_BINARY_DIR}/src/config/${FILENAME} COPYONLY)
endforeach()


# 链接 TA-Lib 库到 TraderBacktestLib
target_link_libraries(TraderBacktestLib ta_lib yaml-cpp ${Boost_LIBRARIES})

# 添加数据文件到输出目录
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/AAPL.csv src/examples/aapl.csv COPYONLY)

# 添加 examples 子目录
add_subdirectory(${EXAMPLES_DIR})
