/**
 *
 */
package com.xiaonei.reg.register.logic;

import com.xiaonei.reg.register.logic.impl.RegAuditHsLogicImpl;
import com.xiaonei.reg.register.logic.impl.RegAuditUnivLogicImpl;

/**
 * RegAuditLogicFactory.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-10-9 下午04:45:01
 */
public class RegAuditLogicFactory {
    public static final String reg_audit_hs = "reg_audit_hs";
    public static final String reg_audit_univ = "reg_audit_univ";

    private static RegAuditLogicFactory _instance = new RegAuditLogicFactory();

    public static RegAuditLogicFactory getInstance() {
        return _instance;
    }

    public IRegAuditLogic getAudit(String met) {
        if (reg_audit_hs.equals(met)) {
            return RegAuditHsLogicImpl.getInstance();
        } else if (reg_audit_univ.equals(met)) {
            return RegAuditUnivLogicImpl.getInstance();
        } else {
            return null;
        }
    }
}
