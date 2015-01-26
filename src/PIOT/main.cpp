//
//  main.cpp
//  PIOT
//
//  Created by Haoran Huang on 1/23/15.
//  Copyright (c) 2015 Haoran Huang. All rights reserved.
//

#include <iostream>
#include <string>
#include "translation.h"
#include "Result.h"

using namespace std;

int main() {
    string pFilePath = "";
    string pFileName = "test";
    string modelFileName = "result.txt";
//    string pFileName = "lubm-dlp";
    translation tran(pFilePath, pFileName);
//    tran.trans();
    char cmdline[100];
    strcpy(cmdline,"gringo ");
    strcat(cmdline,pFileName.c_str());
    strcat(cmdline,".lp | clasp 0 > ");
    strcat(cmdline,modelFileName.c_str());
    system(cmdline);
    
    Result result_manager(pFilePath,modelFileName);
    result_manager.compute_input();
    result_manager.print_ans_set();
    return 0;
}
