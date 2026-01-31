//
// demo_full_latest.cpp
// End-to-end demo using the *latest* pipeline:
//   1) readLinesStockData("aapl.csv", df, "Date")
//   2) Run TA-Lib MACD strategy -> outputs columns: MACD, MACDSignal, MACDHist
//   3) Execute a simple trading logic using TradingModule (latest rewritten)
//      - Long-only
//      - Buy when MACD crosses above Signal
//      - Sell when MACD crosses below Signal
//
// Build (via project CMake):
//   mkdir build && cd build
//   cmake ..
//   cmake --build .
// Run:
//   ./demo_full_latest   (or demo_full_latest.exe)
//
// Note: CMake in src/examples copies aapl.csv next to the executable,
// so readLinesStockData("aapl.csv", ...) will work.
//
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

#include "lineManager/lineManager.h"
#include "stocks.h"
#include "strategy/StrategyFactory.h"
#include "strategy/TALibStrategy.h"
#include "trading_module.h"

static inline bool is_nan(double x) noexcept {
    return std::isnan(x);
}

int main() {
    try {
        // -----------------------------
        // 1) Load CSV into DataFrame
        // -----------------------------
        auto df = LineManager::LineManager::create();
        readLinesStockData("aapl.csv", df, "Date");

        std::cout << "Loaded rows: " << df->getLineCount() << "\n";

        // -----------------------------
        // 2) Compute MACD on Close
        // -----------------------------
        TALibStrategyFactory factory(df);

        StrategyParams params;
        params.setNumericParam("FastPeriod", 12);
        params.setNumericParam("SlowPeriod", 26);
        params.setNumericParam("SignalPeriod", 9);

        auto macd = factory.createStrategy("MACD", "Close", params);
        macd->process();

        // Grab needed columns (latest LineManager supports getLine<>() safely)
        auto dateLine  = df->getLine<int, std::string>("Date");
        auto closeLine = df->getLine<int, double>("Close");
        auto macdLine  = df->getLine<int, double>("MACD");
        auto sigLine   = df->getLine<int, double>("MACDSignal");

        if (!dateLine || !closeLine || !macdLine || !sigLine) {
            std::cerr << "Missing required columns. Ensure CSV has Date/Close and strategy produced MACD/MACDSignal.\n";
            return 1;
        }

        // -----------------------------
        // 3) Simple trading logic
        // -----------------------------
        // Latest TradingModule: correct accounting + optional slippage.
        // commissionPercentage: 3 bps, slippageRate: 5 bps, initial capital: 10000
        TradingModule trader(/*commissionPercentage=*/0.0003,
                /*slippageRate=*/0.0005,
                /*initialCapital=*/10000.0);

        const std::string sym = "AAPL";
        int held_qty = 0; // local long-only position size for demo

        const int n = df->getLineCount();
        if (n < 3) {
            std::cerr << "Not enough rows.\n";
            return 1;
        }

        // We need previous values for cross detection
        double prev_macd = 0.0, prev_sig = 0.0;
        bool prev_valid = false;

        for (int i = 0; i < n; ++i) {
            const auto ts = dateLine->getParam(i);
            const double close = closeLine->getParam(i);

            const double m = macdLine->getParam(i);
            const double s = sigLine->getParam(i);

            // Skip warmup/NaN area produced by TA-Lib
            if (is_nan(m) || is_nan(s) || !(close > 0.0)) {
                prev_valid = false;
                continue;
            }

            if (!prev_valid) {
                prev_macd = m;
                prev_sig  = s;
                prev_valid = true;
                continue;
            }

            const bool cross_up   = (prev_macd <= prev_sig) && (m > s);
            const bool cross_down = (prev_macd >= prev_sig) && (m < s);

            // Buy when cross_up and flat
            if (cross_up && held_qty == 0) {
                const int qty = 50; // demo size
                if (trader.buy(ts, sym, close, qty)) {
                    held_qty += qty;
                    std::cout << ts << " BUY  qty=" << qty << " px=" << close << "\n";
                }
            }

            // Sell when cross_down and holding
            if (cross_down && held_qty > 0) {
                const int qty = held_qty;
                if (trader.sell(ts, sym, close, qty)) {
                    held_qty = 0;
                    std::cout << ts << " SELL qty=" << qty << " px=" << close << "\n";
                }
            }

            prev_macd = m;
            prev_sig  = s;
        }

        // If still holding at the end, close at last available close
        if (held_qty > 0) {
            const int last = n - 1;
            const auto ts = dateLine->getParam(last);
            const double close = closeLine->getParam(last);
            if (trader.sell(ts, sym, close, held_qty)) {
                std::cout << ts << " FINAL SELL qty=" << held_qty << " px=" << close << "\n";
                held_qty = 0;
            }
        }

        // -----------------------------
        // 4) Summary
        // -----------------------------
        std::cout << "\n=== Summary ===\n";
        std::cout << "Final cash: " << std::fixed << std::setprecision(2) << trader.cash() << "\n";
        std::cout << "Realized PnL (price diff only): " << trader.realizedPnL() << "\n";
        std::cout << "Total commission: " << trader.totalCommission() << "\n";

        auto trades = trader.getTradeInfoList();
        std::cout << "Trades: " << trades.size() << "\n";
        for (const auto& t : trades) {
            std::cout << t.date << "  " << t.symbol << "  " << t.type
                      << " qty=" << t.quantity
                      << " px=" << t.price
                      << " comm=" << t.commission
                      << " effect=" << t.position_effect
                      << "\n";
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 2;
    }
}
