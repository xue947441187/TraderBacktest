//
// trading_module.cpp (rewritten)
//
#include "trading_module.h"

TradingModule::TradingModule(double commissionPercentage, double slippageRate, double initialCapital)
        : engine_(initialCapital, commissionPercentage, slippageRate),
          commissionPercentage_(commissionPercentage),
          slippageRate_(slippageRate) {}

void TradingModule::setInitialCapital(double capital) {
    // Rebuild engine with new capital while preserving commission/slippage settings.
    engine_ = EnhancedTradingEngine(capital, commissionPercentage_, slippageRate_);
}

double TradingModule::calculateCommission(double price, int quantity) const {
    return std::abs(price * quantity) * commissionPercentage_;
}

double TradingModule::calculateProfitLoss(double buyPrice, double sellPrice, int quantity) const {
    return (sellPrice - buyPrice) * quantity;
}

bool TradingModule::buy(const std::string& date, const std::string& symbol, double price, int quantity) {
    buyQuantity_ = quantity;
    return engine_.execute_order(symbol, OrderType::MarketBuy, price, quantity, date);
}

bool TradingModule::sell(const std::string& date, const std::string& symbol, double price, int quantity) {
    return engine_.execute_order(symbol, OrderType::MarketSell, price, quantity, date);
}

bool TradingModule::executeTrade(const std::string& date, const std::string& symbol, double price, int quantity) {
    // default to market buy for legacy callers
    return engine_.execute_order(symbol, OrderType::MarketBuy, price, quantity, date);
}

std::vector<TradeInfo> TradingModule::getTradeInfoList() const {
    auto trades = engine_.get_trade_history();
    std::vector<TradeInfo> out;
    out.reserve(trades.size());
    for (const auto& t : trades) {
        TradeInfo ti;
        ti.date = t.timestamp;
        ti.symbol = t.symbol;
        ti.price = t.executed_price;
        ti.quantity = t.executed_quantity;
        ti.type = (order_side(t.order_type) == Side::Buy) ? "buy" : "sell";
        ti.commissionPercentage = t.commission_rate;
        ti.commission = t.commission;
        ti.position_effect = t.position_effect;
        out.push_back(std::move(ti));
    }
    return out;
}
