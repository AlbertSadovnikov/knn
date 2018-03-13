#pragma once

#include <vector>
#include <random>

template<class T> constexpr T MIN_VAL = T(-1000);
template<class T> constexpr T MAX_VAL = T(1000);

template<class T>
T uniform_random_float(T min_val = MIN_VAL<T>, T max_val = MAX_VAL<T>) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<T> dis(min_val, max_val);
    return dis(gen);
}

template<class T>
std::vector<T> random_vector(size_t s, T min_val = MIN_VAL<T>, T max_val = MAX_VAL<T>) {
    std::vector<T> ret(s);
    std::generate(begin(ret), end(ret), [min_val, max_val](){ return uniform_random_float<T>(min_val, max_val); });
    return ret;
}
