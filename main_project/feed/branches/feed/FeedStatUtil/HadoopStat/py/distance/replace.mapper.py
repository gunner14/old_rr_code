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

mdict={}
#badcnt=0
#infinite=0
#rootuid=0
#if len(sys.argv) != 3:
#	print >> sys.stderr ,'Usage:',sys.argv[0]," rootuid round "
#	exit(1)
#rootuid = int(sys.argv[1])
#round = int(sys.argv[2])
#depth = round+1

#rootuid=220678672
class UserInfo:
	def __init__(self):
		self.uid = 0
		self.distance = 1000
		self.line = ""
	def ParseYeyin(self,line):
		self.line = line.strip()
		[id,tmp,f,distance]= self.line.split()
		self.uid = int(id)
		self.distance = int(distance)
	def Emit(self):
		#s1 = [str(i) for i in self.friends]
		#print s1
		#print([str(self.uid),"N",str(self.distance)] + s1)
		#print([str(self.uid),"N",str(self.distance)].extend([str(i) for i in self.friends]))
		#return "\t".join([str(self.uid),"N",str(self.distance)] + [str(i) for i in self.friends])
		return "\t".join([str(self.uid),"D",str(self.distance)])
		#return self.line

for line in sys.stdin:
	if not line:
		continue
	line = line.strip()
	try:
		if len(line.split()) == 2:
			print line
			continue
		u = UserInfo()
		u.ParseYeyin(line)
		print u.Emit()	

	except:
		print >> sys.stderr, 'error:'+line.strip()
		traceback.print_exc()
		#sys.exit(1)
		#pass

