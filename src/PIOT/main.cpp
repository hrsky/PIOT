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
using namespace std;

int main() {
    string pFilePath = "";
    string pFileName = "test";
//    string pFileName = "lubm-dlp";
    translation tran(pFilePath, pFileName);
    tran.trans();
    char cmdline[100];
    strcpy(cmdline,"gringo ");
    strcat(cmdline,pFileName.c_str());
    strcat(cmdline,".lp | clasp 0");
    system(cmdline);
    return 0;
}
