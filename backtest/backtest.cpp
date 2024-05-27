//
// Created by 薛新岗 on 2024/5/15.
//

#include <iomanip>
#include <limits>
#include "../include/backtest.h"

void BacktestModule::backtest() {
    TradingModule tradingModule(this->commissionPercentage);

    bool holdingStock = false;


    double capital = this->initCash; // 初始资本
    double totalCommission = 0.0; // 总手续费

    for (int i = 0; i < data.size(); ++i) {
        const StockData stock = data[i];

        double currentMA = getIndicator(stock,"MACD");
        // 根据移动平均线位置执行交易
        if (!holdingStock && stock.close > currentMA) {
            // 计算手续费
            double commission = commissionPercentage * stock.close * capital / stock.close;

            executeBuy(tradingModule, stock, capital, totalCommission,commission);
            holdingStock = true;
        } else if (holdingStock && stock.close < currentMA) {
            // 计算手续费
            double commission = 0;

            executeSell(tradingModule, stock, capital, totalCommission,commission);
            holdingStock = false;
        }
    }

    // 计算收益并打印结果
    calculateProfitAndPrintResult(tradingModule);
}

void BacktestModule::executeBuy(TradingModule& tradingModule, const StockData& stock, double& capital, double& totalCommission,double &commission) {


    // 计算交易数量
//    int quantity = capital / stock.close;
    int quantity = std::floor(capital / stock.close);
    // 执行买入操作
    tradingModule.buy(stock.date, stock.symbol, stock.close, quantity);
    tradingModule.setBuyQuantity(quantity); // 设置买入时的交易数量

    // 更新资本和总手续费
    capital -= (stock.close * quantity);
    totalCommission += commission;
}

void BacktestModule::executeSell(TradingModule& tradingModule, const StockData& stock, double& capital, double& totalCommission,double &commission) {

    // 获取之前买入时的交易数量
    int quantity = tradingModule.getBuyQuantity();

    // 执行卖出操作
    tradingModule.sell(stock.date, stock.symbol, stock.close, quantity);
    // 重置买入时的交易数量为0
    tradingModule.setBuyQuantity(0);

    // 更新资本和总手续费
    capital += (stock.close * quantity);
    totalCommission += commission;
}



void BacktestModule::calculateProfitAndPrintResult(const TradingModule& tradingModule) {
    // 获取交易列表
    std::vector<TradeInfo> tradeList = tradingModule.getTradeInfoList();

    // 计算总收益
    double totalProfit = 0.0;
    for (const auto& trade : tradeList) {
        totalProfit += (trade.quantity * trade.price - trade.commissionPercentage * trade.quantity * trade.price);
    }
    endingCapital = 0;
    // 计算结束时的金额
    endingCapital = this->initCash + totalProfit;

    // 打印结果
    std::cout << "Initial Capital: " << this->initCash << std::endl;
//    std::cout << "Ending Capital: " << endingCapital << std::endl;
    std::cout << "Ending Capital: " << std::fixed << std::setprecision(2) << endingCapital << std::endl;
    std::cout << "Total Profit: " << std::fixed << std::setprecision(2) << totalProfit << std::endl;
}

void BacktestModule::addData(const std::vector<StockData> &data) {
    this->data = data;

}

void BacktestModule::run() {
    this->backtest();

}

void BacktestModule::setCommissionPercentage(double CommissionPercentage) {
    this->commissionPercentage = CommissionPercentage;

}

void BacktestModule::setInitCash(double InitCash) {
    this->initCash = InitCash;
}

void BacktestModule::setEntryMode(const std::string& mode) {
    this->entryMode = mode;
}

void BacktestModule::setExitMode(const std::string& mode) {
    this->exitMode = mode;
}

double getIndicator(const StockData& stock, const std::string& indicatorName) {
    auto it = stock.indicators.find(indicatorName);
    if (it != stock.indicators.end()) {
        return it->second;
    } else {
        return std::numeric_limits<double>::quiet_NaN();
    }
}
