//
// Created by 94744 on 2024/6/17.
//
#include "backtest.h"

// 构造函数实现
ConditionTypeConverter::ConditionTypeConverter() {
    strToEnum = {
            {"GreaterThan", ConditionType::GreaterThan},
            {"LessThan", ConditionType::LessThan},
            {"EqualTo", ConditionType::EqualTo},
            {"GoldenCross", ConditionType::GoldenCross},
            {"DeathCross", ConditionType::DeathCross}
    };

    for (const auto& pair : strToEnum) {
        enumToStr[static_cast<int>(pair.second)] = pair.first;
    }
}

// toEnum方法实现
int ConditionTypeConverter::toEnum(const std::string& str) const {
    auto it = strToEnum.find(str);
    if (it != strToEnum.end()) {
        return static_cast<int>(it->second);
    } else {
        throw std::invalid_argument("Invalid string for ConditionType");
    }
}

// toString方法实现
std::string ConditionTypeConverter::toString(int value) const {
    auto it = enumToStr.find(value);
    if (it != enumToStr.end()) {
        return it->second;
    } else {
        throw std::invalid_argument("Invalid integer for ConditionType");
    }
}
