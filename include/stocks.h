//
// Created by 薛新岗 on 2024/5/15.
//

#ifndef TRADERBACKTEST_STOCKS_H
#define TRADERBACKTEST_STOCKS_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

struct StockData {
    std::string date;
    std::string symbol;
    double open;
    double high;
    double low;
    double close;
    double volume;
    std::unordered_map<std::string, double> indicators; // Custom indicators

    // You can add more fields according to your data format
};

std::vector<StockData> readStockData(const std::string& filename);
#endif //TRADERBACKTEST_STOCKS_H
