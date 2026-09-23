#include <iostream>
#include <random>
#include <vector>
#include "p2a.h"
#include "timer.h"

constexpr size_t SIZE = 16777216;
constexpr size_t DIMENSION_SIZE = 256;
constexpr size_t CONV_SIZE =  262144; // 64 * 64 * 64

size_t rowMajorIndexA(size_t x, size_t y, size_t z) {
    return x + y*DIMENSION_SIZE + z*DIMENSION_SIZE*DIMENSION_SIZE;
}

size_t rowMajorIndexK(size_t x, size_t y, size_t z) {
    return x + y*4 + z*16;
}

size_t rowMajorIndexConv(size_t x, size_t y, size_t z) {
    return x + y*64 + z*64*64;
}

int main() {
    std::vector<uint64_t> rowMajArr(SIZE);
    std::vector<uint64_t> mortonArr(SIZE);
    std::vector<uint64_t> kA(64);
    std::vector<uint64_t> kB(64);
    std::vector<uint64_t> convA(CONV_SIZE);
    std::vector<uint64_t> convB(CONV_SIZE);

    std::mt19937_64 rng(0);
    Timer timer;
    uint64_t rowMajTime = 0;
    uint64_t mortonTime = 0;

    uint64_t next;
    for (size_t i = 0; i < DIMENSION_SIZE; ++i) {
        for (size_t j = 0; j < DIMENSION_SIZE; ++j) {
            for (size_t k = 0; k < DIMENSION_SIZE; ++k) {
                next = rng();
                rowMajArr[rowMajorIndexA(k, j, i)] = next;
                mortonArr[morton3d(k, j, i)] = next;


                if (i < 4 && j < 4 && k < 4) { // initializing the kernels
                    kA[rowMajorIndexK(k, j, i)] = i + j + k;
                    kB[morton3d(k, j, i)] = i + j + k;
                }
            }
        }
    }

    timer.restart();
    for (size_t i = 0; i < 64; ++i) {
        for (size_t j = 0; j < 64; ++j) {
            for (size_t k = 0; k < 64; ++k) {
                uint64_t val = 0;

                for (size_t l = 0; l < 4; ++l) {
                    for (size_t m = 0; m < 4; ++m) {
                        for (size_t n = 0; n < 4; ++n) {
                            val += rowMajArr[rowMajorIndexA(k*4 + n, j*4 + m, i*4 + l)] * kA[rowMajorIndexK(n, m, l)];
                        }
                    }
                } 

                convA[rowMajorIndexConv(k, j, i)] = val;
            }
        }
    }

    rowMajTime = timer.click<Timer::Micros>();
    timer.restart();
    for (size_t i = 0; i < 64; ++i) {
        for (size_t j = 0; j < 64; ++j) {
            for (size_t k = 0; k < 64; ++k) {
                uint64_t val = 0;

                for (size_t l = 0; l < 4; ++l) {
                    for (size_t m = 0; m < 4; ++m) {
                        for (size_t n = 0; n < 4; ++n) {
                            val += mortonArr[morton3d(k*4 + n, j*4 + m, i*4 + l)] * kB[morton3d(n, m, l)];
                        }
                    }
                } 

                convB[morton3d(k, j, i)] = val;
            }
        }
    }

    mortonTime = timer.click<Timer::Micros>();

    for (size_t i = 0; i < 64; ++i) {
        for (size_t j = 0; j < 64; ++j) {
            for (size_t k = 0; k < 64; ++k) {
                assert(convA[rowMajorIndexConv(k, j, i)] == convB[morton3d(k, j, i)]);
            }
        }
    }

    std::cout << rowMajTime << "\n";
    std::cout << mortonTime << "\n";

    return 0;
}
