//
// Created by 薛新岗 on 2024/5/15.
//

#include "stocks.h"
#include "boost/tokenizer.hpp"
#include "lineManager/lineManager.h"
#include "line/line.h"


std::vector<StockData> readStockData(const std::string& filename) {
    std::vector<StockData> stockData;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return stockData; // Return empty vector if file cannot be opened
    }

    std::string line;
    std::getline(file, line); // Skip header if exists

    // Using boost::tokenizer with comma as the delimiter
    typedef boost::tokenizer<boost::escaped_list_separator<char>> Tokenizer;
    while (std::getline(file, line)) {
        Tokenizer tok(line);
        std::vector<std::string> tokens(tok.begin(), tok.end());

        // Ensure we have exactly 6 columns in the CSV row
        if (tokens.size() != 6) {
            std::cerr << "Error: Invalid line format in file: " << line << std::endl;
            continue;
        }

        StockData data;
        data.date = tokens[0];
        data.open = std::stod(tokens[1]);
        data.high = std::stod(tokens[2]);
        data.low = std::stod(tokens[3]);
        data.close = std::stod(tokens[4]);
        data.volume = std::stod(tokens[5]);

        stockData.push_back(data);
    }

    file.close();
    return stockData;
}

void setLineValue(const boost::shared_ptr<LineManager::LineManager>& lineManager,const std::string &columnName,const int & rowIndex,const std::string &token,const std::string & index_name){
    // 根据列名找到对应的 Line 对象，并插入数据
    if ((*lineManager)[columnName].which() == 0) {  // 如果是 string 类型的 Line
        auto linePtr = boost::get<boost::shared_ptr<Line::Line<int, std::string>>>((*lineManager)[columnName]);
        linePtr->setParam(rowIndex, token);  // 日期列是 string 类型
    } else {
        auto linePtr = boost::get<boost::shared_ptr<Line::Line<std::string, double>>>((*lineManager)[columnName]);
        auto dataPtr = boost::get<boost::shared_ptr<Line::Line<int,std::string>>>(((*lineManager)[index_name]));
        Item<int,std::string> item = (*dataPtr)[rowIndex];
        linePtr->setParam(item.value, std::stod(token));  // 其他列是 double 类型
    }
}

void readLineStockData(const std::string& filename, const boost::shared_ptr<LineManager::LineManager>& lineManager,const std::string& index_name="Date") {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;  // 如果无法打开文件则直接返回
    }

    std::string line;

    // 第一步：读取并解析文件的第一行（标题行），提取列名
    std::getline(file, line);
    typedef boost::tokenizer<boost::escaped_list_separator<char>> Tokenizer;
    Tokenizer titleTokenizer(line);
    std::vector<std::string> columnNames(titleTokenizer.begin(), titleTokenizer.end());

    // 用于保存每个列名对应的 Line 对象
//    std::map<std::string, LineVariant> columnLineMap;

    // 读取下一行数据以推断数据类型
    std::getline(file, line);
    Tokenizer dataTokenizer(line);
    std::vector<std::string> firstRowData(dataTokenizer.begin(), dataTokenizer.end());

    // 第二步：根据列名和数据内容动态创建 Line 对象
    for (size_t i = 0; i < columnNames.size(); ++i) {
        const std::string& columnName = columnNames[i];
        const std::string& dataValue = firstRowData[i];

        // 判断数据类型
        if (dataValue.find_first_not_of("0123456789.-") != std::string::npos || columnName == index_name) {
            // 如果包含非数字字符，认为是日期（字符串）
            boost::shared_ptr<Line::Line<int, std::string>> linePtr = boost::make_shared<Line::Line<int, std::string>>();
            linePtr->setColumnName(columnName);
            (*lineManager).addLine(linePtr);
//            columnLineMap[columnName] = linePtr;
        } else {
            // 否则默认为浮点型
            auto linePtr = boost::make_shared<Line::Line<std::string, double>>();
            linePtr->setColumnName(columnName);
            (*lineManager).addLine(linePtr);
//            columnLineMap[columnName] = linePtr;
        }
    }

    int rowIndex = 0;

    // 第三步：处理已读取的第一行数据，将数据插入到相应的 Line 中
    for (size_t i = 0; i < columnNames.size(); ++i) {
        const auto& columnName = columnNames[i];
        const auto& token = firstRowData[i];

        // 根据列名找到对应的 Line 对象，并插入数据
        if ((*lineManager)[columnName].which() == 0) {  // 如果是 string 类型的 Line
            auto linePtr = boost::get<boost::shared_ptr<Line::Line<int, std::string>>>((*lineManager)[columnName]);
            linePtr->setParam(rowIndex, token);  // 日期列是 string 类型
        } else {
            auto linePtr = boost::get<boost::shared_ptr<Line::Line<std::string, double>>>((*lineManager)[columnName]);
            linePtr->setParam("0", std::stod(token));  // 其他列是 double 类型
        }
    }

    rowIndex++;

    // 第四步：继续逐行读取数据，将数据插入到相应的 Line 中
    while (std::getline(file, line)) {
        Tokenizer dataTokenizer(line);
        std::vector<std::string> tokens(dataTokenizer.begin(), dataTokenizer.end());

        if (tokens.size() != columnNames.size()) {
            std::cerr << "Error: Mismatched column count in file: " << line << std::endl;
            continue;
        }

        for (size_t i = 0; i < columnNames.size(); ++i) {
            const auto& columnName = columnNames[i];
            const auto& token = tokens[i];
            setLineValue(lineManager,columnName,rowIndex,token,index_name);
            // 根据列名找到对应的 Line 对象，并插入数据
//            if ((*lineManager)[columnName].which() == 0) {  // 如果是 string 类型的 Line
//                auto linePtr = boost::get<boost::shared_ptr<Line::Line<int, std::string>>>((*lineManager)[columnName]);
//                linePtr->setParam(rowIndex, token);  // 日期列是 string 类型
//            } else {
//                auto linePtr = boost::get<boost::shared_ptr<Line::Line<std::string, double>>>((*lineManager)[columnName]);
//                auto dataPtr = boost::get<boost::shared_ptr<Line::Line<int,std::string>>>(((*lineManager)[index_name]));
//                Item<int,std::string> item = (*dataPtr)[0];
//                linePtr->setParam(item.value, std::stod(token));  // 其他列是 double 类型
//            }
        }

        rowIndex++;
    }

 
    file.close();
}