#!/usr/bin/env python
#!/usr/bin/env python
#cl@105142708#249791720 2
#105142708#249791720 click reply recv

import sys
#res = {}
#(last_key, oldc) = (None,0) (last_u, d) = (None,{})

#100000147      2-2-2006        202167550,200680474,238448156,247453908,224225794
#100000147	N	1000	247453908	200680474	238448156	224225794	202167550
#247453908	D	1001

class UserStruct: 
	def __init__(self,uid = 0):
		self.uid = uid 
		self.distance = 100000
		self.school_info=""
		self.friends= ""
	def SetUid(self,uid):
		self.uid = uid
	def Handle(self,line):
		segs = line.strip().split()
		if(segs[1] == "D"):
			self.distance = min(int(segs[2]), self.distance)
		else:
			self.school_info= segs[1]
			self.friends= segs[2]
			if(len(segs) == 4):
				self.distance = min(int(segs[3]), self.distance)

	def Emit(self):
		if not self.school_info:
			return
		#print "\t".join([str(self.uid),"N",str(self.distance), self.friends,str(distance)]) 
		print "\t".join([str(self.uid),self.school_info,self.friends,str(self.distance)]) 
			
u = UserStruct(0) 
for line in sys.stdin:
	uid = int(line.strip().split()[0])
	if u.uid != 0 and uid != u.uid:
		u.Emit()
		u = UserStruct(uid)
		u.Handle(line)
	else:
		u.SetUid(uid)
		u.Handle(line)

if u.uid != 0:
	u.Emit()
