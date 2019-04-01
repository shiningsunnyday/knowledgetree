#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

struct knode {
    string topic;
    double childScore;
    vector<knode *> children;
};

class KnowledgeTree {
public:
    KnowledgeTree(string topic);
    ~KnowledgeTree();

    void initializeStartingBranches();
    void addToTree(knode *node, string topic);
    knode * findBestParent(vector<knode *>);
    bool terminatingCondition();

private:
    knode *originKnode;
};

#endif // TREE_H
