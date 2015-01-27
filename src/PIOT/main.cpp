//
//  main.cpp
//  PIOT
//
//  Created by Haoran Huang on 1/23/15.
//  Copyright (c) 2015 Haoran Huang. All rights reserved.
//

#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <queue>
#include "translation.h"
#include "Result.h"

using namespace std;
int state = 0;
string pFilePath = "";
string pFileName = "test";
string modelFileName = "result.txt";

void compute_model(vector<string> tbox, vector<string> abox) {
    ofstream outfile;
    outfile.flush();
    outfile.open(pFilePath+pFileName+".lp", ofstream::trunc);
    vector<string>::iterator i;
    for (i = tbox.begin(); i != tbox.end(); i++) {
        outfile << *i << endl;
    }
    for (i = abox.begin(); i != abox.end(); i++) {
        outfile << *i << endl;
    }
    outfile.close();
    
    char cmdline[100];
    strcpy(cmdline,"gringo ");
    strcat(cmdline,pFileName.c_str());
    strcat(cmdline,".lp | clasp 0 > ");
    strcat(cmdline,modelFileName.c_str());
    state = system(cmdline);
    for (vector<string>::iterator it = tbox.begin(); it != tbox.end(); it++) {
        cout << *it << endl;
    }
    Result result_manager(pFilePath,modelFileName);
    result_manager.reset();
    result_manager.compute_input();
    
    state = result_manager.isSat();
    cout << state << endl;
    return;
}

void repair(vector<string> tbox, vector<string> abox) {
    if (tbox.empty())
        return;
    queue< vector<string> > q;
    q.push(tbox);
    while (!q.empty()) {
        tbox = q.front();
        compute_model(tbox, abox);
        q.pop();
        if (state == 1) {
            //check query entail
            if (1) //entail
                break;
            else
                continue;
        }
        if (state == 2) {
            break;
        }
        vector<string>::iterator i;
        for (i = tbox.begin(); i != tbox.end(); i++) {
            vector<string> temp = tbox;
            tbox.erase(i);
            q.push(tbox);
            if (state > 0)
                return;
            tbox = temp;
        }
    }
}

int main() {
//    string pFileName = "lubm-dlp";
    translation tran(pFilePath, pFileName);
//    tran.trans();
    
    int del_count = 1;
    vector<string> tbox, abox;
    tran.classify();
    tbox = tran.get_tbox();
    abox = tran.get_abox();
    repair(tbox, abox);
//    while (del_count < tbox.size()) {
//        
//        del_count++;
//    }
    return 0;
}
