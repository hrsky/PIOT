#include "Rule.h"

Rule::Rule(int i, string s, int w, int pri) {
  this->index = i;
  this->ruleString = s;
  this->weight = w;
  this->priority = pri;
}

bool Rule::operator==(const Rule& r) {
  return this->index == r.index;
}
