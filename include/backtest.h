//
// Created by 薛新岗 on 2024/5/15.
//

#ifndef TRADERBACKTEST_BACKTEST_H
#define TRADERBACKTEST_BACKTEST_H

#include <vector>
#include "trading_module.h" // 引入交易模块的头文件
#include "stocks.h" // 引入交易模块的头文件
#include "indicators.h"

class BacktestModule {
public:
    // 执行回测

    void run();
    void calculateProfitAndPrintResult(const TradingModule &tradingModule);
    void setCommissionPercentage(double commissionPercentage);
    void setInitCash(double InitCash);
    void addData(const std::vector<StockData> &data);
private:
    double endingCapital;
    double commissionPercentage;
    double initCash;

    std::vector<StockData> data;
    void backtest();

    void executeBuy(TradingModule &tradingModule, const StockData &stock, double &capital, double &totalCommission,double &commission);

    void executeSell(TradingModule &tradingModule, const StockData &stock, double &capital, double &totalCommission,double &commission);
};

#endif //TRADERBACKTEST_BACKTEST_H
