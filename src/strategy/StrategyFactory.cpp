//
// Created by 94744 on 2024/9/16.
//

#include "strategy/StrategyFactory.h"
#include "strategy/TALibStrategy.h"

std::shared_ptr<TALibStrategy> TALibStrategyFactory::createStrategy(
        const std::string &strategyName,
        const std::string& columnNames,
        const StrategyParams params)
{
    auto it = registry.find(strategyName);
    if (it != registry.end()) {
        auto strategy = it->second();  // 创建策略
        strategy->setInputColumns(columnNames);
        strategy->setParams(params); // 设置策略参数
        return strategy;
    } else {
        throw std::invalid_argument("Unknown strategy: " + strategyName);
    }
}