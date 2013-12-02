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

 # uid_set = [302882103,394209428,394209428,354146650,354146650,354146650,354146650,354146650,308557935,459840702,459840702,459840702,459840702,
  #           252659039,252659039,252659039,252659039,252659039,459992875,459992875,202301368,202301368,459992875,459992875,459992875,459992875,
   #          352894636,352894636,352894636,335994511,335994511,335994511,352894636,461575714,461575714,461575714,461575714,461575714,369140371,
    #         369140371,309501649,309501649,341476931,341476931,341476931,341476931,341476931,276776970,276776970,276776970,276776970,276776970,
     #        276776970,276776970,276776970,276776970,264462886,264462886,333249732,333249732,333249732,333249732,276776970,276776970,333249732,
     #        255941667,330222437,336123196,336123196,352618732,352618732,336123196,352618732,336123196,336123196,269249169,269249169,405477099]

  uid_set = [438126681,442555358,438127289]

  zone_set = [100000000098,100000000099]

  net_set = ["0100","0200","0000"]

  #ipArea_set = ["0086222400000000","0086230000000000","0086310000000000","0086310100000000","0086310200000000","0086320000000000",
   #             "0086330000000000","0086340000000000","0086341600000000","0086350000000000","0086360000000000","0086370000000000",
    #            "0086410000000000","0086420000000000","0086429000000000","0086440000000000","0086450000000000","0086460000000000",
     #           "0086469000000000","0086500000000000","0086500100000000","0086500200000000","0086513400000000","0086522200000000",
      #          "0086522400000000","0086522600000000","0086532500000000","0086540000000000","0086610000000000","0086640000000000",
       #         "0086652300000000","0086653000000000","0086654000000000","0086659000000000","0086710000000000","0086810000000000",
        #        "0086820000000000","0001000000000000","0086110000000000"]
   
  ipArea_set = ["0086110000000000", "0086120000000000","0086310000000000", "0001000000000000"]  

  #screen_set = ["220*220","320X480","450×800","480×800","490×900","640X960"]
  screen_set = ["320X480","450X800","480X800","640X960","220X300"]

  client_type_set = ["020201001","020201002","020201003","020201004","020201005",
                     "020201006","020201007","020201008","020201009","020201010",
                     "020201011","020201012","020201013","020201014","020201015",
                     "020201997","020201998","020201999"]
  client_os_set = ["iPhone OS4.0","iPhone OS4.0.1","iPhone OS4.0.2","iPhone OS4.1","iPhone OS4.2.1","iPhone OS4.2.7",
                   "iPhone OS4.2.8","iPhone OS4.3","iPhone OS4.3.1","iPhone OS4.3.2","iPhone OS4.3.3","iPhone OS4.3.4",
                   "iPhone OS4.3.5","iPhone OS5.0","iPhone OS5.0.1","iPhone OS5.1","iPhone OS5.1.1"]
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
             	str_para = "["+ str(i) +"]," + "[uid:" + str(para.uid) + "],[" + "adzone:" + str(para.adzoneId) + "],[netstats:" + net  + "],["  + "screenSize:" + str(para.client.screenSize) + "],[" + "type:" + str(para.client.type) + "],[" + "osVersion:" + str(para.client.osVersion) + "],[" + "ipArea:" + str(para.ipArea) + "],"
             	print str_para,
             	str_result = "[adgroup:" + str(result.adgroupId) + "]"
             	print str_result
						
  #i=0
  #while 1==1:
  #  para = AdMobReqPara()
  #  para.uid = random.choice(uid_set)
  #  para.requestTime = 20120427101010
  #  para.appid = "198733"
  #  para.sid = "1111kso29ify9fy8sfh1"
  #  para.adzoneId = random.choice(zone_set)
  #  para.client.screenSize=random.choice(screen_set)
  #  para.client.type=random.choice(client_type_set)
  #  para.client.osVersion=random.choice(client_os_set)
  #  para.ipArea=random.choice(ipArea_set)
  #  para.lastGroupIds = [ 60000505720001 ]
 
  #  result = proxy.GetAds(para)
  #  i += 1
  #  str_para = "["+ str(i) +"]--参数--:\t" + "[uid:" + str(para.uid) + "]  [" + "adzone:" + str(para.adzoneId) + "]  [" + "screenSize:" + str(para.client.screenSize) + "]  [" + "type:" + str(para.client.type) + "]  [" + "osVersion:" + str(para.client.osVersion) + "]  [" + "ipArea:" + str(para.ipArea) + "]"
  #  print str_para
  #  str_result = "["+ str(i) +"]--结果--:\t" + "[adgroup:" + str(result.adgroupId) + "]\n"
  #  print str_result
  #print "---------------------------"
  #print para.client.screenSize
  #print result.uid
  #print result.adgroupId
  #print result.pageData.clickUrl
  #print result.pageData.htmlData
  #print result.adzoneId
  #print result
  #print "---------------------------"


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

