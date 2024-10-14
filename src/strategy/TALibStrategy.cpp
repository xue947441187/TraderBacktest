//
// Created by 94744 on 2024/9/16.
//


#include "strategy/TALibStrategy.h"
#include "ta_libc.h"

void MovingAverageStrategy::process() {
    LineManager::LineVariant lineVariant = (*this->manager)["Open"];
    // 检查列是否存在
    auto linePtr = boost::get<boost::shared_ptr<Line::Line<std::string, double>>>(lineVariant);
    if (linePtr) {
        // 列存在且转换成功，可以在这里使用 linePtr
        // 示例：打印数据
        linePtr->print(); // 假设 Line 类有一个 print 方法
    } else {
        std::cerr << "Error: Column 'Open' is not of the expected type." << std::endl;
    }

    std::cout << "MA" << std::endl;
}



void RSIStrategy::process() {

    std::cout << "RSI" << std::endl;
}
