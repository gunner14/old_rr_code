package com.xiaonei.reg.activity.recallbonus.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.reg.activity.recallbonus.constant.IRecallBonusConst;
import com.xiaonei.reg.activity.recallbonus.model.RecallBonusAccountModel;

public class RecallBonusAccountDao {
	public static RecallBonusAccountDao getInstance() {
		return instance;
	}

	private RecallBonusAccountDao() {
		super();
	}

	private static RecallBonusAccountDao instance = new RecallBonusAccountDao();

	@SuppressWarnings("unused")
	private int save(final int uid, final int current) throws SQLException {
		final String sqlInsert = "INSERT INTO recall_bonus_account (uid, current) VALUES ( ? , ? ) ON DUPLICATE KEY "
				+ "UPDATE current = ( ? ) ";
		OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, uid);
				ps.setInt(2, current);
				ps.setInt(3, current);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}

	private static final String sqlModify = "INSERT INTO recall_bonus_account (uid, current) VALUES ( ? , "+IRecallBonusConst.AMOUNT_DAYLY_INCREMENT+" ) ON DUPLICATE KEY " 
		+ "UPDATE current = current + ?";

	public int increase(final int uid, final int offset) throws SQLException {
		OpUpdate op = new OpUpdate(sqlModify, DataAccessMgr.BIZ_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, uid);
				ps.setInt(2, offset);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}

	private static final String sqlSelect = "SELECT uid, current from recall_bonus_account where uid = ? limit 1";

	public RecallBonusAccountModel selectByUid(final int uid) throws SQLException {
		OpUniq op = new OpUniq(sqlSelect, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, uid);
			}

			@Override
			public RecallBonusAccountModel parse(ResultSet rs) throws SQLException {
				return parseToModel(rs);
			}
		};
		return (RecallBonusAccountModel) DataAccessMgr.getInstance().queryUnique(op);
	}

	protected RecallBonusAccountModel parseToModel(ResultSet rs) throws SQLException {
		RecallBonusAccountModel model = new RecallBonusAccountModel();
        model.setCurrent(rs.getInt("current"));
        model.setUid(rs.getInt("uid"));
        return model;
	}

}
