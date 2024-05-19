//
// Created by 94744 on 2024/5/20.
//

#include <algorithm>
#include <valarray>
#include <numeric>
#include "indicators.h"

// Simple Moving Average (SMA)
std::vector<double> Indicator::SMA(const std::vector<double>& prices, int windowSize) {
    std::vector<double> sma;
    for (int i = 0; i <= prices.size() - windowSize; ++i) {
        double sum = 0.0;
        for (int j = i; j < i + windowSize; ++j) {
            sum += prices[j];
        }
        sma.push_back(sum / windowSize);
    }
    return sma;
}

// Exponential Moving Average (EMA)
std::vector<double> Indicator::EMA(const std::vector<double>& prices, int windowSize) {
    std::vector<double> ema;
    double alpha = 2.0 / (windowSize + 1);
    ema.push_back(prices[0]); // 初始值为第一个价格
    for (int i = 1; i < prices.size(); ++i) {
        double currentEMA = alpha * prices[i] + (1 - alpha) * ema.back();
        ema.push_back(currentEMA);
    }
    return ema;
}

// Weighted Moving Average (WMA)
std::vector<double> Indicator::WMA(const std::vector<double>& prices, int windowSize) {
    std::vector<double> wma;
    double denominator = windowSize * (windowSize + 1) / 2;
    for (int i = 0; i <= prices.size() - windowSize; ++i) {
        double weightedSum = 0.0;
        for (int j = 0; j < windowSize; ++j) {
            weightedSum += prices[i + j] * (windowSize - j);
        }
        wma.push_back(weightedSum / denominator);
    }
    return wma;
}

// Relative Strength Index (RSI)
std::vector<double> Indicator::RSI(const std::vector<double>& prices, int windowSize) {
    std::vector<double> rsi;
    std::vector<double> gains;
    std::vector<double> losses;
    for (int i = 1; i < prices.size(); ++i) {
        double diff = prices[i] - prices[i - 1];
        if (diff >= 0) {
            gains.push_back(diff);
            losses.push_back(0);
        } else {
            gains.push_back(0);
            losses.push_back(-diff);
        }
    }
    double avgGain = std::accumulate(gains.begin(), gains.begin() + windowSize, 0.0) / windowSize;
    double avgLoss = std::accumulate(losses.begin(), losses.begin() + windowSize, 0.0) / windowSize;
    rsi.push_back(100.0 - (100.0 / (1 + avgGain / avgLoss)));
    for (int i = windowSize; i < prices.size(); ++i) {
        avgGain = (avgGain * (windowSize - 1) + gains[i]) / windowSize;
        avgLoss = (avgLoss * (windowSize - 1) + losses[i]) / windowSize;
        rsi.push_back(100.0 - (100.0 / (1 + avgGain / avgLoss)));
    }
    return rsi;
}

// Bollinger Bands
std::vector<std::vector<double>> Indicator::BollingerBands(const std::vector<double>& prices, int windowSize) {
    std::vector<double> middleBand = SMA(prices, windowSize);
    std::vector<double> upperBand;
    std::vector<double> lowerBand;
    for (size_t i = 0; i < middleBand.size(); ++i) {
        double sum = 0.0;
        for (size_t j = std::max(0, static_cast<int>(i) - windowSize + 1); j <= static_cast<int>(i); ++j) {
            sum += std::pow(prices[j] - middleBand[i], 2);
        }
        double stdDev = std::sqrt(sum / std::min(windowSize, static_cast<int>(i) + 1));
        upperBand.push_back(middleBand[i] + 2 * stdDev);
        lowerBand.push_back(middleBand[i] - 2 * stdDev);
    }
    return {upperBand, middleBand, lowerBand};
}



