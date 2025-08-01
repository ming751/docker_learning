#include <iostream>
#include <vector>
#include <iomanip>
#include "my_project/math_utils.hpp"

int main() {
    std::cout << "🐳 Hello from Docker container!" << std::endl;
    std::cout << "📊 C++ Math Utils Demo" << std::endl;
    std::cout << "======================" << std::endl;
    
    // 测试数据集
    std::vector<double> numbers = {1.5, 2.3, 3.7, 4.1, 5.9, 2.8, 6.2, 1.9, 4.5, 3.3};
    
    try {
        // 显示数据
        MyProject::printVector(numbers, "数据集");
        std::cout << std::endl;
        
        // 基本统计
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "📈 基本统计信息:" << std::endl;
        std::cout << "   总和: " << MyProject::sum(numbers) << std::endl;
        std::cout << "   平均值: " << MyProject::average(numbers) << std::endl;
        std::cout << "   最大值: " << MyProject::max(numbers) << std::endl;
        std::cout << "   最小值: " << MyProject::min(numbers) << std::endl;
        std::cout << "   方差: " << MyProject::variance(numbers) << std::endl;
        std::cout << "   标准差: " << MyProject::standardDeviation(numbers) << std::endl;
        
        std::cout << std::endl;
        std::cout << "✅ 所有计算完成!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ 错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}