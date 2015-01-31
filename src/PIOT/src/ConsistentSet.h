#include <vector>
#include "Rule.h"
using namespace std;

class ConsistentSet {
  public:
    //for weighted and cardinality, true for weigthed, false for cardinality
    ConsistentSet(vector<Rule*>&, bool);
    //for preferred cardinality
    ConsistentSet(vector<Rule*>&, int);

    int getSize();
    int getWeight();
    vector<Rule*>& getRules();
    int getPriCarWeight();
    //bool operator<(const ConsistentSet&);
  private:
    vector<Rule*> rules;
    int weight;
    int priCarWeight;
};

class RepairSelector {
  public:
    static void cardinalitySelector(vector<ConsistentSet>& repair);
    static void weightSelector(vector<ConsistentSet>& repair);
    static void preferredCardSelector(vector<ConsistentSet>& repair);
    //static void preferredIncSelector(vector<ConsistentSet>& repair);
};

