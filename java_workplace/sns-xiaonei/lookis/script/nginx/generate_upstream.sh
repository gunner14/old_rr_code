#!/usr/bin/python
import sys
if len(sys.argv)<2:
	sys.stderr.write('Usage: '+sys.argv[0]+'default.srv upstream_name.srv upstream1_name.srv ... \n')
	sys.exit(1)
defsrv=sys.argv[1]
if cmp(defsrv,"default.srv")!=0:
	sys.stderr.write('at least default.srv will be pass to args to generate default proxy\n')
	sys.exit(1)
srvcount=len(sys.argv)-1
finc=open("./inc/upstream.inc","w")
while srvcount>0:
	fr=open(sys.argv[srvcount],"r")
	upstream_name=sys.argv[srvcount][:-4]
	fw=open("./inc/"+upstream_name+'.ups.inc','w');
	finc.write("include inc/"+upstream_name+".ups.inc;\n")
	fw.write("upstream "+upstream_name+" {\n")
	for line in fr:
		if cmp(line[0],'#')!=0:
			line=line[:-1]
			fw.write("server "+line+";\n")
	fw.write("}")
	fw.close()
	fr.close()
	srvcount=srvcount-1
finc.close()
