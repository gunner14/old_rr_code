#!/usr/bin/env python
#!/usr/bin/env python
#cl@105142708#249791720 2
#105142708#249791720 click reply recv

import sys
res = {}
(last_key, oldc) = (None,0)
(last_u, d) = (None,{})

def PrintInfo():
	map = {}
	tmpstr = str(last_u)
	for k in d.keys():
		map[k] = d[k][0] + 3 * d[k][1]
	cnt = 0;
	for k,v in reversed(sorted(map.items(),key = lambda d:d[1]))	:
		tmpstr+= " " + str(k) # + " " + str(v)
		cnt += 1
		if cnt >= 20:
			break
	print tmpstr
	#if cnt == 0:
	#	return
		
for line in sys.stdin:
	(u,f,c,r) =line.strip().split("\t")
	if u == f:
		continue
	key = u + "\t" + f
	f = int(f)
	if last_u and last_u != u :
		PrintInfo()
		last_u = u
		d = {}
		d[f] = [int(c),int(r)]
	else:
		last_u = u
		d[f] = [d.get(f,[0,0])[0] + int(c) ,d.get(f,[0,0])[1]+ int(r)]

if last_u:
	PrintInfo()
