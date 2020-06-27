#include <assert.h>
#include <stdlib.h>

#include <iostream>
#include <memory>
#include <utility>

#include "MergeTree.h"
#include "Tree.h"
#include "safe.h"
#include "union_find.h"

int main(int argc, char const* argv[]) {
    constexpr unsigned size = 10;
    auto edges = safe::vector<TreeNode>();
    auto pairs = safe::vector<std::pair<unsigned, unsigned>>();

    for (unsigned i = 0; i < size - 1; ++i) {
        for (unsigned j = i + 1; j < size; ++j) {
            pairs.push_back(std::make_pair(i, j));
        }
    }

    constexpr unsigned MAGIC = size * size;

    for (unsigned i = 0; i < MAGIC; ++i) {
        unsigned a = rand() % pairs.size(), b = rand() % pairs.size();
        std::swap(pairs[a], pairs[b]);
    }
    for (auto iter = pairs.begin(); iter != pairs.end(); ++iter) {
        std::cout << "[" << iter->first << ", " << iter->second << "] ";
    }
    std::cout << "\n";

    MergeTree::merge_tree(size, edges, pairs);

    std::cout << edges << "\n";

    return 0;
}
