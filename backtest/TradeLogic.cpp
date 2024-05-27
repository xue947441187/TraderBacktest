//
// Created by 94744 on 2024/5/27.
//

#include "../include/backtest.h"


bool isGreaterThan(const StockData& data, const std::string& indicator, double value) {
    return data.indicators.at(indicator) > value;
}

bool isLessThan(const StockData& data, const std::string& indicator, double value) {
    return data.indicators.at(indicator) < value;
}

bool isEqualTo(const StockData& data, const std::string& indicator, double value) {
    return data.indicators.at(indicator) == value;
}

bool isGoldenCross(const StockData& previous, const StockData& current, const std::string& shortMA, const std::string& longMA) {
    return previous.indicators.at(shortMA) <= previous.indicators.at(longMA) &&
           current.indicators.at(shortMA) > current.indicators.at(longMA);
}

bool isDeathCross(const StockData& previous, const StockData& current, const std::string& shortMA, const std::string& longMA) {
    return previous.indicators.at(shortMA) >= previous.indicators.at(longMA) &&
           current.indicators.at(shortMA) < current.indicators.at(longMA);
}
