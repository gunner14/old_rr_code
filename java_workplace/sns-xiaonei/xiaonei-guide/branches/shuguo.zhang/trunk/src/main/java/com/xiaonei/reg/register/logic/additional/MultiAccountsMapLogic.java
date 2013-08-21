package com.xiaonei.reg.register.logic.additional;

import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.register.constants.IMultiAccountMap;
import com.xiaonei.reg.register.dao.MultiAccountsMapDAO;

import java.sql.SQLException;

public class MultiAccountsMapLogic implements IMultiAccountMap {
    private static MultiAccountsMapDAO dao = MultiAccountsMapDAO.getInstance();
    private static MultiAccountsMapLogic _instance = new MultiAccountsMapLogic();

    private MultiAccountsMapLogic() {

    }

    public static MultiAccountsMapLogic getInstance() {
        return _instance;
    }

    public int getUserId(String account) {
        try {
            return dao.getUserId(account);
        } catch (SQLException e) {
            e.printStackTrace();
            return 0;
        }
    }

    public void setAccount(String account, int userId) {
        if (this.getUserId(account) != 0) {
            return;
        }
        int accountType;
        if (StringFormat.isValidEmailAddr(account)) {
            accountType = ACCOUNT_TYPE_EMAIL;
        } else if (StringFormat.isValidMobile(account)) {
            accountType = ACCOUNT_TYPE_MOBILE;
        } else {
            accountType = ACCOUNT_TYPE_XID;
        }
        try {
            dao.setAccount(account, userId, accountType);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}
