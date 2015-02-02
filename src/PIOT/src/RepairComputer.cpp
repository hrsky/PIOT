#include "RepairComputer.h"
#include <cstring>
#include <queue>
#include <time.h>

#define MAXRULESIZE 500

struct qset {
  vector<int> set;
  int start;
  int ri;
    
  int layer;
  qset(vector<int>& s, int i):set(s),start(i),ri(0),layer(0){}
};

struct cset {
  vector<int> set;
  int layer;
  int start;

  cset(vector<int>& s, int l):set(s),layer(l),start(0){}
};

RepairComputer::RepairComputer(vector<int>& rs, map<int,string> &tb, vector<string> &ab, string mfilename, statistics* s):rules(rs),tbox(tb),abox(ab),modelFileName(mfilename),stat(s){}

RepairComputer::RepairComputer(vector< vector<int> >& prs, map<int,string> &tb, vector<string> &ab, string mfilename, statistics* s):prefRules(prs),tbox(tb),abox(ab),modelFileName(mfilename),stat(s){}

RepairComputer::RepairComputer(map<int, vector<int> >& wrs, map<int,string> &tb, vector<string> &ab, string mfilename, statistics* s):weightRules(wrs),tbox(tb),abox(ab),modelFileName(mfilename),stat(s){}

bool RepairComputer::qIncMax(Query& query) {
  bool cut[MAXRULESIZE];
  memset(cut, false, sizeof(cut));

  queue<qset> q;
  q.push(qset(this->rules, 0));

  while(!q.empty()) {
    qset qi = q.front();
    q.pop();
    stat->curr_count = qi.layer;
    if(cut[qi.ri]) continue;

    Result res(modelFileName);

    clock_t com_start_time=clock();
    clock_t com_end_time;
    stat->find_count++;
    if(this->isConsistent(qi.set, res)) {
      com_end_time=clock();
      stat->compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
      stat->write_every_time_stat(1);

      if(!query.entails(&res)) return false;
      cut[qi.ri] = true;
      continue;
    }

    for(size_t i = qi.start; i < qi.set.size(); i++) {
      if(qi.set[i] == 0) continue;
      qset tqi = qi;
      tqi.set[i] = 0;
      tqi.ri = i;
      tqi.start = i + 1;
      tqi.layer++;
      q.push(tqi);
    }
    com_end_time=clock();
    stat->compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
    stat->write_every_time_stat(0);

  }
  return true;
}

