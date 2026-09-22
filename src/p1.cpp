#include <iostream>
#include <random>
#include <vector>
#include "timer.h"

constexpr size_t SIZE = 16000000;
constexpr size_t DIMENSION_SIZE = 4000;

int main() {
    std::vector<int> rowMajArr(SIZE);
    std::vector<int> colMajArr(SIZE);
    std::mt19937_64 rng(0);
    uint64_t next;
    for (size_t i = 0; i < DIMENSION_SIZE; ++i) {
        for (size_t j = 0; j < DIMENSION_SIZE; ++j) {
                next = rng();
                rowMajArr[i*DIMENSION_SIZE + j] = next;
                colMajArr[i + j*DIMENSION_SIZE] = next;
        }
    }

    uint64_t sum1 = 0;
    uint64_t sum2 = 0;
    Timer timer;
    uint64_t rowMajTime = 0;
    uint64_t colMajTime = 0;
    timer.restart();
    for (size_t i = 0; i < DIMENSION_SIZE; ++i) {
        for (size_t j = 0; j < DIMENSION_SIZE; ++j) {
                sum1 += rowMajArr[i*DIMENSION_SIZE + j];
        }
    }
    rowMajTime = timer.click<Timer::Micros>();

    timer.restart();
    for (size_t i = 0; i < DIMENSION_SIZE; ++i) {
        for (size_t j = 0; j < DIMENSION_SIZE; ++j) {
                sum2 += colMajArr[i + j*DIMENSION_SIZE];
        }
    }
    colMajTime = timer.click<Timer::Micros>();

    std::cout << rowMajTime << " " << sum1 << "\n";
    std::cout << colMajTime << " " << sum2 << "\n";


    return 0;
}
