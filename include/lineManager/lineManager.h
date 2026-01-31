//
// Created by 94744 on 2024/9/15.
//

#ifndef TRADERBACKTEST_LINEMANAGER_H
#define TRADERBACKTEST_LINEMANAGER_H


#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <optional>
#include <stdexcept>
#include <utility>

#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/blank.hpp>

#include "lineManager/lineManagerObserver.h"
#include "line/line.h"
#include "lineManagerVisitor.h"

namespace LineManager {

    class LineManager : public boost::enable_shared_from_this<LineManager> {
    public:
        static boost::shared_ptr<LineManager> create() {
            return boost::make_shared<LineManager>();
        }

        // -------------------------
        // ObserverManager（延迟加载）
        // -------------------------
        void initSentinel() {
            if (!sentinel) {
                auto sharedLine = this->shared_from_this();
                sentinel = LineManagerObserverBuilder<boost::shared_ptr<LineManager>>(sharedLine)
                        .withManagerColumnNameChangedObserver()
                        .build();
            }
        }

        // ============================================================
        // ✅ DataFrame 风格 API
        // 目标：调用更像 df.col<int,double>("Close")[idx] = v;
        // ============================================================

        template<typename IndexT, typename ValueT>
        class ValueProxy {
        public:
            ValueProxy(boost::shared_ptr<Line::Line<IndexT, ValueT>> line, IndexT idx)
                    : line_(std::move(line)), idx_(std::move(idx)) {}

            // 赋值：df.col<..>("Close")[idx] = v;
            ValueProxy& operator=(const ValueT& v) {
                line_->setParam(idx_, v);
                return *this;
            }

            // 读取：ValueT v = df.col<..>("Close")[idx];
            operator ValueT() const {
                return line_->getParam(idx_);
            }

            // 更安全的读取：df.col<..>("Close")[idx].try_get()
            std::optional<ValueT> try_get() const {
                try {
                    return line_->getParam(idx_);
                } catch (...) {
                    return std::nullopt;
                }
            }

        private:
            boost::shared_ptr<Line::Line<IndexT, ValueT>> line_;
            IndexT idx_;
        };

        template<typename IndexT, typename ValueT>
        class ColumnProxy {
        public:
            ColumnProxy(LineManager* mgr, std::string name)
                    : mgr_(mgr), name_(std::move(name)) {}

            // df.col<..>("Close").set(indices, values)
            void set(const std::vector<IndexT>& indices, const std::vector<ValueT>& values) {
                mgr_->setColumn<IndexT, ValueT>(name_, indices, values);
            }

            // df.col<..>("Close").set(idx, value)
            void set(const IndexT& idx, const ValueT& value) {
                mgr_->setValue<IndexT, ValueT>(name_, idx, value);
            }

            // df.col<..>("Close").get(idx) -> optional
            std::optional<ValueT> get(const IndexT& idx) const {
                return mgr_->getValue<IndexT, ValueT>(name_, idx);
            }

            // df.col<..>("Close")[idx]  (可读写)
            ValueProxy<IndexT, ValueT> operator[](const IndexT& idx) {
                auto line = mgr_->ensureLine<IndexT, ValueT>(name_);
                return ValueProxy<IndexT, ValueT>(std::move(line), idx);
            }

            // const 版本：只读（如果列不存在/类型不匹配就 throw）
            ValueProxy<IndexT, ValueT> operator[](const IndexT& idx) const {
                auto line = mgr_->getLine<IndexT, ValueT>(name_);
                if (!line) {
                    throw std::out_of_range("Column not found or type mismatch: " + name_);
                }
                return ValueProxy<IndexT, ValueT>(std::move(line), idx);
            }

            const std::string& name() const noexcept { return name_; }

        private:
            LineManager* mgr_;
            std::string name_;
        };

        // df.col<int,double>("Close")
        template<typename IndexT, typename ValueT>
        ColumnProxy<IndexT, ValueT> col(const std::string& columnName) {
            return ColumnProxy<IndexT, ValueT>(this, columnName);
        }

