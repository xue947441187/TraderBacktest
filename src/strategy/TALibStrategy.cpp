//
// Reorganized & modernized TA-Lib strategy layer
// 2026-02-01
//
#include "strategy/TALibStrategy.h"
#include "ta_libc.h"

#include <iostream>
#include <vector>
#include <limits>

// Helper: place TA-Lib compact outputs into full-length vector (NaN padded)
static std::vector<double> expandTalibOutput(size_t fullSize, int outBegIdx, int outNbElement, const double* talibOut) {
    std::vector<double> out(fullSize, std::numeric_limits<double>::quiet_NaN());
    for (int i = 0; i < outNbElement; ++i) {
        const size_t pos = static_cast<size_t>(outBegIdx + i);
        if (pos < out.size()) out[pos] = talibOut[i];
    }
    return out;
}

void MovingAverageStrategy::process() {
    const std::string src = inputColumns.empty() ? defaultPriceColumn() : inputColumns.front();
    auto [idx, data] = getSeriesIntDouble(src);

    const int period = static_cast<int>(params.getNumericParam("period"));
    if (period <= 0) throw std::invalid_argument("period must be > 0");

    std::vector<double> smaResult(data.size(), 0.0);
    int outBegIdx = 0, outNbElement = 0;

    TA_RetCode rc = TA_SMA(0, static_cast<int>(data.size()) - 1, data.data(),
                           period, &outBegIdx, &outNbElement, smaResult.data());
    if (rc != TA_SUCCESS) {
        throw std::runtime_error("TA_SMA failed");
    }

    auto out = expandTalibOutput(data.size(), outBegIdx, outNbElement, smaResult.data());
    writeSeriesIntDouble("SMA_" + std::to_string(period), idx, out);
}

void RSIStrategy::process() {
    const std::string src = inputColumns.empty() ? defaultPriceColumn() : inputColumns.front();
    auto [idx, data] = getSeriesIntDouble(src);

    const int period = static_cast<int>(params.getNumericParam("period"));
    if (period <= 0) throw std::invalid_argument("period must be > 0");

    std::vector<double> rsiResult(data.size(), 0.0);
    int outBegIdx = 0, outNbElement = 0;

    TA_RetCode rc = TA_RSI(0, static_cast<int>(data.size()) - 1, data.data(),
                           period, &outBegIdx, &outNbElement, rsiResult.data());
    if (rc != TA_SUCCESS) {
        throw std::runtime_error("TA_RSI failed");
    }

    auto out = expandTalibOutput(data.size(), outBegIdx, outNbElement, rsiResult.data());
    writeSeriesIntDouble("RSI_" + std::to_string(period), idx, out);
}

void MACDStrategy::process() {
    const std::string src = inputColumns.empty() ? defaultPriceColumn() : inputColumns.front();
    auto [idx, data] = getSeriesIntDouble(src);

    const int fastPeriod = static_cast<int>(params.getNumericParam("FastPeriod"));
    const int slowPeriod = static_cast<int>(params.getNumericParam("SlowPeriod"));
    const int signalPeriod = static_cast<int>(params.getNumericParam("SignalPeriod"));

    if (fastPeriod <= 0 || slowPeriod <= 0 || signalPeriod <= 0) {
        throw std::invalid_argument("MACD periods must be > 0");
    }

    std::vector<double> macd(data.size(), 0.0);
    std::vector<double> signal(data.size(), 0.0);
    std::vector<double> hist(data.size(), 0.0);
    int outBegIdx = 0, outNbElement = 0;

    TA_RetCode rc = TA_MACD(0, static_cast<int>(data.size()) - 1, data.data(),
                            fastPeriod, slowPeriod, signalPeriod,
                            &outBegIdx, &outNbElement,
                            macd.data(), signal.data(), hist.data());
    if (rc != TA_SUCCESS) {
        throw std::runtime_error("TA_MACD failed");
    }

    auto outMacd   = expandTalibOutput(data.size(), outBegIdx, outNbElement, macd.data());
    auto outSignal = expandTalibOutput(data.size(), outBegIdx, outNbElement, signal.data());
    auto outHist   = expandTalibOutput(data.size(), outBegIdx, outNbElement, hist.data());

    writeSeriesIntDouble("MACD", idx, outMacd);
    writeSeriesIntDouble("MACDSignal", idx, outSignal);
    writeSeriesIntDouble("MACDHist", idx, outHist);
}
