//
// Created by 94744 on 2024/9/16.
//

#include "strategy/StrategyFactory.h"
#include "strategy/TALibStrategy.h"

boost::movelib::unique_ptr<TALibStrategy> TALibStrategyFactory::createStrategy(const std::string &strategyName,const std::vector<std::string>& columnNames) {

    {
        std::unique_ptr<TALibStrategy> strategy;

        if (strategyName == "MovingAverage") {
            strategy = std::make_unique<MovingAverageStrategy>();
        } else if (strategyName == "RSI") {
            strategy = std::make_unique<RSIStrategy>();
        }
            // 可以扩展其他策略
        else {
            throw std::invalid_argument("Unknown strategy: " + strategyName);
        }
        // 设置列名
        strategy->setInputColumns(columnNames);

    }
}