//
// Created by 薛新岗 on 2024/5/15.
//

#include "stocks.h"
#include "boost/tokenizer.hpp"
#include "lineManager/lineManager.h"   // ✅ 使用最新 LineManager（DataFrame 风格 API）
#include "line/line.h"

#include <limits>
#include <unordered_map>

// -------------------------
// 工具：判断字符串是否为数字（允许符号、小数点）
// -------------------------
static inline bool isNumberToken(const std::string& s) {
    if (s.empty()) return false;
    // 允许：+ - . 以及数字
    bool has_digit = false;
    for (char c : s) {
        if ((c >= '0' && c <= '9')) { has_digit = true; continue; }
        if (c == '+' || c == '-' || c == '.') continue;
        return false;
    }
    return has_digit;
}

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

// ============================================================
// ✅ DataFrame 风格读取（推荐）：
// - index_name 列：Line<int, string>  (rowIndex -> date string)
// - 数值列：Line<string, double>      (date string -> value)
// - 其他字符串列：Line<int, string>   (rowIndex -> string)
// ============================================================
void readLineStockData(const std::string& filename,
                       const boost::shared_ptr<LineManager::LineManager>& lineManager,
                       const std::string& index_name /*= "Date"*/) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;  // 如果无法打开文件则直接返回
    }

    typedef boost::tokenizer<boost::escaped_list_separator<char>> Tokenizer;
    std::string line;

    // 1) 读取标题行
    if (!std::getline(file, line)) {
        return;
    }
    Tokenizer titleTokenizer(line);
    std::vector<std::string> columnNames(titleTokenizer.begin(), titleTokenizer.end());
    if (columnNames.empty()) return;

    // 2) 读第一行数据（用于推断列类型）
    if (!std::getline(file, line)) {
        return;
    }
    Tokenizer firstTok(line);
    std::vector<std::string> firstRow(firstTok.begin(), firstTok.end());
    if (firstRow.size() < columnNames.size()) firstRow.resize(columnNames.size(), "");

    // 3) 推断列类型并创建列（只创建一次）
    //    规则：index_name 固定为 string；非 index 列如果不是数字 => string 列，否则 double 列
    std::vector<bool> is_string_col(columnNames.size(), false);
    for (size_t i = 0; i < columnNames.size(); ++i) {
        const auto& col = columnNames[i];
        const auto& token = firstRow[i];

        if (col == index_name) {
            is_string_col[i] = true;
            (void)lineManager->col<int, std::string>(col); // 访问即确保列存在
        } else if (!isNumberToken(token)) {
            is_string_col[i] = true;
            (void)lineManager->col<int, std::string>(col);
        } else {
            is_string_col[i] = false;
            (void)lineManager->col<std::string, double>(col);
        }
    }

    int rowIndex = 0;

    // 4) 写入第一行
    //    先写 index，再写其它列（数值列需要 date key）
    {
        // 写 index
        const std::string& dateStr = firstRow[std::distance(columnNames.begin(),
                                                            std::find(columnNames.begin(), columnNames.end(), index_name))];
        lineManager->col<int, std::string>(index_name)[rowIndex] = dateStr;

        // 写其它列
        const std::string dateKey = static_cast<std::string>(lineManager->col<int, std::string>(index_name)[rowIndex]);
        for (size_t i = 0; i < columnNames.size(); ++i) {
            const auto& col = columnNames[i];
            if (col == index_name) continue;

            const auto& token = firstRow[i];
            if (is_string_col[i]) {
                lineManager->col<int, std::string>(col)[rowIndex] = token;
            } else {
                const double v = token.empty() ? std::numeric_limits<double>::quiet_NaN() : std::stod(token);
                lineManager->col<std::string, double>(col)[dateKey] = v;
            }
        }
        ++rowIndex;
    }

    // 5) 继续读取后续行
    while (std::getline(file, line)) {
        Tokenizer tok(line);
        std::vector<std::string> tokens(tok.begin(), tok.end());
        if (tokens.size() < columnNames.size()) tokens.resize(columnNames.size(), "");

        // 写 index
        lineManager->col<int, std::string>(index_name)[rowIndex] = tokens[std::distance(columnNames.begin(),
                                                                                        std::find(columnNames.begin(), columnNames.end(), index_name))];
        const std::string dateKey = static_cast<std::string>(lineManager->col<int, std::string>(index_name)[rowIndex]);

        // 写其它列
        for (size_t i = 0; i < columnNames.size(); ++i) {
            const auto& col = columnNames[i];
            if (col == index_name) continue;

            const auto& token = tokens[i];
            if (is_string_col[i]) {
                lineManager->col<int, std::string>(col)[rowIndex] = token;
            } else {
                const double v = token.empty() ? std::numeric_limits<double>::quiet_NaN() : std::stod(token);
                lineManager->col<std::string, double>(col)[dateKey] = v;
            }
        }

        ++rowIndex;
    }

    file.close();
}

