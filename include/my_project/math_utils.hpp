#pragma once
#include <vector>
#include <string>

namespace MyProject {
    // 基本数学运算
    double sum(const std::vector<double>& numbers);
    double average(const std::vector<double>& numbers);
    double max(const std::vector<double>& numbers);
    double min(const std::vector<double>& numbers);
    
    // 统计函数
    double variance(const std::vector<double>& numbers);
    double standardDeviation(const std::vector<double>& numbers);
    
    // 工具函数
    void printVector(const std::vector<double>& numbers, const std::string& label = "Vector");
}