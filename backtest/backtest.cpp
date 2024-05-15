//
// Created by 薛新岗 on 2024/5/15.
//

#include "backtest.h"

void BacktestModule::backtest(const std::vector<StockData>& data) {
    // 初始化回测环境
    double initialCapital = 100000.0; // 初始资金
    TradingModule tradingModule;

    // 是否持有股票的标志
    bool holdingStock = false;

    // 计算移动平均的窗口大小
    int windowSize = 20;

    // 交易列表
    std::vector<Trade> tradeList;
    for (int i = 0; i < data.size(); ++i) {
        const auto& stock = data[i];

        // 计算移动平均
        std::vector<double> prices;
        for (int j = std::max(0, i - windowSize + 1); j <= i; ++j) {
            prices.push_back(data[j].close);
        }
        std::vector<double> movingAverage = Indicator::MA(prices, windowSize);
        double currentMA = movingAverage.back();

        // 根据移动平均线位置执行交易
        if (!holdingStock && stock.close > currentMA) {
            tradingModule.executeTrade(stock.date, "AAPL", stock.close, 10);
            holdingStock = true;

            // 添加交易记录到交易列表
            Trade trade;
            trade.date = stock.date;
            trade.symbol = "AAPL";
            trade.price = stock.close;
            trade.quantity = 10;
            tradeList.push_back(trade);
        } else if (holdingStock && stock.close < currentMA) {
            tradingModule.executeTrade(stock.date, "AAPL", stock.close, -10); // 卖出数量为负数表示卖出
            holdingStock = false;

            // 添加交易记录到交易列表
            Trade trade;
            trade.date = stock.date;
            trade.symbol = "AAPL";
            trade.price = stock.close;
            trade.quantity = -10; // 卖出数量为负数表示卖出
            tradeList.push_back(trade);
        }
    }

    // 计算收益
    double profit = 0.0;
    for (const auto& trade : tradeList) {
        profit += (trade.quantity * trade.price);
    }

    // 打印收益
    std::cout << "Total Profit: " << profit << std::endl;

    }