// ============================================================
// ✅ 更快的批量读取：一次性把整列塞进去（比逐格 setParam 快很多）
// - index_name:  int -> string
// - 数值列:      string(date) -> double
// - 其它字符串列: int -> string
// ============================================================
void readLinesStockData(const std::string& filename,
                        const boost::shared_ptr<LineManager::LineManager>& lineManager,
                        const std::string& index_name /*= "Date"*/) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    typedef boost::tokenizer<boost::escaped_list_separator<char>> Tokenizer;
    std::string line;

    // 1) 标题
    if (!std::getline(file, line)) return;
    Tokenizer titleTokenizer(line);
    std::vector<std::string> columnNames(titleTokenizer.begin(), titleTokenizer.end());
    if (columnNames.empty()) return;

    // 2) 读所有行（先存下来，方便做列向量）
    std::vector<std::vector<std::string>> rows;
    rows.reserve(4096);

    while (std::getline(file, line)) {
        Tokenizer tok(line);
        std::vector<std::string> row(tok.begin(), tok.end());
        if (row.size() < columnNames.size()) row.resize(columnNames.size(), "");
        rows.emplace_back(std::move(row));
    }
    file.close();

    const size_t n = rows.size();
    if (n == 0) return;

    // 找到 index_name 在第几列（如果找不到，默认用第 0 列）
    size_t index_col_pos = 0;
    for (size_t i = 0; i < columnNames.size(); ++i) {
        if (columnNames[i] == index_name) { index_col_pos = i; break; }
    }

    // 3) 构建 rowIndex 与 dateKey
    std::vector<int> rowIndices;
    rowIndices.reserve(n);
    std::vector<std::string> dateKeys;
    dateKeys.reserve(n);

    for (size_t r = 0; r < n; ++r) {
        rowIndices.push_back(static_cast<int>(r));
        dateKeys.push_back(rows[r][index_col_pos]);
    }

    // 4) 写入 index 列（int -> string）
    lineManager->col<int, std::string>(index_name).set(rowIndices, dateKeys);

    // 5) 根据第一行推断其他列类型并批量写入
    for (size_t c = 0; c < columnNames.size(); ++c) {
        const auto& colName = columnNames[c];
        if (colName == index_name) continue;

        // 找一个非空 token 来推断类型（比只看第 1 行更稳）
        std::string sample;
        for (size_t r = 0; r < n; ++r) {
            if (!rows[r][c].empty()) { sample = rows[r][c]; break; }
        }

        const bool is_string = !isNumberToken(sample);

        if (is_string) {
            std::vector<std::string> colVals;
            colVals.reserve(n);
            for (size_t r = 0; r < n; ++r) colVals.push_back(rows[r][c]);
            lineManager->col<int, std::string>(colName).set(rowIndices, colVals);
        } else {
            std::vector<double> colVals;
            colVals.reserve(n);
            for (size_t r = 0; r < n; ++r) {
                const auto& token = rows[r][c];
                colVals.push_back(token.empty() ? std::numeric_limits<double>::quiet_NaN() : std::stod(token));
            }
            // 数值列：以 dateKeys 作为 index（string -> double）
            lineManager->col<int, double>(colName).set(rowIndices, colVals);

        }
    }
}
