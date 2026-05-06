//
// EnhancedTradingEngine.cpp (rewritten)
//
#include "EnhancedTradingEngine.h"

#include <cmath>

double EnhancedTradingEngine::apply_slippage(OrderType type, double price) const noexcept {
    if (price <= 0.0) return price;
    if (slippage_rate_ <= 0.0) return price;
    // Buy pays worse, sell receives worse
    return is_buy_order(type) ? price * (1.0 + slippage_rate_)
                              : price * (1.0 - slippage_rate_);
}

bool EnhancedTradingEngine::validate_order(const std::string& symbol, OrderType type, double price, int quantity) const {
    if (symbol.empty()) return false;
    if (quantity <= 0) return false;
    if (!(price > 0.0)) return false;

    const double exec_price = apply_slippage(type, price);
    const double gross = exec_price * quantity;
    const double commission = std::abs(gross) * commission_rate_;

    if (is_buy_order(type)) {
        // Need enough cash for (cost + commission)
        return (gross + commission) <= available_cash_;
    }

    // Sell: must have enough shares (long-only)
    const Position* pos = pos_mgr_.find_position(symbol);
    const int held = pos ? pos->total_quantity : 0;
    return quantity <= held;
}

std::string EnhancedTradingEngine::classify_effect(const std::string& symbol, OrderType type, int qty) const {
    const Position* pos = pos_mgr_.find_position(symbol);
    const int held = pos ? pos->total_quantity : 0;

    if (is_buy_order(type)) {
        return (held == 0) ? "open" : "increase";
    }

    if (held == 0) return "invalid";
    if (qty == held) return "close";
    return "reduce";
}

bool EnhancedTradingEngine::execute_order(const std::string& symbol,
                                          OrderType type,
                                          double price,
                                          int quantity,
                                          const std::string& trade_timestamp) {
    if (!validate_order(symbol, type, price, quantity)) return false;

    const double exec_price = apply_slippage(type, price);
    const std::string effect = classify_effect(symbol, type, quantity);

    EnhancedTrade trade(
            trade_timestamp,
            symbol,
            type,
            price,
            exec_price,
            quantity,
            commission_rate_,
            slippage_rate_,
            effect
    );

    update_account(trade);
    trade_history_.push_back(trade);
    return true;
}

void EnhancedTradingEngine::update_account(const EnhancedTrade& trade) {
    const double gross = trade.executed_price * trade.executed_quantity;
    total_commission_paid_ += trade.commission;

    if (is_buy_order(trade.order_type)) {
        available_cash_ -= (gross + trade.commission);
    } else {
        available_cash_ += (gross - trade.commission);
    }

    // Update position and realized pnl
    const double pnl_delta = pos_mgr_.process_trade(trade);
    total_realized_pnl_ += pnl_delta;
}

std::vector<EnhancedTrade> EnhancedTradingEngine::get_trade_history() const {
    auto sorted = trade_history_;
    std::sort(sorted.begin(), sorted.end(),
              [](const EnhancedTrade& a, const EnhancedTrade& b) {
                  return a.timestamp < b.timestamp;
              });
    return sorted;
}
