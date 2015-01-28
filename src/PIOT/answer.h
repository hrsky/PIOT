/*************************************************************************
	> File Name: answer.h
 ************************************************************************/

#ifndef ANSWER_H
#define ANSWER_H

#include <iostream>
#include <vector>
using namespace std;

struct Pred {
    int pindex;
    int arity;
};

struct Atom {
//    int pre;
  Pred pre;
  vector<int> t;
};

struct Answer {
    vector<Atom> a;
};

#endif
