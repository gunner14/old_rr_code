package com.xiaonei.reg.guide.dao;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;

public class GuideUserIdentityDAO {
    private static GuideUserIdentityDAO instance = new GuideUserIdentityDAO();

    public static GuideUserIdentityDAO getInstance() {
        return instance;
    }
    
    private final static String sqlGetStatus = "SELECT identity FROM guide_user_identity WHERE userid=?";
    public int getIdentity(final int userId) throws SQLException {
        OpUniq op = new OpUniq(sqlGetStatus, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
            }
        };
        return DataAccessMgr.getInstance().queryInt(op);
    }

    
    public int saveIdentity(final int userId, final int identity) throws SQLException {

        String sqlSave = "INSERT INTO guide_user_identity (userid, identity) VALUES(?,?)  ON DUPLICATE KEY " +
        		"UPDATE identity = (" + identity + ") ";
        OpUpdate op = new OpUpdate(sqlSave, DataAccessMgr.BIZ_STAT) {
		
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userId);
				ps.setInt(2, identity);
			}
        };
        return DataAccessMgr.getInstance().update(op);
    }

}
