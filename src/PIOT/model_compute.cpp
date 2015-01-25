/*************************************************************************
	> File Name: model_compute.cpp
 ************************************************************************/

#include<iostream>
#include<string>
#include "model_compute.h"
using namespace std;

model_compute::model_compute(string pFilePath, string pipFileName) {
    this->pFilePath = pFilePath;
    this->pipFileName = pipFileName;
}