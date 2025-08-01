#include "my_project/math_utils.hpp"
#include <numeric>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

namespace MyProject {
    double sum(const std::vector<double>& numbers) {
        if (numbers.empty()) {
            return 0.0;
        }
        return std::accumulate(numbers.begin(), numbers.end(), 0.0);
    }
    
    double average(const std::vector<double>& numbers) {
        if (numbers.empty()) {
            throw std::invalid_argument("Cannot calculate average of empty vector");
        }
        return sum(numbers) / numbers.size();
    }
    
    double max(const std::vector<double>& numbers) {
        if (numbers.empty()) {
            throw std::invalid_argument("Cannot find max of empty vector");
        }
        return *std::max_element(numbers.begin(), numbers.end());
    }
    
    double min(const std::vector<double>& numbers) {
        if (numbers.empty()) {
            throw std::invalid_argument("Cannot find min of empty vector");
        }
        return *std::min_element(numbers.begin(), numbers.end());
    }
    
    double variance(const std::vector<double>& numbers) {
        if (numbers.size() < 2) {
            throw std::invalid_argument("Need at least 2 numbers to calculate variance");
        }
        
        double mean = average(numbers);
        double sum_squared_diff = 0.0;
        
        for (const double& num : numbers) {
            double diff = num - mean;
            sum_squared_diff += diff * diff;
        }
        
        return sum_squared_diff / (numbers.size() - 1);
    }
    
    double standardDeviation(const std::vector<double>& numbers) {
        return std::sqrt(variance(numbers));
    }
    
    void printVector(const std::vector<double>& numbers, const std::string& label) {
        std::cout << label << ": [";
        for (size_t i = 0; i < numbers.size(); ++i) {
            std::cout << std::fixed << std::setprecision(2) << numbers[i];
            if (i < numbers.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }
}