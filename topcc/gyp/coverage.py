#!/usr/bin/env python

# Copyright (c) 2012 Renren Inc. All rights reserved.

import sys
import os
import subprocess

if __name__ == '__main__':
  #out = open(sys.argv[-1], 'w')
  os.chdir(sys.argv[1])
  for f in sys.argv[2:-1]:
    cmdstr = 'gcov ' + f
    outstr = subprocess.Popen(cmdstr + ' 2>/dev/null', stdout=subprocess.PIPE, shell=True).communicate()[0]
    lines = outstr.split('\n')
    for line in lines:
      if line.startswith('File \'/usr'):
        break
      print(line)
      #out.write('%s\n' % line)


