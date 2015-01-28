/*************************************************************************
	> File Name: translation.cpp
 ************************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include "translation.h"
#include <cstring>
using namespace std;

translation::translation(string pFilePath, string pFileName) {
  this->pFileName = pFileName;
  this->pFilePath = pFilePath;
  p_index = 0;
  tbox.clear();
  abox.clear();
}

void translation::trans() {
  ifstream infile;
  ofstream outfile;
  infile.open(pFilePath+pFileName);
  outfile.open(pFilePath+pFileName+".lp", ofstream::trunc);
  string line;
  char buff[1024];
  if (infile.is_open()) {
    while(infile.good() && !infile.eof()) {
      memset(buff,0,1024);
      infile.getline(buff,1204);
      line = buff;
      cout << line.c_str() << endl;
      if (isRule(line)) {
        cout << "1" << endl;
        string s = " not p";
        char buf[10];
        memset(buf,0,10);
        sprintf(buf, "%d", p_index);
        s += buf;
        s += ".\r\n";

        line.replace(line.length()-2,1, ",");
        line.erase(line.length()-1);
        line.append(s);
        line.append(choice_rule(p_index));
        outfile << line;
        p_index++;
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

void translation::classify() {
  ifstream infile;
  infile.open(pFilePath+pFileName);
  string str;
  char buff[1024];
  if (infile.is_open()) {
    while(infile.good() && !infile.eof()) {
      memset(buff,0,1024);
      infile.getline(buff,1204);
      str = buff;
      if (isRule(str)) {
        tbox.push_back(str);
      } else {
        if (isAbox(str)) {
          abox.push_back(str);
        }
      }
    }
  }
}

vector<string> translation::get_tbox() {
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
