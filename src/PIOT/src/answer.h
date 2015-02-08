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
  int tbox_size;
  int abox_size;
  int curr_count;
  int find_count;
  double found_query_time;
  double compute_time;
  int input_type;
  bool is_find;
  ofstream outfile;
  time_t start_time;
  double total_time;
  
  statistics(string statFileName) {
    classify_time = 0;
    found_query_time = 0;
    total_time = 0;
    tbox_size = 0;
    abox_size = 0;
    curr_count = 0;
    find_count = 0;
    compute_time = 0;
    input_type = 0;
    is_find = false;
    start_time=time(NULL);
    outfile.open(statFileName, ofstream::trunc);
  }
  void find_query() {
    if (found_query_time == 0) {
      is_find = 1;
      time_t end_time=time(NULL);
      found_query_time = difftime(end_time,start_time);
    }
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
      str << found_query_time;
      str >> temp;
      str.clear();
      toWrite += "Find Query Time is: " + temp + "s.\r\n";
    } else {
      toWrite += "query has not been found.\r\n";
    }

    str << classify_time;
    str >> temp;
    str.clear();
    toWrite += "Classify Time: " + temp + "s.\r\n";
    
    time_t end_time=time(NULL);
    total_time = difftime(end_time,start_time);
    str << total_time;
    str >> temp;
    str.clear();
    toWrite += "Total Time: " + temp + "s.\r\n";

    if (total_time > 3600){
      toWrite += "Time out and repair has not been completed.\r\n";
    }
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
    
    time_t end_time=time(NULL);
    total_time = difftime(end_time,start_time);
    str << total_time;
    str >> temp;
    str.clear();
    toWrite += "The program has been running " + temp + "s.\r\n";
    outfile << toWrite << endl;
  }
};

#endif
