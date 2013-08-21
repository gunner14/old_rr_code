import re

v1 = re.compile(r'''(\d+)\s(\d+)\s(\w+-\w+)\s(\w+)''', re.U|re.S)
v2 = re.compile(r'''Addr: (\d+)\.(\d+)\.(\d+)\.(\d+)''', re.U|re.S)

x = []
city = {}
ipdep = []

def check(ip):
  for i in ipdep:
    if ip >= i[0] and ip <= i[1]:
      return i
  else:
    return None


if __name__ == "__main__":
  print "get ip library",
  f = open("iplog")
  for line in f:
    ss = line.decode("u8")
    grp = v1.search(ss)
    if grp:
      beg = long(grp.group(1))
      end = long(grp.group(2))
      name = grp.group(3)
      py = grp.group(4)
      ipdep.append((beg, end, name, py))
  f.close()
  print " ok:", len(ipdep)
  print "start fenxi log",

  f1 = open("o.log")
  count = 0
  for line in f1:
    if count % 1000 == 0:
      print count
    count = count + 1
    grp = v2.search(line)
    if grp:
      ip1 = long(grp.group(1))
      ip2 = long(grp.group(2))
      ip3 = long(grp.group(3))
      ip4 = long(grp.group(4))
      ip = ip1 * 256 * 256 * 256 + ip2 * 256 * 256 + ip3 * 256 + ip4
      #print grp.groups(), ip, count
      res = check(ip)
      if res:
        if city.has_key(res[2]):
          city[res[2]] = city[res[2]] + 1
        else:
          city[res[2]] = 1
        #print grp.groups(), res[3]
  f1.close() 
  print " ok:", len(city)
  f2 = open("res.log", "w")
  for i,j in city.items():
    ii = i.encode('u8')
    #print ii, j
    f2.write("%s,%d\n" % (ii,j))
  f2.close()
  print "--end--"
