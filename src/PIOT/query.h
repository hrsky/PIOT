/*************************************************************************
	> File Name: query.h
 ************************************************************************/

#ifndef QUERY_H
#define QUERY_H
#include<iostream>
#include "model_compute.h"

class query {
public:
  query(model_compute *compute, string pFilePath, string query);
  string get_query();
  string process_query();
  bool isSat();
  bool change_query(string query);

private:
  model_compute *compute;
  string pFilePath;
  string query;
}

#endif