        template<typename IndexT, typename ValueT>
        ColumnProxy<IndexT, ValueT> col(const std::string& columnName) const {
            // const manager：不允许隐式创建列（但 proxy 仍可读）
            return ColumnProxy<IndexT, ValueT>(const_cast<LineManager*>(this), columnName);
        }

        // ============================================================
        // ✅ 简化版 API（比 visitor/variant 更好用）
        // ============================================================

        // 新增/覆盖一整列（推荐）
        template <typename IndexT, typename ValueT>
        void setColumn(const std::string& colName,
                       const std::vector<IndexT>& indices,
                       const std::vector<ValueT>& values) {
            if (indices.size() != values.size()) {
                throw std::runtime_error("Indices and values must have the same size.");
            }

            auto line = boost::make_shared<Line::Line<IndexT, ValueT>>();
            line->setParams(indices, values);
            line->setColumnName(colName);
            addOrReplaceLineInternal(LineVariant(line), colName);
        }

        // 单点写入（不存在则创建空列）
        template <typename IndexT, typename ValueT>
        void setValue(const std::string& colName, const IndexT& idx, const ValueT& val) {
            auto line = ensureLine<IndexT, ValueT>(colName);
            line->setParam(idx, val);
        }

        // 单点读取：找不到返回 nullopt（不 throw）
        template <typename IndexT, typename ValueT>
        std::optional<ValueT> getValue(const std::string& colName, const IndexT& idx) const {
            auto line = getLine<IndexT, ValueT>(colName);
            if (!line) return std::nullopt;
            try {
                return line->getParam(idx);
            } catch (...) {
                return std::nullopt;
            }
        }

        // 直接拿到强类型 Line 指针（避免 visitor）
        template <typename IndexT, typename ValueT>
        boost::shared_ptr<Line::Line<IndexT, ValueT>> getLine(const std::string& colName) const {
            auto it = name_to_pos.find(colName);
            if (it == name_to_pos.end()) return nullptr;

            const auto& var = lines[it->second];
            if (auto p = boost::get<boost::shared_ptr<Line::Line<IndexT, ValueT>>>(&var)) {
                return *p;
            }
            return nullptr;
        }

        // 确保列存在且类型匹配；不存在就创建
        template <typename IndexT, typename ValueT>
        boost::shared_ptr<Line::Line<IndexT, ValueT>> ensureLine(const std::string& colName) {
            auto existing = getLine<IndexT, ValueT>(colName);
            if (existing) return existing;

            // 如果列存在但类型不匹配，直接报错（避免 silent bug）
            if (hasColumn(colName)) {
                throw std::runtime_error("Column exists but type mismatch: " + colName);
            }

            auto line = boost::make_shared<Line::Line<IndexT, ValueT>>();
            line->setColumnName(colName);
            addOrReplaceLineInternal(LineVariant(line), colName);
            return line;
        }

        bool hasColumn(const std::string& colName) const {
            return name_to_pos.find(colName) != name_to_pos.end();
        }

        // ============================================================
        // 你原来的接口：全部保留（内部已加速）
        // ============================================================

        LineVariant operator[](const std::string &columnName) const {
            auto it = name_to_pos.find(columnName);
            if (it == name_to_pos.end()) return {};
            return lines[it->second];
        }

        // 设置列名（按当前顺序修改）
        void setColumnName(const std::vector<std::string> &col_names) {
            const auto n = std::min(col_names.size(), lines.size());
            for (size_t i = 0; i < n; i++) {
                boost::apply_visitor(SetColumnNameVisitor(col_names[i]), lines[i]);
            }
            rebuildIndex();
        }

        // 兼容原 overload：内部转到模板 setColumn（更少重复）
        void setColumnValues(const std::string& colName, const std::vector<int>& indices, const std::vector<std::string>& values) {
            setColumn<int, std::string>(colName, indices, values);
        }
        void setColumnValues(const std::string& colName, const std::vector<int>& indices, const std::vector<double>& values) {
            setColumn<int, double>(colName, indices, values);
        }
        void setColumnValues(const std::string& colName, const std::vector<std::string>& indices, const std::vector<double>& values) {
            setColumn<std::string, double>(colName, indices, values);
        }
        void setColumnValues(const std::string& colName, const std::vector<std::string>& indices, const std::vector<int>& values) {
            setColumn<std::string, int>(colName, indices, values);
        }

