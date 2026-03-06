#include <cstdint>

#include "KDTree.h"


#include <vector>





int main() {

    const vector<uint64_t> attr_set = {0,1};
    const vector<uint64_t> attr_set2 = {0,1};


    KDTree tree({{1,2},{3,3},{4,4}}, 4, 2,attr_set );
    KDTree tree2({{1,2},{3,3},{4,4}}, 4, 2, attr_set2);


    tree.build_tree();
    tree2.build_tree();

    join({tree2.bitvector,tree.bitvector},{0,2},{0,1}}, 6,3);

    return 0;
}



