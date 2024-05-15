//
// Created by 薛新岗 on 2024/5/15.
//

#ifndef TRADERBACKTEST_BACKTEST_H
#define TRADERBACKTEST_BACKTEST_H

#include <vector>
#include "../trading/trading_module.h" // 引入交易模块的头文件
#include "../stocks/stocks.h" // 引入交易模块的头文件
#include "../indicators/indicators.h"

class BacktestModule {
public:
    // 执行回测
    static void backtest(const std::vector<StockData>& data);
};

#endif //TRADERBACKTEST_BACKTEST_H
