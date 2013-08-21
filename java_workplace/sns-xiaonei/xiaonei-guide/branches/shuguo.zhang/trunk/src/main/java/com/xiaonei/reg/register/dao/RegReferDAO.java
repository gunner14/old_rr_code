package com.xiaonei.reg.register.dao;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.xce.XceAdapter;

public class RegReferDAO extends XnDAO {
	
	private static final RegReferDAO instance = new RegReferDAO();
	
	private RegReferDAO(){
		
	}

	public static RegReferDAO getInstance(){
		return instance;
	}
	
	public int save(final int userid, final int pageId, final String refer)
			throws SQLException {
		final String sqlInsert = "insert into reg_refer (userid, page_id, refer) values (?, ?, ?)";
		final OpUpdate op = new OpUpdate(sqlInsert, XceAdapter.DB_REG_INVITE_ACT) {
			@Override
			public void setParam(final PreparedStatement ps)
					throws SQLException {
				ps.setInt(1, userid);
				ps.setInt(2, pageId);
				ps.setString(3, refer);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}

}
