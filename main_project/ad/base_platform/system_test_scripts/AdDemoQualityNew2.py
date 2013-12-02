#! /usr/bin/env python
import sys
sys.path.append('/data/xce')
sys.path.append('/data/xce/IcePy-3.3.0/python')
import traceback, Ice, time
import random
Ice.loadSlice("-I. --all ./Util.ice ./AdDemoQuality.ice")
from xce.ad import *

if len(sys.argv) < 2:
  index = 0
else:
  index = sys.argv[1]

status = 0
ic = None
try:
#  ic = Ice.initialize(["--Ice.Default.Locator=Ad/Locator:default -h 10.3.18.186 -p 14800", "--Ice.Trace.Network=0","--Ice.MessageSizeMax=512000"])
  ic = Ice.initialize(["--Ice.Default.Locator=Ad/Locator:default -h f0.xce.d.xiaonei.com -p 14800", "--Ice.Trace.Network=0","--Ice.MessageSizeMax=512000"])
#  ic =  Ice.initialize(["--Ice.Default.Locator=Ad/Locator:default -h 10.3.18.39 -p 14800", "--Ice.Trace.Network=0","--Ice.MessageSizeMax=512000"])
  name = "M@AdDemoLoaderCache" + str(index)
  print "Sending request to " + name
  #name = "M@AdQuality1"
  proxy = AdDemoQualityManagerPrx.checkedCast(ic.stringToProxy(name))
  #proxy = DemoLoaderCachePrx.checkedCast(ic.stringToProxy(name))

  #ret = proxy.GetQualitysByZone(100000000001)
  #ret = proxy.GetQualityUsByZone(100000000070)
  ret = proxy.GetDemoPool()
#  ret = proxy.GetAppPool()
#  ret_sort_app = proxy.GetAppSortPos()
#  fresh_ad = proxy.GetFreshAdMap()
  count =0
  #ecpm = proxy.GetAverageEcpm()
  #display_less_gid = proxy.GetDisplayLessAds()
  #print len(ret)
  
#  for key,item in ret_sort_app.items():
#    seq = ""
#   for pos in item:
#     seq += str(pos)
#     seq += " "
#    print str(key) + " pos: " + seq 

  
  keys = ret.keys()
  
   
  for key in keys:
    #print key
   # print key + " " + str(ret[key].impr) + " " + str(ret[key].click)
    tokens = key
    tokens = tokens.split("_")
    if len(tokens) <3:
      print str(key) + str(ret[key])
      continue
    zoneid = tokens[0]
    gid = tokens[2]
    spid = tokens[1]
    gender = tokens[3]
    stage = tokens[4]
    print str(key) + " "  +str(ret[key])
    #if int(gender) == -1 and int(stage) == -1:
    #print "zoneid:" + str(zoneid) + ",gid:" + str(gid) +",spid:" + str(spid) + ",gender:" +str(gender) + ",stage:" + str(stage) +",ctr:" +str(ret[key])
   
  #print "len of fresh ad is " + str(len(fresh_ad))
 
  
  
#  for key in fresh_ad.keys():
#    print "key = " + str(key)
#    tmp = fresh_ad[key]
    #print "len of ads is " + str(len(fresh_ad[key]))
    #for gid in tmp.keys():
      #print "key = " + str(key) + " gid = " + str(gid) + " " + str(tmp[gid])

 #print "The size of return of GetDemoPool is " +  str(len(ret))
 
  
    
  '''
  #for key,value in ecpm.items():
  #  print str(key) + " " + str(value)
  #
  #for key,item in display_less_gid.items():
  #  for gid in item:
  #    print "key=" + str(key) + " gid=" + str(gid) + "  " + str(item[gid])
  '''



 # for key in fresh_ad.keys():
 #   for ad in fresh_ad[key].keys():
 #     print ad
 # print len(ret)
      

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

