/*************************************************************************
	> File Name: translation.h
 ************************************************************************/

#ifndef translation_h
#define translation_h

#include <iostream>
#include <string>

class translation {
public:
  translation(string pFilePath, string pFileName);
  string trans();  //translate the program to pi(p) and return the pi(p)'s filename
  string choice_rule(int index); //generate the choice rule.
  boolean isRule(string line); //determine whehther the line is a rule.
private:
  string pFileName;
  string pFilePath;
  int p_index;
}
#endif
