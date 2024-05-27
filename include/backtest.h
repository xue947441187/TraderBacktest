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
    void setEntryMode(const std::string& mode);
    void setExitMode(const std::string& mode);

private:
    double endingCapital;
    double commissionPercentage;
    double initCash;
    std::string entryMode = "close";
    std::string exitMode = "close";

    std::vector<StockData> data;
    void backtest();

    void executeBuy(TradingModule &tradingModule, const StockData &stock, double &capital, double &totalCommission,double &commission);

    void executeSell(TradingModule &tradingModule, const StockData &stock, double &capital, double &totalCommission,double &commission);
};


double getIndicator(const StockData& stock, const std::string& indicatorName);

// 交易逻辑
bool isGreaterThan(const StockData& data, const std::string& indicator, double value);
bool isLessThan(const StockData& data, const std::string& indicator, double value);
bool isEqualTo(const StockData& data, const std::string& indicator, double value);
bool isGoldenCross(const StockData& previous, const StockData& current, const std::string& shortMA, const std::string& longMA);
bool isDeathCross(const StockData& previous, const StockData& current, const std::string& shortMA, const std::string& longMA);

#endif //TRADERBACKTEST_BACKTEST_H
