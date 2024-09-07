//
// Created by 薛新岗 on 2024/5/15.
//

#include <iomanip>
#include <limits>
#include "backtest.h"

void BacktestModule::backtest() {
    TradingModule tradingModule(this->commissionPercentage);

    bool holdingStock = false;


    double capital = this->initCash; // 初始资本
    double totalCommission = 0.0; // 总手续费
    StockData previous;
    StockData current;
    this->setCurrentMode(this->entryMode);
    for (int i = 0; i < data.size(); ++i) {
        current = data[i];
        double currentMA = getIndicator(current,this->currentMode);
        if (i<=1){
            continue;
        }else if (std::isnan(currentMA) || std::isnan(getIndicator(data[i-1],this->currentMode))){
            continue;
        }
        previous = data[i-1];

        if (holdingStock)
            this->setCurrentMode(this->exitMode);
        else
            this->setCurrentMode(this->entryMode);

        // 根据移动平均线位置执行交易
        if (!holdingStock && this->checkTradingConditions(previous,current,this->currentMode,0.3,"goldenCross")) {
            // 计算手续费
            double commission = commissionPercentage * current.close * capital / current.close;
            executeBuy(tradingModule, current, capital, totalCommission,commission);
            holdingStock = true;
        } else if (holdingStock && this->checkTradingConditions(previous,current,this->currentMode,0.7,"deathCross")) {
            // 计算手续费
            double commission = 0;
            executeSell(tradingModule, current, capital, totalCommission,commission);
            holdingStock = false;
        }
    }

    // 计算收益并打印结果
    calculateProfitAndPrintResult(tradingModule);
}

/**
 *
 * @param previous 获取上一个参数
 * @param current 当前参数
 * @param indicator 指标名称
 * @param value 设定值
 * @param conditionType   判断方式
 * @return
 */
bool BacktestModule::checkTradingConditions(const StockData& previous, const StockData& current, const std::string& indicator, double value, const std::string& conditionType) {
    if (conditionType == "greaterThan") {
        return current.indicators.at(indicator) > value;
    } else if (conditionType == "lessThan") {
        return current.indicators.at(indicator) < value;
    } else if (conditionType == "equalTo") {
        return current.indicators.at(indicator) == value;
    } else if (conditionType == "goldenCross") {
        return previous.indicators.at(indicator) <= current.indicators.at(this->currentMode);
//               current.indicators.at(indicator) > current.indicators.at("MACDSignal");
    } else if (conditionType == "deathCross") {
        return previous.indicators.at(indicator) >= current.indicators.at(this->currentMode);
//               current.indicators.at(indicator) < current.indicators.at(this->currentMode);
    } else {
        throw std::invalid_argument("Invalid condition type");
    }
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
//    std::cout << stock.date << "Buy-> quantity: " << quantity <<"," <<"close: " << stock.close << std::endl;
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
//    std::cout << stock.date << "sell-> quantity: " << quantity <<"," <<"close: " << stock.close << std::endl;
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

void BacktestModule::setCurrentMode(const std::string &mode) {
    this->currentMode = mode;
}

void BacktestModule::setEntryType(const std::string &type) {
this->entryType = type;
}

void BacktestModule::setExitType(const std::string &type) {
    this->exitType = type;
}


double getIndicator(const StockData& stock, const std::string& indicatorName) {
    auto it = stock.indicators.find(indicatorName);
    if (it != stock.indicators.end()) {
        return it->second;
    } else {
        return std::numeric_limits<double>::quiet_NaN();
    }
}
