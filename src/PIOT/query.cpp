/*************************************************************************
  > File Name: query.cpp
 ************************************************************************/

#include<iostream>
#include<map>
#include "query.h"

using namespace std;

map<int, vector<Atom> > models;
int varibles[100];

bool check_entailment(vector<query_atom>& query, unsigned index) {
    if(index >= query.size()) return true;
    Pred p = query[index].p;
    vector<int> t = query[index].t;

    vector<Atom> atoms = models[p.pindex];

    for(unsigned i = 0; i < atoms.size(); i++) {
        vector<int> cv;
        int c;
        for(c = 0; c < p.arity; c++) {
            if(t[c] >= 0 && t[c] != atoms[i].t[c]) break;
            if(t[c] < 0) {
                int v = abs(t[c]);
                if(varibles[v] == 0) {
                    cv.push_back(v);
                    varibles[v] = atoms[i].t[c];
                }
                else {
                    if(varibles[v] != atoms[i].t[c]) break;
                }
            }
        }
        if(c == p.arity) {
            bool result = check_entailment(query, index + 1);
            if(result) return true;

        }
        for(unsigned icv = 0; icv < cv.size(); icv++) varibles[cv[icv]] = 0;

    }
    return false;
}

Query::Query(string query) {

}

bool Query::entails(Result* result) {
    models.clear();
    Answer answer = (result->get_answer())[0];
    for(size_t i = 0;  i < answer.a.size(); i++) {
        int p = answer.a[i].pre;
        models[p].push_back(answer.a[i]);
    }

    return check_entailment(this->_query, 0);
}
