#include "RepairComputer.h"
#include <cstring>
#include <queue>
#include <time.h>

#define MAXRULESIZE 10000

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

RepairComputer::RepairComputer(vector<int>& rs, map<int,string> &tb, string resultp, statistics* s):rules(rs),tbox(tb),resultPath(resultp),stat(s){
    modelFileName = resultPath + "model";
}

RepairComputer::RepairComputer(vector< vector<int> >& prs, map<int,string> &tb, string resultp, statistics* s):prefRules(prs),tbox(tb),resultPath(resultp),stat(s){
    modelFileName = resultPath + "model";
}

RepairComputer::RepairComputer(map<int, vector<int> >& wrs, map<int,string> &tb, string resultp, statistics* s):weightRules(wrs),tbox(tb),resultPath(resultp),stat(s){
    modelFileName = resultPath + "model";
}

bool RepairComputer::qIncMax(Query& query) {
  bool cut[MAXRULESIZE];
  memset(cut, false, sizeof(cut));

  queue<qset> q;
  q.push(qset(this->rules, 0));

  while(!q.empty()) {
    if (stat->total_time > 1000) {
      return false;
    }
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
      stat->find_query();
      cut[qi.ri] = true;
      continue;
    }

    for(size_t i = 0; i < qi.set.size(); i++) {
      if(qi.set[i] == 0) continue;
      qset tqi = qi;
      tqi.set[i] = 0;
      tqi.ri = i;
      tqi.start = i + 1;
      tqi.layer++;
//        for (int j = 0; j < qi.set.size(); j++) {
//            cout << tqi.set[j] << " ";
//        }
//        cout << endl;
      q.push(tqi);
    }
    com_end_time=clock();
    stat->compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
    stat->write_every_time_stat(0);
  }
  return true;
}

int RepairComputer::bfsPriSubset(Query& query, vector< vector<int> > pset, int priority) {
  if (stat->total_time > 1000) {
    return -1;
  }
  if(priority >= (int)pset.size()) {
    stat->find_count++;
    Result res(modelFileName);
    clock_t com_start_time=clock();
    clock_t com_end_time;
    bool consistent = this->isConsistent(pset, res);

    if(consistent && !query.entails(&res)) {
      com_end_time=clock();
      stat->compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
      stat->write_every_time_stat(1);
      return -1;
    }
      
    if(consistent) {
      stat->find_query();
      com_end_time=clock();
      stat->compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
      stat->write_every_time_stat(1);
      return 1;
    }
    else {
      com_end_time=clock();
      stat->compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
      stat->write_every_time_stat(0);
      return 0;
    }
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
    if (stat->total_time > 1000) {
      return false;
    }
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
      stat->find_query();
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
  if (stat->total_time > 1000) {
    return -1;
  }
  if(priority >= (int)pset.size()) {
    Result res(modelFileName);
    stat->find_count++;
    
    clock_t com_start_time=clock();
    clock_t com_end_time;
    bool consistent = this->isConsistent(pset, res);

    if(consistent && !query.entails(&res)) {
      com_end_time=clock();
      stat->compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
      stat->write_every_time_stat(1);
      return -1;
    }
      
    if(consistent) {
      stat->find_query();
      com_end_time=clock();
      stat->compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
      stat->write_every_time_stat(1);
      return 1;
    }
    else {
      com_end_time=clock();
      stat->compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
      stat->write_every_time_stat(0);
      return 0;
    }
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

bool RepairComputer::isConsistent(vector<int>& rules, Result& result) {
  string tboxFileName = resultPath + "tbox";
  string aboxFileName = resultPath + "abox";
  ofstream outfile;
  outfile.flush();
  outfile.open(tboxFileName, ofstream::trunc);
  int index = 0;
  for (vector<int>::iterator i = rules.begin(); i != rules.end(); i++) {
    index = *i;
    if (index == 0) {
      continue;
    }
    outfile << tbox[index] << endl;
      cout << index << " ";
  }
    cout << endl;
    
  string cmdline = "clingo " + tboxFileName + " " + aboxFileName + " > " + modelFileName;
  system(cmdline.c_str());

  result.reset();
  result.compute_input();

  int state = result.isSat();
  if (state == 1)
      return true;
  return false;
}

bool RepairComputer::isConsistent(vector< vector<int> >& rules, Result& result) {
  string tboxFileName = resultPath + "tbox";
  string aboxFileName = resultPath + "abox";
  ofstream outfile;
  outfile.flush();
  outfile.open(tboxFileName, ofstream::trunc);
  int index = 0;
  for (vector< vector<int> >::iterator i = rules.begin(); i != rules.end(); i++) {
    for (vector<int>::iterator j = i->begin(); j != i->end(); j++) {
      index = *j;
      if (index == 0) {
        continue;
      }
      outfile << tbox[index] << endl;
      cout << index << " ";
    }
  }
    cout << endl;

  string cmdline = "clingo " + tboxFileName + " " + aboxFileName + " > " + modelFileName;
  system(cmdline.c_str());

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
      if (stat->total_time > 1000) {
        return false;
      }
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
        stat->find_query();
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
