//
// trading_module.h (rewritten)
// A high-level facade around EnhancedTradingEngine, keeping your old API.
// This makes backtest code simple while ensuring correct accounting.
//
#ifndef TRADERBACKTEST_TRADING_MODULE_H
#define TRADERBACKTEST_TRADING_MODULE_H

#include <vector>
#include <string>
#include <cmath>

#include "EnhancedTradingEngine.h"

// Kept for compatibility with your existing backtest result code.
struct TradeInfo {
    std::string date;
    std::string symbol;
    double price = 0.0;               // executed price
    int quantity = 0;
    std::string type;                 // "buy" or "sell"
    double commissionPercentage = 0.0;
    double commission = 0.0;
    std::string position_effect;      // open/increase/reduce/close
};

class TradingModule {
public:
    // commissionPercentage matches your existing parameter name.
    // initialCapital optional; if not set, call setInitialCapital() before trading.
    explicit TradingModule(double commissionPercentage = 0.0,
                           double slippageRate = 0.0,
                           double initialCapital = 0.0);

    // Legacy API:
    // - buy/sell record a trade if it is valid.
    // - returns bool to let caller handle rejected trades (was void before).
    bool buy(const std::string& date, const std::string& symbol, double price, int quantity);
    bool sell(const std::string& date, const std::string& symbol, double price, int quantity);

    // Optional legacy method (kept). Executes market buy by default.
    bool executeTrade(const std::string& date, const std::string& symbol, double price, int quantity);

    // Simple helpers
    double calculateCommission(double price, int quantity) const;
    double calculateProfitLoss(double buyPrice, double sellPrice, int quantity) const;

    // Results
    std::vector<TradeInfo> getTradeInfoList() const;
    double cash() const noexcept { return engine_.cash(); }
    double realizedPnL() const noexcept { return engine_.total_realized_pnl(); }
    double totalCommission() const noexcept { return engine_.total_commission_paid(); }

    // Old behavior
    void setBuyQuantity(int quantity) { buyQuantity_ = quantity; }
    int getBuyQuantity() const noexcept { return buyQuantity_; }

    void setInitialCapital(double capital);

private:
    EnhancedTradingEngine engine_;
    double commissionPercentage_ = 0.0;
    double slippageRate_ = 0.0;

    int buyQuantity_ = 0;
};

#endif //TRADERBACKTEST_TRADING_MODULE_H
