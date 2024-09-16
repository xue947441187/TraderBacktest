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
        boost::shared_ptr<LineManager> getPtr() {
            return shared_from_this();
        }
//     初始化 ObserverManager（延迟加载）
        void initSentinel() {
            if (!sentinel) {
                auto sharedLine = this->shared_from_this();
//            sentinel = LineManagerObserverBuilder<boost::shared_ptr<LineManagerObserverManager>>()
//                    .withLineAddedObserver().build();
            }
        }
        // 设置列名
        void setColumnName(const std::vector<std::string>& col_names) {
            column_names = col_names;
        notify(EventType::ColumnNameChanged);
        }

        // 插入数据
        void insertData(const std::string& data) {
            data_storage.push_back(data);
//        notify(EventType::DataInserted);
            updateRowCount();
        }

        // 删除指定的 Line
        void deleteLine(size_t index) {
            if (index < lines.size()) {
                lines.erase(lines.begin() + index);
//            notify(EventType::LineDeleted);
                updateRowCount();
            } else {
                std::cerr << "Error: Line index out of range" << std::endl;
            }
        }


        // 添加新的 Line
        void addLine(const boost::shared_ptr<Line::Line<int, double>>& line) {
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


    void notify(EventType eventType) {
        if (sentinel) {
            sentinel->notifyAll(eventType);
        }
    }

        std::vector<std::string> column_names;
        std::vector<std::string> data_storage;
        std::vector<LineVariant> lines;
        size_t row_count = 0;
        boost::shared_ptr<LineManagerObserverManager> sentinel;
    };
}

#endif //TRADERBACKTEST_LINEMANAGER_H
