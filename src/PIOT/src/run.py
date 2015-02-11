#!/usr/bin/env python
# coding=utf-8
import os
import time
import subprocess
from signal import SIGTERM   

queryFileName = "../examples/lubm-query";

cmd = "./cal 4 ../examples/lubm-30000/ lubm-with-priority ../result/lubm-30000/ " + queryFileName;
os.system(cmd);

cmd = "./cal 3 ../examples/lubm-u1/ lubm-with-priority ../result/lubm-u1/ " + queryFileName;
os.system(cmd);

cmd = "./cal 4 ../examples/lubm-u1/ lubm-with-priority ../result/lubm-u1/ " + queryFileName;
os.system(cmd);

for i in range(3,5):
  cmd = "./cal " + str(i) + " ../examples/lubm-u2/ lubm-with-priority ../result/lubm-u2/ " + queryFileName;
  os.system(cmd);

for i in range(3,5):
  cmd = "./cal " + str(i) + " ../examples/lubm-u5/ lubm-with-priority ../result/lubm-u5/ " + queryFileName;
  os.system(cmd);

for i in range(3,5):
  cmd = "./cal " + str(i) + " ../examples/lubm-30000-2/ lubm-with-priority ../result/lubm-30000-2/ " + queryFileName;
  os.system(cmd);

for i in range(3,5):                                                                           
  cmd = "./cal " + str(i) + " ../examples/lubm-u1-2/ lubm-with-priority ../result/lubm-u1-2/ " + queryFileName;
  os.system(cmd); 

for i in range(3,5):
  cmd = "./cal " + str(i) + " ../examples/lubm-u2-2/ lubm-with-priority ../result/lubm-u2-2/ " + queryFileName;
  os.system(cmd);

for i in range(2,5):
  if (i == 3) or (i == 4):
    cmd = "./cal " + str(i) + " ../examples/lubm-u5-2/ lubm-with-priority ../result/lubm-u5-2/ " + queryFileName;
  else:
    cmd = "./cal " + str(i) + " ../examples/lubm-u5-2/ lubm-with-weight ../result/lubm-u5-2/ " + queryFileName;
  os.system(cmd);


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
