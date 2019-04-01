#include <iostream>
#include "console.h"
#include "util.h"
#include "tree.h"
#include "simpio.h"
#include "strlib.h"

using namespace std;

// creates tree by prompting user for topic and depth, creating tree and printing out the recursive cout functions
KnowledgeTree * createTree() {
    while(true) {
        string newTopic = getLine("Enter a topic (or enter to quit): ");
        if(newTopic.length() == 0) return NULL;
        string depth = getLine("Enter the desired depth of the tree (or enter for default of 3): ");
        if(depth.length() == 0) depth = "3";
        while(!stringIsInteger(depth))
            depth = getLine("Please enter an integer greater than 0");
        KnowledgeTree *tree = new KnowledgeTree(newTopic, stoi(depth));
        tree->fillTree(0);
        cout << "Final tree:" << endl;
        tree->recursiveToString(0, NULL);
        cout << "Tree by Level:" << endl;
        tree->mapToString();
        cout << "Trains of thought:" << endl;
        string cachedTrain = "";
        tree->trainOfThought(cachedTrain);
    }
}

int main() {
    KnowledgeTree *tree = createTree();
    return 0;
}
