//
// Created by 94744 on 2024/5/20.
//


#include "indicators.h"
void Indicator::SMA(std::vector<StockData>& data, int period) {
    int dataSize = data.size();
    std::vector<double> closePrices(dataSize);

    for (int i = 0; i < dataSize; ++i) {
        closePrices[i] = data[i].close;
    }

    std::vector<double> out(dataSize - period + 1);
    TA_RetCode retCode;
    int outBeg, outNbElement;

    retCode = TA_SMA(0, dataSize - 1, closePrices.data(), period, &outBeg, &outNbElement, out.data());

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error calculating SMA: " << retCode << std::endl;
        return;
    }

    for (int i = 0; i < outNbElement; ++i) {
        data[outBeg + i].indicators["SMA"] = out[i];
    }


}

void Indicator::MACD(std::vector<StockData>& data, int fastPeriod, int slowPeriod, int signalPeriod) {
    int dataSize = data.size();
    std::vector<double> closePrices(dataSize);

    for (int i = 0; i < dataSize; ++i) {
        closePrices[i] = data[i].close;
    }

    std::vector<double> macd(dataSize), macdSignal(dataSize), macdHist(dataSize);
    TA_RetCode retCode;
    int outBeg, outNbElement;

    retCode = TA_MACD(0, dataSize - 1, closePrices.data(), fastPeriod, slowPeriod, signalPeriod,
                      &outBeg, &outNbElement, macd.data(), macdSignal.data(), macdHist.data());

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error calculating MACD: " << retCode << std::endl;
        return;
    }

    for (int i = 0; i < outNbElement; ++i) {
        data[outBeg + i].indicators["MACD"] = macd[i];
        data[outBeg + i].indicators["MACDSignal"] = macdSignal[i];
        data[outBeg + i].indicators["MACDHist"] = macdHist[i];
    }
}
