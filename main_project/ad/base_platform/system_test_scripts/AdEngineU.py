#! /usr/bin/env python
import sys
sys.path.append('/data/xce')
sys.path.append('/data/xce/IcePy-3.3.0/python')
import traceback, Ice, time
import random
Ice.loadSlice("-I. --all ./Util.ice  ./AdEngine.ice")
from xce.ad import *
res = {}
status = 0
ic = None
try:
#  ic =  Ice.initialize(["--Ice.Default.Locator=Ad/Locator:default -h 10.3.18.39 -p 14800", "--Ice.Trace.Network=0"])
#  ic =  Ice.initialize(["--Ice.Default.Locator=Ad/Locator:default -h f0.xce.d.xiaonei.com -p 14800", "--Ice.Trace.Network=0"])

  ic =  Ice.initialize(["--Ice.Default.Locator=Ad/Locator:default -h 10.7.18.58 -p 14800", "--Ice.Trace.Network=0"])
  #name = "R@AdEngineS0"
  #proxy = AdRecommendCachePrx.checkedCast(ic.stringToProxy(name))
  #print proxy.Reload(1244171572)

  #for i in range(9):
  name = "M@AdEngineU0"
  print name
  m_proxy = AdMatchManagerPrx.checkedCast(ic.stringToProxy(name))
    #print m_proxy.GetAds('', 'http://apps.renren.com/happyfarm/?trackRef=bookmark&origin=3791', 0)
  #print m_proxy.GetAds('', 'http://share.renren.com/share/GetShare.do', 0)
  print m_proxy.GetAdsTest(242563746,4,1,30,"0086110000000000","0086110000000000",0,4,0,'http://share.renren.com/share/GetShare.do', 0)	
#GetAdsTest(::Ice::Int uid, ::Ice::Int stage,
#     ::Ice::Int gender, ::Ice::Int age, const ::std::string& currentArea,
#     const ::std::string& ipArea, ::Ice::Long school, ::Ice::Int grade,
#     int count, const string & referer, int rotate_index)

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

