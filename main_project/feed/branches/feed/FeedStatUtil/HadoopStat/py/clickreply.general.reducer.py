#!/usr/bin/env python
#!/usr/bin/env python
#cl@105142708#249791720 2
#105142708#249791720 click reply recv

import sys
res = {}
for line in sys.stdin:
	(stype,click,reply,disp,disphead) =[int(i) for i in line.strip().split("\t")]
	if stype not in res:
		res[stype] = [click,reply,disp,disphead]
	else:
		tmp= []
		tmp = map(sum,zip(res[stype],[click,reply,disp,disphead])) 
		res[stype] = tmp
		
for k,v in res.iteritems():
	print k,v[0],v[1],v[2],v[3]
