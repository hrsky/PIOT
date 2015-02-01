#ifndef REPAIRCOMPUTER_H
#define REPAIRCOMPUTER_H

#include <vector>
#include "result.h"
#include "query.h"

using namespace std;

class RepairComputer {
  public:
    RepairComputer(vector<int>&);
    RepairComputer(vector< vector<int> >&);

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

    bool isConsistent(vector<int>&, Result&);
    bool isConsistent(vector< vector<int> >&, Result&);

    int bfsPriSubset(Query&, vector< vector<int> >, int);
    int cardPriMaxSubset(Query&, vector< vector<int> >, int);
};

#endif
