//
// Created by 薛新岗 on 2024/5/15.
//

#include "stocks.h"

std::vector<StockData> readStockData(const std::string& filename) {
    std::vector<StockData> stockData;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return stockData; // Return empty vector if file cannot be opened
    }

    std::string line;
    std::getline(file, line); // Skip header if exists



    while (std::getline(file, line)) {
        StockData data;
        std::istringstream iss(line);
        std::string token;

        // Assuming CSV format: Date,Open,High,Low,Close,Volume
        std::getline(iss, token, ',');
        data.date = token;

        std::getline(iss, token, ',');
        data.open = std::stod(token);

        std::getline(iss, token, ',');
        data.high = std::stod(token);

        std::getline(iss, token, ',');
        data.low = std::stod(token);

        std::getline(iss, token, ',');
        data.close = std::stod(token);

        std::getline(iss, token, ',');
        data.volume = std::stod(token);

        stockData.push_back(data);
    }

    file.close();
    return stockData;
}
