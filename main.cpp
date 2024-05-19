//
// Created by 薛新岗 on 2024/5/15.
//
#include "library.h"
#include "trading/trading_module.h"
#include "backtest/backtest.h"

int main() {
    std::vector<StockData> data = readStockData("AAPL.csv");
    BacktestModule backtestModule;
    // 执行回测
    backtestModule.setInitCash(100000);
    backtestModule.setCommissionPercentage(0.001);
    backtestModule.addData(data);
    backtestModule.run();


    return 0;
}
