#ifndef COLORINGCLASSIFER_CODED_BLOOM_FILTER_H
#define COLORINGCLASSIFER_CODED_BLOOM_FILTER_H

#include "multi_way_bf.h"
#include "BOB_hash.h"
#include "utils.h"


template<int num_bits, int k, int class_num = 2>
class CodedBloomFilter: public MultiWayBloomFilter<num_bits, k, log2(class_num)>
{
    constexpr static int num_bf = log2(class_num);
public:
    const string name;
    constexpr static int _class_num = class_num;

    CodedBloomFilter() : name("CodedBF")
    {
    }

    void insert(uint64_t key, int class_id)
    {
        int idx = 0;
        while (class_id) {
            if (class_id & 1) {
                MultiWayBloomFilter<num_bits, k, log2(class_num)>::insert_bf(key, idx);
            }
            idx += 1;
            class_id /= 2;
        }
    }

    int query(uint64_t key)
    {
        return MultiWayBloomFilter<num_bits, k, log2(class_num)>::query_multiway(key);
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

#endif //COLORINGCLASSIFER_CODED_BLOOM_FILTER_H
