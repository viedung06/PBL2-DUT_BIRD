#include "math_lib/MathLib.hpp"

namespace math_lib {

namespace {

// Hệ số chuỗi Taylor/Maclaurin cho sin(x) = x - x^3/3! + x^5/5! - x^7/7! ...
constexpr double C1  = 1.0;
constexpr double C3  = -1.0 / 6.0;                    // 3! = 6
constexpr double C5  = 1.0 / 120.0;                   // 5! = 120
constexpr double C7  = -1.0 / 5040.0;                 // 7! = 5040
constexpr double C9  = 1.0 / 362880.0;                // 9! = 362880
constexpr double C11 = -1.0 / 39916800.0;              // 11! = 39916800
constexpr double C13 = 1.0 / 6227020800.0;            // 13! = 6227020800
constexpr double C15 = -1.0 / 1307674368000.0;        // 15! = 1307674368000
constexpr double C17 = 1.0 / 355687428096000.0;       // 17! = 355687428096000

// Hàm chuẩn hóa góc về khoảng [-PI, PI] rồi rút về [-PI/2, PI/2]
double sinCore(double x) {
    // 1. Thu gọn góc lớn về (-2*PI, 2*PI)
    long long turns = static_cast<long long>(x / TWO_PI);
    x -= turns * TWO_PI;

    // 2. Đưa góc về chính xác đoạn [-PI, PI]
    while (x > PI) {
        x -= TWO_PI;
    }
    while (x < -PI) {
        x += TWO_PI;
    }

    // 3. Sử dụng tính chất đối xứng để đưa về đoạn [-PI/2, PI/2]
    if (x > HALF_PI) {
        x = PI - x;
    } else if (x < -HALF_PI) {
        x = -PI - x;
    }

    // 4. Khai triển chuỗi Taylor theo sơ đồ Horner để tối ưu độ chính xác và tốc độ
    double x2 = x * x;
    return x * (C1 + x2 * (C3 + x2 * (C5 + x2 * (C7 + x2 * (C9 + x2 * (C11 + x2 * (C13 + x2 * (C15 + x2 * C17))))))));
}

} // anonymous namespace

double sin(double x) {
    return sinCore(x);
}

float sin(float x) {
    return static_cast<float>(sinCore(static_cast<double>(x)));
}

double cos(double x) {
    return sinCore(x + HALF_PI);
}

float cos(float x) {
    return static_cast<float>(sinCore(static_cast<double>(x) + HALF_PI));
}

} // namespace math_lib

