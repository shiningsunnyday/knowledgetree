#ifndef BRANCH_H
#define BRANCH_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "tree.h"

using namespace std;

class KnowledgeBranch : KnowledgeTree {
public:
    KnowledgeBranch();
    ~KnowledgeBranch();

    bool terminatingCondition();

private:
    knode *startKnode;
};

#endif // BRANCH_H
