#include "RepairComputer.h"
#include <cstring>
#include <queue>

#define MAXRULESIZE 500

struct qset {
  vector<int> set;
  int start;
  int ri;

  qset(vector<int>& s, int i):set(s),start(i),ri(0){}
};

struct cset {
  vector<int> set;
  int layer;
  int start;

  cset(vector<int>& s, int l):set(s),layer(l),start(0){}
};

RepairComputer::RepairComputer(vector<int>& rs):rules(rs){}

RepairComputer::RepairComputer(vector< vector<int> >& prs):prefRules(prs){}

bool RepairComputer::qIncMax(Query& query) {
  bool cut[MAXRULESIZE];
  memset(cut, false, sizeof(cut));

  queue<qset> q;
  q.push(qset(this->rules, 0));

  while(!q.empty()) {
    qset qi = q.front();
    q.pop();

    if(cut[qi.ri]) continue;

    Result res;

    if(this->isConsistent(qi.set, res)) {
      if(!query.entails(res)) return false;
      cut[qi.ri] = true;
      continue;
    }

    for(size_t i = qi.start; i < qi.set.size(); i++) {
      if(qi.set[i] == 0) continue;
      qset tqi = qi;
      tqi.set[i] = 0;
      tqi.ri = i;
      tqi.start = i + 1;
      q.push(tqi);
    }
  }
}

int RepairComputer::bfsPriSubset(Query& query, vector< vector<int> > pset, int priority) {
  if(priority >= (int)pset.size()) {
    Result res;
    bool consistent = this->isConsistent(pset, res);
    if(!query.entails(res)) return -1;

    if(consistent) return 1;
    else return 0;
  }

  queue<qset> q;
  vector<int>& set = pset[priority];
  q.push(qset(set, 0));

  bool cut[MAXRULESIZE];
  memset(cut, false, sizeof(cut));

  int lsucc = 0;

  while(!q.empty()) {
    qset qi = q.front();
    q.pop();

    if(cut[qi.ri]) continue;

    pset[priority] = qi.set;

    int scode = bfsPriSubset(query, pset, priority + 1);

    if(scode == -1) return -1;

    if(scode == 1) {
      lsucc = 1;
      cut[qi.ri] = true;
      continue;
    }

    for(size_t i = qi.start; i < qi.set.size(); i++) {
      if(qi.set[i] == 0) continue;
      qset tqi = qi;
      tqi.set[i] = 0;
      tqi.ri = i;
      tqi.start = i + 1;
      q.push(tqi);
    }
  }

  return lsucc;
}

bool RepairComputer::qPrefIncMax(Query& query) {
  if(this->bfsPriSubset(query, this->prefRules, 0) == 1) return true;
  else return false;
}

bool RepairComputer::qCardMax(Query& query) {
  queue<cset> q;
  q.push(cset(this->rules, 0));

  int depth = this->rules.size();

  while(!q.empty()) {
    cset qi = q.front();
    q.pop();

    if(qi.layer > depth) break;

    Result res;

    if(this->isConsistent(qi.set, res)) {
      if(!query.entails(res)) return false;
      depth = qi.layer;
      continue;
    }

    if(qi.layer >= depth) continue;

    for(size_t i = qi.start; i < qi.set.size(); i++) {
      if(qi.set[i] == 0) continue;
      cset tqi = qi;
      tqi.set[i] = 0;
      tqi.start = i + 1;
      tqi.layer++;
      q.push(tqi);
    }
  }
}

int RepairComputer::cardPriMaxSubset(Query& query, vector< vector<int> > pset, int priority) {
  if(priority >= (int)pset.size()) {
    Result res;
    bool consistent = this->isConsistent(pset, res);

    if(consistent && !query.entails(res)) return -1;

    if(consistent) return 1;
    else return 0;
  }

  queue<cset> q;
  vector<int>& set = pset[priority];
  q.push(cset(set, 0));

  int lsucc = 0;
  int depth = set.size();

  while(!q.empty()) {
    cset qi = q.front();
    q.pop();

    if(qi.layer > depth) break;
    pset[priority] = qi.set;

    int scode = bfsPriSubset(query, pset, priority + 1);

    if(scode == -1) return -1;

    if(scode == 1) {
      depth = qi.layer;
      lsucc = 1;
      continue;
    }

    if(qi.layer >= depth) continue;

    for(size_t i = qi.start; i < qi.set.size(); i++) {
      if(qi.set[i] == 0) continue;
      cset tqi = qi;
      tqi.set[i] = 0;
      tqi.layer++;
      tqi.start = i + 1;
      q.push(tqi);
    }
  }

  return lsucc;
}

bool RepairComputer::qPreCardMax(Query& q) {
  if(this->cardPriMaxSubset(q, this->prefRules, 0) == 1) return true;
  else return false;
}
