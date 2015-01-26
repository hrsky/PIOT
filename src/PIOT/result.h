/*************************************************************************
	> File Name: result.h
 ************************************************************************/

#ifndef RESULT_H
#define RESULT_H

#include <iostream>
#include <map>
#include <string>
#include "answer.h"
using namespace std;
#define MAX_SET_NUM 1000
class Result {
public:
  Result(string pFilePath, string modelFileName);
  vector<Answer> compute_input();
  vector<Answer> get_answer();
  void print_ans_set();
  void access_atom(string line);
  bool isSat();
private:
  string pFilePath;
  string modelFileName;
  bool Sat;
  map<string,int> pi;
  vector<Answer> ans_set;
};

#endif
