#! /usr/bin/env python
import sys
sys.path.append('/data/xce')
sys.path.append('/data/xce/IcePy-3.3.0/python')
import traceback, Ice, time
import random
import datetime
Ice.loadSlice("-I.. --all ../Util.ice  ./AdMob.ice")
#Ice.loadSlice(" ./AdMob.ice")
from xce.ad import *
res = {}
status = 0
ic = None
try:
  ic =  Ice.initialize(["--Ice.Default.Locator=AdMob/Locator:default -h 10.3.22.26 -p 14880", "--Ice.Trace.Network=0"])

  proxy = AdMobPrx.checkedCast(ic.stringToProxy("M@AdMobEngine0"))

  para = AdMobReqPara()
  para.uid = 221416299
  para.requestTime = 20120427101010
  para.appid = "198733"
  para.sid = "11111"
  para.adzoneId = 100000000098
  para.client.screenSize="320*150"
  para.lastGroupIds = [ 10000505720001 ]
  para.ipArea="0086110000000000"
 
  #proxy.BindUser(1,111,111)
  #timestart = datetime.datetime.now()
  #for i in range(1,1000):
  result = proxy.GetAdsForTest(para)
  #timeend = datetime.datetime.now()
  #timeval = timeend - timestart
  #print timestart
  #print timeend
  #print timeval
  #result = Server_proxy.GetAds(AdMobReqPara())
  #print "---------------------------"
  #print para.client.screenSize
  #print result.uid
  #print result.adgroupId
  #print result.pageData.clickUrl
  #print result.pageData.htmlData
  #print result.adzoneId
  print result
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