// Stochastic Oscillator
std::vector<double> Indicator::StochasticOscillator(const std::vector<double>& prices, int windowSize) {
    std::vector<double> stochOsc;
    for (int i = windowSize - 1; i < prices.size(); ++i) {
        double minPrice = *std::min_element(prices.begin() + i - windowSize + 1, prices.begin() + i + 1);
        double maxPrice = *std::max_element(prices.begin() + i - windowSize + 1, prices.begin() + i + 1);
        double stoch = (prices[i] - minPrice) / (maxPrice - minPrice) * 100.0;
        stochOsc.push_back(stoch);
    }
    return stochOsc;
}

// Moving Average Convergence Divergence (MACD)
std::vector<std::vector<double>> Indicator::MACD(const std::vector<double>& prices, int shortPeriod, int longPeriod, int signalPeriod) {
    std::vector<double> shortEMA = EMA(prices, shortPeriod);
    std::vector<double> longEMA = EMA(prices, longPeriod);
    std::vector<double> macd;
    for (size_t i = 0; i < prices.size(); ++i) {
        macd.push_back(shortEMA[i] - longEMA[i]);
    }
    std::vector<double> signalLine = EMA(macd, signalPeriod);
    return {macd, signalLine};
}


// Volume Weighted Moving Average (VWAP)
std::vector<double> Indicator::VWAP(const std::vector<double>& prices, const std::vector<double>& volumes, int windowSize) {
    std::vector<double> vwap;
    for (int i = 0; i <= prices.size() - windowSize; ++i) {
        double sumPriceVolume = 0.0;
        double sumVolume = 0.0;
        for (int j = i; j < i + windowSize; ++j) {
            sumPriceVolume += prices[j] * volumes[j];
            sumVolume += volumes[j];
        }
        vwap.push_back(sumPriceVolume / sumVolume);
    }
    return vwap;
}

// Momentum Indicator
std::vector<double> Indicator::Momentum(const std::vector<double>& prices, int windowSize) {
    std::vector<double> momentum;
    for (int i = windowSize - 1; i < prices.size(); ++i) {
        momentum.push_back(prices[i] - prices[i - windowSize + 1]);
    }
    return momentum;
}

// Average True Range (ATR)
std::vector<double> Indicator::ATR(const std::vector<double>& highs, const std::vector<double>& lows, const std::vector<double>& closes, int windowSize) {
    std::vector<double> atr;
    std::vector<double> trueRanges;
    for (size_t i = 1; i < highs.size(); ++i) {
        double highLow = highs[i] - lows[i];
        double highClose = std::abs(highs[i] - closes[i - 1]);
        double lowClose = std::abs(lows[i] - closes[i - 1]);
        trueRanges.push_back(std::max({highLow, highClose, lowClose}));
    }

    double sum = std::accumulate(trueRanges.begin(), trueRanges.begin() + windowSize, 0.0);
    atr.push_back(sum / windowSize);

    for (size_t i = windowSize; i < trueRanges.size(); ++i) {
        double currentATR = (atr.back() * (windowSize - 1) + trueRanges[i]) / windowSize;
        atr.push_back(currentATR);
    }

    return atr;
}

// Average Directional Index (ADX)
std::vector<double> Indicator::ADX(const std::vector<double>& highs, const std::vector<double>& lows, const std::vector<double>& closes, int windowSize) {
    std::vector<double> adx;
    std::vector<double> tr = ATR(highs, lows, closes, 1);
    std::vector<double> plusDM;
    std::vector<double> minusDM;
    for (size_t i = 1; i < highs.size(); ++i) {
        double upMove = highs[i] - highs[i - 1];
        double downMove = lows[i - 1] - lows[i];
        plusDM.push_back((upMove > downMove && upMove > 0) ? upMove : 0);
        minusDM.push_back((downMove > upMove && downMove > 0) ? downMove : 0);
    }

    std::vector<double> smoothedPlusDM = EMA(plusDM, windowSize);
    std::vector<double> smoothedMinusDM = EMA(minusDM, windowSize);

    for (size_t i = 0; i < smoothedPlusDM.size(); ++i) {
        double diPlus = 100 * (smoothedPlusDM[i] / tr[i + 1]);
        double diMinus = 100 * (smoothedMinusDM[i] / tr[i + 1]);
        double dx = 100 * (std::abs(diPlus - diMinus) / (diPlus + diMinus));
        adx.push_back(dx);
    }

    adx = EMA(adx, windowSize);

    return adx;
}

