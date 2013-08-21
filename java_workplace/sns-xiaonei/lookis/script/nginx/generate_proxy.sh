#!/usr/bin/python
import sys,os
if len(sys.argv)<2:
	sys.stderr.write('Usage: '+sys.argv[0]+' host.proxy \n')
	sys.exit(1)
path=os.path.abspath(os.path.dirname(sys.argv[0]))
fr=open(sys.argv[1],"r")
finc=open(path+"/inc/proxy.inc",'w')
for line in fr:
	proxy_pass=line.split()[0]
	url_patten=line.split()[1]
	fw=open(path+"/inc/"+url_patten+".proxy.inc",'w')
	finc.write("include inc/"+url_patten+".proxy.inc;\n")
	fw.write("location ~ ^/"+url_patten+" {\n")
	fw.write("proxy_pass	http://"+proxy_pass+";\n")
	fw.write("include	proxy_setting.inc;\n")
	fw.write("}\n")
	fw.close()
fr.close()
#generate default proxy
fw=open(path+"/inc/default.proxy.inc",'w')
fw.write("location / {\n")
fw.write("proxy_pass	http://default;\n")
fw.write("include	proxy_setting.inc;\n")
fw.write("}\n")
fw.close()
finc.write("include inc/default.proxy.inc;\n")
finc.close()
