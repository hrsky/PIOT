/*************************************************************************
  > File Name: query.h
 ************************************************************************/

#ifndef QUERY_H
#define QUERY_H
#include<iostream>
#include "answer.h"
#include "result.h"
#include "Dict.h"

class Query {
  public:
    Query(string query);
    bool entails(Result* result);
    string get_query();
    string process_query();
  private:
    vector<Atom> _query;
    map<string, int> varibles;
    int addVariable(string&);
};


#endif
