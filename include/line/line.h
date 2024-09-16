//
// Created by 94744 on 2024/9/13.
//

#ifndef TRADERBACKTEST_LINE_H
#define TRADERBACKTEST_LINE_H

#include <iostream>
#include <unordered_map>
#include <boost/variant.hpp>
#include <iomanip>
#include <fstream>
#include "base/mObserver.h"
#include "base/item.h"
#include "base/ComparableItem.h"
#include "base/IndexComparableItem.h"
#include "base/ValueComparableItem.h"
#include <any>
#include "boost/optional.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "line/lineObserver.h"

namespace Line{
    // Line 类模板
    template <typename _index, typename _value>
    class Line : public boost::enable_shared_from_this<Line<_index, _value>>{
    private:
        boost::shared_ptr<ItemContainer<_index, _value>> _line;
        boost::shared_ptr<IndexComparableItem<_index,_value,_index>> index;
        boost::shared_ptr<ValueComparableItem<_index,_value,_value>> value;
        std::string column_name;                        // 列名
        size_t row_count{};                               // 数据总行数
        boost::shared_ptr<LineObserverManager> sentinel; // 修改类型为 LineObserverManager




        void notify(EventType eventType) {
            if (sentinel) {
                sentinel->notifyAll(eventType);
            }
        }
    public:
        Line(){
            _line = boost::make_shared<ItemContainer<_index, _value>>();
            index = IndexComparableItem<_index,_value,_index>::createComparableItem(this->_line);
            value = ValueComparableItem<_index,_value,_value>::createComparableItem(this->_line);
        }
        // 初始化 sentinel
        void initSentinel() {
            if (!sentinel) {
                auto sharedLine = this->shared_from_this();
                sentinel = LineObserverBuilder<boost::shared_ptr<Line<_index, _value>>>(sharedLine)
                        .withLineAddedObserver()
                        .withLineDataUpdatedObserver()
                        .build();

            }
        }
        // 设置列名
        void setColumnName(const std::string& name) {
            column_name = name;
        }
        size_t len(){
            notify(EventType::RowCountChanged);
            return row_count;
        }
        // 获取列名
        std::string getColumnName() const {
            return column_name;
        }

        // 设置参数并自动更新行数
        void setParam(const _index& idx, const _value& val) {
            this->initSentinel();  // 第一次调用 setParam 时初始化 sentinel

            auto& index_view = _line->template get<index_tag>();
            auto it = index_view.find(idx);
            if (it != index_view.end()) {
                // 如果索引已存在，更新值
                index_view.modify(it, [&](Item<_index, _value>& item) {
                    item.value = val;
                });
            } else {
                // 插入新的项，并增加行数
                _line->insert({ idx, val });
                ++row_count;  // 新增一行数据，行数增加
            }
            notify(EventType::LineAdded);
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

        // 获取行数
        size_t getRowCount() const {
            return row_count;
        }

        // 计算当前数据总数量
        void getTotalDataCount() {
            this->row_count = _line->size();  // 返回当前 _line 中的数据项总数
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

        // 打印列名和数值，不打印索引
        void print() const {
            std::cout << std::left << std::setw(10) << column_name
                      << std::setw(10) << "Value" << std::endl;

            auto& index_view = _line->template get<index_tag>();
            for (const auto& item : index_view) {
                std::cout << std::setw(10) << " "  // 不打印索引，只打印数据
                          << std::setw(10) << item.value << std::endl;
            }
        }

    };


}




#endif //TRADERBACKTEST_LINE_H
