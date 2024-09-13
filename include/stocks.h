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
#include "mObserver.h"
#include "line/line.h"

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








using LineIntDouble =boost::shared_ptr<Line<int,double>> ;
using LineIntString = boost::shared_ptr<Line<int,std::string>>;
using LineStringDouble=boost::shared_ptr<Line<std::string,double>>;

using LineVariant = boost::variant<LineIntString,LineIntDouble,LineStringDouble>;

class LineManager {
public:
    static std::unique_ptr<LineManager> create() {
        return std::unique_ptr<LineManager>(new LineManager());
    }

    // 初始化 ObserverManager（延迟加载）
//    void initializeObservers() {
//        if (!observerManager) {
//            observerManager = LineManagerObserverBuilder()
//                    .withRowCountObserver()
//                    .withColumnNameObserver()
//                    .withDataInsertedObserver()
//                    .withLineDeletedObserver()
//                    .withLineAddedObserver()
//                    .build();
//        }
//    }

    // 设置列名
    void setColumnName(const std::vector<std::string>& col_names) {
        column_names = col_names;
//        notify(EventType::ColumnNameChanged);
    }

    // 插入数据
    void insertData(const std::string& data) {
        data_storage.push_back(data);
//        notify(EventType::DataInserted);
        updateRowCount();
    }

    // 删除指定的 Line
    void deleteLine(size_t index) {
        if (index < lines.size()) {
            lines.erase(lines.begin() + index);
//            notify(EventType::LineDeleted);
            updateRowCount();
        } else {
            std::cerr << "Error: Line index out of range" << std::endl;
        }
    }

    // 添加新的 Line
    void addLine(const boost::shared_ptr<Line<int, double>>& line) {
        lines.push_back(line);
//        notify(EventType::LineAdded);
        updateRowCount();
    }

    // 更新行数
    void updateRowCount() {
        row_count = lines.size();
//        notify(EventType::RowCountChanged);
    }

    size_t getRowCount() const {
        return row_count;
    }

private:
    LineManager() = default; // 私有构造函数


//    void notify(EventType eventType) {
//        if (observerManager) {
//            observerManager->notifyAll(eventType);
//        }
//    }

//    std::shared_ptr<LineManagerObserverBuilder> observerManager;
    std::vector<std::string> column_names;
    std::vector<std::string> data_storage;
    std::vector<LineVariant> lines;
    size_t row_count = 0;
};




std::vector<StockData> readStockData(const std::string& filename);
#endif //TRADERBACKTEST_STOCKS_H
