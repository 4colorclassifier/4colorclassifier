#include <iostream>
#include "coloring_classifier.hpp"

#define MAX_DATA 11000

#include "params.h"

using namespace std;

int load_data(const char * filename, uint64_t * data)
{
    FILE * f = fopen(filename, "rb");
    if (!f) {
        cout << "File " << filename << " not found." << endl;
        exit(-1);
    }
    fread(data, 8, MAX_DATA, f);
    fclose(f);

    printf("%s load done.\n", filename);

    return MAX_DATA;
}

template<int data_num>
int case_run(uint64_t * data, FILE * output_file)
{
    auto cc = new ColoringClassifier<data_num * ratio / 100, color>;

    int pos_num = data_num * pos_percentage / 100;
    int neg_num = data_num - pos_num;

    cc->set_pos_edge(data, pos_num);
    cc->set_neg_edge(data + pos_num, neg_num);

    bool build_result = cc->build();
    bool correctness_flag = true;

    int w_cnt = 0;
    if (build_result) {
        for (int i = 0; i < pos_num; ++i) {
            int r = cc->query(data[i]);
            if (r != 0) {
                w_cnt++;
            }
        }
        for (int i = pos_num; i < data_num; ++i) {
            int r = cc->query(data[i]);
            if (r != 1) {
                correctness_flag = false;
            }
        }
    }
    else {
    }

//    ostringstream st;
//    st << color << " " << ratio << " " << data_num << " " << pos_percentage << " ";
//    st << build_result << " " << correctness_flag << " " << cc->edge_collision_num << " " << w_cnt;
//    st << endl;
//
//    fprintf(output_file, "%s", st.str().c_str());

    printf("%d %d done.\n", color, ratio);

    delete cc;

    return 0;
}

int work(int data_id, const char * filename)
{
    FILE * output_file = stdout;
    if (filename) {
        output_file = fopen(filename, "a+");
    }

    char dataset_name[100];
    sprintf(dataset_name, "../data/data_bytes/%03d.dat", data_id);
    uint64_t * data = new uint64_t[MAX_DATA];

    load_data(dataset_name, data);

    printf("%d start.\n", data_id); fflush(stdout);
    case_run<10000>(data, output_file);
    printf("%d end.\n", data_id); fflush(stdout);

    delete[] data;

    if (filename) {
        fclose(output_file);
    }

    return 0;
}

int main(int argc, char ** argv)
{
    char * filename = NULL;
    if (argc == 2) {
        filename = argv[1];
    }

    for (int i = 0; i < 20; ++i) {
        work(i, filename);
//        thread_pool.submit([i, filename]{ work(i, filename); });
    }

    return 0;
}