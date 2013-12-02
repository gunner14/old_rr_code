#! /usr/bin/env python
import sys
sys.path.append('/data/xce')
sys.path.append('/data/xce/IcePy-3.3.0/python')
import traceback, Ice, time
import random
Ice.loadSlice("-I. --all ./Util.ice  ./FeedEdmSender.ice")
#from xce.ad import *
import xce.edm
import MyUtil

ic = None
status=0
try:
  ic =  Ice.initialize(["--Ice.Default.Locator=Ad/Locator:default -h 10.3.18.39 -p 14800", "--Ice.Trace.Network=0"])
#  ic =  Ice.initialize(["--Ice.Default.Locator=Ad/Locator:default -h f0.xce.d.xiaonei.com -p 14800", "--Ice.Trace.Network=0"])

  serviceName = "FEDM@FeedEdmSender" + "0";
  obj = xce.edm.FeedEdmSenderPrx.checkedCast(ic.stringToProxy(serviceName))
  ip_map = { "ip" : "60.194.42.1"}
  ip_map["limit_check"] = "1"
  ip_map["platform"] = "1"
  ip_map["coordinate"] = ""
  #id = 227902933
  id = 43412685
#  id = 246074010
 #id = 246074010
  obj.Receive(id,ip_map)

  obj.ClearFrequencyCacheByUser(id);


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

