#!/usr/bin/env python
# coding=utf-8
import os
import subprocess
inputFilePathDic = ["/lubm-30000/", "/lubm-u1/", "/lubm-u2/", "/lubm-u5/", "/lubm-30000-2/", "/lubm-u1-2/", "/lubm-u2-2/", "/lubm-u5-2/"];
queryFileName = "../examples/lubm-query";
def command_line():
  for i in range(0,5):
    cmd = "./cal " + str(i) + " ";
    for inputFilePath in inputFilePathDic:
      temp = cmd;
      cmd += "../examples" + inputFilePath + " ";
      if (i == 3) or (i == 4):
        cmd += "lubm-with-priority";
      elif (i == 2):
        cmd += "lubm-with-weight";
      else:
        cmd += "lubm";
      cmd += " ../result" + inputFilePath + " " + queryFileName;
      print(cmd);
      os.system(cmd);
      cmd = temp;
command_line()
