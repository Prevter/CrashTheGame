#include "utils.hpp"

namespace util {

    std::mt19937& getRng() {
        static thread_local std::mt19937 gen(std::random_device{}());
        return gen;
    }

    int32_t randInt(int32_t min, int32_t max) {
        std::uniform_int_distribution<int32_t> dist(min, max);
        return dist(getRng());
    }

}