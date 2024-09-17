//
// Created by 94744 on 2024/9/17.
//

#ifndef TRADERBACKTEST_LINEMANAGERVISITOR_H
#define TRADERBACKTEST_LINEMANAGERVISITOR_H
#include <utility>

#include "string"
#include "boost/variant.hpp"
#include "line/line.h"
#include "base/item.h"
namespace LineManager {
    using LineIntDouble =boost::shared_ptr<Line::Line<int,double>> ;
    using LineIntString = boost::shared_ptr<Line::Line<int,std::string>>;
    using LineStringDouble=boost::shared_ptr<Line::Line<std::string,double>>;
    using LineStringInt=boost::shared_ptr<Line::Line<std::string,int>>;
    using LineVariant = boost::variant<LineIntString,LineIntDouble,LineStringDouble,LineStringInt>;


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
        std::string operator()(const LineStringInt& line) const {
            return line->getColumnName();
        }
    };

    struct SetColumnValueVisitor : public boost::static_visitor<void> {
        void operator()(const LineIntString& line, const int& index, const std::string& value) const {
            line->setParam(index,value);
        }

        void operator()(const LineIntDouble& line,const int& index, const double & value) const {
            line->setParam(index,value);
        }

        void operator()(const LineStringDouble& line,const std::string& index, const double& value) const {
            line->setParam(index,value);
        }

        void operator()(const LineStringDouble& line, const std::string& index, double value) const {
            line->setParam(index,value);
        }

    };

    struct SetLineValueVisitor : public boost::static_visitor<void> {
        int rowIndex;
        std::string token;
        boost::shared_ptr<Line::Line<int, std::string>> indexLine;

        SetLineValueVisitor(int rowIdx, const std::string& tok, const boost::shared_ptr<Line::Line<int, std::string>>& idxLine)
                : rowIndex(rowIdx), token(tok), indexLine(idxLine) {}

        // 处理 int, std::string 类型的 Line
        void operator()(boost::shared_ptr<Line::Line<int, std::string>>& linePtr) const {
            linePtr->setParam(rowIndex, token);
        }

        // 处理 std::string, double 类型的 Line
        void operator()(boost::shared_ptr<Line::Line<std::string, double>>& linePtr) const {
            Item<int, std::string> item = (*indexLine)[rowIndex];  // 从 indexLine 获取索引值
            linePtr->setParam(item.value, std::stod(token));
        }

        // 可以添加更多类型的 Line 支持（如有必要）
    };


    struct SetColumnNameVisitor : public boost::static_visitor<void> {
        explicit SetColumnNameVisitor(std::string newColumnName)
                : newColumnName(std::move(newColumnName)) {}

        void operator()(LineIntString& line) const {
            line->setColumnName(newColumnName);
        }

        void operator()(LineIntDouble& line) const {
            line->setColumnName(newColumnName);
        }

        void operator()(LineStringDouble& line) const {
            line->setColumnName(newColumnName);
        }

        void operator()(LineStringInt & line) const {
            line->setColumnName(newColumnName);
        }


    private:
        std::string newColumnName;
    };


}

#endif //TRADERBACKTEST_LINEMANAGERVISITOR_H
