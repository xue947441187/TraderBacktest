//
// Created by 94744 on 2024/5/27.
//

#include "backtest.h"


bool TradeLogic::isGreaterThan(const StockData& data, const std::string& indicator, double value) {
    return data.indicators.at(indicator) > value;
}

bool TradeLogic::isLessThan(const StockData& data, const std::string& indicator, double value) {
    return data.indicators.at(indicator) < value;
}

bool TradeLogic::isEqualTo(const StockData& data, const std::string& indicator, double value) {
    return data.indicators.at(indicator) == value;
}

bool TradeLogic::isGoldenCross(const StockData& previous, const StockData& current, const std::string& indicator, const std::string& currentMode) {
    return previous.indicators.at(indicator) <= current.indicators.at(currentMode);
//           current.indicators.at(shortMA) > current.indicators.at(longMA);
}

bool TradeLogic::isDeathCross(const StockData& previous, const StockData& current, const std::string& indicator, const std::string& currentMode) {
    return previous.indicators.at(indicator) >= current.indicators.at(currentMode);
//           current.indicators.at(shortMA) < current.indicators.at(longMA);
}

bool TradeLogic::handleComparison(const StockData &data, int currentIndex, const std::string &indicator, double value,
                                  std::string &currentMode) {
//    if (currentMode)
        return true;
}

