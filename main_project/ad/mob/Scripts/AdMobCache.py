#! /usr/bin/env python
import sys
sys.path.append('/data/xce')
sys.path.append('/data/xce/IcePy-3.3.0/python')
import traceback, Ice, time
import random
#Ice.loadSlice("-I. --all ./Util.ice  ./AdMob.ice")
Ice.loadSlice(" ./AdMob.ice")
from xce.ad import *
res = {}
status = 0
ic = None
try:
  ic =  Ice.initialize(["--Ice.Default.Locator=AdMob/Locator:default -h 10.3.22.26 -p 14880", "--Ice.Trace.Network=0"])

  proxy = AdMobMonitorPrx.checkedCast(ic.stringToProxy("B@AdMobEngine0"))
  #x = [ AdMobPool::Type.MEMBER, AdMobPool::Type.CAMPAIGN, AdMobPool::Type.GROUP,  AdMobPool::Type.ZONE,  AdMobPool::Type.PLATFORM,  AdMobPool::Type.BRAND3G,  AdMobPool::Type.RESOLUTION ]
  #x = [ Type.MEMBER, Type.CAMPAIGN, Type.GROUP, Type.ZONE, Type.PLATFORM, Type.BRAND3G, Type.RESOLUTION ]
  x = [1,2,3]
  for i in x:
    result = proxy.GetPoolSize(i)
    print result
    result = proxy.GetPoolAll(i,10000)
    print result
    print " "

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

