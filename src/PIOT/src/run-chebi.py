#!/usr/bin/env python
# coding=utf-8
import os
import time
import subprocess
from signal import SIGTERM   
inputFilePathDic = ["/chebi-1/", "/chebi-2/", "/chebi-3/", "/chebi-4/", "/chebi-5/", "/chebi-10/", "/chebi-20/", "/chebi-40/", "/chebi-1-2/", "/chebi-2-2/", "/chebi-3-2/", "/chebi-4-2/", "/chebi-5-2/", "/chebi-10-2/", "/chebi-20-2/", "/chebi-40-2/"];
queryFileName = "../examples/chebi-query";
def command_line():
  for i in range(0,5):
    cmd = "./cal " + str(i) + " ";
    for inputFilePath in inputFilePathDic:
      temp = cmd;
      cmd += "../examples" + inputFilePath + " ";
      if (i==3) or (i==4):
        cmd += "chebi-with-priority";
      elif (i==2):
        cmd += "chebi-with-weight";
      else:
        cmd += "chebi";
      cmd += " ../result" + inputFilePath + " " + queryFileName;
      print(cmd);
      p = subprocess.Popen(cmd, shell=True);
      t = 0
      while (p.poll() is None):
        time.sleep(1);
        t += 1;
        if (t > 900):
          p.terminate();
      cmd = temp;
  
command_line()
