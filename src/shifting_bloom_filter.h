#ifndef COLORINGCLASSIFER_SHIFT_BLOOM_FILTER_H
#define COLORINGCLASSIFER_SHIFT_BLOOM_FILTER_H

#include "BOB_hash.h"

template<int num_bits, int k, int class_num = 2>
class ShiftingBloomFilter
{
public:
    constexpr static int _class_num = class_num;
private:
    uint32_t bf[num_bits + class_num];

    void insert_bf(uint64_t key, int idx)
    {
        for (int i = 0; i < k; ++i) {
            int pos = BOB_hashs[i](&key) % num_bits;
            bf[pos / 32] |= 1u << ((pos + idx) % 32);
        }
    }

    uint32_t query_bf(uint64_t key)
    {
        uint32_t ret = (1u << class_num) - 1;
        for (int i = 0; i < k; ++i) {
            int pos = BOB_hashs[i](&key) % num_bits;
            uint32_t rotate = uint32_t((bf[pos / 32] >> (pos % 32)) | (bf[pos / 32] << (32 - (pos % 32))));
            ret &= rotate;
            if (!ret)
                return 0;
        }
        return ret;
    }

public:
    const string name;

    ShiftingBloomFilter() : name("ShiftBF")
    {
        memset(bf, 0, sizeof(bf));
    }

    void insert(uint64_t key, int class_id)
    {
        insert_bf(key, class_id);
    }

    int query(uint64_t key)
    {
        int result = -1;
        uint32_t query_result = query_bf(key);

        for (int i = 0; i < class_num; ++i) {
            if (query_result & (1u << i)) {
                if (result != -1) {
                    return -2;
                }
                result = i;
            }
        }

        return result;
    }

    bool build(KVList & kvs, int num)
    {
        for (int i = 0; i < num; ++i) {
            insert(kvs[i].first, kvs[i].second);
        }

        return true;
    }

    bool exp_build(uint64_t * keys, int num)
    {
        for (int i = 0; i < num / 2; ++i) {
            insert(keys[i], 0);
        }

        for (int i = num / 2; i < num; ++i) {
            insert(keys[i], 1);
        }

        return true;
    }

};

#endif //COLORINGCLASSIFER_SHIFT_BLOOM_FILTER_H
