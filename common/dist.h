#pragma once

#include <vector>
#include <assert.h>

template<class T>
T euclidean_distance(const std::vector<T>& a, const std::vector<T>& b) {
    assert(a.size()==b.size());
    T ret = T(0);
    for(size_t i = 0; i < a.size(); i++) {
        ret += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return std::sqrt(ret);
}

template<class T>
T dist(const std::vector<T>& a, const std::vector<T>& b) {
    return euclidean_distance<T>(a, b);
}
