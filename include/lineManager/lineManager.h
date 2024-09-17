//
// Created by 94744 on 2024/9/15.
//

#ifndef TRADERBACKTEST_LINEMANAGER_H
#define TRADERBACKTEST_LINEMANAGER_H

#include "string"
#include "boost/shared_ptr.hpp"
#include "boost/variant.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/make_shared.hpp"
#include "boost/blank.hpp"
#include "optional"
#include "lineManager/lineManagerObserver.h"
#include "line/line.h"

namespace LineManager{
    using LineIntDouble =boost::shared_ptr<Line::Line<int,double>> ;
    using LineIntString = boost::shared_ptr<Line::Line<int,std::string>>;
    using LineStringDouble=boost::shared_ptr<Line::Line<std::string,double>>;
    using LineVariant = boost::variant<LineIntString,LineIntDouble,LineStringDouble>;

    class LineManager : public boost::enable_shared_from_this<LineManager> {
    public:
        // 使用 make_shared 正确创建 LineManager 实例
        static boost::shared_ptr<LineManager> create() {
            return boost::make_shared<LineManager>();
        }
//     初始化 ObserverManager（延迟加载）
        void initSentinel() {
            if (!sentinel) {
                auto sharedLine = this->shared_from_this();
            sentinel = LineManagerObserverBuilder<boost::shared_ptr<LineManager>>(sharedLine)
                    .withLineAddedObserver()
                    .withManagerColumnNameChangedObserver()
                    .build();
            }
        }
        LineVariant operator[](const std::string& columnName) const {
            for (const auto& col : lines) {
                if (boost::apply_visitor(GetColumnNameVisitor(), col) == columnName) {
                    return col;  // 返回匹配列的 shared_ptr
                }
            }
              // 找不到列名时返回空指针
            return {};
        }

        // 设置列名
        void setColumnName(const std::vector<std::string>& col_names) {
            for (int i=0;i<col_names.size();i++){
                boost::apply_visitor(SetColumnNameVisitor(col_names[i]),lines[i]);
            }
            getAllColumnName();
//        notify(EventType::ColumnNameChanged);
        }

        void getAllColumnName() const{
            for(const auto &line: lines){
                    auto col_name= boost::apply_visitor(GetColumnNameVisitor(),line);
                    std::cout <<"|\t"<< col_name << "\t";
            }
            std::cout <<"|"<< std::endl;
        }
        void deleteLine(const std::string& col_name){
            for (int i=0; i<lines.size();i++){
                auto colName= boost::apply_visitor(GetColumnNameVisitor(),lines[i]);
                if (col_name == colName){
                    lines.erase(lines.begin() + i);
                }
            }
        }
        // 删除指定的 Line
        void deleteLine(size_t index) {
            if (index < lines.size()) {
                lines.erase(lines.begin() + index);
//            notify(EventType::LineDeleted);
            } else {
                std::cerr << "Error: Line index out of range" << std::endl;
            }
        }


        // 添加新的 Line
        void addLine(const boost::shared_ptr<Line::Line<int, double>>& line) {
            initSentinel();
            lines.push_back(line);
//        notify(EventType::LineAdded);
        }

        void addLine(const LineVariant& line) {
            initSentinel();
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

        LineManager() = default;

private:
        // 私有构造函数
        struct GetColumnNameVisitor : public boost::static_visitor<std::string> {
            std::string operator()(const LineIntString& line) const {
                return line->getColumnName();
            }

            std::string operator()(const LineIntDouble& line) const {
                return line->getColumnName();
            }

            std::string operator()(const LineStringDouble& line) const {
                return line->getColumnName();
            }
        };

        struct SetColumnNameVisitor : public boost::static_visitor<void> {
            explicit SetColumnNameVisitor(const std::string& newColumnName)
                    : newColumnName(newColumnName) {}

            void operator()(LineIntString& line) const {
                line->setColumnName(newColumnName);
            }

            void operator()(LineIntDouble& line) const {
                line->setColumnName(newColumnName);
            }

            void operator()(LineStringDouble& line) const {
                line->setColumnName(newColumnName);
            }


        private:
            std::string newColumnName;
        };


        void notify(EventType eventType) {
        if (sentinel) {
            sentinel->notifyAll(eventType);
        }
    }

        std::vector<LineVariant> lines;
        size_t row_count = 0;
        boost::shared_ptr<LineManagerObserverManager> sentinel;
    };
}

#endif //TRADERBACKTEST_LINEMANAGER_H
