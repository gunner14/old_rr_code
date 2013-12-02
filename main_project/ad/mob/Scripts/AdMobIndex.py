#! /usr/bin/env python
import sys
sys.path.append('/data/xce')
sys.path.append('/data/xce/IcePy-3.3.0/python')
import traceback, Ice, time
import random
#Ice.loadSlice("-I. --all ./Util.ice  ./AdMob.ice")
Ice.loadSlice(" ./AdMob.ice")
#import AdMob
from xce.ad import *
res = {}
status = 0
ic = None
try:
  ic =  Ice.initialize(["--Ice.Default.Locator=AdMob/Locator:default -h 10.3.22.26 -p 14880", "--Ice.Trace.Network=0"])

#  print name
  #Server_proxy = AdMobPrx.checkedCast(ic.stringToProxy("M@AdMob0"))

  proxy = AdMobMonitorPrx.checkedCast(ic.stringToProxy("B@AdMobEngine0"))
  para = AdMobReqPara()
  para.uid = 221416299
  para.requestTime = 20120427101010
  para.appid = "198733"
  para.sid = "11111"
  para.client.osVersion = "android 1.5"
  para.client.model = "HTC"
  para.adzoneId = 100000000098
  para.client.screenSize="800*100"
  para.locationRealTime.longitude=116.3071
  para.locationRealTime.latitude=39.96931
  #print para  
  #x = [ INDEXTYPE.GENDERINDEX, INDEXTYPE.STAGEINDEX, INDEXTYPE.AGEINDEX, INDEXTYPE.IPAREAINDEX, INDEXTYPE.GRADEINDEX, INDEXTYPE.SCHOOLINDEX, INDEXTYPE.PLATFORMINDEX, INDEXTYPE.NETWORKINDEX, INDEXTYPE.BRAND3GINDEX, INDEXTYPE.RESOLUTIONINDEX, INDEXTYPE.LBSINDEX ]
  #for i in x:
  #  result = proxy.GetBrandCandidateGroups(para,i);
  #  print '[' + str(i) + ']'
  #  print result
  #  print " "
  #result = proxy.GetBrandIndexInfo( AdMobIndex.Type.LBS, 100000000094)
  print "Get Brand Candidate Groups:"
  result = proxy.GetBrandCandidateGroups( para, 1 )
  print result
  
  print "Get Self Candidate Groups:"
  result = proxy.GetSelfCandidateGroups( para, 1 )
  print result
  
  print "Get Rotate Groups:"
  result2 = proxy.GetRotateGroups(100000000098)
  print result2

  x = [ 1,2,3,4,5,6,7,8,9,10,11,12]
  y = [ "NONE", "GENDER", "STAGE", "AGE", "GRADE", "SCHOOL", "AREA", "SCHOOLAREA", "PLATFORM", "NETWORK", "BRAND3G", "RESOLUTION", "LBS", "MAX" ]
  print "Get Brand Index All Groups:"
  for i in x:
    result2 = proxy.GetBrandIndexInfo(i,100000000098)
    print y[i]
    print result2

  print "Get Self Index All Groups:"
  for i in x:
    result2 = proxy.GetSelfIndexInfo(i,100000000098)
    print y[i]
    print result2

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

