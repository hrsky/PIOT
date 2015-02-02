#ifndef REPAIRCOMPUTER_H
#define REPAIRCOMPUTER_H

#include <vector>
#include <map>
#include "result.h"
#include "query.h"
#include "answer.h"
#include <string>
using namespace std;

class RepairComputer {
  public:
    RepairComputer(vector<int>&, map<int,string> &, vector<string> &, string, statistics*);
    RepairComputer(vector< vector<int> >&, map<int,string> &, vector<string> &, string, statistics*);
    RepairComputer(map<int, vector<int> >&, map<int,string> &, vector<string> &, string, statistics*);

    void incMax();
    void prefIncMax();
    void cardMax();
    void preCardMax();

    bool qIncMax(Query&);
    bool qPrefIncMax(Query&);
    bool qCardMax(Query&);
    bool qPreCardMax(Query&);
    bool qWeightMax(Query&);
  private:
    vector<int> rules;
    vector< vector<int> > prefRules;
    map<int, vector<int> > weightRules;

    map<int,string> tbox;
    vector<string> abox;
    string modelFileName;
    statistics* stat;

    bool isConsistent(vector<int>&, Result&);
    bool isConsistent(vector< vector<int> >&, Result&);

    int bfsPriSubset(Query&, vector< vector<int> >, int);
    int cardPriMaxSubset(Query&, vector< vector<int> >, int);
};

#endif