// Accumulation/Distribution Line (ADL)
std::vector<double> Indicator::ADL(const std::vector<double>& highs, const std::vector<double>& lows, const std::vector<double>& closes, const std::vector<double>& volumes) {
    std::vector<double> adl;
    double cumulativeADL = 0.0;
    for (size_t i = 0; i < highs.size(); ++i) {
        double mfm = ((closes[i] - lows[i]) - (highs[i] - closes[i])) / (highs[i] - lows[i]);
        double mfv = mfm * volumes[i];
        cumulativeADL += mfv;
        adl.push_back(cumulativeADL);
    }
    return adl;
}

// Double Exponential Moving Average (DEMA)
std::vector<double> Indicator::DEMA(const std::vector<double>& prices, int windowSize) {
    std::vector<double> ema1 = EMA(prices, windowSize);
    std::vector<double> ema2 = EMA(ema1, windowSize);
    std::vector<double> dema;
    for (size_t i = 0; i < ema1.size(); ++i) {
        dema.push_back(2 * ema1[i] - ema2[i]);
    }
    return dema;
}

// Triple Exponential Moving Average (TEMA)
std::vector<double> Indicator::TEMA(const std::vector<double>& prices, int windowSize) {
    std::vector<double> ema1 = EMA(prices, windowSize);
    std::vector<double> ema2 = EMA(ema1, windowSize);
    std::vector<double> ema3 = EMA(ema2, windowSize);
    std::vector<double> tema;
    for (size_t i = 0; i < ema1.size(); ++i) {
        tema.push_back(3 * (ema1[i] - ema2[i]) + ema3[i]);
    }
    return tema;
}

// Relative Strength Momentum Indicator (RSMI)
std::vector<double> Indicator::RSMI(const std::vector<double>& prices, int windowSize) {
    std::vector<double> rsmi;
    std::vector<double> momentum = Momentum(prices, windowSize);
    std::vector<double> rsi = RSI(prices, windowSize);
    for (size_t i = 0; i < momentum.size(); ++i) {
        rsmi.push_back(momentum[i] * rsi[i] / 100);
    }
    return rsmi;
}

// Accelerator/Decelerator Oscillator (ADO)
std::vector<double> Indicator::ADO(const std::vector<double>& prices, int shortPeriod, int longPeriod) {
    std::vector<double> ao = MACD(prices, shortPeriod, longPeriod, 0)[0];
    std::vector<double> ado;
    std::vector<double> sma = SMA(ao, shortPeriod);
    for (size_t i = 0; i < ao.size(); ++i) {
        ado.push_back(ao[i] - sma[i]);
    }
    return ado;
}

// Chaikin Oscillator
std::vector<double> Indicator::ChaikinOscillator(const std::vector<double>& closes, const std::vector<double>& volumes, int shortPeriod, int longPeriod) {
    std::vector<double> adl = ADL(closes, closes, closes, volumes);
    std::vector<double> shortEMA = EMA(adl, shortPeriod);
    std::vector<double> longEMA = EMA(adl, longPeriod);
    std::vector<double> cho;
    for (size_t i = 0; i < shortEMA.size(); ++i) {
        cho.push_back(shortEMA[i] - longEMA[i]);
    }
    return cho;
}

// Price Volume Trend (PVT)
std::vector<double> Indicator::PVT(const std::vector<double>& prices, const std::vector<double>& volumes) {
    std::vector<double> pvt;
    double cumulativePVT = 0.0;
    for (size_t i = 1; i < prices.size(); ++i) {
        cumulativePVT += volumes[i] * (prices[i] - prices[i - 1]) / prices[i - 1];
        pvt.push_back(cumulativePVT);
    }
    return pvt;
}

