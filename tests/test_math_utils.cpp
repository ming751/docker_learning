#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <functional>
#include "my_project/math_utils.hpp"

// 简单的测试框架
class TestRunner {
private:
    int tests_run = 0;
    int tests_passed = 0;
    
public:
    void run_test(const std::string& test_name, std::function<void()> test_func) {
        tests_run++;
        try {
            test_func();
            tests_passed++;
            std::cout << "✅ " << test_name << " - PASSED" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "❌ " << test_name << " - FAILED: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "❌ " << test_name << " - FAILED: Unknown exception" << std::endl;
        }
    }
    
    void print_summary() {
        std::cout << std::endl;
        std::cout << "📊 测试总结: " << tests_passed << "/" << tests_run << " 通过" << std::endl;
        if (tests_passed == tests_run) {
            std::cout << "🎉 所有测试通过!" << std::endl;
        }
    }
    
    int get_failed_count() const {
        return tests_run - tests_passed;
    }
};

// 浮点数比较辅助函数
bool almost_equal(double a, double b, double epsilon = 1e-9) {
    return std::abs(a - b) < epsilon;
}

// 测试用例
void test_sum_basic() {
    std::vector<double> numbers = {1.0, 2.0, 3.0, 4.0, 5.0};
    double result = MyProject::sum(numbers);
    assert(almost_equal(result, 15.0));
}

void test_sum_empty() {
    std::vector<double> empty_vector;
    double result = MyProject::sum(empty_vector);
    assert(almost_equal(result, 0.0));
}

void test_average_basic() {
    std::vector<double> numbers = {2.0, 4.0, 6.0};
    double result = MyProject::average(numbers);
    assert(almost_equal(result, 4.0));
}

void test_average_empty() {
    std::vector<double> empty_vector;
    try {
        MyProject::average(empty_vector);
        assert(false); // 应该抛出异常
    } catch (const std::invalid_argument&) {
        // 预期的异常
    }
}

void test_max_basic() {
    std::vector<double> numbers = {1.5, 3.2, 2.1, 4.7, 2.9};
    double result = MyProject::max(numbers);
    assert(almost_equal(result, 4.7));
}

void test_min_basic() {
    std::vector<double> numbers = {1.5, 3.2, 2.1, 4.7, 2.9};
    double result = MyProject::min(numbers);
    assert(almost_equal(result, 1.5));
}

void test_variance_basic() {
    std::vector<double> numbers = {2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    double result = MyProject::variance(numbers);
    // 手动计算的期望值
    assert(result > 0); // 基本检查
}

void test_standard_deviation_basic() {
    std::vector<double> numbers = {2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    double variance_result = MyProject::variance(numbers);
    double std_dev_result = MyProject::standardDeviation(numbers);
    assert(almost_equal(std_dev_result, std::sqrt(variance_result)));
}

void test_variance_insufficient_data() {
    std::vector<double> single_element = {5.0};
    try {
        MyProject::variance(single_element);
        assert(false); // 应该抛出异常
    } catch (const std::invalid_argument&) {
        // 预期的异常
    }
}

int main() {
    std::cout << "🧪 运行 Math Utils 单元测试" << std::endl;
    std::cout << "============================" << std::endl;
    
    TestRunner runner;
    
    // 运行所有测试
    runner.run_test("Sum - Basic", test_sum_basic);
    runner.run_test("Sum - Empty Vector", test_sum_empty);
    runner.run_test("Average - Basic", test_average_basic);
    runner.run_test("Average - Empty Vector", test_average_empty);
    runner.run_test("Max - Basic", test_max_basic);
    runner.run_test("Min - Basic", test_min_basic);
    runner.run_test("Variance - Basic", test_variance_basic);
    runner.run_test("Standard Deviation - Basic", test_standard_deviation_basic);
    runner.run_test("Variance - Insufficient Data", test_variance_insufficient_data);
    
    runner.print_summary();
    
    return runner.get_failed_count();
}