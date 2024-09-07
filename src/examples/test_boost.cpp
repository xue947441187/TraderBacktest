//
// Created by 94744 on 2024/9/8.
//
// test_boost.cpp
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>  // 必须包含这个头文件

int main() {
    namespace fs = boost::filesystem;  // 为 Boost.Filesystem 命名空间创建别名


    try {
        // 创建一个测试目录
        fs::path test_dir("test_boost_directory");
        if (fs::create_directory(test_dir)) {
            std::cout << "目录创建成功。" << std::endl;

            // 创建一个测试文件
            fs::path test_file = test_dir / "test_file.txt";
            std::ofstream ofs(test_file.string());  // 确保包含 <fstream>
            if (ofs) {
                ofs << "Boost 工作正常！";
                std::cout << "文件创建并写入成功。" << std::endl;
            } else {
                std::cerr << "文件创建失败。" << std::endl;
                return 1;
            }

            // 删除文件
            fs::remove(test_file);
            std::cout << "文件删除成功。" << std::endl;

            // 删除目录
            fs::remove(test_dir);
            std::cout << "目录删除成功。" << std::endl;
        } else {
            std::cerr << "目录创建失败。" << std::endl;
            return 1;
        }

    } catch (const fs::filesystem_error& e) {
        std::cerr << "文件系统错误: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "标准异常: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
