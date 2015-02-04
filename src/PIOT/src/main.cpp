//
//  main.cpp
//  PIOT
//
//  Created by Haoran Huang on 1/23/15.
//  Copyright (c) 2015 Haoran Huang. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <fstream>
#include <map>
#include "translation.h"
#include "result.h"
#include "query.h"
#include "RepairComputer.h"
#include <time.h>

using namespace std;
int state = 0;

//0:IncMax, 1:CardMax, 2:WeightMac, 3:PrefIncMax, 4:PrefCardMax
int input_type = 4;

string pFilePath = "../examples/lubm/";
string pFileName = "lubm-with-priority";
string resultPath = "../result/lubm/";
//string pFilePath = "../examples/test/";
//string pFileName = "test-with-priority";
//string resultPath = "../result/test/";
string modelFileName = resultPath + "model";
string statFileName = resultPath + "statistics.txt";
Query query("student(X)");

/*
void initial() {
    ifstream infile;
    infile.open(modelFileName);
    infile.close();
}

Result compute_model(vector<Rule> tbox, vector<string> abox) {
    ofstream outfile;
    outfile.flush();
    outfile.open(pFilePath+pFileName+".lp", ofstream::trunc);
    for (vector<Rule>::iterator i = tbox.begin(); i != tbox.end(); i++) {
        outfile << i->ruleString << endl;
    }
    for (vector<string>::iterator it = abox.begin(); it != abox.end(); it++) {
        outfile << *it << endl;
    }
    outfile.close();

    char cmdline[100];
    strcpy(cmdline,"gringo ");
    strcat(cmdline,(pFilePath+pFileName).c_str());
    strcat(cmdline,".lp | clasp 0 > ");
    strcat(cmdline,modelFileName.c_str());
    cout << cmdline << endl;
    state = system(cmdline);
//    for (vector<Rule>::iterator i = tbox.begin(); i != tbox.end(); i++) {
//        cout << i->ruleString << endl;
//    }
    Result result_manager(pFilePath,modelFileName);
    result_manager.reset();
    result_manager.compute_input();

    state = result_manager.isSat();
    cout << state << endl;
    return result_manager;
}

void repair(vector<Rule> tbox, vector<string> abox) {
    if (tbox.empty())
        return;
    int count = tbox.size();
    int index = 0;
    bool issat = false;
    clock_t start_time=clock();
    queue< vector<Rule> > q;
    
    q.push(tbox);
    Query query("a d");
    while (!q.empty()) {
        tbox = q.front();
        stat.find_count++;
        stat.curr_count = tbox.size();
        if (count - tbox.size() > 0) {
            index = 0;
        }
        
        clock_t com_start_time=clock();
        Result r = compute_model(tbox, abox);
        clock_t com_end_time=clock();
        stat.compute_time = static_cast<double>(com_end_time-com_start_time)/CLOCKS_PER_SEC;
        stat.write_every_time_stat(state, static_cast<double>(com_end_time-start_time)/CLOCKS_PER_SEC);
        
        q.pop();
        if (state == 1) {
            //check query entail
            if (query.entails(&r)) { //entail
                cout << "Success" << endl;
                stat.is_find = true;
                break;
            } else
                issat = true;
        }
        vector<Rule>::iterator i;
        count = tbox.size();
        int n_index = 0;
        for (i = tbox.begin(); i != tbox.end(); i++) {
            n_index++;
            if (n_index <= index)
                continue;
            if (issat) {
                continue;
            }
            vector<Rule> temp = tbox;
            tbox.erase(i);
            q.push(tbox);
            tbox = temp;
        }
        issat = false;
        index++;
    }
    clock_t end_time=clock();
    stat.repair_time = static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC;
    stat.write_total_statistics();
}

*/

int main() {
    translation tran(pFilePath, pFileName);
    statistics stat(statFileName);
    
    stat.input_type = input_type;
    
    vector<Rule> tbox;
    vector<string> abox;
    
//    tran.trans(2);
//    tran.trans(3);
    clock_t start_time=clock();
    tran.classify(input_type);

    tbox = tran.get_tbox();
    abox = tran.get_abox();
    stat.tbox_size = tbox.size();
    stat.abox_size = abox.size();
    
    map<int, string> tb;
    bool isFound = false;
    if (input_type == 0 || input_type == 1) {
        vector<int> rules;
        for (vector<Rule>::iterator i = tbox.begin(); i != tbox.end(); i++) {
            tb.insert(pair<int,string>(tb.size()+1,i->ruleString));
            rules.push_back(tb.size()+1);
        }
        clock_t end_time=clock();
        stat.classify_time = static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC;
        RepairComputer repair(rules, tb, abox, modelFileName,&stat);

        if (input_type == 0) {
            isFound = repair.qIncMax(query);
        } else {
            isFound = repair.qCardMax(query);
        }
    } else if (input_type == 3 || input_type == 4) {
        vector< vector<int> > rules;
        for (vector<Rule>::iterator i = tbox.begin(); i != tbox.end(); i++) {
            for (vector<Rule>::iterator j = i+1; j != tbox.end(); j++) {
                if (i->priority < j->priority) {
                    Rule temp(*i);
                    *i = *j;
                    *j = temp;
                }
            }
        }
        vector<int> r;
        int priority = tbox.begin()->priority;
        for (vector<Rule>::iterator i = tbox.begin(); i != tbox.end(); i++) {
            if (i->priority != priority) {
                rules.push_back(r);
                r.clear();
                priority = i->priority;
            }
            tb.insert(pair<int,string>(tb.size()+1,i->ruleString));
            r.push_back(tb.size()+1);
        }
        rules.push_back(r);
        cout << rules.size() << endl;
        clock_t end_time=clock();
        stat.classify_time = static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC;
        
        RepairComputer repair(rules, tb, abox, modelFileName,&stat);
        if (input_type == 3) {
            isFound = repair.qPrefIncMax(query);
        } else {
            isFound = repair.qPreCardMax(query);
        }
    } else if (input_type == 2) {
        vector<int> r;
        map<int, vector<int> > rules;
        for (vector<Rule>::iterator i = tbox.begin(); i != tbox.end(); i++) {
            for (vector<Rule>::iterator j = i+1; j != tbox.end(); j++) {
                if (i->weight > j->weight) {
                    Rule temp(*i);
                    *i = *j;
                    *j = temp;
                }
            }
        }
        int weight = tbox.begin()->weight;
        for (vector<Rule>::iterator i = tbox.begin(); i != tbox.end(); i++) {
            if (i->weight != weight) {
                rules.insert(pair<int, vector<int> >(weight, r));
                r.clear();
                weight = i->weight;
            }
            tb.insert(pair<int,string>(tb.size()+1,i->ruleString));
            r.push_back(tb.size()+1);
        }
        rules.insert(pair<int, vector<int> >(weight, r));
        clock_t end_time=clock();
        stat.classify_time = static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC;
        
        RepairComputer repair(rules, tb, abox, modelFileName, &stat);
        isFound = repair.qWeightMax(query);
    }
    
    if (isFound) {
        cout << "Found!" << endl;
        stat.is_find = true;
    } else {
        cout << "Not Found!" << endl;
    }
    clock_t end_time=clock();
    stat.repair_time = static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC;
    stat.write_total_statistics();
    
    return 0;
}
