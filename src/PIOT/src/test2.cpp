#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
#include <map>

using namespace std;

int count = 0;

void printSet(vector<int>& s) {
  for(size_t i = 0; i < s.size(); i++) {
    if(s[i] == 0) continue;
    printf("%d", s[i]);
    if(i < s.size() - 1) printf(" ");
  }
  printf("\n");
}

void printPriSet(map<int, vector<int> > ps) {
  for(map<int, vector<int> >::iterator it = ps.begin(); it != ps.end(); it++) {
    vector<int> s = it->second;
    for(size_t i = 0; i < s.size(); i++) {
      if(s[i] == 0) continue;
      printf("%d ", s[i]);
      if(i < s.size() - 1) printf(" ");
    }
    if(it != --ps.end()) printf("|");
  }
  printf("\n");
}


void printPriSet(vector< vector<int> > ps) {
  for(size_t p = 0; p < ps.size(); p++) {
    vector<int>& s = ps[p];
    for(size_t i = 0; i < s.size(); i++) {
      if(s[i] == 0) continue;
      printf("%d ", s[i]);
      if(i < s.size() - 1) printf(" ");
    }
    if(p < ps.size() - 1) printf("|");
  }
  printf("\n");
}

//Deep first search, doesn't satisfy maximization
void subset(vector<int>& s, size_t index, size_t i) {
  if(index >= s.size()) return;
  printSet(s);
  count++;
  for(; i < s.size(); i++) {
    if(s[i] == 0) continue;
    int t = s[i];
    s[i] = 0;
    subset(s, index + 1, i + 1);
    s[i] = t;
  }
}

void priSubset(map<int, vector<int> >& ps, map<int, vector<int> >::iterator p,
  size_t index, size_t i) {
  if(p == ps.end()) {
    printPriSet(ps);
    count++;
    return;
  }

  vector<int>& s = p->second;
  if(index >= s.size()) return;

  map<int, vector<int> >::iterator tit = p;
  tit++;
  priSubset(ps, tit, 0, 0);

  for(; i < s.size(); i++) {
    if(s[i] == 0) continue;

    int t = s[i];
    s[i] = 0;
    priSubset(ps, p, index + 1, i + 1);
    s[i] = t;
  }
}

//Breath first search, cut branches by maximization
struct qset {
  vector<int> set;
  int start;
  int ri;

  qset(vector<int>& s, int i):set(s),start(i),ri(0){}
};

bool consistent = false;

void bfsSubset(vector<int>& set) {
  bool cut[500];
  memset(cut, false, sizeof(cut));

  queue<qset> q;
  q.push(qset(set, 0));

  while(!q.empty()) {
    qset qi = q.front();
    q.pop();

    if(cut[qi.ri]) continue;

    printSet(qi.set);
    count++;

    if(consistent) {
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

bool bfsPriSubset(vector< vector<int> > pset, int priority) {
  if(priority >= (int)pset.size()) {
    printPriSet(pset);
    count++;
    if(consistent) return true;
  }

  queue<qset> q;
  vector<int>& set = pset[priority];
  q.push(qset(set, 0));

  bool cut[500];
  memset(cut, false, sizeof(cut));

  bool lsucc = false;

  while(!q.empty()) {
    qset qi = q.front();
    q.pop();

    if(cut[qi.ri]) continue;

    pset[priority] = qi.set;

    bool success = bfsPriSubset(pset, priority + 1);

    if(success) {
      lsucc = true;
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

struct cset {
  vector<int> set;
  int layer;
  int start;

  cset(vector<int>& s, int l):set(s),layer(l),start(0){}
};

void cardMaxSubset(vector<int> set) {
  queue<cset> q;
  q.push(cset(set, 0));

  int depth = set.size();

  while(!q.empty()) {
    cset qi = q.front();
    q.pop();

    if(qi.layer > depth) break;

    printSet(qi.set);
    count++;

    if(consistent) {
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

bool cardPriMaxSubset(vector< vector<int> > pset, int priority) {
  if(priority >= (int)pset.size()) {
    printPriSet(pset);
    count++;
    if(consistent) return true;
  }

  queue<cset> q;
  vector<int>& set = pset[priority];
  q.push(cset(set, 0));

  bool lsucc = false;
  int depth = set.size();

  while(!q.empty()) {
    cset qi = q.front();
    q.pop();

    if(qi.layer > depth) break;
    pset[priority] = qi.set;

    bool success = bfsPriSubset(pset, priority + 1);

    if(success) {
      depth = qi.layer;
      lsucc = true;
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

int main() {
  //map<int, vector<int> > pset;
  vector< vector<int> > pset;

  vector<int> iset;
  iset.push_back(1);
  iset.push_back(2);
  iset.push_back(3);
  iset.push_back(4);
  vector<int> iset2;
  iset2.push_back(5);
  iset2.push_back(6);

  pset.push_back(iset);
  pset.push_back(iset2);
//  subset(iset, 0, 0);
//  subset(pset, it, 0, 0);

  //bfsSubset(iset);
  bfsPriSubset(pset, 0);
/*  queue< vector<int> > q;
  q.push(iset);
  int index = 0;
  while(!q.empty()) {
    vector<int> s = q.front();
    printSet(s);
    q.pop();

    int n_index = 0;
    for(vector<int>::iterator it = s.begin(); it != s.end(); it++) {
      n_index++;
      if(n_index < index) continue;
      vector<int> ts = s;
      s.erase(it);
      q.push(s);
      s = ts;
    }
    index++;
  }
*/
  printf("\nSetNum:%d\n", count);

  return 0;
}
