//
// Created by 薛新岗 on 2024/5/15.
//
//#include "../include/trading_module.h"
//#include "include/backtest.h"
//#include "../include/stocks.h"
//#include "../include/indicators.h"
//int main() {
//    std::vector<StockData> data = readStockData("AAPL.csv");
//    Indicator indicator;
//    indicator.SMA(data, 20); // Calculate 20-period SMA
//
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
//
//
////    std::vector<double> prices(data.size());
////    std::transform(data.begin(), data.end(), prices.begin(),
////                   [](const StockData& stock) { return stock.close; });
//
////    BacktestModule backtestModule;
////    // 执行回测
////    backtestModule.setInitCash(100000);
////    backtestModule.setCommissionPercentage(0.001);
////    backtestModule.addData(data);
////    backtestModule.run();
//
//
//    return 0;
//}
#include <vector>
#include "iostream"
#include "ta_libc.h"
// 计算简单移动平均线（SMA）
void calculateSMA(const std::vector<double>& prices, int period) {
    // 分配内存
    TA_Real *inputPrices = new TA_Real[prices.size()];
    TA_Real *outputSMA = new TA_Real[prices.size()];

    // 将价格数据转换为 TA-Lib 可接受的形式
    for (size_t i = 0; i < prices.size(); ++i) {
        inputPrices[i] = prices[i];
    }

    // 计算 SMA
    TA_RetCode retCode = TA_MA(0, prices.size() - 1, inputPrices, period, TA_MAType_SMA, &period, &period, outputSMA);

    if (retCode == TA_SUCCESS) {
        // 输出结果
        std::cout << "Simple Moving Averages (Period " << period << "):" << std::endl;
        for (int i = 0; i < period; ++i) {
            std::cout << "Day " << i + 1 << ": " << outputSMA[i] << std::endl;
        }
    } else {
        std::cerr << "Error: Unable to calculate SMA." << std::endl;
    }

    // 释放内存
    delete[] inputPrices;
    delete[] outputSMA;
}


int main() {
    // 输入价格数据
    std::vector<double> prices = {10.0, 12.0, 11.5, 13.0, 15.5, 14.0, 16.0, 17.5, 18.0, 19.0};

    // 设置 SMA 的周期
    int period = 5;

    // 计算 SMA
    calculateSMA(prices, period);

    return 0;
}