// Bollinger Band Width
std::vector<double> Indicator::BollingerBandWidth(const std::vector<double>& prices, int windowSize) {
    std::vector<double> bbWidth;
    std::vector<double> middleBand = SMA(prices, windowSize);
    for (size_t i = 0; i < middleBand.size(); ++i) {
        double sum = 0.0;
        for (size_t j = std::max(0, static_cast<int>(i) - windowSize + 1); j <= static_cast<int>(i); ++j) {
            sum += std::pow(prices[j] - middleBand[i], 2);
        }
        double stdDev = std::sqrt(sum / std::min(static_cast<size_t>(windowSize), i + 1));
        double upperBand = middleBand[i] + 2 * stdDev;
        double lowerBand = middleBand[i] - 2 * stdDev;
        bbWidth.push_back(upperBand - lowerBand);
    }
    return bbWidth;
}


// Williams %R
std::vector<double> Indicator::WilliamsR(const std::vector<double>& highs, const std::vector<double>& lows, const std::vector<double>& closes, int windowSize) {
    std::vector<double> williamsR;
    for (size_t i = windowSize - 1; i < closes.size(); ++i) {
        double highestHigh = *std::max_element(highs.begin() + i - windowSize + 1, highs.begin() + i + 1);
        double lowestLow = *std::min_element(lows.begin() + i - windowSize + 1, lows.begin() + i + 1);
        williamsR.push_back((highestHigh - closes[i]) / (highestHigh - lowestLow) * -100.0);
    }
    return williamsR;
}

// Parallel Channel
std::vector<double> Indicator::ParallelChannel(const std::vector<double>& highs, const std::vector<double>& lows, int windowSize) {
    std::vector<double> channel;
    for (size_t i = windowSize - 1; i < highs.size(); ++i) {
        double highestHigh = *std::max_element(highs.begin() + i - windowSize + 1, highs.begin() + i + 1);
        double lowestLow = *std::min_element(lows.begin() + i - windowSize + 1, lows.begin() + i + 1);
        channel.push_back(highestHigh - lowestLow);
    }
    return channel;
}

// Know Sure Thing (KST)
std::vector<double> Indicator::KST(const std::vector<double>& prices, int ROC1Period, int ROC2Period, int ROC3Period, int ROC4Period, int SMA1Period, int SMA2Period, int SMA3Period, int SMA4Period) {
    std::vector<double> roc1 = Momentum(prices, ROC1Period);
    std::vector<double> roc2 = Momentum(prices, ROC2Period);
    std::vector<double> roc3 = Momentum(prices, ROC3Period);
    std::vector<double> roc4 = Momentum(prices, ROC4Period);

    std::vector<double> sma1 = SMA(roc1, SMA1Period);
    std::vector<double> sma2 = SMA(roc2, SMA2Period);
    std::vector<double> sma3 = SMA(roc3, SMA3Period);
    std::vector<double> sma4 = SMA(roc4, SMA4Period);

    std::vector<double> kst;
    for (size_t i = 0; i < sma1.size(); ++i) {
        kst.push_back(sma1[i] + sma2[i] * 2 + sma3[i] * 3 + sma4[i] * 4);
    }
    return kst;
}

