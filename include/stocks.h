//
// Created by 薛新岗 on 2024/5/15.
//

#ifndef TRADERBACKTEST_STOCKS_H
#define TRADERBACKTEST_STOCKS_H
#include <iostream>
#include <unordered_map>

#include <fstream>
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

#include "item.h"
#include "ComparableItem.h"
#include "IndexComparableItem.h"
#include "ValueComparableItem.h"

// Line 类模板
template <typename _index, typename _value>
class Line {
private:
    boost::shared_ptr<ItemContainer<_index, _value>> _line;
    boost::shared_ptr<IndexComparableItem<_index,_value,_index>> index;
    boost::shared_ptr<ValueComparableItem<_index,_value,_value>> value;

public:
    Line(){
        _line = boost::make_shared<ItemContainer<_index, _value>>();
        index = IndexComparableItem<_index,_value,_index>::createComparableItem(this->_line);
        value = ValueComparableItem<_index,_value,_value>::createComparableItem(this->_line);
    }

    // 设置参数
    void setParam(const _index& idx, const _value& val) {
        auto& index_view = _line->template get<index_tag>();
        auto it = index_view.find(idx);
        if (it != index_view.end()) {
            // 如果 index 存在，则更新
            index_view.modify(it, [&](Item<_index, _value>& item) {
                item.value = val;
            });
        }
        else {
            // 否则，插入新的项
            _line->insert({ idx, val });
        }
    }


    // 获取参数
    _value getParam(const _index& idx) const {
        auto& index_view = _line->template get<index_tag>();
        auto it = index_view.find(idx);
        if (it != index_view.end()) {
            return it->value;
        }
        throw std::out_of_range("Index not found");
    }
    // 查询符合条件的项（按索引）
    std::vector<_value> queryIndex(const std::function<bool(const _index&)>& condition) const {
        std::vector<_value> result;
        auto& index_view = _line->template get<index_tag>();
        for (const auto& item : index_view) {
            if (condition(item.index)) {
                result.push_back(item.value);
            }
        }
        return result;
    }



    // 查询符合条件的项（按值）
    std::vector<Item<_index, _value>> queryValue(const std::function<bool(const _value&)>& condition) const {
        std::vector<Item<_index, _value>> result;
        auto& value_view = _line->template get<value_tag>();
        for (const auto& item : value_view) {
            if (condition(item.value)) {
                result.push_back(item);
            }
        }
        return result;
    }

    // 打印所有参数
    void printParams() const {
        std::cout << "Line Parameters:" << std::endl;
        auto& index_view = _line->template get<index_tag>();
        for (const auto& item : index_view) {
            std::cout << "Index: " << item.index << ", Value: " << item.value << std::endl;
        }
    }

    // 访问 ComparableItem 的成员
    boost::shared_ptr<IndexComparableItem<_index,_value,_index>> getIndexComparableItem() const {
        return index;
    }

    boost::shared_ptr<ValueComparableItem<_index,_value,_value>> getValueComparableItem() const {
        return value;
    }


};




std::vector<StockData> readStockData(const std::string& filename);
#endif //TRADERBACKTEST_STOCKS_H
