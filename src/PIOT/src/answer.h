/*************************************************************************
	> File Name: answer.h
 ************************************************************************/

#ifndef ANSWER_H
#define ANSWER_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>

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
  int tbox_size;
  int abox_size;
  int curr_count;
  int find_count;
  double compute_time;
  int input_type;
  bool is_find;
  ofstream outfile;
  clock_t start_time;
  
  statistics(string statFileName) {
    classify_time = 0;
    repair_time = 0;
    tbox_size = 0;
    abox_size = 0;
    curr_count = 0;
    find_count = 0;
    compute_time = 0;
    input_type = 0;
    is_find = false;
    start_time=clock();
    outfile.open(statFileName, ofstream::trunc);
  }
  void write_total_statistics() {
    stringstream str;
    string temp;
    string toWrite = "---------------Total Statistics -------------------\r\n";
    
    str << tbox_size;
    str >> temp;
    str.clear();
    toWrite += "Initial Tbox Size: " + temp + "   ";
      
    str << abox_size;
    str >> temp;
    str.clear();
    toWrite += "Abox Size: " + temp + "\r\n";
    
    if (input_type == 0) {
      toWrite += "The benchmark is under normal set, ";
    } else if (input_type == 1) {
      
    } else if (input_type == 2) {
      toWrite += "The benchmark with weight, ";
    } else if (input_type == 3) {
      toWrite += "The benchmark with priority, ";
    }
    
    if (is_find) {
      toWrite += "query has been found and repair success.\r\n";
    } else {
      toWrite += "query has not been found.\r\n";
    }

    str << classify_time;
    str >> temp;
    str.clear();
    toWrite += "Classify Time: " + temp + "s.\r\n";
      
    str << repair_time;
    str >> temp;
    str.clear();
    toWrite += "Repair Time: " + temp + "s.\r\n";

    outfile << toWrite << endl;
    outfile.close();
  }

  void write_every_time_stat(int state) {
    stringstream str;
    string temp;
    str << find_count;
    str >> temp;
    string toWrite = "---------------Currently Computing " + temp + "--------------\r\n";
    str.clear();
    
    str << curr_count;
    str >> temp;
    str.clear();
    toWrite += "Deleted Tbox Size: " + temp + ".\r\n";
    
    str << compute_time;
    str >> temp;
    str.clear();
    toWrite += "Computing Time: " + temp + "s.\r\n";
    if (state)
      toWrite += "IsSatisfied: True.\r\n";
    else
      toWrite += "IsSatisfied: False.\r\n";
    
    clock_t end_time=clock();
    str << (static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC);
    str >> temp;
    str.clear();
    toWrite += "The program has been running " + temp + "s.\r\n";
    outfile << toWrite << endl;
  }
};

#endif
