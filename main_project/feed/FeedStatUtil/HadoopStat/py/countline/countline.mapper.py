#!/usr/bin/env python
import sys
import types,traceback
import os
import re
current_path = str(os.getcwd())
sys.path.append(current_path)


#format from yeyin.zhang
#100000147	2-2-2006	202167550,200680474,238448156,247453908,224225794
#format after process
#100000147	N	99	247453908	200680474	238448156	224225794	202167550
cnt = 0
for line in sys.stdin:
	cnt = cnt + 1
print cnt
