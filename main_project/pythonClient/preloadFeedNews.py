#! /usr/bin/env python
import sys, traceback, Ice
Ice.loadSlice("-I/opt/Ice-3.2/slice -I../UtilSlice/slice -I../OceSlice/slice --all ../UtilSlice/slice/Util.ice ../UtilSlice/slice/ObjectCache.ice ../OceSlice/slice/Feed.ice")
import MyUtil
import mop.hi.svc.feed
status = 0
ic = None
try:
    ic = Ice.initialize(["--Ice.Default.Locator=XiaoNei/Locator:default -h XiaoNeiRegistry -p 12000:default -h XiaoNeiRegistryReplica1 -p 12001:default -h XiaoNeiRegistryReplica2 -p 12002"])
    #ic = Ice.initialize(sys.argv)
    users = []
    for i in range(0, 100):
        print "Initializing proxy for ending with", i
        users.append( MyUtil.ObjectCachePrx.checkedCast(ic.stringToProxy("SC@FeedNews" + str(i))) )
        #users.append( mop.hi.svc.feed.FeedNewsManagerPrx.checkedCast(ic.stringToProxy("FN@FeedNewsTest")) )
    #uid=int(200353733)
    #users[uid % 10].reloadUser(200353733)
    f = file("preloadFeedNews.txt")
    while True:
        line = f.readline()
        if len(line) == 0:
            break
        str = line.split(",")
        for i in str:
            uid=int(i)
            eid=uid % 100
            print eid, uid
            #users[eid].reloadUser(uid)
            #users[eid].reloadUserBasic(uid)
            #users[eid].reloadUserDesc(uid)
            #users[eid].reloadUserVisitCount(uid)
            try:
                users[eid].preloadObject(0, uid)
            except:
                traceback.print_exc();
            #nets[eid].reloadNetworkSeq(uid);
            #nets[eid].reloadHighSchoolInfoSeq(uid);
            #nets[eid].reloadUniversityInfoSeq(uid);
            #nets[eid].reloadWorkplaceInfoSeq(uid);
            #config[eid].reload(uid);
    f.close()

except:
    traceback.print_exc()
    status = 1
if ic:
    try:
        ic.destroy()
    except:
        traceback.print_exc()
        status = 1
sys.exit(status)

