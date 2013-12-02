#! /usr/bin/env python
import sys, traceback, Ice
Ice.loadSlice("-I../UtilSlice/slice -I../OceSlice/slice --all ../UtilSlice/slice/Util.ice ../UtilSlice/slice/ObjectCache.ice ../OceSlice/slice/User.ice ../OceSlice/slice/UserNetwork.ice")
import MyUtil
import mop.hi.svc.model
status = 0
ic = None
try:
    ic = Ice.initialize(["--Ice.Default.Locator=XiaoNei/Locator:default -h XiaoNeiRegistry -p 12000:default -h XiaoNeiRegistryReplica1 -p 12001:default -h XiaoNeiRegistryReplica2 -p 12002"])
    #ic = Ice.initialize(sys.argv)
    users = []
    nets = []
    #config = []
    for i in range(0, 10):
        print "Initializing proxy for ending with", i
        users.append( mop.hi.svc.model.UserManagerPrx.checkedCast(ic.stringToProxy("UM@User" + str(i))) )
        nets.append( mop.hi.svc.model.NetworkManagerPrx.checkedCast(ic.stringToProxy("NM@UserNetwork" + str(i))) )
        #config.append( mop.hi.svc.model.UserConfigManagerPrx.checkedCast(ic.stringToProxy("M@UserConfig" + str(i))) )
    #uid=int(200353733)
    #users[uid % 10].reloadUser(200353733)
    f = file("reloadUser.txt")
    while True:
        line = f.readline()
        if len(line) == 0:
            break
        str = line.split(",")
        for i in str:
            uid=int(i)
            eid=uid % 10
            print eid, uid
            users[eid].reloadUser(uid)
            users[eid].reloadUserBasic(uid)
            users[eid].reloadUserDesc(uid)
            nets[eid].reloadNetworkSeq(uid);
            nets[eid].reloadHighSchoolInfoSeq(uid);
            nets[eid].reloadUniversityInfoSeq(uid);
            nets[eid].reloadWorkplaceInfoSeq(uid);
            #config[eid].reloadUserConfig(uid);
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

