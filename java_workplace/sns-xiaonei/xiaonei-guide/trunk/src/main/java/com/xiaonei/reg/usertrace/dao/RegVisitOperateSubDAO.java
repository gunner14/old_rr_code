package com.xiaonei.reg.usertrace.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.usertrace.model.RegVisitOperateSub;
import com.xiaonei.xce.XceAdapter;

import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Iterator;

public class RegVisitOperateSubDAO extends XnDAO {
	private static RegVisitOperateSubDAO instance = new RegVisitOperateSubDAO();

	public static RegVisitOperateSubDAO getInstance() {
		return instance;
	}

	private RegVisitOperateSubDAO() {
		super();
	}

	/**
	 * RegVisitOperate
	 * 
	 * @param RegVisitOperateSub
	 * @return
	 * @throws SQLException
	 */
	public int insertOprSubLog(final RegVisitOperateSub logOprSub, String domain)
			throws SQLException {
		String sqlInsert = "insert into act_subs (action_id, user_id, user_email, time, s1, s2, s3, s4) values (?, ?, ?, now(), ?, ?, ?, ?)";
		String bizName = XceAdapter.DB_REG_INVITE_ACT;
		if (domain != null && domain.indexOf("kaixin.com") > -1) {
			bizName = DataAccessMgr.BIZ_KX_REG_INVITE;
		}
		OpUpdate op = new OpUpdate(sqlInsert, bizName) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, logOprSub.getActionid());
				ps.setInt(2, logOprSub.getUserid());
				ps.setString(3, logOprSub.getEmail());
				ps.setString(4, logOprSub.getS1());
				ps.setString(5, logOprSub.getS2());
				ps.setString(6, logOprSub.getS3());
				ps.setString(7, logOprSub.getS4());
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
	
	public int[] batchSave(final HashMap<Integer, RegVisitOperateSub> infos, String domain) throws SQLException {
	    String sqlInsert = "insert into act_subs (action_id, user_id, user_email, time, s1, s2, s3, s4) values (?, ?, ?, now(), ?, ?, ?, ?)";
        String bizName = XceAdapter.DB_REG_INVITE_ACT;
        if (domain != null && domain.indexOf("kaixin.com") > -1) {
            bizName = DataAccessMgr.BIZ_KX_REG_INVITE;
        }
        OpUpdate op = new OpUpdate(sqlInsert, bizName) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                Iterator<Integer> iterator = infos.keySet().iterator();
                while(iterator.hasNext()){
                    Object oj=iterator.next();
                    RegVisitOperateSub logOprSub = (RegVisitOperateSub)infos.get(oj);
                    ps.setInt(1, logOprSub.getActionid());
                    ps.setInt(2, logOprSub.getUserid());
                    ps.setString(3, logOprSub.getEmail());
                    ps.setString(4, logOprSub.getS1());
                    ps.setString(5, logOprSub.getS2());
                    ps.setString(6, logOprSub.getS3());
                    ps.setString(7, logOprSub.getS4());
                    ps.addBatch();
                }
            }
        };
        return DataAccessMgr.getInstance().batchUpdate(op);
    }
}
