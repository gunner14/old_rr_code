#!/usr/bin/env python
import sys
import types
import os
import re
import traceback
current_path = str(os.getcwd())
sys.path.append(current_path)

#cl@105142708#249791720 2
#105142708#249791720 click reply recv

def CheckType(type):
	return True
	tmp = type / 100
	return tmp != 29
def CheckToid(id):
	return id / 100 % 10 == 1 
mdict={}

def InsertV2(stype,cl,rp,ds,dshead):
	if not mdict.has_key(stype):
		mdict[stype] = [cl,rp,ds,dshead] 
		return
	mdict[stype][0]+=cl
	mdict[stype][1]+=rp
	mdict[stype][2]+=ds
	mdict[stype][3]+=dshead

def HandleClick(segs):
	#[feedid,toid,ip,url] =segs
	feedid=segs[0]
	toid=segs[1]
	ip=segs[2]
	url=segs[3]
	if toid == "0":
		return
	if(re.search("ref=hotnewsfeed", url)):
		m = re.search(r"(?<=sfet=)\d+",url)
		if(m):
			stype = m.group(0);
			if(not CheckType(int(stype))):
				return
			InsertV2(int(stype),1,0,0,0)

#def HandleReply(segs):
#    [toid,actor,stype,source] =segs
#    if(not CheckType(int(stype))):
#        return

def HandleReply_new(segs):
	[type,seconds,toid,actor,stype,source,feed] =segs
	if(not CheckType(int(stype))):
		return
	InsertV2(int(stype),0,1,0,0)

def HandleRecv(segs):
	if(len(segs) <=2):
		return
	actor = segs[0]
	stype = segs[1]
	for i in range(2, len(segs)):
		print segs[i]+"\t"+actor+"\t0\t0\t1"

def HandleRecv_new(segs):
	if(len(segs) <=2):
		return
	stype = int(segs[3])
	if(not CheckType(int(stype))):
		return
	InsertV2(stype,0,0,1,0)
#dV2 time:1324788663 fid:14243418930 source:10790464096 stype:102 actor:229074170 newsmerge:438582837 minimerge:438582837 targetsize:856 targets
def HandleRecvV2(segs):
	if(len(segs) <=9):
		return
	stypeseg = (segs[4])
	targetsizeseg = (segs[8])
	stype=stypeseg[6:]
	targetsize=targetsizeseg[11:]
	InsertV2(int(stype),0,0,1,int(targetsize))


for line in sys.stdin:
	if(line):
		try:
			line = line.strip()
			segs = line.split(" ")
			if(segs[0] == 'r'): #reply
				HandleReply_new(segs)
			elif len(segs) >= 4 and segs[3][0:4] == "http":#click
				HandleClick(segs)
			elif len(segs) > 1 and segs[0] == "d":#recv
				HandleRecv_new(segs)
			elif len(segs) > 1 and segs[0] == "dV2":#recv
				HandleRecvV2(segs)
		except:
			traceback.print_exc()
			pass

for stype,cr in mdict.items():
	print '\t'.join([str(stype),str(cr[0]),str(cr[1]),str(cr[2]),str(cr[3])])


