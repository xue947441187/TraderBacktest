//
// Created by 薛新岗 on 2024/5/15.
//
#include "indicators.h"


std::vector<double> Indicator::MA(const std::vector<double>& prices, int windowSize) {
    std::vector<double> movingAverage(prices.size(), 0.0);

    for (int i = 0; i < prices.size(); ++i) {
        double sum = 0.0;
        int count = 0;
        for (int j = std::max(0, i - windowSize + 1); j <= i; ++j) {
            sum += prices[j];
            ++count;
        }
        movingAverage[i] = sum / count;
    }

    return movingAverage;
}
