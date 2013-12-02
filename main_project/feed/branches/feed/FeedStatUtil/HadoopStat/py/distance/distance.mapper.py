#!/usr/bin/env python
import sys
import types,traceback
import os
import re
current_path = str(os.getcwd())
sys.path.append(current_path)

#cl@105142708#249791720 2
#105142708#249791720 click reply recv

	
testers = set([239556923,221489137,238489851,240599893,128487631,225092473,241610785,220678672,322856518,34344507,343518722,347348947,347349486,347735846,200000032])
test = True 

mdict={}


#format from yeyin.zhang
#100000147	2-2-2006	202167550,200680474,238448156,247453908,224225794
#format after process
#100000147	N	99	247453908	200680474	238448156	224225794	202167550
class UserInfo:
	def __init__(self):
		self.uid = 0
		self.friends = set()
		self.distance = 1000
		self.line = ""
	def ParseYeyin(self,line):
		[id,tmp,f] = line.strip().split()[:3]
		self.line = line.strip()
		self.uid = int(id)
		for i in f.strip().split(','):
			if i != "":
				self.friends.add(int(i))
	def ParseNormal(self,line):# bug
		[id,tmp,f] = line.strip().split()
		self.uid = int(id)
		for i in f.strip().split(','):
			self.friends.add(int(i))
	def Emit(self):
		#s1 = [str(i) for i in self.friends]
		#print s1
		#print([str(self.uid),"N",str(self.distance)] + s1)
		#print([str(self.uid),"N",str(self.distance)].extend([str(i) for i in self.friends]))
		#return "\t".join([str(self.uid),"N",str(self.distance)] + [str(i) for i in self.friends])
		return self.line

for line in sys.stdin:
	if not line:
		continue
	try:
		u = UserInfo()
		#u.ParseNormal(line)
		u.ParseYeyin(line)
		print u.Emit()	
		distance_f = u.distance + 1
		for f in u.friends:
			print "\t".join([str(f) , "D" , str(distance_f)] )
	except:
		traceback.print_exc()
		sys.exit(1)

