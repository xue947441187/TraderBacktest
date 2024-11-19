# 获取所有测试文件
file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
set(CSV_FILE ${CMAKE_CURRENT_SOURCE_DIR}/aapl.csv)

# 为每个测试文件创建独立的可执行文件
foreach(TEST_FILE ${TEST_SOURCES})
    # 获取测试文件名
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    # 创建测试可执行文件
    add_executable(${TEST_NAME} ${TEST_FILE})

    # 链接主库 backtest
    target_link_libraries(${TEST_NAME} PRIVATE backtest)

    # 添加 TA-Lib 的动态库到测试可执行文件的运行路径
    add_custom_command(TARGET ${TEST_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:ta_lib>
            ${CSV_FILE}
            $<TARGET_FILE_DIR:${TEST_NAME}>
    )
endforeach()