        void getAllColumnName() const {
            for (const auto &line: lines) {
                auto col_name = boost::apply_visitor(GetColumnNameVisitor(), line);
                std::cout << "|	" << col_name << "	";
            }
            std::cout << "|" << std::endl;
        }

        void deleteLine(const std::string &col_name) {
            auto it = name_to_pos.find(col_name);
            if (it == name_to_pos.end()) return;

            const size_t pos = it->second;
            lines.erase(lines.begin() + static_cast<long>(pos));
            rebuildIndex();
        }

        void deleteLine(size_t index) {
            if (index < lines.size()) {
                lines.erase(lines.begin() + static_cast<long>(index));
                rebuildIndex();
            } else {
                std::cerr << "Error: Line index out of range" << std::endl;
            }
        }

        // 添加新的 Line（不覆盖；原逻辑保留）
        void addLine(const boost::shared_ptr<Line::Line<int, double>>& line) {
            initSentinel();
            lines.emplace_back(line);
            name_to_pos[line->getColumnName()] = lines.size() - 1;
            updateRowCount();
        }

        void addLine(const LineVariant &line) {
            initSentinel();
            lines.push_back(line);
            name_to_pos[boost::apply_visitor(GetColumnNameVisitor(), line)] = lines.size() - 1;
            updateRowCount();
        }

        void updateRowCount() {
            line_count = lines.size();
        }

        std::string getIndexName() {
            return this->indexName;
        }

        size_t getLineCount() const {
            return line_count;
        }

        LineManager() = default;

    private:
        void notify(EventType eventType) {
            if (sentinel) {
                sentinel->notifyAll(eventType);
            }
        }

        // ✅ 覆盖同名列：更像 DataFrame 的行为
        void addOrReplaceLineInternal(LineVariant line, const std::string& colName) {
            initSentinel();

            auto it = name_to_pos.find(colName);
            if (it == name_to_pos.end()) {
                lines.emplace_back(std::move(line));
                name_to_pos[colName] = lines.size() - 1;
            } else {
                lines[it->second] = std::move(line);
            }
            updateRowCount();
        }

        void rebuildIndex() {
            name_to_pos.clear();
            for (size_t i = 0; i < lines.size(); ++i) {
                name_to_pos[boost::apply_visitor(GetColumnNameVisitor(), lines[i])] = i;
            }
            updateRowCount();
        }

    private:
        std::vector<LineVariant> lines;
        std::unordered_map<std::string, size_t> name_to_pos;
        size_t line_count = 0;
        std::string indexName = "Date";
        boost::shared_ptr<LineManagerObserverManager> sentinel;
    };

} // namespace LineManager







/***
 * 原本代码
 *
 *
 * ***/
