package com.xiaonei.xce.idcache;

import idcache.VirtualIdInfo;

public class VirtualIdInfoC extends VirtualIdInfo {

    VirtualIdInfoC() {
        virtualId = -1;
        userId = -1;
        transId = -1;
    }

    VirtualIdInfoC(VirtualIdInfo info) {
        virtualId = info.virtualId;
        userId = info.userId;
        transId = info.transId;
    }

    public boolean isVirtual() {
        return virtualId > 0;
    }

    public boolean isValid() {
        return userId > 0;
    }

    public String toString() {
        String result = "VirtualIdInfoC:[";
        result += "virtualId:" + virtualId;
        result += ",userId:" + userId;
        result += ",transId:" + transId;
        result += "]";
        return result;
    }

}
