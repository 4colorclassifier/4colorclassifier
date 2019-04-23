#include <iostream>
#include <ctime>
#include <random>
#include "coloring_classifier.h"
#include "shift_coloring_classifier.h"
#include "coded_bloom_filter.h"
#include "multi_bloom_filter.h"
#include "shifting_bloom_filter.h"
#include "counting_bloom_filter.h"

#define MAXN 100000

void test_two_set()
{
    // generate 10k data randomly
    vector<pair<uint64_t, uint32_t>> data(MAXN);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(0, 1ull << 40);

    unordered_set<uint64_t> filter;

    for (int i = 0; i < MAXN; ++i) {
        uint64_t item;
        while (1) {
            item = dis(gen);
            if (filter.find(item) == filter.end()) {
                filter.insert(item);
                break;
            }
        }
        data[i].first = item;
        data[i].second = i % 2;
    }

    auto cc = new ShiftingColoringClassifier<int(MAXN * 1.11), 4, 2>();
    // auto cc = new MultiBloomFilter<int(MAXN * 10), 4, 2>();

    bool build_result = cc->build(data, MAXN);
    cout << "Build 4-color classifier for " << MAXN << " items" << endl;
    cout << "Using " << int(MAXN * 1.11) << " buckets" << endl;
    cout << "Build result: " << (build_result ? "success": "failed") << endl;

    if (build_result) {
        int err_cnt = 0;
        for (int i = 0; i < MAXN; ++i) {
            uint32_t result = cc->query(data[i].first);
            err_cnt += (result != data[i].second);
        }
        cout << "Error count: " << err_cnt << endl;
    }
}

void cbf_test()
{
    auto cbf = CountingBloomFilter<8, 2, 2, 2>();
    int a = 0, b = 0, c = 0;
    while(a >= 0)
    {
        cin >> a >> b;
        switch (a)
        {
            case 1:
                cin >> c;
                cbf.insert(b, c);
                break;
            case 2:
                cbf.remove(b);
                break;
            case 3:
                cin >> c;
                cout << bool(cbf.query_bf(b, c)) << endl;
                break;
            case 4:
                cout << cbf.query_multiway(b).size() << endl;
                break;
            case 5:
                cbf.update(1, cbf.flip);
                break;
        }
        cout << cbf.bf[0] << " " << cbf.count[0] << endl;
    }
}

int main()
{
    test_two_set();
    return 0;
}