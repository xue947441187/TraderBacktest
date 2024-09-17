//
// Created by 薛新岗 on 2024/5/15.
//

#ifndef TRADERBACKTEST_STOCKS_H
#define TRADERBACKTEST_STOCKS_H
#include <iostream>
#include <unordered_map>
#include <boost/variant.hpp>
#include <iomanip>
#include <fstream>
#include "base/mObserver.h"
#include "line/line.h"
#include "boost/any.hpp"
#include "boost/function.hpp"
#include "map"
#include "lineManager/lineManager.h"

struct StockData {
    std::string date;
    std::string symbol;
    double open;
    double high;
    double low;
    double close;
    double volume;
    std::unordered_map<std::string, double> indicators; // Custom indicators
    // You can add more fields according to your data format
};







// 类型映射
class LineFactory {
public:
    // 注册类型工厂函数
    void registerType(const std::string& indexType, const std::string& valueType, boost::function<boost::any()> factory) {
        typeMap_[{indexType, valueType}] = std::move(factory);
    }

    // 创建 Line 实例
    boost::any createLine(const std::string& indexType, const std::string& valueType) const {
        auto it = typeMap_.find({indexType, valueType});
        if (it != typeMap_.end()) {
            return it->second();
        }
        throw std::runtime_error("Unknown type combination");
    }

private:
    std::map<std::pair<std::string, std::string>, boost::function<boost::any()>> typeMap_;
};


//
//// LineManager 类
//class LineManager {
//public:
//    // 注册类型
//    void registerType(const std::string& indexType, const std::string& valueType, boost::function<boost::any()> factory) {
//        factory_.registerType(indexType, valueType, std::move(factory));
//    }
//
//    // 创建 Line 实例
//    boost::shared_ptr<Line::Line<boost::any, boost::any>> createLine(const std::string& indexType, const std::string& valueType) {
//        auto lineAny = factory_.createLine(indexType, valueType);
//        return boost::any_cast<boost::shared_ptr<Line::Line<boost::any, boost::any>>>(lineAny);
//    }
//
//    // 打印所有创建的 Line 实例信息（示例）
//    void printAllLines() const {
//        for (const auto& entry : lines_) {
//            std::cout << "Line with index type: " << entry.first.first << " and value type: " << entry.first.second << std::endl;
//        }
//    }
//
//private:
//    LineFactory factory_;
//    std::map<std::pair<std::string, std::string>, boost::shared_ptr<Line::Line<boost::any, boost::any>>> lines_;
//};


std::vector<StockData> readStockData(const std::string& filename);
void readLineStockData(const std::string& filename, const boost::shared_ptr<LineManager::LineManager>& lineManager,const std::string& index_name);
void readLinesStockData(const std::string& filename, const boost::shared_ptr<LineManager::LineManager>& lineManager,const std::string& index_name);
#endif //TRADERBACKTEST_STOCKS_H
