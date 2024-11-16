//
// Created by 薛新岗 on 2024/5/15.
//

#ifndef TRADERBACKTEST_INDICATORS_H
#define TRADERBACKTEST_INDICATORS_H
#include <vector>
#include <string>
#include "ta_libc.h"
#include "stocks.h"


class Indicator {
public:
    void SMA(std::vector<StockData>& data, int period);
    void MACD(std::vector<StockData>& data, int fastPeriod, int slowPeriod, int signalPeriod);

};

#endif //TRADERBACKTEST_INDICATORS_H
