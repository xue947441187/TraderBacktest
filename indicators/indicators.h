//
// Created by 薛新岗 on 2024/5/15.
//

#ifndef TRADERBACKTEST_INDICATORS_H
#define TRADERBACKTEST_INDICATORS_H
#include <vector>
// 定义技术指标结构
struct TechnicalIndicator {
    // 添加你需要的技术指标字段

};


class Indicator {
public:
    static std::vector<double> MA(const std::vector<double>& prices, int windowSize);
};

#endif //TRADERBACKTEST_INDICATORS_H
