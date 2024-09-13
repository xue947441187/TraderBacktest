//
// Created by 94744 on 2024/9/8.
//
// test_boost.cpp
#include <iostream>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>
#include "stocks.h"
//#include "ComparableItem.h"
using namespace boost;
using namespace boost::asio;
#include "line/lineObserver.h"
#include "line/line.h"


int main() {
//    // 实例化 Line 对象，使用 int 作为索引，std::string 作为值类型
//    Line<int, std::string> line;
//
//    // 插入参数
//    line.setParam(1, "Value 1");
//    line.setParam(2, "Value 2");
//    line.setParam(3, "Value 3");
//    line.setParam(4, "Value 4");
//
//    // 获取参数
//    try {
//        std::string val = line.getParam(2);
//        std::cout << "Get Param: " << val << std::endl;
//    }
//    catch (const std::out_of_range& e) {
//        std::cout << e.what() << std::endl;
//    }
//
//    // 查询所有大于 1 的项（使用 IndexComparableItem）
//    auto indexComparable = line.getIndexComparableItem();
//    ItemContainer<int, std::string> indexResults = (*indexComparable) > 1;
////    for (const auto& item : indexResults) {
////        std::cout << "index" << item.index <<"value:"<<item.value << std::endl;
////    }
//    // 打印查询结果
//    for (const auto& item : indexResults) {
//        std::cout << "Index Comparable Query (idx > 1): " << item.index << std::endl;
//    }
////    std::cout << "Index Comparable Query (idx > 1): " << indexResults << std::endl;
//
//    // 查询所有小于等于 "Value 3" 的项（使用 ValueComparableItem）
//    auto valueComparable = line.getValueComparableItem();
//    valueComparable->get_key();
//    ItemContainer<int, std::string> valueResults = (*valueComparable) >= "Value 3";
//    for (const auto& item : valueResults) {
//        std::cout << "value Comparable Query (idx > 1): " << item.index << std::endl;
//    }
////    printItemContainer(valueResults);
//
//    // 打印查询结果
////    std::cout << "Value Comparable Query (value <= \"Value 3\"): " << valueResults << std::endl;
//
//    // 打印所有参数
//    line.printParams();

    // 创建 LineManager 实例
    auto lineManager = LineManager::create();
    // 初始化观察者
//    lineManager->initializeObservers();

    // 设置列名
    lineManager->setColumnName({"Column1", "Column2"});

    // 插入数据
    lineManager->insertData("Sample data");

    // 创建 Line 实例
    auto line1 = boost::make_shared<Line<int, double>>();
//    auto manager = LineObserverBuilder<Line<int, double>>(line1)
//    .withLineAddedObserver();
    line1->setParam(1, 3.14);
//    Line<int, double> line2;
//    line2.setParam(2, 2.718);/**/
//
//    // 添加新的 Line
//    lineManager->addLine(line1);
//    lineManager->addLine(line2);

    // 打印行数
    std::cout << "Current row count: " << lineManager->getRowCount() << std::endl;

    // 删除指定的 Line
    lineManager->deleteLine(0);

    // 打印行数
    std::cout << "Current row count after deletion: " << lineManager->getRowCount() << std::endl;

    return 0;


    return 0;


}