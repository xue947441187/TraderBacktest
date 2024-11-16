//
// Created by 94744 on 2024/9/16.
//


#include <numeric>
#include "strategy/TALibStrategy.h"
#include "ta_libc.h"
void MovingAverageStrategy::process() {
    // 获取并检查数据列
    auto DatelineVariant = (*this->manager)[(*this->manager).getIndexName()];
    auto lineVariant = (*this->manager)["Open"];

    // 提取索引列与数据列
    auto indexPtr = boost::get<boost::shared_ptr<Line::Line<int, std::string>>>(DatelineVariant);
    auto linePtr = boost::get<boost::shared_ptr<Line::Line<int, double>>>(lineVariant);

    // 检查数据列是否有效
    if (!linePtr) {
        std::cerr << "Error: Column 'Open' is not of the expected type." << std::endl;
        return;
    }

    // 提取数据并将其转换为 std::vector<double>
    std::vector<double> data = linePtr->getValuesList();
    int dataSize = data.size();

    // 如果没有数据，直接返回
    if (dataSize == 0) {
        std::cerr << "Error: No data available for SMA calculation." << std::endl;
        return;
    }

    // 构造 finalIndices (可以用于记录数据的索引)
    std::vector<int> finalIndices(dataSize);
    std::iota(finalIndices.begin(), finalIndices.end(), 0); // 用 std::iota 填充索引数组

    // 获取 period 参数
    const int period = this->params.getNumericParam("period");

    // 为计算 SMA 准备输出缓冲区
    std::vector<double> smaResult(dataSize, 0.0);
    int outBegIdx = 0, outNbElement = 0;

    // 调用 TA_SMA 计算简单移动平均值
    TA_RetCode retCode = TA_SMA(0, dataSize - 1, data.data(), period, &outBegIdx, &outNbElement, smaResult.data());

    // 检查是否计算成功
    if (retCode != TA_SUCCESS) {
        std::cerr << "Error calculating SMA!" << std::endl;
        return;
    }

    // 输出结果
    std::cout << "SMA calculated successfully!" << std::endl;
    // 打印结果（可选）
    // for (int i = 0; i < outNbElement; ++i) {
    //     std::cout << "SMA[" << (outBegIdx + i) << "] = " << smaResult[outBegIdx + i] << std::endl;
    // }
    std::vector<double> out(dataSize, 0.0);
    for (int i = 0; i < outNbElement; ++i) {
        out[outBegIdx + i] = smaResult[i];
    }
    // 设置计算的 SMA 数据到 manager
    this->manager->setColumnValues("SMA" + std::to_string(period), finalIndices, smaResult);

    std::cout << "MA processing complete." << std::endl;
}


void RSIStrategy::process() {

    std::cout << "RSI" << std::endl;
}
