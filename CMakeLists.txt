cmake_minimum_required(VERSION 3.21)
project(TraderBacktest)


add_subdirectory(indicators)
set(CMAKE_CXX_STANDARD 11)
add_library(TraderBacktestLib library.cpp trading/trading_module.cpp trading/trading_module.h backtest/backtest.cpp backtest/backtest.h indicators/indicators.h indicators/ma.cpp stocks/stocks.cpp stocks/stocks.h
        indicators/indicators.cpp)
#add_library(TraderBacktest library.cpp)
add_executable(TraderBacktest main.cpp)
target_link_libraries(TraderBacktest TraderBacktestLib)