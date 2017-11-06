#ifndef COLORINGCLASSIFER_UTILS_H
#define COLORINGCLASSIFER_UTILS_H

constexpr int log2(int n)
{
    return ((n <= 2) ? 1 : 1 + log2(n / 2));
}

typedef vector<pair<uint64_t, uint32_t>> KVList;

#endif //COLORINGCLASSIFER_UTILS_H
