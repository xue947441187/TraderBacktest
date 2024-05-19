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
    this->addTrade(const_cast<std::string &>(date), const_cast<std::string &>(symbol), price, quantity, "buy", commission);

}


void TradingModule::sell(const std::string& date, const std::string& symbol, double price, int quantity) {
    // 计算手续费
    double commission = commissionPercentage * price * quantity;
    this->addTrade(date,symbol,price,quantity,"sell",commission);

}
std::vector<TradeInfo> TradingModule::getTradeInfoList() const {
    return tradeInfoList;
}

void TradingModule::addTrade(const  std::string &date,const std::string &symbol,double price,int quantity,const std::string &type,double commission) {
    // 添加交易信息到列表中
    TradeInfo trade;
    trade.date = date;
    trade.symbol = symbol;
    trade.price = price;
    trade.quantity = quantity;
    trade.type = type;
    trade.commissionPercentage = commissionPercentage;
    trade.commission = commission;
    tradeInfoList.push_back(trade);
    this->totalcommission = this->totalcommission + commission;
}
void TradingModule::setBuyQuantity(int quantity) {
    buyQuantity = quantity;
}

int TradingModule::getBuyQuantity() {
    return buyQuantity;
}