#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

enum relation { PARENT, SIBLING, CHILD };

vector<string> readFromSystem(string argString);
relation getRelation(string topic1, string topic2);
vector<string> promisingChildren(string topic);

#endif // UTIL_H
