//
// backtest.cpp (rewritten)
// Fixes:
// - Remove incorrect "totalProfit" calculation
// - Use TradingModule's cash/positions to compute ending capital
// - Validate buy/sell execution
//
#include <iomanip>
#include <limits>
#include <cmath>
#include "backtest.h"

void BacktestModule::backtest() {
    // TradingModule now owns the cash/commission accounting.
    TradingModule tradingModule(this->commissionPercentage, /*slippageRate=*/0.0, /*initialCapital=*/this->initCash);

    bool holdingStock = false;

    StockData previous;
    StockData current;

    this->setCurrentMode(this->entryMode);

    for (int i = 0; i < static_cast<int>(data.size()); ++i) {
        current = data[i];

        // indicator sanity
        if (i <= 1) continue;
        if (std::isnan(getIndicator(current, this->currentMode)) ||
            std::isnan(getIndicator(data[i - 1], this->currentMode))) {
            continue;
        }

        previous = data[i - 1];

        // switch mode based on holding
        this->setCurrentMode(holdingStock ? this->exitMode : this->entryMode);

        // Entry: goldenCross
        if (!holdingStock && this->checkTradingConditions(previous, current, this->currentMode, 0.3, "goldenCross")) {
            // buy as many shares as possible
            const int quantity = static_cast<int>(std::floor(tradingModule.cash() / current.close));
            if (quantity > 0) {
                const bool ok = tradingModule.buy(current.date, current.symbol, current.close, quantity);
                if (ok) {
                    tradingModule.setBuyQuantity(quantity);
                    holdingStock = true;
                }
            }
        }
            // Exit: deathCross
        else if (holdingStock && this->checkTradingConditions(previous, current, this->currentMode, 0.7, "deathCross")) {
            const int quantity = tradingModule.getBuyQuantity();
            if (quantity > 0) {
                const bool ok = tradingModule.sell(current.date, current.symbol, current.close, quantity);
                if (ok) {
                    tradingModule.setBuyQuantity(0);
                    holdingStock = false;
                }
            }
        }
    }

    calculateProfitAndPrintResult(tradingModule);
}

bool BacktestModule::checkTradingConditions(const StockData& previous,
                                            const StockData& current,
                                            const std::string& indicator,
                                            double value,
                                            const std::string& conditionType) {
    if (conditionType == "greaterThan") {
        return current.indicators.at(indicator) > value;
    } else if (conditionType == "lessThan") {
        return current.indicators.at(indicator) < value;
    } else if (conditionType == "equalTo") {
        return current.indicators.at(indicator) == value;
    } else if (conditionType == "goldenCross") {
        return previous.indicators.at(indicator) <= current.indicators.at(this->currentMode);
    } else if (conditionType == "deathCross") {
        return previous.indicators.at(indicator) >= current.indicators.at(this->currentMode);
    }
    throw std::invalid_argument("Invalid condition type");
}

// These helpers are now unused but kept to satisfy the header.
// We keep them simple and forward to TradingModule.
void BacktestModule::executeBuy(TradingModule& tradingModule, const StockData& stock, double& capital, double& totalCommission, double& commission) {
    (void)capital; (void)totalCommission; (void)commission;
    const int quantity = static_cast<int>(std::floor(tradingModule.cash() / stock.close));
    if (quantity > 0) {
        tradingModule.buy(stock.date, stock.symbol, stock.close, quantity);
        tradingModule.setBuyQuantity(quantity);
    }
}

void BacktestModule::executeSell(TradingModule& tradingModule, const StockData& stock, double& capital, double& totalCommission, double& commission) {
    (void)capital; (void)totalCommission; (void)commission;
    const int quantity = tradingModule.getBuyQuantity();
    if (quantity > 0) {
        tradingModule.sell(stock.date, stock.symbol, stock.close, quantity);
        tradingModule.setBuyQuantity(0);
    }
}

void BacktestModule::calculateProfitAndPrintResult(const TradingModule& tradingModule) {
    // If strategy ends flat, endingCapital == cash.
    // If still holding, you may want to mark-to-market using last close.
    endingCapital = tradingModule.cash();

    std::cout << "Initial Cash: " << std::fixed << std::setprecision(2) << this->initCash << "\n";
    std::cout << "Ending Cash : " << std::fixed << std::setprecision(2) << endingCapital << "\n";
    std::cout << "Realized PnL: " << std::fixed << std::setprecision(2) << tradingModule.realizedPnL() << "\n";
    std::cout << "Commission : " << std::fixed << std::setprecision(2) << tradingModule.totalCommission() << "\n";

    // Print trades
    const auto trades = tradingModule.getTradeInfoList();
    std::cout << "\nTrades (" << trades.size() << "):\n";
    for (const auto& t : trades) {
        std::cout << t.date << "  " << t.symbol << "  " << t.type
                  << "  qty=" << t.quantity
                  << "  px=" << std::fixed << std::setprecision(4) << t.price
                  << "  comm=" << std::fixed << std::setprecision(4) << t.commission
                  << "  effect=" << t.position_effect
                  << "\n";
    }
}
