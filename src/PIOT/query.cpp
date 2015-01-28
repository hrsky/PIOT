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
  Pred p = query[index].pre;
  vector<int> t = query[index].t;

  vector<Atom> atoms = models[p.pindex];

  for(unsigned i = 0; i < atoms.size(); i++) {
    vector<int> cv;
    int c;
    for(c = 0; c < p.arity; c++) {
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
    if(c == p.arity) {
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
  string delim = ",";
  vector<string> atomstr;
  split(query, delim, atomstr);

  for(size_t i = 0; i < atomstr.size(); i++) {
    string s = atomstr[i];
    Atom a;
    size_t f = s.find_first_not_of("(", 0);
    size_t l = s.find_last_not_of(")", 0);
    string sp = s.substr(0, f);
    int ip = Dict::getInstance().addPre(sp);
    string vstring = s.substr(f, l - f);
    vector<string> vss;
    string delim = ",";
    split(vstring, delim, vss);
    a.pre.pindex = ip;
    a.pre.arity = vss.size();

    for(size_t i = 0; i < vss.size(); i++) {
      if(vss[i][0] > 'a') {
        a.t.push_back(Dict::getInstance().addInd(vss[i]));
      }
      else {
        a.t.push_back(-1 * addVariable(vss[i]));
      }
    }
  }
}

bool Query::entails(Result* result) {
  models.clear();
  memset(varA, 0, sizeof(varA));
  Answer answer = (result->get_answer())[0];
  for(size_t i = 0;  i < answer.a.size(); i++) {
    int p = answer.a[i].pre.pindex;
    models[p].push_back(answer.a[i]);
  }

  return check_entailment(this->_query, 0);
}
