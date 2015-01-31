/*************************************************************************
	> File Name: result.cpp
 ************************************************************************/

#include <iostream>
#include <map>
#include <fstream>
#include <cstring>
#include <vector>
#include "result.h"
#include "answer.h"
#include "Dict.h"

using namespace std;

Result::Result(string pFilePath, string modelFileName) {
  this->pFilePath = pFilePath;
  this->modelFileName = modelFileName;
  Sat = 0;
  ans_set.clear();
}

void Result::reset() {
  Sat = 0;
  ans_set.clear();
}

vector<Answer> Result::compute_input() {
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
      sprintf(buf,"Answer: %lu",ans_set.size()+1);
      if (is_a_set) {
        access_atom(line);
      }

      if (strcmp(buff,buf) == 0) {
        is_a_set = true;
      } else {
        is_a_set = false;
      }
      strcpy(buf,"SATISFIABLE");
      if (strcmp(buff,buf) == 0) {
        Sat = 1;
      }
      strcpy(buf,"UNSATISFIABLE");
      if (strcmp(buff,buf) == 0) {
        Sat = 0;
      }
    }
  }
  return ans_set;
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

void Result::access_atom(string line) {
  char buf[30];
  bool state = 0;
  Answer ans;
  vector<string> vec;
  split(line, vec, " ");
  vector<string>::iterator it;
  for(it = vec.begin(); it != vec.end(); it++) {
    Atom atom;
    vector<string> pre;
//    cout << *it << " ";
    split(*it, pre, "(");
    atom.pre = Dict::getInstance().addPre(pre[0]);
    
    if (pre.size() > 1) {
      string iStr = it->substr(it->find_first_of("(")+1,it->find_last_of(")")-it->find_first_of("(")-1);
      vector<string> itemp;
      split(iStr, itemp, ",");
      vector<string>::iterator item;
      for (item = itemp.begin(); item != itemp.end(); item++) {
        atom.t.push_back(Dict::getInstance().addInd(*item));
      }
    }
    ans.a.push_back(atom);
  }
//  cout << endl;
  ans_set.push_back(ans);
}

vector<Answer> Result::get_answer() {
  return ans_set;
}

void Result::print_ans_set() {
  vector<Answer>::iterator ans;
  for (ans = ans_set.begin(); ans != ans_set.end(); ans++) {
    vector<Atom>::iterator atom;
    for (atom = ans->a.begin(); atom != ans->a.end(); atom++) {
      cout << atom->pre;
      vector<int>::iterator i;
      for (i = atom->t.begin(); i != atom->t.end(); i++) {
        if (i == atom->t.begin()) {
          cout << ": ";
        }
        cout << *i << " ";
      }
      if (atom+1 != ans->a.end()) {
        cout << ",";
      }
    }
    cout << endl;
  }
}

int Result::isSat() {
  return Sat;
}
