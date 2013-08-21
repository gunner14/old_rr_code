package com.xiaonei.recall_email.auto_login.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.recall_email.auto_login.model.RecallEmailAutoLoginModel;

public class RecallKeyDAO {
	private static final RecallKeyDAO instance = new RecallKeyDAO();

	public static RecallKeyDAO getInstance() {
		return instance;
	}

	public RecallEmailAutoLoginModel get(final String key) throws SQLException {
		String sql = "select userid,login_key,insert_time,is_overtime from recall_email_auto_login where login_key=?";
		OpUniq op = new OpUniq(sql, DataAccessMgr.BIZ_RROP_LETTER) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, key);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				RecallEmailAutoLoginModel model = new RecallEmailAutoLoginModel();
				model.setUserid(rs.getInt("userid"));
				model.setLoginKey(rs.getString("login_key"));
				model.setInsertTime(rs.getDate("insert_time"));
				model.setOverTime(rs.getInt("is_overtime"));
				return model;
			}
		};
		return (RecallEmailAutoLoginModel) DataAccessMgr.getInstance()
				.queryUnique(op);
	}

	public void updateOverTime(final String key) throws SQLException {
		String sql = "update recall_email_auto_login set is_overtime=1 where login_key=?";
		OpUpdate op = new OpUpdate(sql, DataAccessMgr.BIZ_RROP_LETTER) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, key);
			}

		};
		DataAccessMgr.getInstance().update(op);
	}

	public void insert(final RecallEmailAutoLoginModel model)
			throws SQLException {
		String sql = "insert into recall_email_auto_login set userid=?, login_key=?";
		OpUpdate op = new OpUpdate(sql, DataAccessMgr.BIZ_RROP_LETTER) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setLong(1, model.getUserid());
				ps.setString(2, model.getLoginKey());
			}

		};
		DataAccessMgr.getInstance().update(op);
	}

}