//
//#include "string"
//#include "boost/shared_ptr.hpp"
//#include "boost/variant.hpp"
//#include "boost/enable_shared_from_this.hpp"
//#include "boost/make_shared.hpp"
//#include "boost/blank.hpp"
//#include "optional"
//#include "lineManager/lineManagerObserver.h"
//#include "line/line.h"
//#include "lineManagerVisitor.h"
//
//namespace LineManager {
//
//
//    class LineManager : public boost::enable_shared_from_this<LineManager> {
//    public:
//        // 使用 make_shared 正确创建 LineManager 实例
//        static boost::shared_ptr<LineManager> create() {
//            return boost::make_shared<LineManager>();
//        }
//
////     初始化 ObserverManager（延迟加载）
//        void initSentinel() {
//            if (!sentinel) {
//                auto sharedLine = this->shared_from_this();
//                sentinel = LineManagerObserverBuilder<boost::shared_ptr<LineManager>>(sharedLine)
////                        .withLineAddedObserver()
//                        .withManagerColumnNameChangedObserver()
//                        .build();
//            }
//        }
//
//        LineVariant operator[](const std::string &columnName) const {
//            for (const auto &col: lines) {
//                if (boost::apply_visitor(GetColumnNameVisitor(), col) == columnName) {
//                    return col;  // 返回匹配列的 shared_ptr
//                }
//            }
//            // 找不到列名时返回空指针
//            return {};
//        }
//
//        // 设置列名
//        void setColumnName(const std::vector<std::string> &col_names) {
//            for (int i = 0; i < col_names.size(); i++) {
//                boost::apply_visitor(SetColumnNameVisitor(col_names[i]), lines[i]);
//            }
//            getAllColumnName();
////        notify(EventType::ColumnNameChanged);
//        }
//
//        void setColumnValues(const std::string& colName, const std::vector<int>& indices, const std::vector<std::string>& values) {
//            auto line = boost::make_shared<Line::Line<int,std::string>>();
//            line->setParams(indices, values);
//            line->setColumnName(colName);
//            this->addLine(line);
//        }
//
//        void setColumnValues(const std::string& colName, const std::vector<int>& indices, const std::vector<double>& values) {
//            auto line = boost::make_shared<Line::Line<int,double>>();
//            line->setParams(indices, values);
//            line->setColumnName(colName);
//            this->addLine(line);
//        }
//        void setColumnValues(const std::string& colName, const std::vector<std::string>& indices, const std::vector<double>& values) {
//            auto line = boost::make_shared<Line::Line<std::string,double>>();
//            line->setParams(indices, values);
//            line->setColumnName(colName);
//            this->addLine(line);
//        }
//        void setColumnValues(const std::string& colName, const std::vector<std::string>& indices, const std::vector<int>& values) {
//            auto line = boost::make_shared<Line::Line<std::string,int>>();
//            line->setParams(indices, values);
//            line->setColumnName(colName);
//            this->addLine(line);
//        }
//
//
//        void getAllColumnName() const {
//            for (const auto &line: lines) {
//                auto col_name = boost::apply_visitor(GetColumnNameVisitor(), line);
//                std::cout << "|\t" << col_name << "\t";
//            }
//            std::cout << "|" << std::endl;
//        }
//
//        void deleteLine(const std::string &col_name) {
//            for (int i = 0; i < lines.size(); i++) {
//                auto colName = boost::apply_visitor(GetColumnNameVisitor(), lines[i]);
//                if (col_name == colName) {
//                    lines.erase(lines.begin() + i);
//                }
//            }
//        }
//
//        // 删除指定的 Line
//        void deleteLine(size_t index) {
//            if (index < lines.size()) {
//                lines.erase(lines.begin() + index);
////            notify(EventType::LineDeleted);
//            } else {
//                std::cerr << "Error: Line index out of range" << std::endl;
//            }
//        }
//
//
//        // 添加新的 Line
//        void addLine(const boost::shared_ptr<Line::Line<int, double>> &line) {
//            initSentinel();
//            lines.emplace_back(line);
////        notify(EventType::LineAdded);
//        }
//
//        void addLine(const LineVariant &line) {
//            initSentinel();
//            lines.push_back(line);
////        notify(EventType::LineAdded);
//            updateRowCount();
//        }
//
//        // 更新行数
//        void updateRowCount() {
//            line_count = lines.size();
////        notify(EventType::RowCountChanged);
//        }
//        std::string getIndexName(){
//            return this->indexName;
//        };
//
//        size_t getLineCount() const {
//            return line_count;
//        }
//
//        LineManager() = default;
//
//    private:
//        // 私有构造函数
//        void notify(EventType eventType) {
//            if (sentinel) {
//                sentinel->notifyAll(eventType);
//            }
//        }
//
//        std::vector<LineVariant> lines;
//        size_t line_count = 0;
//        std::string indexName="Date";
//        boost::shared_ptr<LineManagerObserverManager> sentinel;
//    };
//}
//
//
#endif //TRADERBACKTEST_LINEMANAGER_H
