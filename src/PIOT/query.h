/*************************************************************************
	> File Name: query.h
 ************************************************************************/

#ifndef QUERY_H
#define QUERY_H
#include<iostream>
#include "answer.h"
#include "result.h"

struct query_atom {
    Pred p;
    vector<int> t;

/*    query_atom(string& s) {
        size_t f = s.find_first_not_of("(", 0);
        size_t l = s.find_last_not_of(")", 0);
        string sp = s.substr(0, f);
        int ip = Dict::getInstance().addPre(sp);
        string vstring = s.substr(f, l - f);
        vector<string> vss;
        string delim = ",";
        split(vstring, delim, vss);
        int cp = vss.size();
        p.p = ip;
        p.arity = cp;

        for(size_t i = 0; i < vss.size(); i++) {
            if(vss[i][0] > 'a') {
                t.push_back(Dict::getInstance().addInd(vss[i]));

            }
            else {
                t.push_back(-1 * addVariable(vss[i]));
            }

        }
    }*/
};

class Query {
public:
  Query(string query);
  bool entails(Result* result);
  string get_query();
  string process_query();
private:
  vector<query_atom> _query;
};


#endif
