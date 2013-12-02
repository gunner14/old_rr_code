#! /usr/bin/env python
import sys
sys.path.append('/data/xce')
sys.path.append('/data/xce/IcePy-3.3.0/python')
import traceback, Ice, time
import random
Ice.loadSlice("-I. --all ./Util.ice  ./AdEdmSender.ice")
#from xce.ad import *
import xce.ad
import MyUtil
import time

TIMES = 2000000

ic = None
status=0
try:
  ic =  Ice.initialize(["--Ice.Default.Locator=Edm/Locator:default -h 10.7.18.58 -p 14810", "--Ice.Trace.Network=0"])
  #ic =  Ice.initialize(["--Ice.Default.Locator=Edm/Locator:default -h 10.3.23.66 -p 14810", "--Ice.Trace.Network=0"])
#  ic =  Ice.initialize(["--Ice.Default.Locator=Ad/Locator:default -h f0.xce.d.xiaonei.com -p 14800", "--Ice.Trace.Network=0"])

  serviceName = "ADEDM@AdEdmSender" + "0";
  obj = xce.ad.AdEdmSenderPrx.checkedCast(ic.stringToProxy(serviceName))
  #ip_map = { "ip" : "114.80.68.233"}
  ip_map = { "ip" : "210.22.116.236"}
  ip_map = { "check_flag" : "0"}
  ip_map = { "user_status_set" : "3"}
  #ip_map = { "user_status_set" : "-1"}
  #id = 221203579
  #seq = [253807329,229942474,227902933,410145570,235365847,30314];
  #seq = [438688951,438703573]
  #seq = [438680732]
  #seq = [438690833]
  #seq = [438680732]
  #seq = [439365852]
  #seq = [439364628]
  #seq = [253807329]
  #seq = [439366598]
  #seq = [463603606]
  #seq = [384127650]
  #seq = [438629549]
  #seq = [390473797]
  #seq = [383801072]
  seq = [463603606]

  i = 0;
  for id in seq:
  #id = 89
  #id = int(sys.argv[1]) 
  #id = 198000014156
  #id = 261967480 #shuang 
  #id =4389631025
      #print "client: " + str(i);
    print "before Expose,userID:" + str(id)
    #adgroup_id = 198000014156;
    #obj.Expose(id, adgroup_id, i);
    obj.SendEdm(id, ip_map); 
    #index = 0;
    #obj.Expose(id, adgroup_id, index);
    print "after Expose"
  #obj.ClearLoginCacheByUser(id)
    time.sleep(5);

except Exception, e:
    print "MemoryGarden : " + str(e)
    traceback.print_exc()
    status = 1
if ic:
    try:
        ic.destroy()
    except:
        traceback.print_exc()
        status = 1
sys.exit(status)

