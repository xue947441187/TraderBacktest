//
// Created by 薛新岗 on 2024/5/15.
//
#include "library.h"
#include "trading/trading_module.h"
#include "backtest/backtest.h"

int main() {
    std::vector<StockData> data = readStockData("AAPL.csv");

    // 执行回测
    BacktestModule::backtest(data);


    return 0;
}
