#ifndef COLORINGCLASSIFER_MULTI_WAY_BLOOM_FILTER_H
#define COLORINGCLASSIFER_MULTI_WAY_BLOOM_FILTER_H

#include <cstdio>
#include <cstring>
#include "BOB_hash.h"

template<uint64_t num_bits, int k, int way>
class MultiWayBloomFilter
{
    static constexpr uint64_t bit_per_bf = num_bits / way;
    static constexpr uint64_t dword_per_bf = (bit_per_bf + 31) / 32;
    uint32_t bf[way * dword_per_bf];
protected:
    void insert_bf(uint64_t key, int idx)
    {
        for (int i = 0; i < k; ++i) {
            uint64_t pos = BOB_hashs[i](&key) % bit_per_bf;
            pos = pos * way + idx;
            bf[pos / 32] |= 1u << (pos % 32);
        }
    }

    uint32_t query_bf(uint64_t key, int idx)
    {
        uint32_t ret = 1;
        for (int i = 0; i < k; ++i) {
            uint64_t pos = BOB_hashs[i](&key) % bit_per_bf;
            pos = pos * way + idx;
            ret &= 1 & (bf[pos / 32] >> (pos % 32));
            if (!ret) return 0;
        }
        return 1;
    }

    uint32_t query_multiway(uint64_t key)
    {
        uint32_t ret = (1u << way) - 1;
        for (int i = 0; i < k; ++i) {
            uint64_t pos = BOB_hashs[i](&key) % bit_per_bf;
            pos = pos * way;

            uint64_t item = (uint64_t(bf[pos / 32 + 1]) << 32) | bf[pos / 32];
            item >>= (pos % 32);
            ret &= item;
            if (!ret) {
                return 0;
            }
        }

        return ret;
    }
public:
    MultiWayBloomFilter() {
        memset(bf, 0, sizeof(bf));

        // only support way less than 32 ><
        if (way > 32) {
            fprintf(stderr, "Way must less than 32");
            exit(-1);
        }
    }
};

#endif //COLORINGCLASSIFER_MULTI_WAY_BLOOM_FILTER_H
