package com.xiaonei.reg.guide.dao;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;

public class GuideUserDecisionDAO {
    private static GuideUserDecisionDAO instance = new GuideUserDecisionDAO();

    public static GuideUserDecisionDAO getInstance() {
        return instance;
    }
    
    private final static String sqlGetStatus = "SELECT decision FROM guide_user_decision WHERE userid=?";
    public int getDecision(final int userId) throws SQLException {
        OpUniq op = new OpUniq(sqlGetStatus, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
            }
        };
        return DataAccessMgr.getInstance().queryInt(op);
    }

    
    public int saveDecision(final int userId, final int decision) throws SQLException {

        String sqlSave = "INSERT INTO guide_user_decision (userid, decision) VALUES("+userId+", "+decision+") ON DUPLICATE KEY " +
        		"UPDATE decision = (" + decision + ") ";
        OpUpdate op = new OpUpdate(sqlSave, DataAccessMgr.BIZ_STAT) {
//		
//			@Override
//			public void setParam(PreparedStatement ps) throws SQLException {
//				ps.setInt(1, userId);
//				ps.setInt(2, status);
//			}
        };
        return DataAccessMgr.getInstance().update(op);
    }

}
