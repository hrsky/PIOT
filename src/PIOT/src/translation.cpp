/*************************************************************************
	> File Name: translation.cpp
 ************************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include "translation.h"
#include <cstring>
#include <cstdlib>
#include <time.h> 
#include <stdio.h>

using namespace std;

translation::translation(string pFilePath, string pFileName) {
  this->pFileName = pFileName;
  this->pFilePath = pFilePath;
  p_index = 0;
  tbox.clear();
  abox.clear();
}

void translation::trans(int output_type) {
  ifstream infile;
  ofstream outfile;
  infile.open(pFilePath+pFileName);
  if (output_type == 2) {
    outfile.open(pFilePath+pFileName+"-with-weight", ofstream::trunc);
  } else if (output_type == 3) {
    outfile.open(pFilePath+pFileName+"-with-priority", ofstream::trunc);
  } else {
    outfile.open(pFilePath+pFileName+".lp", ofstream::trunc);
  }
  string line;
  char buff[1024];
  srand((unsigned)time(NULL));
  if (infile.is_open()) {
    while(infile.good() && !infile.eof()) {
      memset(buff,0,1024);
      infile.getline(buff,1204);
      line = buff;
      cout << line.c_str() << endl;
      if (isRule(line)) {
        outfile << line << endl;
        memset(buff,0,1024);
        if (output_type == 2) {
          sprintf(buff, "%d", rand()%10+1);
          line = buff;
          outfile << line << endl;
        } else if (output_type == 3) {
          sprintf(buff, "%d", rand()%3);
          line = buff;
          outfile << line << endl;
        }
//        cout << "1" << endl;
//        string s = " not p";
//        char buf[10];
//        memset(buf,0,10);
//        sprintf(buf, "%d", p_index);
//        s += buf;
//        s += ".\r\n";
//
//        line.replace(line.length()-2,1, ",");
//        line.erase(line.length()-1);
//        line.append(s);
//        line.append(choice_rule(p_index));
//        outfile << line;
//        p_index++;
      } else {
        outfile << line << endl;
      }
    }
  }
  outfile.close();
}

bool isAbox(string line) {
  bool isabox = true;
  if (line.find("%") == 0) {
    isabox = false;
  } else {
    isabox = true;
  }
  return isabox;
}

void translation::classify(int input_type) {
  ifstream infile;
  infile.open(pFilePath+pFileName);
  string str;
  char buff[1024];
  p_index = 0;
  if (infile.is_open()) {
    while(infile.good() && !infile.eof()) {
      memset(buff,0,1024);
      infile.getline(buff,1204);
      str = buff;
      if (isRule(str)) {
        Rule r(p_index,str);
        if (input_type == 2) {
          memset(buff,0,1024);
          infile.getline(buff,1204);
          r.weight = atoi(buff);
        } else if (input_type == 3) {
          memset(buff,0,1024);
          infile.getline(buff,1204);
          r.priority = atoi(buff);
        }
        tbox.push_back(r);
        p_index++;
      } else {
        if (isAbox(str)) {
          abox.push_back(str);
        }
      }
    }
  }
}

vector<Rule> translation::get_tbox() {
  return tbox;
}

vector<string> translation::get_abox() {
  return abox;
}

string translation::choice_rule(int index) {
  string c_line;
  char buf[100];
  memset(buf,0,100);
  sprintf(buf, "a%d :- not p%d.\r\np%d :- not a%d.\r\n", p_index,p_index,p_index,p_index);
  c_line = buf;
  return c_line;
}

bool translation::isRule(string line) {
  bool isrule = true;
  if (line.find(":-") != line.npos) {
    isrule = true;
  } else {
    isrule = false;
  }
  return isrule;
}
