package com.xiaonei.reg.register.logic.additional;

import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.register.constants.IAccountType;
import com.xiaonei.reg.register.constants.XIdBlackList;

public class RegXidLogic {

    public static boolean isXid(int accType, String regEmail) {
        if (accType != IAccountType.XID) {
            return false;
        }
        return isXid(regEmail);
    }

    public static boolean isXid(String account) {
        if (StringFormat.isValidEmailAddr(account)) {
            return false;
        }
        if (StringFormat.isValidMobile(account)) {
            return false;
        }
        return true;
    }

    public static boolean isInBlackList(String xid) {
        Boolean rt = XIdBlackList.getBlackList().get(xid);
        if (null != rt) {
            return rt.booleanValue();
        }
        return false;
    }

}
