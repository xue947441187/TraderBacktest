//
// Reorganized & modernized TA-Lib strategy layer (DataFrame-style LineManager)
// Compatible with existing factory API (string columnNames + StrategyParams)
//
// 2026-02-01
//
#ifndef TRADERBACKTEST_TALIBSTRATEGY_H
#define TRADERBACKTEST_TALIBSTRATEGY_H

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <algorithm>

#include "lineManager/lineManager.h" // expects optimized LineManager with df-style col<>() API

// -------------------------
// StrategyParams
// -------------------------
class StrategyParams {
public:
    std::map<std::string, double> numericParams;
    std::map<std::string, std::string> stringParams;

    void setNumericParam(const std::string& key, double value) { numericParams[key] = value; }
    void setStringParam(const std::string& key, const std::string& value) { stringParams[key] = value; }

    double getNumericParam(const std::string& key) const {
        auto it = numericParams.find(key);
        if (it != numericParams.end()) return it->second;
        throw std::invalid_argument("Numeric parameter not found: " + key);
    }

    // If you want a safe default without throwing:
    double getNumericParamOr(const std::string& key, double defaultValue) const {
        auto it = numericParams.find(key);
        return (it != numericParams.end()) ? it->second : defaultValue;
    }

    std::string getStringParam(const std::string& key) const {
        auto it = stringParams.find(key);
        if (it != stringParams.end()) return it->second;
        throw std::invalid_argument("String parameter not found: " + key);
    }

    std::string getStringParamOr(const std::string& key, const std::string& defaultValue) const {
        auto it = stringParams.find(key);
        return (it != stringParams.end()) ? it->second : defaultValue;
    }
};

// -------------------------
// TALibStrategy Base
// -------------------------
class TALibStrategy {
public:
    virtual ~TALibStrategy() = default;

    // Run strategy and write outputs back to manager
    virtual void process() = 0;

    // For logging/UI
    virtual std::string strategyName() const = 0;

    // Backward compatible: accepts CSV string like "Close" or "Open,High,Low,Close"
    void setInputColumns(const std::string& columnNamesCsv) {
        inputColumns = splitCsv(columnNamesCsv);
        if (inputColumns.empty()) {
            inputColumns.push_back(defaultPriceColumn());
        }
    }

    // Preferred overload
    void setInputColumns(const std::vector<std::string>& cols) {
        inputColumns = cols;
        if (inputColumns.empty()) inputColumns.push_back(defaultPriceColumn());
    }

    void setParams(const StrategyParams& p) { params = p; }

protected:
    explicit TALibStrategy(const boost::shared_ptr<LineManager::LineManager>& manager)
            : manager(manager) {}

    // Default input column if caller provides nothing
    static std::string defaultPriceColumn() { return "Close"; }

    // Utility: split "A,B,C" into {"A","B","C"} (trim spaces)
    static std::vector<std::string> splitCsv(const std::string& csv) {
        std::vector<std::string> out;
        std::stringstream ss(csv);
        std::string token;
        while (std::getline(ss, token, ',')) {
            // trim spaces
            token.erase(token.begin(), std::find_if(token.begin(), token.end(), [](unsigned char ch){ return !std::isspace(ch); }));
            token.erase(std::find_if(token.rbegin(), token.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), token.end());
            if (!token.empty()) out.push_back(token);
        }
        return out;
    }

    // Extract numeric series by row index (int -> double).
    // This matches the classic OHLC storage pattern and keeps strategies simple & fast.
    // Returns: (indices [0..n-1], values)
    std::pair<std::vector<int>, std::vector<double>> getSeriesIntDouble(const std::string& colName) const {

        auto line = manager->getLine<int, double>(colName);
        if (!line) {
            throw std::invalid_argument("Column not found or type mismatch (expected int->double): " + colName);
        }
        auto values = line->getValuesList();
        std::vector<int> idx(values.size());
        for (size_t i = 0; i < values.size(); ++i)
            idx[i] = static_cast<int>(i);

        return {std::move(idx), std::move(values)};
    }

    // Write output numeric series to a column (int -> double) with same index size.
    void writeSeriesIntDouble(const std::string& outColName,
                              const std::vector<int>& indices,
                              const std::vector<double>& values) const {
        manager->col<int, double>(outColName).set(indices, values);
    }

protected:
    boost::shared_ptr<LineManager::LineManager> manager;
    std::vector<std::string> inputColumns;
    StrategyParams params;
};

// -------------------------
// Concrete Strategies
// -------------------------
class MovingAverageStrategy : public TALibStrategy {
public:
    explicit MovingAverageStrategy(const boost::shared_ptr<LineManager::LineManager>& manager)
            : TALibStrategy(manager) {}

    std::string strategyName() const override { return "MovingAverage"; }
    void process() override;
};

class RSIStrategy : public TALibStrategy {
public:
    explicit RSIStrategy(const boost::shared_ptr<LineManager::LineManager>& manager)
            : TALibStrategy(manager) {}

    std::string strategyName() const override { return "RSI"; }
    void process() override;
};

class MACDStrategy : public TALibStrategy {
public:
    explicit MACDStrategy(const boost::shared_ptr<LineManager::LineManager>& manager)
            : TALibStrategy(manager) {}

    std::string strategyName() const override { return "MACD"; }
    void process() override;
};

#endif // TRADERBACKTEST_TALIBSTRATEGY_H