int RepairComputer::bfsPriSubset(Query& query, vector< vector<int> > pset, int priority) {
  if(priority >= (int)pset.size()) {
    Result res(modelFileName);
    bool consistent = this->isConsistent(pset, res);

    if(consistent && !query.entails(&res)) return -1;

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
    stat->curr_count = qi.layer;
    if(cut[qi.ri]) continue;

    pset[priority] = qi.set;

    clock_t com_start_time=clock();
    clock_t com_end_time;
    stat->find_count++;

    int scode = bfsPriSubset(query, pset, priority + 1);

    com_end_time=clock();
    stat->compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
    if (scode == -1 || scode == 1) {
      stat->write_every_time_stat(1);
    } else {
      stat->write_every_time_stat(scode);
    }

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
      tqi.layer++;
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
    stat->curr_count = qi.layer;
    if(qi.layer > depth) break;

    Result res(modelFileName);

    clock_t com_start_time=clock();
    clock_t com_end_time;
    stat->find_count++;

    if(this->isConsistent(qi.set, res)) {
      com_end_time = clock();
      stat->compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
      stat->write_every_time_stat(1);

      if(!query.entails(&res)) return false;
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
    com_end_time=clock();
    stat->compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
    stat->write_every_time_stat(0);
  }

  return true;
}

int RepairComputer::cardPriMaxSubset(Query& query, vector< vector<int> > pset, int priority) {
  if(priority >= (int)pset.size()) {
    Result res(modelFileName);
    bool consistent = this->isConsistent(pset, res);

    if(consistent && !query.entails(&res)) return -1;

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
    stat->curr_count = qi.layer;
    if(qi.layer > depth) break;
    pset[priority] = qi.set;

    clock_t com_start_time=clock();
    clock_t com_end_time;
    stat->find_count++;

    int scode = bfsPriSubset(query, pset, priority + 1);

    com_end_time=clock();
    stat->compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
    if (scode == -1 || scode == 1) {
      stat->write_every_time_stat(1);
    } else {
      stat->write_every_time_stat(scode);
    }

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

bool RepairComputer::isConsistent(vector<int>& rules, Result& result) {
  ofstream outfile;
  outfile.flush();
  outfile.open(modelFileName+".lp", ofstream::trunc);
  int index = 0;
  for (vector<int>::iterator i = rules.begin(); i != rules.end(); i++) {
    index = *i;
    if (index == 0) {
      continue;
    }
    outfile << tbox[index] << endl;
  }
  for (vector<string>::iterator it = abox.begin(); it != abox.end(); it++) {
    outfile << *it << endl;
  }
  outfile.close();

  char cmdline[100];
  strcpy(cmdline,"clingo ");
  strcat(cmdline,(modelFileName).c_str());
  strcat(cmdline,".lp > ");
  strcat(cmdline,modelFileName.c_str());
  system(cmdline);

  result.reset();
  result.compute_input();

  int state = result.isSat();
  if (state == 1)
      return true;
  return false;
}

bool RepairComputer::isConsistent(vector< vector<int> >& rules, Result& result) {
  ofstream outfile;
  outfile.flush();
  outfile.open(modelFileName+".lp", ofstream::trunc);
  int index = 0;
  for (vector< vector<int> >::iterator i = rules.begin(); i != rules.end(); i++) {
    for (vector<int>::iterator j = i->begin(); j != i->end(); j++) {
      index = *j;
      if (index == 0) {
        continue;
      }
      outfile << tbox[index] << endl;
    }
  }
  for (vector<string>::iterator it = abox.begin(); it != abox.end(); it++) {
    outfile << *it << endl;
  }
  outfile.close();

  char cmdline[100];
  strcpy(cmdline,"clingo ");
  strcat(cmdline,(modelFileName).c_str());
  strcat(cmdline,".lp > ");
  strcat(cmdline,modelFileName.c_str());
  system(cmdline);

  result.reset();
  result.compute_input();

  int state = result.isSat();
  if (state == 1)
    return true;
  return false;
}

struct wset {
  vector<int> set;
  int start;
  int mWeight;
    
  int layer;
  wset(vector<int>& s, int st):set(s),start(st),mWeight(0),layer(0){}
};

bool RepairComputer::qWeightMax(Query& query) {
  int ruleweights[MAXRULESIZE];
  memset(ruleweights, 0, sizeof(ruleweights));

  vector<int> rules;
  vector<int> weightSize;

  int is = 0;
  for(map<int, vector<int> >::iterator it = this->weightRules.begin(); it != this->weightRules.end();
      it++) {
    for(size_t i = 0; i < it->second.size(); i++) {
      rules.push_back(it->second[i]);
      ruleweights[is + i] = it->first;
    }
    if(it != this->weightRules.begin())
      weightSize.push_back(weightSize.back() + it->second.size());
    else
      weightSize.push_back(it->second.size());

    is += it->second.size();
  }

  for(size_t st = 0; st < weightSize.size(); st++) {
    queue<wset> q;
    q.push(wset(rules, 0));

    bool hasConsistent = false;
    int minMWeight = -1;
    while(!q.empty()) {
      wset qi = q.front();
      q.pop();
    
      stat->curr_count = qi.layer;
      if(minMWeight != -1 && qi.mWeight > minMWeight) continue;

      clock_t com_start_time=clock();
      clock_t com_end_time;
      stat->find_count++;
        
      Result res(modelFileName);
      bool consistent = this->isConsistent(qi.set, res);
      if(consistent) {
        com_end_time=clock();
        stat->compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
        stat->write_every_time_stat(1);
        if(!query.entails(&res)) return false;
        minMWeight = qi.mWeight;
        hasConsistent = true;
        continue;
      }

      com_end_time=clock();
      stat->compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
      stat->write_every_time_stat(0);

      for(int i = qi.start; i < weightSize[st]; i++) {
        if(qi.set[i] == 0) continue;
        wset tqi = qi;
        tqi.start = i + 1;
        tqi.mWeight += ruleweights[i];
        tqi.set[i] = 0;
        tqi.layer++;
        q.push(tqi);
      }
    }
    if(hasConsistent) break;
  }

  return true;
}