// Parabolic SAR
std::vector<double> Indicator::ParabolicSAR(const std::vector<double>& highs, const std::vector<double>& lows, double accelerationFactorStart, double accelerationFactorIncrement) {
    std::vector<double> sar;
    double af = accelerationFactorStart;
    double ep = highs[0]; // extreme point
    double sarPrev = lows[0];
    bool uptrend = true;

    for (size_t i = 1; i < highs.size(); ++i) {
        double sarCurrent;
        if (uptrend) {
            sarCurrent = sarPrev + af * (ep - sarPrev);
            if (highs[i] > ep) {
                ep = highs[i];
                af = std::min(af + accelerationFactorIncrement, 0.2);
            }
            if (lows[i] < sarCurrent) {
                uptrend = false;
                sarCurrent = ep;
                ep = lows[i];
                af = accelerationFactorStart;
            }
        } else {
            sarCurrent = sarPrev + af * (ep - sarPrev);
            if (lows[i] < ep) {
                ep = lows[i];
                af = std::min(af + accelerationFactorIncrement, 0.2);
            }
            if (highs[i] > sarCurrent) {
                uptrend = true;
                sarCurrent = ep;
                ep = highs[i];
                af = accelerationFactorStart;
            }
        }
        sar.push_back(sarCurrent);
        sarPrev = sarCurrent;
    }

    return sar;
}

// Directional Movement Index (DMI)
std::vector<double> Indicator::DMI(const std::vector<double>& highs, const std::vector<double>& lows, const std::vector<double>& closes, int windowSize) {
    std::vector<double> plusDM;
    std::vector<double> minusDM;
    for (size_t i = 1; i < highs.size(); ++i) {
        double upMove = highs[i] - highs[i - 1];
        double downMove = lows[i - 1] - lows[i];
        plusDM.push_back((upMove > downMove && upMove > 0) ? upMove : 0);
        minusDM.push_back((downMove > upMove && downMove > 0) ? downMove : 0);
    }

    std::vector<double> smoothedPlusDM = EMA(plusDM, windowSize);
    std::vector<double> smoothedMinusDM = EMA(minusDM, windowSize);
    std::vector<double> tr = ATR(highs, lows, closes, 1);

    std::vector<double> plusDI;
    std::vector<double> minusDI;
    for (size_t i = 0; i < smoothedPlusDM.size(); ++i) {
        plusDI.push_back(100 * (smoothedPlusDM[i] / tr[i + 1]));
        minusDI.push_back(100 * (smoothedMinusDM[i] / tr[i + 1]));
    }

    std::vector<double> dx;
    for (size_t i = 0; i < plusDI.size(); ++i) {
        dx.push_back(100 * (std::abs(plusDI[i] - minusDI[i]) / (plusDI[i] + minusDI[i])));
    }

    std::vector<double> adx = EMA(dx, windowSize);

    return adx;
}

// Negative Volume Index (NVI)
std::vector<double> Indicator::NVI(const std::vector<double>& closes, const std::vector<double>& volumes) {
    std::vector<double> nvi;
    nvi.push_back(1000.0); // Initial value
    for (size_t i = 1; i < closes.size(); ++i) {
        if (volumes[i] < volumes[i - 1]) {
            double change = (closes[i] - closes[i - 1]) / closes[i - 1] * 100;
            nvi.push_back(nvi.back() + change);
        } else {
            nvi.push_back(nvi.back());
        }
    }
    return nvi;
}

// Relative Price Oscillator (RPO)
std::vector<double> Indicator::RPO(const std::vector<double>& prices, int shortPeriod, int longPeriod) {
    std::vector<double> shortEMA = EMA(prices, shortPeriod);
    std::vector<double> longEMA = EMA(prices, longPeriod);
    std::vector<double> rpo;
    for (size_t i = 0; i < prices.size(); ++i) {
        rpo.push_back(100 * (shortEMA[i] - longEMA[i]) / longEMA[i]);
    }
    return rpo;
}

// Trading Channel Index (TCI)
std::vector<double> Indicator::TCI(const std::vector<double>& prices, int windowSize) {
    std::vector<double> tci;
    for (size_t i = windowSize - 1; i < prices.size(); ++i) {
        double sum = 0.0;
        for (size_t j = i - windowSize + 1; j <= i; ++j) {
            sum += (prices[j] - prices[j - 1]);
        }
        tci.push_back(sum / windowSize);
    }
    return tci;
}
