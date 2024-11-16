//
// Created by 薛新岗 on 2024/5/15.
//
#include <chrono>
#include "trading_module.h"
#include "backtest.h"
#include "stocks.h"
#include "indicators.h"
int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<StockData> data = readStockData("aapl.csv");
    Indicator indicator;
    indicator.SMA(data, 20); // Calculate 20-period SMA
    indicator.MACD(data,5,12,21);
//    for (const auto& entry : data) {
//        std::cout << "Date: " << entry.date
//                  << ", Symbol: " << entry.symbol
//                  << ", Open: " << entry.open
//                  << ", High: " << entry.high
//                  << ", Low: " << entry.low
//                  << ", Close: " << entry.close
//                  << ", Volume: " << entry.volume
//                  << std::endl;
//        for (const auto& indicators : entry.indicators) {
//            std::cout << indicators.first << ": " << indicators.second << " ";
//        }
//        std::cout << std::endl;
//    }


    BacktestModule backtestModule;
    // 执行回测
    backtestModule.setInitCash(100000);
    backtestModule.setCommissionPercentage(0.001);
    backtestModule.addData(data);
    backtestModule.setEntryMode("MACD");
    backtestModule.setExitMode("SMA");
    backtestModule.setEntryType("greaterThan");
    backtestModule.setExitType("deathCross");
    backtestModule.run();

    auto end = std::chrono::high_resolution_clock::now();
    // 计算持续时间
    std::chrono::duration<double> duration = end - start;

    // 输出运行时间
    std::cout << "运行时间: " << duration.count() << " 秒" << std::endl;

    return 0;
}

