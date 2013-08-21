import re
newupload = re.compile(r'''\[(\d+):(\d+):(\d+)/(\d+):.+ blumid: 303486564''', re.U|re.S)

grp = None
pid = ""
file= ""
size= ""
i=0
j=0
kv={}
f = open("tt2.log", 'r')
for x in  f:
  grp = newupload.search(x)
  if grp:
    pid = grp.group(1)
    i = i+1
    #print "new_upload: ",i, pid, grp.group(3), grp.group(4)
    continue
  if not file:
    grp = re.search("\[" + pid + ":" + pid + ":.+No.0 \[(.+)\] length: (\d+)", x)
    if grp:
      file = grp.group(1)
      size = grp.group(2)
      j =j+1
      #print "new_file: ",j
  else:
    grp = re.search("\[" + pid + ":" + pid + ":.+generate jpg path: (.+) (.+) (.+) (.+) ", x)
    if grp:
      print file, " url: ", grp.group(1), grp.group(2), grp.group(3), grp.group(4)
      kv[file] = grp.groups()
      pid = ""
      file=""
      size=""
f.close()
#print "-----------------"
#for i,j in kv.items():
#  print i,j
