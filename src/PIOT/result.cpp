/*************************************************************************
	> File Name: result.cpp
 ************************************************************************/

#include <iostream>
#include <map>
#include <fstream>
#include <cstring>
#include "result.h"
#include "answer.h"

using namespace std;

Result::Result(string pFilePath, string modelFileName) {
  this->pFilePath = pFilePath;
  this->modelFileName = modelFileName;
  Sat = false;
  pi.clear();
  set_count = 0;
}

Answer* Result::compute_input() {
  ifstream infile;
  infile.open(pFilePath+modelFileName);
  char buff[1024];
  string line;
  bool is_a_set = false;
  if (infile.is_open()) {
    while (infile.good() && !infile.eof()) {
      memset(buff,0,1024);
      infile.getline(buff,1024);
      line = buff;
      char buf[30];
      memset(buf,0,30);
      sprintf(buf,"Answer: %d",set_count+1);
      if (is_a_set) {
        access_atom(line);
      }

      if (strcmp(buff,buf) == 0) {
        is_a_set = true;
        set_count++;
      } else {
        is_a_set = false;
      }
      strcpy(buf,"SATISFIABLE");
      if (strcmp(buff,buf) == 0) {
        Sat = true;
      }
    }
  }
  return ans_set;
}

void Result::access_atom(string line) {
  char buf[30];
  bool state = 0;
  for (int i = 0; i < line.length(); i++) {
    cout << line.at(i) << endl;
//    if (strcmp(line.at(i),"(") == 0 || strcmp(line.at(i)," ") == 0) {
//      cout << line.at(i) << endl;
//    }
//    strcat(buf,line.at(i));
  }
}

int split(const string& str, vector<string>& ret_, string sep = ",")
{
    if (str.empty())
    {
        return 0;
    }
    
    string tmp;
    string::size_type pos_begin = str.find_first_not_of(sep);
    string::size_type comma_pos = 0;
    
    while (pos_begin != string::npos)
    {
        comma_pos = str.find(sep, pos_begin);
        if (comma_pos != string::npos)
        {
            tmp = str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
        }
        else
        {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }
        
        if (!tmp.empty())
        {
            ret_.push_back(tmp);
            tmp.clear();
        }
    }
    return 0;
}

Answer* Result::get_answer() {
  return ans_set;
}

void Result::print_ans_set() {
  for (int i = 0; i < set_count; i++) {
    int atom_count = ans_set[i].count;
    Atom* atemp = ans_set[i].a;
    cout << "Set is :" << endl;
    for (int j = 0; j < atom_count; j++) {
      cout << pi[atemp[j].pre] << ": ";
      int t_count = atemp[i].count;
      for (int k = 0; k < t_count; k++) {
        cout << pi[atemp[j].t[k]] << " ";
      }
      cout << endl;
    }
    cout << endl;
  }
}

bool Result::isSat() {
  return Sat;
}