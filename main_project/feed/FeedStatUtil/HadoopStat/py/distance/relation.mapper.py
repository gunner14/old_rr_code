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
badcnt=0
infinite=0
rootuid=0
if len(sys.argv) != 3:
	print >> sys.stderr ,'Usage:',sys.argv[0]," rootuid round "
	exit(1)
rootuid = int(sys.argv[1])
round = int(sys.argv[2])
depth = round+1

#rootuid=220678672
class UserInfo:
	def __init__(self):
		self.uid = 0
		self.friends = set()
		self.distance = 1000
		self.line = ""
	def ParseYeyin(self,line):
		self.line = line.strip()
		segs = self.line.split()
		if len(segs) < 3:
			return False
		if len(segs) == 4:
			self.distance = int(segs[3])
		[id,tmp,f] = segs[:3]

		self.uid = int(id)
		if self.uid == rootuid:
			self.distance = 0
		for i in f.strip().split(','):
			if i != "":
				self.friends.add(int(i))
		if len(segs) == 3:
			self.line += ("\t" + str(self.distance))
		return True
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
		if not u.ParseYeyin(line):
			continue
		print u.Emit()	

		if u.distance == 1000:
			infinite+=1
			continue

		distance_f = u.distance + 1
		if distance_f != depth:
			badcnt += 1
			continue

		for f in u.friends:
			#print "\t".join([str(f) , "D" , str(distance_f)] )
			if f in mdict: 	
				mdict[f] = min(mdict[f],distance_f)
				#old = mdict[f]
				#if old < 0 :
				#	if -old > distance_f:
				#		mdict[f] = distance_f:	
				#else:
				#	if old > distance_f:
				#		mdict[f] = distance_f
			else:
				mdict[f] = distance_f
	except:
		print >> sys.stderr, 'error:'+line.strip()
		traceback.print_exc()
		#sys.exit(1)
		#pass

for k,v in mdict.iteritems():
	print "\t".join([str(k) , "D" , str(v)] )


print >> sys.stderr, 'badcnt:',badcnt, "infinite:",infinite, "mdict:",len(mdict)
