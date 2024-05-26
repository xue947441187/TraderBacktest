//
// Created by 薛新岗 on 2024/5/15.
//

#ifndef TRADERBACKTEST_INDICATORS_H
#define TRADERBACKTEST_INDICATORS_H
#include <vector>
#include <string>
#include "ta_libc.h"
#include "stocks.h"

// 定义技术指标结构
struct TechnicalIndicator {
    std::string name;           // 技术指标名称
    int period;                 // 技术指标的周期
    double value; // 存储技术指标的值
    std::string date;

};


class Indicator {
public:
    void SMA(std::vector<StockData>& data, int period);

};

#endif //TRADERBACKTEST_INDICATORS_H
