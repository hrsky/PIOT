/*************************************************************************
	> File Name: answer.h
 ************************************************************************/

#ifndef ANSWER_H
#define ANSWER_H

#include <iostream>
#include <vector>
using namespace std;

struct Atom {
  int pre;
  vector<int> t;
};

struct Answer {
  vector<Atom> a;
};

class statistics {
public:
  double classify_time;
  double repair_time;
  int deleted_count;
  int find_count;
  double compute_time;
  int input_type;
  int is_find;
  statistics() {
    classify_time = 0;
    repair_time = 0;
    deleted_count = 0;
    find_count = 0;
    compute_time = 0;
    input_type = 0;
    is_find = 0;
  }
  void write_statistics() {
  }
};

#endif
