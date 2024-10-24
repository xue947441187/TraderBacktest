//
// Created by 94744 on 2024/9/16.
//

#ifndef TRADERBACKTEST_TALIBSTRATEGY_H
#define TRADERBACKTEST_TALIBSTRATEGY_H

#include <utility>

#include "lineManager/lineManager.h"
#include "map"

class StrategyParams {
public:
    std::map<std::string, double> numericParams;  // 存储数值型参数
    std::map<std::string, std::string> stringParams;  // 存储字符串型参数

    void setNumericParam(const std::string& key, double value) {
        numericParams[key] = value;
    }

    double getNumericParam(const std::string& key) const {
        auto it = numericParams.find(key);
        if (it != numericParams.end()) {
            return it->second;
        }
        throw std::invalid_argument("Numeric parameter not found: " + key);
    }

    void setStringParam(const std::string& key, const std::string& value) {
        stringParams[key] = value;
    }

    std::string getStringParam(const std::string& key) const {
        auto it = stringParams.find(key);
        if (it != stringParams.end()) {
            return it->second;
        }
        throw std::invalid_argument("String parameter not found: " + key);
    }
};



class TALibStrategy {
public:
    virtual void process() = 0;
    virtual void setInputColumns(const std::string& columnNames){
        if (columnNames.size() != 1) {
            closeColumn = "close";
        }else{
            closeColumn = columnNames[0];  // 假设传入的列名是 "close"

        }
    };
    void setParams(const StrategyParams & params) {
        this->params = params;
    }
    explicit TALibStrategy(boost::shared_ptr<LineManager::LineManager> & manager):
            manager(manager){}
protected :
    boost::shared_ptr<LineManager::LineManager> manager;
    std::string closeColumn;
    StrategyParams params;

};



class MovingAverageStrategy : public TALibStrategy {
public:
    void process() override;
    explicit MovingAverageStrategy(boost::shared_ptr<LineManager::LineManager> manager)
            : TALibStrategy(manager) {}
};


class RSIStrategy : public TALibStrategy {
public:
    void process() override;
    explicit RSIStrategy(boost::shared_ptr<LineManager::LineManager> manager)
    : TALibStrategy(manager) {}
};




#endif //TRADERBACKTEST_TALIBSTRATEGY_H
