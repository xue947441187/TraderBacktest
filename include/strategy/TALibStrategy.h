//
// Created by 94744 on 2024/9/16.
//

#ifndef TRADERBACKTEST_TALIBSTRATEGY_H
#define TRADERBACKTEST_TALIBSTRATEGY_H
#include "lineManager/lineManager.h"

class TALibStrategy {
public:
    virtual void process(LineManager::LineManager* manager,std::string col_name) = 0;
    virtual void setInputColumns(const std::vector<std::string>& columnNames) = 0;

};

class MovingAverageStrategy : public TALibStrategy {
public:
    void process(LineManager::LineManager* manager,std::string col_name) override;
    void setInputColumns(const std::vector<std::string>& columnNames) override;
private:
    std::string closeColumn;
};

class RSIStrategy : public TALibStrategy {
public:
    void process(LineManager::LineManager* manager, std::string col_name) override;
    void setInputColumns(const std::vector<std::string>& columnNames) override;
private:
    std::vector<std::string> closeColumns;
};


#endif //TRADERBACKTEST_TALIBSTRATEGY_H
