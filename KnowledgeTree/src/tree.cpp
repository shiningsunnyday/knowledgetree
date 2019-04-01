#include "tree.h"
#include "util.h"
#include "vector.h"

KnowledgeTree::KnowledgeTree(string topic, int depth) {
    this->originKnode = new knode { topic, 0, Vector<knode *>() };
    this->levels = 0;
    this->depth = depth;
    this->levelsMapping = Map<int, Set<knode *>> {{-1, Set<knode *> {originKnode}}, {0, Set<knode *> {originKnode} }};
}

// Didn't have time to completely implement this but this would give user more manual control over the starting
// subtopics, which can improve the accuracy of the rest of the tree
void KnowledgeTree::initializeStartingBranches() {
    levels++;
    for(string promisingChild: readFromSystem(stringReplace(this->originKnode->topic, " ", ","))) {
        cout << "Promising child " << promisingChild << endl;
        relationship relationship = getRelationship(this->originKnode->topic, promisingChild);
        cout << "Is of relationship " << relationship.relation << " " << relationship.childScore << endl;
        if(relationship.relation == CHILD) {
            this->originKnode->children.add(new knode { promisingChild, relationship.childScore, Vector<knode *>() });
        }
    }
}

// Class method that fills tree at a given level by doing a breadth-first iteration over all nodes at the level
// using the get_initial .py function to get all potential children and adding the ones of relation child
// via addToTree
// is terminated when terminatingCondition is reached, which for simplicity is the depth of the tree
// i.e. originKnode is level 0
void KnowledgeTree::fillTree(int level) {
    cout << "Filling tree at level " << level << endl;
    for(knode * knodeAtLevel: levelsMapping[level]) {
        cout << "Building (sub)tree with " << knodeAtLevel->topic << endl;
        if(!levelsMapping[-1].contains(knodeAtLevel)) continue;
        Vector<string> topicResult = readFromSystem(stringReplace(knodeAtLevel->topic, " ",","));
        if(topicResult.size() == 0) continue;
        for(int i = 0; i < topicResult.size(); i++) {
            cout << "Adding to tree " << topicResult[i] << endl;
            addToTree(knodeAtLevel, topicResult[i], level + 1);
            cout << "Added. Tree now looks like:" << endl;
            recursiveToString();
        }
    }
    if(levelsMapping[level + 1].size() > 0 && !terminatingCondition()) fillTree(level + 1);
}

// adds given topic to node by first searching node's children for the best potential parent (highest childScore)
// if no parent is found, gets added as one of node's own children
// also added to the set of knodes at curLevel for later reference
void KnowledgeTree::addToTree(knode *node, string topic, int curLevel) {
    relationship initial = getRelationship(node->topic, topic);
    knode * bestParent = findBestParent(node->children, topic);
    if(bestParent != NULL) {
        cout << "Best parent found " << bestParent->topic << endl;
        cout << "Recursing down to level " << curLevel + 1 << endl;
        addToTree(bestParent, topic, curLevel + 1);
    } else {
        cout << "No more parents found, so I'll add it at level " << curLevel << endl;
        if(curLevel > levels) levels = curLevel;
        knode * newkNode = new knode { topic, initial.childScore, Vector<knode *>() };
        node->children.add(newkNode);
        levelsMapping[curLevel].add(newkNode);
        // is also added to levelsMapping[-1] which keeps track of all knodes for later reference
        levelsMapping[-1].add(newkNode);
    }
}

// helper function that given a list of knod->> *'s, selects the parent with the highest
// relationship score, returns NULL if none are parents
knode * KnowledgeTree::findBestParent(Vector<knode *> candidates, string topic) {
    knode * bestParent = NULL;
    double bestScore = 0;
    for(knode * parent: candidates) {
        cout << "Potential parent is " << parent->topic << endl;
        relationship relationship = getRelationship(parent->topic, topic);
        if(relationship.relation == PARENT and relationship.childScore > bestScore) {
            bestParent = parent;
            bestScore = relationship.childScore;
        }
    }
    return bestParent;
}

// given a starting node, couts all nested topics
void KnowledgeTree::recursiveToString(int level, knode * startNode) {
    if(startNode == NULL) startNode = originKnode;
    for(int i = 0; i < level; i++) cout << "   ";
    cout << startNode->topic << endl;
    for(knode * child: startNode->children)
        recursiveToString(level + 1, child);
}

// for each level, prints out each member and all of its children, used for testing purposes
void KnowledgeTree::mapToString() {
    for(int i : levelsMapping.keys()) {
        if(i < 0) continue;
        cout << "Level " << i << ":" << endl;
        for(knode * levelKnode : levelsMapping[i]) {
            cout << levelKnode->topic << " which has children: ";
            for(knode * childKnode : levelKnode->children) {
                cout << " " << childKnode->topic << endl;
            }
        }
    }
}

// Given a starting node, prints out all segments of topics stemming down from it
// cachedTrain is used in this depth-first approach to store temporary segments
void KnowledgeTree::trainOfThought(string &cachedTrain, knode * startingNode) {
    if(startingNode == NULL) startingNode = originKnode;
    if(startingNode->children.size() == 0) cout << cachedTrain + startingNode->topic << endl;
    cachedTrain += (startingNode->topic + " -> ");
    for(knode * child : startingNode->children)
        trainOfThought(cachedTrain, child);
    // by now all segments stemming from startingNode is complete, so cachedTrain will remove startingNode
    int index = stringIndexOf(cachedTrain, startingNode->topic);
    cachedTrain = cachedTrain.substr(0, index);
}

// terminates tree once levels exceeds depth (> because of zero-indexing)
bool KnowledgeTree::terminatingCondition() {
    return levels > depth;
}
