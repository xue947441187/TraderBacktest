//
// Reorganized Strategy Factory (registry + aliases + consistent column parsing)
// 2026-02-01
//
#ifndef TRADERBACKTEST_STRATEGYFACTORY_H
#define TRADERBACKTEST_STRATEGYFACTORY_H

#include <map>
#include <functional>
#include <memory>
#include <string>
#include <stdexcept>

#include "strategy/TALibStrategy.h"

class TALibStrategyFactory {
public:
    explicit TALibStrategyFactory(const boost::shared_ptr<LineManager::LineManager>& lineManager)
            : lineManager(lineManager) {
        // Canonical names
        registry["MovingAverage"] = [this]() { return std::make_shared<MovingAverageStrategy>(this->lineManager); };
        registry["RSI"]           = [this]() { return std::make_shared<RSIStrategy>(this->lineManager); };
        registry["MACD"]          = [this]() { return std::make_shared<MACDStrategy>(this->lineManager); };

        // Aliases
        registry["SMA"] = registry["MovingAverage"];
        registry["MA"]  = registry["MovingAverage"];
    }

    // Backward compatible signature: columnNames is CSV (e.g. "Close" or "Open,High,Low,Close")
    std::shared_ptr<TALibStrategy> createStrategy(const std::string& strategyName,
                                                  const std::string& columnNamesCsv,
                                                  StrategyParams params);

private:
    std::map<std::string, std::function<std::shared_ptr<TALibStrategy>()>> registry;
    boost::shared_ptr<LineManager::LineManager> lineManager;
};

#endif // TRADERBACKTEST_STRATEGYFACTORY_H
