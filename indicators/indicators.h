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
    // Simple Moving Average (SMA)
    static std::vector<double> SMA(const std::vector<double>& prices, int windowSize);

    // Exponential Moving Average (EMA)
    static std::vector<double> EMA(const std::vector<double>& prices, int windowSize);

    // Weighted Moving Average (WMA)
    static std::vector<double> WMA(const std::vector<double>& prices, int windowSize);

    // Relative Strength Index (RSI)
    static std::vector<double> RSI(const std::vector<double>& prices, int windowSize);

    // Bollinger Bands
//    static std::vector<double> BollingerBands(const std::vector<double>& prices, int windowSize);
    static std::vector<std::vector<double>> BollingerBands(const std::vector<double>& prices, int windowSize);

    // Stochastic Oscillator
    static std::vector<double> StochasticOscillator(const std::vector<double>& prices, int windowSize);

    // Moving Average Convergence Divergence (MACD)
//    static std::vector<double> MACD(const std::vector<double>& prices, int shortPeriod, int longPeriod, int signalPeriod);
    static std::vector<std::vector<double>> MACD(const std::vector<double>& prices, int shortPeriod, int longPeriod, int signalPeriod);

    // Volume Weighted Moving Average (VWAP)
    static std::vector<double> VWAP(const std::vector<double>& prices, const std::vector<double>& volumes, int windowSize);

    // Momentum Indicator
    static std::vector<double> Momentum(const std::vector<double>& prices, int windowSize);

    // Average True Range (ATR)
    static std::vector<double> ATR(const std::vector<double>& highs, const std::vector<double>& lows, const std::vector<double>& closes, int windowSize);

    // Average Directional Index (ADX)
    static std::vector<double> ADX(const std::vector<double>& highs, const std::vector<double>& lows, const std::vector<double>& closes, int windowSize);

    // Accumulation/Distribution Line (ADL)
    static std::vector<double> ADL(const std::vector<double>& highs, const std::vector<double>& lows, const std::vector<double>& closes, const std::vector<double>& volumes);

    // Double Exponential Moving Average (DEMA)
    static std::vector<double> DEMA(const std::vector<double>& prices, int windowSize);

    // Triple Exponential Moving Average (TEMA)
    static std::vector<double> TEMA(const std::vector<double>& prices, int windowSize);

    // Relative Strength Momentum Indicator (RSMI)
    static std::vector<double> RSMI(const std::vector<double>& prices, int windowSize);

    // Accelerator/Decelerator Oscillator
    static std::vector<double> ADO(const std::vector<double>& prices, int shortPeriod, int longPeriod);

    // Chaikin Oscillator
    static std::vector<double> ChaikinOscillator(const std::vector<double>& closes, const std::vector<double>& volumes, int shortPeriod, int longPeriod);

    // Price Volume Trend (PVT)
    static std::vector<double> PVT(const std::vector<double>& prices, const std::vector<double>& volumes);

    // Bollinger Band Width
    static std::vector<double> BollingerBandWidth(const std::vector<double>& prices, int windowSize);

    // Williams %R
    static std::vector<double> WilliamsR(const std::vector<double>& highs, const std::vector<double>& lows, const std::vector<double>& closes, int windowSize);

    // Parallel Channel
    static std::vector<double> ParallelChannel(const std::vector<double>& highs, const std::vector<double>& lows, int windowSize);

    // Know Sure Thing (KST)
    static std::vector<double> KST(const std::vector<double>& prices, int ROC1Period, int ROC2Period, int ROC3Period, int ROC4Period, int SMA1Period, int SMA2Period, int SMA3Period, int SMA4Period);

    // Parabolic SAR
    static std::vector<double> ParabolicSAR(const std::vector<double>& highs, const std::vector<double>& lows, double accelerationFactorStart, double accelerationFactorIncrement);

    // Directional Movement Index (DMI)
    static std::vector<double> DMI(const std::vector<double>& highs, const std::vector<double>& lows, const std::vector<double>& closes, int windowSize);

    // Negative Volume Index (NVI)
    static std::vector<double> NVI(const std::vector<double>& closes, const std::vector<double>& volumes);

    // Relative Price Oscillator (RPO)
    static std::vector<double> RPO(const std::vector<double>& prices, int shortPeriod, int longPeriod);

    // Trading Channel Index (TCI)
    static std::vector<double> TCI(const std::vector<double>& prices, int windowSize);

};

#endif //TRADERBACKTEST_INDICATORS_H
