#include "src/Core.h"

using namespace EasyEngine;
using namespace Components;

struct Pos {
    int x, y;
};

struct Block {
    Vector2 pos;
    Size size;
};

int main() {
    fmt::println("=== Matrix2D Inverse Function Test ===");
    
    // 测试 1: 2x2 矩阵
    fmt::println("\n--- Testing 2x2 Matrix ---");
    Matrix2D<double> mat2(2, 2);
    mat2(0, 0) = 4; mat2(0, 1) = 7;
    mat2(1, 0) = 2; mat2(1, 1) = 6;
    Matrix2D<double> mmm = mat2;
    
    fmt::println("Original 2x2 matrix:");
    for (size_t i = 0; i < mmm.rows(); ++i) {
        for (size_t j = 0; j < mmm.cols(); ++j) {
            fmt::print("{:>8.2f}", mmm(i, j));
        }
        fmt::println("");
    }
    
    auto inv2 = mat2.inverse();
    if (inv2.rows() > 0 && inv2.cols() > 0) {
        fmt::println("Inverse 2x2 matrix:");
        for (size_t i = 0; i < inv2.rows(); ++i) {
            for (size_t j = 0; j < inv2.cols(); ++j) {
                fmt::print("{:>8.4f}", inv2(i, j));
            }
            fmt::println("");
        }
        
        // 验证：A * A⁻¹ = I
        auto identity2 = mat2 * inv2;
        fmt::println("Verification (A * A⁻¹):");
        for (size_t i = 0; i < identity2.rows(); ++i) {
            for (size_t j = 0; j < identity2.cols(); ++j) {
                fmt::print("{:>8.4f}", identity2(i, j));
            }
            fmt::println("");
        }
    }
    
    // 测试 2: 3x3 矩阵
    fmt::println("\n--- Testing 3x3 Matrix ---");
    Matrix2D<double> mat3(3, 3);
    mat3(0, 0) = 1; mat3(0, 1) = 2; mat3(0, 2) = 3;
    mat3(1, 0) = 0; mat3(1, 1) = 1; mat3(1, 2) = 4;
    mat3(2, 0) = 5; mat3(2, 1) = 6; mat3(2, 2) = 0;
    
    fmt::println("Original 3x3 matrix:");
    for (size_t i = 0; i < mat3.rows(); ++i) {
        for (size_t j = 0; j < mat3.cols(); ++j) {
            fmt::print("{:>8.2f}", mat3(i, j));
        }
        fmt::println("");
    }
    
    auto inv3 = mat3.inverse();
    if (inv3.rows() > 0 && inv3.cols() > 0) {
        fmt::println("Inverse 3x3 matrix:");
        for (size_t i = 0; i < inv3.rows(); ++i) {
            for (size_t j = 0; j < inv3.cols(); ++j) {
                fmt::print("{:>8.4f}", inv3(i, j));
            }
            fmt::println("");
        }
        
        // 验证：A * A⁻¹ = I
        auto identity3 = mat3 * inv3;
        fmt::println("Verification (A * A⁻¹):");
        for (size_t i = 0; i < identity3.rows(); ++i) {
            for (size_t j = 0; j < identity3.cols(); ++j) {
                fmt::print("{:>8.4f}", identity3(i, j));
            }
            fmt::println("");
        }
    }
    
    // 测试 3: 4x4 矩阵
    fmt::println("\n--- Testing 4x4 Matrix ---");
    Matrix2D<double> mat4(4, 4);
    mat4(0, 0) = 2; mat4(0, 1) = 1; mat4(0, 2) = 1; mat4(0, 3) = 0;
    mat4(1, 0) = 4; mat4(1, 1) = 3; mat4(1, 2) = 3; mat4(1, 3) = 1;
    mat4(2, 0) = 8; mat4(2, 1) = 7; mat4(2, 2) = 9; mat4(2, 3) = 5;
    mat4(3, 0) = 6; mat4(3, 1) = 7; mat4(3, 2) = 9; mat4(3, 3) = 8;
    
    fmt::println("Original 4x4 matrix:");
    for (size_t i = 0; i < mat4.rows(); ++i) {
        for (size_t j = 0; j < mat4.cols(); ++j) {
            fmt::print("{:>8.2f}", mat4(i, j));
        }
        fmt::println("");
    }
    
    auto inv4 = mat4.inverse();
    if (inv4.rows() > 0 && inv4.cols() > 0) {
        fmt::println("Inverse 4x4 matrix:");
        for (size_t i = 0; i < inv4.rows(); ++i) {
            for (size_t j = 0; j < inv4.cols(); ++j) {
                fmt::print("{:>8.4f}", inv4(i, j));
            }
            fmt::println("");
        }
        
        // 验证：A * A⁻¹ = I
        auto identity4 = mat4 * inv4;
        fmt::println("Verification (A * A⁻¹):");
        for (size_t i = 0; i < identity4.rows(); ++i) {
            for (size_t j = 0; j < identity4.cols(); ++j) {
                fmt::print("{:>8.4f}", identity4(i, j));
            }
            fmt::println("");
        }
    }
    
    // 测试 4: 非方阵（应该失败）
    fmt::println("\n--- Testing Non-Square Matrix (3x2) ---");
    Matrix2D<double> mat_non_square(3, 2);
    for (size_t i = 0; i < mat_non_square.rows(); ++i) {
        for (size_t j = 0; j < mat_non_square.cols(); ++j) {
            mat_non_square(i, j) = i * 2 + j + 1;
        }
    }
    
    fmt::println("Original 3x2 matrix:");
    for (size_t i = 0; i < mat_non_square.rows(); ++i) {
        for (size_t j = 0; j < mat_non_square.cols(); ++j) {
            fmt::print("{:>8.2f}", mat_non_square(i, j));
        }
        fmt::println("");
    }
    
    auto inv_non_square = mat_non_square.inverse();
    if (inv_non_square.rows() == 0 || inv_non_square.cols() == 0) {
        fmt::println("As expected, non-square matrix inversion failed!");
    }
    
    fmt::println("\n=== All tests completed! ===");
    return 0;
}