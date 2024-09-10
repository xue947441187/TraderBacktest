//
// Created by 薛新岗 on 2024/9/10.
//
#include "loadConfig.h"


int main(){
    // 使用单例模式获取配置实例
    loadConfig& config = loadConfig::getInstance("../config/configs.yaml");

    config.printConfig();
    return 0;
}