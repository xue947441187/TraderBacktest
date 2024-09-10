//
// Created by 薛新岗 on 2024/5/15.
//

#include "stocks.h"
#include "boost/tokenizer.hpp"

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
//
//template <typename _index, typename _value>
//ItemContainer<_index, _value>
//IndexComparableItem<_index, _value>::query(const std::function<bool(const _key &)> &condition) const {
//    ItemContainer<_index, _value> result;
//
//    // 获取索引视图
//    auto& index_view = this->_line->get<index_tag>();
//    for (const auto& item : index_view) {
//        if (condition(item.index)) {
//            result.insert(item);  // 插入整个 Item 对象
//        }
//    }
//
//    return result;
//}
//
//template <typename _index, typename _value>
//ItemContainer<_index, _value>
//ValueComparableItem<_index, _value>::query(const std::function<bool(const _key &)> &condition) const {
//
//    ItemContainer<_index, _value> result;
//
//    // 获取索引视图
//    auto& index_view = this->_line->get<index_tag>();
//    for (const auto& item : index_view) {
//        if (condition(item.index)) {
//            result.insert(item);  // 插入整个 Item 对象
//        }
//    }
//
//    return result;
//}


