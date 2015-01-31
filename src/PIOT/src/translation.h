/*************************************************************************
	> File Name: translation.h
 ************************************************************************/

#ifndef translation_h
#define translation_h

#include <iostream>
#include <string>
#include "Rule.h"
#include <vector>
using namespace std;
class translation {
public:
  translation(string pFilePath, string pFileName);
  void trans(int output_type);  //translate the program to pi(p) and return the pi(p)'s filename
  string choice_rule(int index); //generate the choice rule.
  bool isRule(string line); //determine whehther the line is a rule.
  void classify(int input_type);
  vector<Rule> get_tbox();
  vector<string> get_abox();
private:
  string pFileName;
  string pFilePath;
  int p_index;
  vector<Rule> tbox;
  vector<string> abox;
};
#endif
