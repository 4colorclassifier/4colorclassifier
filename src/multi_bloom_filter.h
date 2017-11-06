#ifndef COLORINGCLASSIFER_MULTI_BLOOM_FILTER_H
#define COLORINGCLASSIFER_MULTI_BLOOM_FILTER_H

#include "multi_way_bf.h"
#include "BOB_hash.h"

template<int num_bits, int k, int class_num = 2>
class MultiBloomFilter: public MultiWayBloomFilter<num_bits, k, class_num>
{
public:
    const string name;
    constexpr static int _class_num = class_num;

    MultiBloomFilter() : name("MultiBF")
    {
    }

    void insert(uint64_t key, int class_id)
    {
        MultiWayBloomFilter<num_bits, k, class_num>::insert_bf(key, class_id);
    }

    int query(uint64_t key)
    {
        int result = -2;
        uint32_t qr = MultiWayBloomFilter<num_bits, k, class_num>::query_multiway(key);
//        if (!qr) {
//            return -2;
//        }

//        for (int i = 0; i < class_num; ++i) {
//            if (MultiWayBloomFilter<num_bits, k, class_num>::query_bf(key, i))
//                return i;
//        }

        for (int i = 0; i < class_num; ++i) {
            if (qr & (1u << i)) {
                if (result >= 0) {
                    return -1;
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

#endif //COLORINGCLASSIFER_MULTI_BLOOM_FILTER_H
