//
// Created by 薛新岗 on 2024/5/15.
//

#include "trading_module.h"
#include <iostream>

void TradingModule::executeTrade(const std::string& date, const std::string& symbol, double price, int quantity) {
    // Simulate executing a trade
    std::cout << "Executing trade: Date: " << date << ", Symbol: " << symbol << ", Price: " << price
              << ", Quantity: " << quantity << std::endl;

    // Calculate commission
    double commission = calculateCommission(price, quantity);

    // Record the trade
    Trade trade;
    trade.date = date;
    trade.symbol = symbol;
    trade.price = price;
    trade.quantity = quantity;
    trade.commission = commission;

    // Perform any other necessary actions, such as updating account balances, etc.
}

double TradingModule::calculateCommission(double price, int quantity) {
    // Simple commission calculation, can be adjusted based on your requirements
    return 0.005 * price * quantity; // Assuming commission rate of 0.5%
}

double TradingModule::calculateProfitLoss(double buyPrice, double sellPrice, int quantity) {
    // Simple profit/loss calculation
    return (sellPrice - buyPrice) * quantity;
}

TradingModule::TradingModule(double commissionPercentage) : commissionPercentage(commissionPercentage) {}
void TradingModule::buy(const std::string& date, const std::string& symbol, double price, int quantity) {
    // 计算手续费
    double commission = commissionPercentage * price * quantity;

    // 添加交易信息到列表中
    TradeInfo trade;
    trade.date = date;
    trade.symbol = symbol;
    trade.price = price;
    trade.quantity = quantity;
    trade.type = "buy";
    trade.commissionPercentage = commissionPercentage;
    tradeInfoList.push_back(trade);
}

void TradingModule::sell(const std::string& date, const std::string& symbol, double price, int quantity) {
    // 计算手续费
    double commission = commissionPercentage * price * quantity;

    // 添加交易信息到列表中
    TradeInfo trade;
    trade.date = date;
    trade.symbol = symbol;
    trade.price = price;
    trade.quantity = quantity;
    trade.type = "sell";
    trade.commissionPercentage = commissionPercentage;
    tradeInfoList.push_back(trade);
}
std::vector<TradeInfo> TradingModule::getTradeInfoList() const {
    return tradeInfoList;
}
