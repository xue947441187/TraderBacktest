//
// Created by 94744 on 2024/5/20.
//

#include <algorithm>
#include <valarray>
#include <numeric>
#include <stdexcept>
#include "../include/indicators.h"

void Indicator::SMA(std::vector<StockData>& data, int period) {
    int dataSize = data.size();
    std::vector<double> closePrices(dataSize);
    for (int i = 0; i < dataSize; ++i) {
        closePrices[i] = data[i].close;
    }

    std::vector<double> outSMA(dataSize);
    int outBeg, outNbElement;

    TA_RetCode retCode = TA_MA(0, dataSize - 1, closePrices.data(), period, TA_MAType_SMA, &outBeg, &outNbElement, outSMA.data());

//    if (retCode == TA_SUCCESS) {
//        for (int i = 0; i < outNbElement; ++i) {
//            data[outBeg + i].indicators["SMA_" + std::to_string(period)] = outSMA[i];
//        }
//    } else {
//        std::cerr << "Error calculating SMA: " << retCode << std::endl;
//    }

}
