//
// Created by 94744 on 2024/9/16.
//

#ifndef TRADERBACKTEST_STRATEGYFACTORY_H
#define TRADERBACKTEST_STRATEGYFACTORY_H
#include "strategy/TALibStrategy.h"
#include "boost/move/unique_ptr.hpp"
#include "boost/make_unique.hpp"
#include "boost/system/errc.hpp"
class TALibStrategyFactory {
public:
    boost::movelib::unique_ptr<TALibStrategy> createStrategy(const std::string& strategyName,const std::vector<std::string>& columnNames);

private:
    TALibStrategyFactory(LineManager::LineManager lineManager)
    : lineManager(lineManager)
    {}

    LineManager::LineManager lineManager;
};

#endif //TRADERBACKTEST_STRATEGYFACTORY_H
