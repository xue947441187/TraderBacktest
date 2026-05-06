//
// Created by 薛新岗 on 2024/5/15.
//

#ifndef TRADERBACKTEST_BACKTEST_H
#define TRADERBACKTEST_BACKTEST_H

#include <vector>
#include "trading_module.h" // 引入交易模块的头文件
#include "stocks.h" // 引入交易模块的头文件
// 枚举类定义
enum class ConditionType {
    GreaterThan,
    LessThan,
    EqualTo,
    GoldenCross,
    DeathCross
};

// ConditionTypeConverter类定义
class ConditionTypeConverter {
public:
    ConditionTypeConverter();

    int toEnum(const std::string& str) const;
    std::string toString(int value) const;

private:
    std::unordered_map<std::string, ConditionType> strToEnum;
    std::unordered_map<int, std::string> enumToStr;
};


class TradeLogic{
public:
    bool handleComparison(const StockData& data,int currentIndex, const std::string& indicator, double value,std::string& currentMode);
    static bool isGreaterThan(const StockData& data, const std::string& indicator, double value);
    static bool isLessThan(const StockData& data, const std::string& indicator, double value);
    static bool isEqualTo(const StockData& data, const std::string& indicator, double value);
    static bool isGoldenCross(const StockData& previous, const StockData& current, const std::string& shortMA, const std::string& longMA);
    static bool isDeathCross(const StockData& previous, const StockData& current, const std::string& shortMA, const std::string& longMA);

};
class BacktestModule {

public:
    // 执行回测

    void run();
    void calculateProfitAndPrintResult(const TradingModule &tradingModule);
    void setCommissionPercentage(double commissionPercentage);
    void setInitCash(double InitCash);
    void addData(const std::vector<StockData> &data);
    void setEntryMode(const std::string& mode);
    void setExitMode(const std::string& mode);
    void setEntryType(const std::string& type);
    void setExitType(const std::string& type);
    BacktestModule(){
        this->tradeLogic = TradeLogic();
    };
private:
    double endingCapital{};
    double commissionPercentage{};
    double initCash{};
    std::string entryMode = "close";
    std::string exitMode = "close";
    std::string currentMode;
    std::string entryType;
    std::string exitType;

    TradeLogic tradeLogic;
    TradingModule tradingModule;

    std::vector<StockData> data;

    void setCurrentMode(const std::string& mode);
    void backtest();

    void executeBuy(TradingModule &tradingModule, const StockData &stock, double &capital, double &totalCommission,double &commission);

    void executeSell(TradingModule &tradingModule, const StockData &stock, double &capital, double &totalCommission,double &commission);

    bool
    checkTradingConditions(const StockData &previous, const StockData &current, const std::string &indicator,
                           double value,
                           const std::string &conditionType);


};


double getIndicator(const StockData& stock, const std::string& indicatorName);


// 交易逻辑


#endif //TRADERBACKTEST_BACKTEST_H
