//
// Created by 94744 on 2024/9/16.
//


#include "strategy/TALibStrategy.h"

void MovingAverageStrategy::setInputColumns(const std::vector<std::string> &columnNames) {
    if (columnNames.size() != 1) {
        closeColumn = "close";
    }else{
        closeColumn = columnNames[0];  // 假设传入的列名是 "close"

    }


}
void MovingAverageStrategy::process(LineManager::LineManager *manager, std::string col_name) {

    std::cout << "MA" << std::endl;
}

void RSIStrategy::process(LineManager::LineManager *manager, std::string col_name) {

    std::cout << "RSI" << std::endl;
}

void RSIStrategy::setInputColumns(const std::vector<std::string> &columnNames) {

}