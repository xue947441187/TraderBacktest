//
// Created by 薛新岗 on 2024/5/15.
//

#ifndef TRADERBACKTEST_TRADING_MODULE_H
#define TRADERBACKTEST_TRADING_MODULE_H


#include <vector>
#include <string>
#include <cmath>


struct Trade {
    std::string date;
    std::string symbol;
    double price;
    int quantity;
    double commission;
};
struct TradeInfo {
    std::string date;
    std::string symbol;
    double price;
    int quantity;
    std::string type; // "buy" 或 "sell"
    double commissionPercentage; // 手续费百分比
    double commission;
};

class TradingModule {
public:
    TradingModule(double commissionPercentage = 0); // 构造函数，设置手续费百分比
    void executeTrade(const std::string& date, const std::string& symbol, double price, int quantity);
    double calculateCommission(double price, int quantity);
    double calculateProfitLoss(double buyPrice, double sellPrice, int quantity);
    void buy(const std::string& date, const std::string& symbol, double price, int quantity);
    void sell(const std::string& date, const std::string& symbol, double price, int quantity);
    std::vector<TradeInfo> getTradeInfoList() const;
    void setBuyQuantity(int quantity);
    int getBuyQuantity();
private:
    double totalcommission;
    double commissionPercentage; // 手续费百分比
    std::vector<TradeInfo> tradeInfoList; // 交易信息列表

    int buyQuantity = 0; // 存储之前买入时的交易数量，初始值为0
    void addTrade(const std::string &date,const std::string &symbol,double price,int quantity,const std::string &type,double commission);
};


#endif //TRADERBACKTEST_TRADING_MODULE_H
