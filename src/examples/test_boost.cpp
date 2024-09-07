//
// Created by 94744 on 2024/9/8.
//
// test_boost.cpp
#include <iostream>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>

using namespace boost;
using namespace boost::asio;

// 测试 boost::asio
void test_asio() {
    io_context io;

    // 创建一个定时器
    steady_timer timer(io, std::chrono::seconds(1));
    timer.async_wait([](const boost::system::error_code& error) {
        if (!error) {
            std::cout << "Timer expired!" << std::endl;
        }
    });

    io.run();
}

// 测试 boost::regex
void test_regex() {
    std::string text = "Boost 1.80.0";
    regex expr("\\d+\\.\\d+\\.\\d+");
    smatch matches;

    if (regex_search(text, matches, expr)) {
        std::cout << "Version found: " << matches[0] << std::endl;
    } else {
        std::cout << "No version found" << std::endl;
    }
}

// 测试 boost::shared_ptr
void test_shared_ptr() {
    shared_ptr<int> p1(new int(42));
    std::cout << "Value: " << *p1 << std::endl;

    // 创建另一个 shared_ptr 共享同一个对象
    shared_ptr<int> p2 = p1;
    std::cout << "Shared value: " << *p2 << std::endl;
}

int main() {
    std::cout << "Testing Boost library functions..." << std::endl;

    test_asio();
    test_regex();
    test_shared_ptr();

    return 0;
}
