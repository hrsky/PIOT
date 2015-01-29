/*************************************************************************
  > File Name: query.h
 ************************************************************************/

#ifndef RULE_H
#define RULE_H
#include<iostream>
using namespace std;

class Rule {
  public:
    Rule(int i, string s, int w = 1, int pri = 1);
    bool operator==(const Rule&);

    int index;
    string ruleString;
    int priority;
    int weight;
};

#endif
