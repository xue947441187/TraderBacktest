//
// Reorganized Strategy Factory
// 2026-02-01
//
#include "strategy/StrategyFactory.h"

std::shared_ptr<TALibStrategy> TALibStrategyFactory::createStrategy(
        const std::string& strategyName,
        const std::string& columnNamesCsv,
        StrategyParams params) {

    auto it = registry.find(strategyName);
    if (it == registry.end()) {
        throw std::invalid_argument("Unknown strategy: " + strategyName);
    }

    auto strategy = it->second();
    strategy->setInputColumns(columnNamesCsv);  // CSV parsing happens inside base class
    strategy->setParams(params);
    return strategy;
}
