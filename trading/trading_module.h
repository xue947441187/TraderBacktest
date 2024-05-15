//
// Created by 薛新岗 on 2024/5/15.
//

#ifndef TRADERBACKTEST_TRADING_MODULE_H
#define TRADERBACKTEST_TRADING_MODULE_H


#include <vector>
#include <string>


struct Trade {
    std::string date;
    std::string symbol;
    double price;
    int quantity;
    double commission;
};

class TradingModule {
public:
    void executeTrade(const std::string& date, const std::string& symbol, double price, int quantity);
    double calculateCommission(double price, int quantity);
    double calculateProfitLoss(double buyPrice, double sellPrice, int quantity);
};


#endif //TRADERBACKTEST_TRADING_MODULE_H
