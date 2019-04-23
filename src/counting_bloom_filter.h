#ifndef COLORINGCLASSIFER_COUNTING_BLOOM_FILTER_H
#define COLORINGCLASSIFER_COUNTING_BLOOM_FILTER_H

#include <cstdio>
#include <cstring>
#include <set>
#include "multi_way_bf.h"
#include "BOB_hash.h"

template<int num_bits, int k, int class_num = 2, int count_bits=4>
class CountingBloomFilter
{
public:
    static constexpr uint64_t bit_per_bf = num_bits / class_num;
    static constexpr uint64_t dword_per_bf = (bit_per_bf + 31) / 32;
    uint32_t bf[class_num * dword_per_bf];
    uint32_t count[(bit_per_bf * count_bits + 31) / 32];
    uint32_t class_idx;
    pair<set<uint32_t>, set<uint32_t>> flip;  // bit-change list, first for 0 to 1 and second for 1 to 0

    CountingBloomFilter(uint32_t c_idx=0) : class_idx(c_idx)
    {
        memset(bf, 0, sizeof(bf));
        memset(count, 0, sizeof(count));
        if (k > 32)
        {
            fprintf(stderr, "Way must less than 32");
            exit(-1);
        }
    }

    void inc_count(uint64_t pos)
    {
        pos = pos * count_bits;
        uint32_t c = count[pos / 32] << (32 - count_bits - pos % 32) >> (32 - count_bits);
        if (c == 0)
        {
            flip.first.insert(pos / count_bits);
            flip.second.erase(pos / count_bits);
        }
        if (c < (1u << count_bits) - 1)
            count[pos / 32] += 1u << (pos % 32);
    }

    bool dec_count(uint64_t pos)
    {
        pos = pos * count_bits;
        uint32_t c = count[pos / 32] << (32 - count_bits - pos % 32) >> (32 - count_bits);
        if (c == 0)
            return false;
        if (c == 1)
        {
            flip.first.erase(pos / count_bits);
            flip.second.insert(pos / count_bits);
        }
        count[pos / 32] -= 1u << (pos % 32);
        return (c == 1);
    }

    void insert(uint64_t key, int idx=-1)
    {
        if (idx == -1)
            idx = class_idx;
        for (int i = 0; i < k; ++i)
        {
            uint64_t pos = BOB_hashs[i](&key) % bit_per_bf;
            if (idx == class_idx)
            {
                inc_count(pos);
            }
            pos = pos * class_num + idx;
            bf[pos / 32] |= 1u << (pos % 32);
        }
    }

    void remove(uint64_t key)
    {
        for (int i = 0; i < k; ++i)
        {
            uint64_t pos = BOB_hashs[i](&key) % bit_per_bf;
            bool dec_flag = dec_count(pos);
            pos = pos * class_num + class_idx;
            if (dec_flag)
                bf[pos / 32] &= ~(1u << (pos % 32));
        }
    }

    uint32_t query_bf(uint64_t key, int idx=-1)  // detect if key in class idx by bf
    {
        if (idx == -1)
            idx = class_idx;
        uint32_t ret = 1;
        for (int i = 0; i < k; ++i)
        {
            uint64_t pos = BOB_hashs[i](&key) % bit_per_bf;
            pos = pos * class_num + idx;
            ret &= 1 & (bf[pos / 32] >> (pos % 32));
            if (!ret)
                return 0;
        }
        return 1;
    }

    vector<int> query_multiway(uint64_t key)  // find key in which class
    {
        uint32_t ret = (1u << class_num) - 1;
        for (int i = 0; i < k; ++i)
        {
            uint64_t pos = BOB_hashs[i](&key) % bit_per_bf;
            pos = pos * class_num;

            uint64_t item = (uint64_t(bf[pos / 32 + 1]) << 32) | bf[pos / 32];
            item >>= (pos % 32);
            ret &= item;
        }

        vector<int> result;
        for (int i = 0; i < class_num; ++i)
            if (ret & (1u << i))
                result.push_back(i);
        return result;
    }

    void update(int idx, pair<set<uint32_t>, set<uint32_t>> flipped)  // update with bit-change list
    {
        uint64_t pos;
        for (auto it = flipped.first.begin(); it != flipped.first.end(); it++)
        {
            pos = *it * class_num + idx;
            bf[pos / 32] |= 1u << (pos % 32);
        }
        for (auto it = flipped.second.begin(); it != flipped.second.end(); it++)
        {
            pos = *it * class_num + idx;
            bf[pos / 32] &= ~(1u << (pos % 32));
        }
    }

};
#endif //COLORINGCLASSIFER_COUNTING_BLOOM_FILTER_H
