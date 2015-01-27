/*************************************************************************
	> File Name: translation.h
 ************************************************************************/

#ifndef translation_h
#define translation_h

#include <iostream>
#include <string>
#include <vector>
using namespace std;
class translation {
public:
  translation(string pFilePath, string pFileName);
  void trans();  //translate the program to pi(p) and return the pi(p)'s filename
  string choice_rule(int index); //generate the choice rule.
  bool isRule(string line); //determine whehther the line is a rule.
  void classify();
  vector<string> get_tbox();
  vector<string> get_abox();
private:
  string pFileName;
  string pFilePath;
  int p_index;
  vector<string> tbox;
  vector<string> abox;
};
#endif
