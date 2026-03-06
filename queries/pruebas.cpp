#include <cstdint>
//
// Created by Sergi on 22-12-2025.
//

#include <fstream>
#include<bits/stdc++.h>
#include<ratio>
#include<chrono>
#include<ctime>


using namespace std::chrono;


#include "../src/KDTree.cpp"

high_resolution_clock::time_point start_select, stop_select;
double total_time_select = 0.0;
duration<double> time_span_select;

int main(int argc, char** argv)
{
    vector<uint64_t> att_R = {0,2};
    vector<uint64_t> att_S = {1,2};
    vector<uint64_t> att_T = {2,3};



    uint64_t grid_side =8; // es como +infty para wikidata

    //cout << "R" << endl;
    KDTree qdag_rel_R({{2,1},{4,1},{1,2},{4,3}}, grid_side ,2, att_R);
    //cout << "S" << endl;
    KDTree qdag_rel_S({{3,1},{2,2},{3,3}}, grid_side, 2, att_S);
    //cout << "T" << endl;
    KDTree qdag_rel_T({{1,4},{2,3}}, grid_side, 2, att_T);
    qdag_rel_R.build_tree();
    qdag_rel_S.build_tree();
    qdag_rel_T.build_tree();
    cout<< qdag_rel_R.points.size() << endl;
    cout<< qdag_rel_S.points.size() << endl;
    cout<< qdag_rel_T.points.size() << endl;

    for (auto i : qdag_rel_R.bitvector) {
        cout << i << endl;
    }
    for (auto i : qdag_rel_S.bitvector) {
        cout << i << endl;
    }
    for (auto i : qdag_rel_T.bitvector) {
        cout << i << endl;
    }

    // cout << ((((float)qdag_rel_R.size()*8) + ((float)qdag_rel_S.size()*8) + ((float)qdag_rel_T.size()*8) )/(rel_R->size()*2 + rel_S->size()*2 + rel_T->size()*2)) << "\t";


    vector<vector<bit_vector>> Q(3);

    Q[0] = qdag_rel_R.bitvector;
    Q[1] = qdag_rel_S.bitvector;
    Q[2] = qdag_rel_T.bitvector;

    vector<bit_vector> Join_Result;


    high_resolution_clock::time_point start, stop;
    double total_time = 0.0;
    duration<double> time_span;

    Join_Result = join(Q, {att_R,att_S,att_T}, 12,4); // warmup join

    start = high_resolution_clock::now();

    Join_Result = join(Q, {att_R,att_S,att_T}, 12,4);

    stop = high_resolution_clock::now();
    time_span = duration_cast<microseconds>(stop - start);
    total_time = time_span.count();

    for (auto i : Join_Result) {
        cout << i << endl;
    }

    cout << /*"Multiway Join ended in " <<*/ total_time /*<< " seconds"*/ << endl;

    return 0;
}