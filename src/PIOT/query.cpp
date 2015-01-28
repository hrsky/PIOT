/*************************************************************************
  > File Name: query.cpp
 ************************************************************************/

#include<iostream>
#include<map>
#include<cstring>
#include"query.h"

using namespace std;

map<int, vector<Atom> > models;
int varA[100];

void split(string& s, string& de, vector<string>& re) {
  size_t last = 0;
  size_t index = s.find_first_of(de, last);

  while(index != string::npos) {
    re.push_back(s.substr(last, index - last));
    last = index + 1;
    index = s.find_first_of(de, last);
  }

  if(index - last > 0) {
    re.push_back(s.substr(last, index - last));
  }
}

bool check_entailment(vector<Atom>& query, unsigned index) {
  if(index >= query.size()) return true;
  int p = query[index].pre;
  vector<int> t = query[index].t;

  vector<Atom> atoms = models[p];

  for(unsigned i = 0; i < atoms.size(); i++) {
    vector<int> cv;
    size_t c;
    for(c = 0; c < t.size(); c++) {
      if(t[c] >= 0 && t[c] != atoms[i].t[c]) break;
      if(t[c] < 0) {
        int v = abs(t[c]);
        if(varA[v] == 0) {
          cv.push_back(v);
          varA[v] = atoms[i].t[c];
        }
        else {
          if(varA[v] != atoms[i].t[c]) break;
        }
      }
    }
    if(c == t.size()) {
      bool result = check_entailment(query, index + 1);
      if(result) return true;

    }
    for(unsigned icv = 0; icv < cv.size(); icv++) varA[cv[icv]] = 0;
  }
  return false;
}

int Query::addVariable(string& s) {
  map<string, int>::iterator it = varibles.find(s);
  if(it != varibles.end()) {
    return it->second;
  }
  varibles.insert(pair<string, int>(s, varibles.size() + 1));
  return varibles.size();
}

Query::Query(string query) {
  cout << "query string:" << query << endl;
  string delim1 = ",";
  string delim2 = " ";
  vector<string> atomstr;
  split(query, delim2, atomstr);

  for(size_t i = 0; i < atomstr.size(); i++) {
    cout << "atomstr " << i << " " << atomstr[i] << endl;
    string s = atomstr[i];
    Atom a;
    size_t f = s.find_first_of("(", 0);
    size_t l = s.find_last_of(")", 0);

    if(f == string::npos) {
      a.pre = Dict::getInstance().addPre(s);
    }
    else {
      string sp = s.substr(0, f);
      int ip = Dict::getInstance().addPre(sp);
      string vstring = s.substr(f + 1, l - f - 1);
      vector<string> vss;
      string delim = ",";
      split(vstring, delim1, vss);
      a.pre = ip;

      for(size_t i = 0; i < vss.size(); i++) {
        if(vss[i][0] > 'a') {
          a.t.push_back(Dict::getInstance().addInd(vss[i]));
        }
        else {
          a.t.push_back(-1 * addVariable(vss[i]));
        }
      }
    }
    this->_query.push_back(a);
  }
  for(size_t i = 0; i < this->_query.size(); i++) cout << "query" << this->_query[i].pre << " ";
  cout << endl;
}

bool Query::entails(Result* result) {
  models.clear();
  memset(varA, 0, sizeof(varA));
  Answer answer = (result->get_answer())[0];
  for(size_t i = 0;  i < answer.a.size(); i++) {
    int p = answer.a[i].pre;
    models[p].push_back(answer.a[i]);
  }
  return check_entailment(this->_query, 0);
}
