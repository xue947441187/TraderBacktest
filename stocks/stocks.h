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

struct StockData {
    std::string date;
    std::string symbol;
    double open;
    double high;
    double low;
    double close;
    double volume;
    // You can add more fields according to your data format
};

std::vector<StockData> readStockData(const std::string& filename);

#endif //TRADERBACKTEST_STOCKS_H
