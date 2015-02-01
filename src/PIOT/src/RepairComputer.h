#ifndef REPAIRCOMPUTER_H
#define REPAIRCOMPUTER_H

#include <vector>
#include <map>
#include "result.h"
#include "query.h"
#include <string>
using namespace std;

class RepairComputer {
  public:
    RepairComputer(vector<int>&, map<int,string> &, vector<string> &, string);
    RepairComputer(vector< vector<int> >&, map<int,string> &, vector<string> &, string);

    void incMax();
    void prefIncMax();
    void cardMax();
    void preCardMax();

    bool qIncMax(Query&);
    bool qPrefIncMax(Query&);
    bool qCardMax(Query&);
    bool qPreCardMax(Query&);
  private:
    vector<int> rules;
    vector< vector<int> > prefRules;
    map<int,string> tbox;
    vector<string> abox;
    string modelFileName;
    
    bool isConsistent(vector<int>&, Result&);
    bool isConsistent(vector< vector<int> >&, Result&);

    int bfsPriSubset(Query&, vector< vector<int> >, int);
    int cardPriMaxSubset(Query&, vector< vector<int> >, int);
};

#endif
