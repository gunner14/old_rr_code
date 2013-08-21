package com.xiaonei.reg.register.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;

import java.sql.PreparedStatement;
import java.sql.SQLException;

public class MultiAccountsMapDAO extends XnDAO {
    private static final String sqlGetUserId = "select user_id from multi_accounts_map where account=?";
    private static final String sqlSetAccount = "insert into multi_accounts_map set account=?,user_id=?,account_type=?";
    private static MultiAccountsMapDAO _instance = new MultiAccountsMapDAO();

    private MultiAccountsMapDAO() {

    }

    public static MultiAccountsMapDAO getInstance() {
        return _instance;
    }

    public int getUserId(final String account) throws SQLException {
        OpUniq op = new OpUniq(sqlGetUserId, DataAccessMgr.BIZ_USER_COMMON) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, account);
            }
        };
        return DataAccessMgr.getInstance().queryInt(op);
    }

    public int setAccount(final String account, final int userId,
                          final int accountType) throws SQLException {
        OpUpdate op = new OpUpdate(sqlSetAccount, DataAccessMgr.BIZ_USER_COMMON) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                super.setParam(ps);
                ps.setString(1, account);
                ps.setInt(2, userId);
                ps.setInt(3, accountType);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }
}
