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
    Matrix2D<int> matrix(4, 6);
    Matrix2D<int> m_add(4, 6, 10);
    Matrix2D<int> m_multiply(4, 8, 2);
    static int n = 0;
    for (auto& i : matrix) {
        i = ++n;
    }
    auto time_point_1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());
//    matrix.rotate(false);、
//    matrix.transpose();
    Matrix2D<int> new_matrix(2, 4, 1);
    new_matrix.multiply(m_multiply);
    for (int i = 0; i < new_matrix.rows(); ++i) {
        for (int j = 0; j < new_matrix.cols(); ++j) {
            fmt::print("{:4} ", new_matrix(i, j));
        }
        fmt::println("");
    }
    auto time_point_2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());
    fmt::println("Sum: {}ms", time_point_2.count() - time_point_1.count());
    return 0;
}