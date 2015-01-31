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
#include <queue>
#include "translation.h"
#include "result.h"
#include "query.h"
#include <time.h>

using namespace std;
int state = 0;
int input_type = 0;

statistics stat;
string pFilePath = "../examples/lubm/";
string pFileName = "lubm-dlp";
//string pFilePath = "../examples/test/";
//string pFileName = "test";
string resultPath = "../result/" + pFileName + "/";
string modelFileName = resultPath + "model_result.txt";
string statFileName = resultPath + "statistics.txt";


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
    strcat(cmdline,pFileName.c_str());
    strcat(cmdline,".lp | clasp 0 > ");
    strcat(cmdline,modelFileName.c_str());
    state = system(cmdline);
    for (vector<Rule>::iterator i = tbox.begin(); i != tbox.end(); i++) {
        cout << i->ruleString << endl;
    }
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
    queue< vector<Rule> > q;
    q.push(tbox);
    Query query("a d");
    while (!q.empty()) {
        tbox = q.front();
        if (count - tbox.size() > 0) {
            index = 0;
        }
        Result r = compute_model(tbox, abox);
        q.pop();
        if (state == 1) {
            //check query entail
            if (query.entails(&r)) { //entail
                cout << "Success" << endl;
                break;
            } else
                issat = true;
        }
        if (state == 2) {
            break;
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
}


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
    
    tbox = tran.get_tbox();
    abox = tran.get_abox();
//    repair(tbox, abox);

    return 0;
}
