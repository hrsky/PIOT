/*************************************************************************
	> File Name: model_compute.h
 ************************************************************************/

#ifndef model_compute_h
#define model_compute_h
#include <iostream>
#include <string>
#include "answer.h"
#define Max_Set_Num = 1000;

class model_compute {
public:
  model_compute(string pFilePath, string pipFileName);
  void compute_max_set(string msOutFile, bool isOutput);
  void print_max_set();
  int get_ans_set_count();
  answer get_ans_set_by_index(int index);
  int get_max_set_count();
  answer get_max_set_by_index(int index);
  ~model_compute();
private:
  string pFilePath;
  string pipFileName;
  answer ans_set[Max_Set_Num];
  answer max_set[Max_Set_Num];
}
#endif

