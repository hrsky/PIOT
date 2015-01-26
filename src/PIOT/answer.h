/*************************************************************************
	> File Name: answer.h
 ************************************************************************/

#ifndef ANSWER_H
#define ANSWER_H

#include<iostream>
using namespace std;

struct Atom {
    int pre;
    int *t;
    int count;
};

struct Answer {
    Atom *a;
    int count;
};

#endif
