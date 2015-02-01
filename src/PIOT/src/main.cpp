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
int input_type = 0;

//string pFilePath = "../examples/lubm/";
//string pFileName = "lubm";
//string resultPath = "../result/lubm/";
string pFilePath = "../examples/test/";
string pFileName = "test";
string resultPath = "../result/test/";
string modelFileName = resultPath + "model";
string statFileName = resultPath + "statistics.txt";

statistics stat(statFileName);

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
    stat.input_type = input_type;
    
    vector<Rule> tbox;
    vector<string> abox;
    
//    tran.trans(2);
//    tran.trans(3);
    clock_t start_time=clock();
    tran.classify(input_type);
    clock_t end_time=clock();
    stat.classify_time = static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC;

    tbox = tran.get_tbox();
    abox = tran.get_abox();
    
    map<int, string> tb;
    
    if (input_type == 3) {
        int priority = -111;
        vector< vector<int> > rules;
        vector<int> r;
        for (vector<Rule>::iterator i = tbox.begin(); i != tbox.end(); i++) {
            for (vector<Rule>::iterator j = i+1; j != tbox.end(); j++) {
                if (i->priority < j->priority) {
                    Rule temp(*i);
                    *i = *j;
                    *j = temp;
                }
            }
        }
        for (vector<Rule>::iterator i = tbox.begin(); i != tbox.end(); i++) {
            if (i->priority != priority && priority != -111) {
                rules.push_back(r);
                r.clear();
                priority = i->priority;
            }
            tb.insert(pair<int,string>(tb.size()+1,i->ruleString));
            r.push_back(tb.size()+1);
        }
        rules.push_back(r);
    } else if (input_type == 0) {
        vector<int> rules;
        for (vector<Rule>::iterator i = tbox.begin(); i != tbox.end(); i++) {
            tb.insert(pair<int,string>(tb.size()+1,i->ruleString));
            rules.push_back(tb.size()+1);
        }
        RepairComputer repair(rules, tb, abox, modelFileName);
        Query query("a d");
        if (repair.qIncMax(query)) {
            cout << "Found!" << endl;
        } else {
            cout << "Not Found!" << endl;
        }
    }
    
    stat.tbox_size = tbox.size();
    stat.abox_size = abox.size();

    return 0;
}
