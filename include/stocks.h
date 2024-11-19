//
// Created by 薛新岗 on 2024/5/15.
//

#ifndef TRADERBACKTEST_STOCKS_H
#define TRADERBACKTEST_STOCKS_H
#include <iostream>
#include <unordered_map>
#include <boost/variant.hpp>
#include <iomanip>
#include <fstream>
#include "base/mObserver.h"
#include "line/line.h"
#include "boost/any.hpp"
#include "boost/function.hpp"
#include "map"
#include "lineManager/lineManager.h"

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
//void readLineStockData(const std::string& filename, const boost::shared_ptr<LineManager::LineManager>& lineManager,const std::string& index_name);
void readLinesStockData(const std::string& filename, const boost::shared_ptr<LineManager::LineManager>& lineManager,const std::string& index_name);
#endif //TRADERBACKTEST_STOCKS_H
