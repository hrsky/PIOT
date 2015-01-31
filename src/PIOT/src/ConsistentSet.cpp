#include "ConsistentSet.h"
#include <map>
#include <algorithm>
#include <cmath>

ConsistentSet::ConsistentSet(vector<Rule*>& rs, bool c):rules(rs){
  if(c) {
    this->weight = 0;
    for(size_t i = 0; i < rules.size(); i++) {
      this->weight += rules[i]->weight;
    }
    this->priCarWeight = this->weight;
  }
}

ConsistentSet::ConsistentSet(vector<Rule*>& rs, int maxPriSize):rules(rs) {
  this->priCarWeight = 0;
  for(size_t i = 0; i < rules.size(); i++) {
    this->priCarWeight += pow(maxPriSize, rules[i]->priority);
  }
  this->weight = this->priCarWeight;
}

int ConsistentSet::getSize() {
  return this->rules.size();
}

int ConsistentSet::getWeight() {
  return this->weight;
}

vector<Rule*>& ConsistentSet::getRules() {
  return this->rules;
}

int ConsistentSet::getPriCarWeight() {
  return this->priCarWeight;
}
/*bool ConsistentSet::operator<(const ConsistentSet& c) {*/
  //map<int, vector<int> > sp1;
  //map<int, vector<int> > sp2;

  //for(size_t i = 0; i < this->rules.size(); i++) {
    //sp1[i].push_back(this->rules[i]->index);
  //}
  //for(size_t i = 0; i < c.rules.size(); i++) {
    //sp2[i].push_back(c.rules[i]->index);
  //}

//}

void RepairSelector::cardinalitySelector(vector<ConsistentSet>& repairs) {
  int maxsize = 0;
  for(vector<ConsistentSet>::iterator it = repairs.begin(); it != repairs.end();
      it++) {
    int size = it->getSize();
    if(size >= maxsize) maxsize = size;
  }
  for(vector<ConsistentSet>::iterator it = repairs.begin(); it != repairs.end();) {
    if(it->getSize() != maxsize) it = repairs.erase(it);
    else it++;
  }
}

void RepairSelector::weightSelector(vector<ConsistentSet>& repairs) {
  int maxweight = 0;
  for(vector<ConsistentSet>::iterator it = repairs.begin(); it != repairs.end();
      it++) {
    int weight = it->getWeight();
    if(weight > maxweight) maxweight = weight;
  }
  for(vector<ConsistentSet>::iterator it = repairs.begin(); it != repairs.end();) {
    if(it->getWeight() != maxweight) it = repairs.erase(it);
    else it++;
  }
}

void RepairSelector::preferredCardSelector(vector<ConsistentSet>& repairs) {
  int maxweight = 0;
  for(vector<ConsistentSet>::iterator it = repairs.begin(); it != repairs.end();
      it++) {
    int weight = it->getPriCarWeight();
    if(weight > maxweight) maxweight = weight;
  }
  for(vector<ConsistentSet>::iterator it = repairs.begin(); it != repairs.end();) {
    if(it->getPriCarWeight() != maxweight) it = repairs.erase(it);
    else it++;
  }
}

/*void RepairSelector::preferredIncSelector(vector<ConsistentSet>& repair) {*/

/*}*/
