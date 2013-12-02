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

			
cnt = 0
for line in sys.stdin:
	cnt += int(line.strip())
print cnt
