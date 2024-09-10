//
// Created by 薛新岗 on 2024/9/10.
//

#ifndef TRADERBACKTEST_LOADCONFIG_H
#define TRADERBACKTEST_LOADCONFIG_H
#include <yaml.h>
#include "string"
#include <yaml-cpp/yaml.h>
#include <iostream>



//// 暂时先不使用这个
// 定义 loadConfig 类
class loadConfig {
private:
    YAML::Node config;

    // 私有构造函数
    explicit loadConfig(const std::string& filename) {
        config = YAML::LoadFile(filename);
    }

public:
    // 禁止复制构造函数和赋值操作符
    loadConfig(const loadConfig&) = delete;
    loadConfig& operator=(const loadConfig&) = delete;

    // 获取配置的单例实例
    static loadConfig& getInstance(const std::string& filename) {
        static loadConfig instance(filename); // 唯一实例
        return instance;
    }
    YAML::Node getTypesConfig() const{
        return config["types"];
    }
    // 获取配置节点
    YAML::Node getConfig() const {
        return config;
    }

    // 其他处理配置的方法
    void printConfig() const {
        std::cout << config << std::endl;
    }
};



#endif //TRADERBACKTEST_LOADCONFIG_H
