//
// Created by 94744 on 2024/9/16.
//


#include <numeric>
#include "strategy/TALibStrategy.h"
#include "ta_libc.h"

void MovingAverageStrategy::process() {

    auto [finalIndices,data] = this->extractColumnData<int,double>();

    // 获取 period 参数
    const int period = this->params.getNumericParam("period");

    // 为计算 SMA 准备输出缓冲区
    std::vector<double> smaResult(data.size(), 0.0);
    int outBegIdx = 0, outNbElement = 0;

    // 调用 TA_SMA 计算简单移动平均值
    TA_RetCode retCode = TA_SMA(0, data.size() - 1, data.data(), period, &outBegIdx, &outNbElement, smaResult.data());

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
    std::vector<double> out(data.size(), 0.0);
    for (int i = 0; i < outNbElement; ++i) {
        out[outBegIdx + i] = smaResult[i];
    }
    // 设置计算的 SMA 数据到 manager
    this->manager->setColumnValues("SMA_" + std::to_string(period), finalIndices, out);

    std::cout << "MA processing complete." << std::endl;
}


void RSIStrategy::process() {
    auto [index_,data_] = this->extractColumnData<int,double>();

    int outBegIdx = 0, outNbElement = 0;
    const int period = this->params.getNumericParam("period");
    std::vector<double> rsiResult(data_.size() -1 ,0.0);


    TA_RetCode retCode = TA_RSI(0,data_.size()-1,data_.data(),period,&outBegIdx,&outNbElement,rsiResult.data());
    if (retCode != TA_SUCCESS) {
        std::cerr << "Error calculating RSI!" << std::endl;
        return;
    }

    std::cout << "SMA calculated successfully!" << std::endl;
    // 打印结果（可选）
    // for (int i = 0; i < outNbElement; ++i) {
    //     std::cout << "SMA[" << (outBegIdx + i) << "] = " << smaResult[outBegIdx + i] << std::endl;
    // }
    std::vector<double> out(data_.size(), 0.0);
    for (int i = 0; i < outNbElement; ++i) {
        out[outBegIdx + i] = rsiResult[i];
    }
    // 设置计算的 SMA 数据到 manager
    this->manager->setColumnValues("RSI_" + std::to_string(period), index_, out);

    std::cout << "RSI" << std::endl;
}


void MACDStrategy::process() {
    auto [index,data] = this->extractColumnData<int,double>();
    int outBegIdx = 0, outNbElement = 0;
    const int fastPeriod = this->params.getNumericParam("FastPeriod");
    const int slowPeriod = this->params.getNumericParam("SlowPeriod");
    const int signalperiod = this->params.getNumericParam("SignalPeriod");
    std::vector<double> macdResult(data.size() -1 ,0.0);
    std::vector<double> singalResult(data.size() -1 ,0.0);
    std::vector<double> histResult(data.size() -1 ,0.0);
    TA_RetCode retCode = TA_MACD(0,data.size()-1,data.data(),fastPeriod,slowPeriod,signalperiod,&outBegIdx,&outNbElement,macdResult.data(),singalResult.data(),histResult.data());
    if (retCode != TA_SUCCESS){
        std::cerr << "Error calculating MACD!" << std::endl;
        return;
    }
    std::vector<double> outmacdResult(data.size(), 0.0);
    std::vector<double> outsingalResult(data.size(), 0.0);
    std::vector<double> outhistResult(data.size(), 0.0);
    for (int i = 0; i < outNbElement; ++i) {
        outmacdResult[outBegIdx + i] = macdResult[i];
        outsingalResult[outBegIdx + i] = singalResult[i];
        outhistResult[outBegIdx + i] = histResult[i];
    }
    this->manager->setColumnValues("MACD",index,outmacdResult);
    this->manager->setColumnValues("MACDSignal",index,outsingalResult);
    this->manager->setColumnValues("MACDHist",index,outhistResult);
}



template<typename IndexType,typename DataType>
std::pair<std::vector<IndexType>, std::vector<DataType>> TALibStrategy::extractColumnData() {
    auto DatelineVariant = (*this->manager)[(*this->manager).getIndexName()];
    auto lineVariant = (*this->manager)["Open"];

    // 提取索引列与数据列
    auto indexPtr = boost::get<boost::shared_ptr<Line::Line<int, std::string>>>(DatelineVariant);
    auto linePtr = boost::get<boost::shared_ptr<Line::Line<int, double>>>(lineVariant);

    // 检查数据列是否有效
    if (!linePtr) {
        std::cerr << "Error: Column 'Open' is not of the expected type." << std::endl;
        return std::pair<std::vector<IndexType>, std::vector<DataType>>();
    }

    // 提取数据并将其转换为 std::vector<double>
    std::vector<double> data = linePtr->getValuesList();
    int dataSize = data.size();

    // 如果没有数据，直接返回
    if (dataSize == 0) {
        std::cerr << "Error: No data available for SMA calculation." << std::endl;
        return std::pair<std::vector<IndexType>, std::vector<DataType>>();
    }

    // 构造 finalIndices (可以用于记录数据的索引)
    std::vector<int> finalIndices(dataSize);
    std::iota(finalIndices.begin(), finalIndices.end(), 0); // 用 std::iota 填充索引数组
    return std::make_pair(finalIndices,data);
}