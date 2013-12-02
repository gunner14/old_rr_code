#! /usr/bin/env python
import sys
sys.path.append('/data/xce')
sys.path.append('/data/xce/IcePy-3.3.0/python')
import traceback, Ice, time
import random
import datetime
import random
Ice.loadSlice("-I.. --all ../Util.ice  ./AdMob.ice")
#Ice.loadSlice(" ./AdMob.ice")
from xce.ad import *
res = {}
status = 0
ic = None
try:
  ic =  Ice.initialize(["--Ice.Default.Locator=AdMob/Locator:default -h 10.3.22.26 -p 14880", "--Ice.Trace.Network=0"])

  proxy = AdMobPrx.checkedCast(ic.stringToProxy("M@AdMobEngine0"))

  uid_set = [438126681,442555358,438127289]

  zone_set = [100000000098,100000000099]

  net_set = ["0100"]

  ipArea_set = ["0086110000000000"]   

  screen_set = ["320X480","450X800","480X800","640X960","220X300"]

  client_type_set = ["020201001"]
  client_os_set = ["iPhone OS4.0"]
  #client_os_set = ["android4.0x"]

  i = 0
  for uid in uid_set:
    for zone in zone_set:
      for net in net_set:
        for ipArea in ipArea_set:
          for screen in screen_set:
	    for client_type in client_type_set:
	      for client_os in client_os_set:
   	     	para = AdMobReqPara()
             	para.uid = uid
		para.netStatus = net
             	para.requestTime = 20120427101010
             	para.appid = "198733"
             	para.sid = "1111kso29ify9fy8sfh1"
             	para.adzoneId = zone
             	para.client.screenSize=screen
             	para.client.type=client_type
             	para.client.osVersion=client_os
		para.ipArea=ipArea
             	para.lastGroupIds = [ 60000505720001 ]
 
             	result = proxy.GetAds(para)
             	i += 1
             	str_para = "["+ str(i) +"]," + "[uid:" + str(para.uid) + "],[" + "screenSize:" + str(para.client.screenSize) + "],[" + "ipArea:" + str(para.ipArea) + "],"
             	#str_para = "["+ str(i) +"]," + "[uid:" + str(para.uid) + "],[" + "adzone:" + str(para.adzoneId) + "],[netstats:" + net  + "],["  + "screenSize:" + str(para.client.screenSize) + "],[" + "type:" + str(para.client.type) + "],[" + "osVersion:" + str(para.client.osVersion) + "],[" + "ipArea:" + str(para.ipArea) + "],"
             	print str_para,
             	str_result = "[adgroup:" + str(result.adgroupId) + "]"
             	print str_result
						

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

