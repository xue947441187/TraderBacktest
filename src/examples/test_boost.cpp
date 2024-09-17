//
// Created by 94744 on 2024/9/8.
//
// test_boost.cpp
#include <iostream>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
using namespace boost;
using namespace boost::asio;
//#include "line/lineObserver.h"
#include "line/line.h"
#include "lineManager/lineManager.h"
#include "stocks.h"

int main() {

    // 创建 LineManager 实例
    auto lineManager = LineManager::LineManager::create();
    readLineStockData("aapl.csv",lineManager,"Date");
    // 初始化观察者
//    lineManager->initializeObservers();



    // 插入数据
//    lineManager.insertData("Sample data");

    // 创建 Line 实例
//    auto line1 = boost::make_shared<Line::Line<int, double>>();
//    auto manager = LineObserverBuilder<Line<int, double>>(line1)
//    .withLineAddedObserver();
//    line1->setParam(1, 3.14);
//    auto line2 = boost::make_shared<Line::Line<int, double>>();
//    line2->setParam(2, 2.718);/**/
//
//    // 添加新的 Line
//    lineManager->addLine(line1);
//    lineManager->addLine(line2);
    // 设置列名
//    lineManager->setColumnName({"Column1", "Column2"});
    // 打印行数
    std::cout << "Current row count: " << lineManager->getRowCount() << std::endl;
//    auto  lineVar = (*lineManager)["Open"];
    auto line = boost::get<boost::shared_ptr<Line::Line<std::string, double>>>((*lineManager)["Open"]);
    auto a = (*line->getValueComparableItem()) > 3.1;
    for (auto &s:a ){
        std::cout<< s.index << "\t" <<s.value << std::endl;
    }
    //    line.apply_visitor()
    // 删除指定的 Line
//    lineManager->deleteLine(0);

    // 打印行数
    std::cout << "Current row count after deletion: " << lineManager->getRowCount() << std::endl;

    return 0;


}