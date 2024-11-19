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


void readLinesStockData(const std::string& filename, const boost::shared_ptr<LineManager::LineManager>& lineManager, const std::string& index_name = "Date") {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    typedef boost::tokenizer<boost::escaped_list_separator<char>> Tokenizer;

    // 第一步：读取并解析文件的第一行（标题行），提取列名
    std::getline(file, line);
    Tokenizer titleTokenizer(line);
    std::vector<std::string> columnNames(titleTokenizer.begin(), titleTokenizer.end());

    // 第二步：创建存储列数据的批量容器
    std::vector<std::vector<std::string>> allRowData;
    allRowData.reserve(1000); // 预分配空间以优化性能

    int rowIndex = 0;

    // 第三步：逐行读取数据并存储到容器中
    while (std::getline(file, line)) {
        Tokenizer dataTokenizer(line);
        std::vector<std::string> rowData(dataTokenizer.begin(), dataTokenizer.end());

        // 检查数据是否完整，不足的部分用空字符串填充
        if (rowData.size() < columnNames.size()) {
            rowData.resize(columnNames.size(), ""); // 用空字符串填充
        }

        allRowData.push_back(std::move(rowData)); // 将数据批量插入到容器中
        rowIndex++;
    }

    // 批量插入每列数据
    for (size_t i = 0; i < columnNames.size(); ++i) {
        const std::string& columnName = columnNames[i];

        // 如果是 index_name 这一列，需要单独处理
        if (index_name == columnName) {
            std::vector<int> indices;
            std::vector<std::string> indexValues;
            for (size_t j = 0; j < rowIndex; ++j) {
                const std::string& token = allRowData[j][i];
                indices.push_back(j);  // 假设 index 是基于行号
                indexValues.push_back(token.empty() ? "" : token);  // 处理空值
            }
            // 这里调用 lineManager 设置 index_name 的值
            lineManager->setColumnValues(columnName, indices, indexValues);
        } else {
            std::vector<int> indices;
            std::vector<double> values;  // 默认处理为 double 类型

            // 对其他列进行普通处理
            for (size_t j = 0; j < rowIndex; ++j) {
                const std::string& token = allRowData[j][i];
                indices.push_back(j);  // 假设 index 是基于行号

                // 处理空值并插入
                if ((*lineManager)[columnName].which() == 0) {  // string 类型
                    values.push_back(token.empty() ? std::numeric_limits<double>::quiet_NaN() : std::stod(token));
                } else {  // double 类型
                    values.push_back(token.empty() ? std::numeric_limits<double>::quiet_NaN() : std::stod(token));
                }
            }

            // 使用批量函数插入数据
            lineManager->setColumnValues(columnName, indices, values);
        }
    }

    file.close();
}
