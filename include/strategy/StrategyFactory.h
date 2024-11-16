//
// Created by 94744 on 2024/9/16.
//

#ifndef TRADERBACKTEST_STRATEGYFACTORY_H
#define TRADERBACKTEST_STRATEGYFACTORY_H
#include <utility>

#include "strategy/TALibStrategy.h"
#include "boost/system/errc.hpp"




class TALibStrategyFactory {
public:
    std::unique_ptr<TALibStrategy> createStrategy(const std::string& strategyName,const std::string& columnNames,const StrategyParams params);

    explicit TALibStrategyFactory(boost::shared_ptr<LineManager::LineManager> & lineManager)
            : lineManager(lineManager)
    {

        registry["MovingAverage"] = [lineManager]() { return std::make_unique<MovingAverageStrategy>(lineManager); };

        registry["RSI"] = [lineManager]() { return std::make_unique<RSIStrategy>(lineManager); };
        // 其他策略注册
//         registry["MACD"] = [lineManager]() { return std::make_unique<MACDStrategy>(lineManager); };

    }
private:
    std::map<std::string, std::function<std::unique_ptr<TALibStrategy>()>> registry;


    boost::shared_ptr<LineManager::LineManager> lineManager;
};




#endif //TRADERBACKTEST_STRATEGYFACTORY_H