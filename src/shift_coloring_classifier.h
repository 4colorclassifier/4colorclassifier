#ifndef COLORINGCLASSIFER_SHIFT_COLORING_FILTER_H
#define COLORINGCLASSIFER_SHIFT_COLORING_FILTER_H

#include <iostream>
#include "coloring_classifier.h"
#include "utils.h"

using namespace std;

template<uint32_t bucket_num, uint32_t color_num, uint32_t class_num>
class ShiftingColoringClassifier: protected ColoringClassifier<bucket_num, color_num>
{
    typedef ColoringClassifier<bucket_num, color_num> Parent;
    static constexpr int max_offset = log2(class_num);
public:
    string name;
    constexpr static int _class_num = class_num;

    ShiftingColoringClassifier() {
        name = "CC" + string(1, char('0' + color_num));
    }

    bool build(vector<pair<uint64_t, uint32_t>> & kvs, int data_num)
    {
        int counters[class_num][2];
        memset(counters, 0, sizeof(counters));

        for (int i = 0; i < data_num; ++i) {
            auto & kv = kvs[i];
            uint32_t val = kv.second;
            for (int k = 0; k < max_offset; ++k) {
                counters[k][(val >> k) & 1] += 1;
            }
        }

//        for (int i = 0; i < max_offset; ++i) {
//            cout << counters[i][0] << " " << counters[i][1] << endl;
//        }
//
//        cout << "try insert..." << endl;
        for (int i = 0; i < data_num; ++i) {
            uint64_t key = kvs[i].first;
            uint32_t val = kvs[i].second;

            typename Parent::CCEdge e(key);

            for (int k = 0; k < max_offset; ++k) {
                if ((val >> k) & 1) {
                    Parent::pos_edges.push_back(new typename Parent::CCEdge(e, k));
                }
                else {
                    Parent::neg_edges.push_back(new typename Parent::CCEdge(e, k));
                }
            }
        }

        return ColoringClassifier<bucket_num, color_num>::build();
    }

    uint32_t query(uint64_t key)
    {
        typename Parent::CCEdge e(key);

        uint32_t ret = 0;

        for (int k = 0; k < max_offset; ++k) {
            int c1 = Parent::get_bucket_val((e.hash_val_a + k) % bucket_num);
            int c2 = Parent::get_bucket_val((e.hash_val_b + k) % bucket_num);
            ret |= ((c1 == c2 ? 0 : 1u) << k);
        }

        return ret;
    }
};

#endif //COLORINGCLASSIFER_SHIFT_COLORING_FILTER_H
