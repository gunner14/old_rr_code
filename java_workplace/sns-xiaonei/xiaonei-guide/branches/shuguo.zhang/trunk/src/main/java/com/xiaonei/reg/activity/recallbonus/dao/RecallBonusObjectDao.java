package com.xiaonei.reg.activity.recallbonus.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.reg.activity.recallbonus.model.RecallBonusObjectModel;

public class RecallBonusObjectDao {
	public static RecallBonusObjectDao getInstance() {
		return instance;
	}

	private RecallBonusObjectDao() {
		super();
	}

	private static RecallBonusObjectDao instance = new RecallBonusObjectDao();


	/*private static final String sqlSelectForUpdate = "SELECT id, bonus_id, bonus_name,bonus_cost, bonus_rate,bonus_count,bonus_max from recall_bonus_object where bonus_id = ? limit 1 FOR UPDATE";
	public RecallBonusAccountModel selectByUidForUpdate(final int uid) throws SQLException {
		OpUniq op = new OpUniq(sqlSelectForUpdate, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, uid);
			}

			@Override
			public RecallBonusModel parse(ResultSet rs) throws SQLException {
				return parseToModel(rs);
			}
		};
		return (RecallBonusAccountModel) DataAccessMgr.getInstance().queryUnique(op);
	}*/
	
	private static final String sqlUpdate = "UPDATE recall_bonus_object SET bonus_count=bonus_count+1 WHERE id = ? and bonus_count<bonus_max";
	public int increase(final int bid) throws SQLException {
		OpUpdate op = new OpUpdate(sqlUpdate, DataAccessMgr.BIZ_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, bid);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}

	
	private static final String sqlSelect = "SELECT id, bonus_id, bonus_name,bonus_cost, bonus_rate,bonus_count,bonus_max from recall_bonus_object limit 50";
	@SuppressWarnings("unchecked")
	public List<RecallBonusObjectModel> select()throws SQLException {
		OpList op = new OpList(sqlSelect, DataAccessMgr.BIZ_STAT){
			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseToModel(rs);
			}
			
		};
		return (List<RecallBonusObjectModel>)DataAccessMgr.getInstance().queryList(op);
		
	}
	
	private static final String sqlSelectById = "SELECT id, bonus_id, bonus_name,bonus_cost, bonus_rate,bonus_count,bonus_max from recall_bonus_object where bonus_id = ? limit 1";
	public RecallBonusObjectModel selectByBonusid(final int bid) throws SQLException {
		OpUniq op = new OpUniq(sqlSelectById, DataAccessMgr.BIZ_STAT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, bid);
			}

			@Override
			public RecallBonusObjectModel parse(ResultSet rs) throws SQLException {
				return parseToModel(rs);
			}
		};
		return (RecallBonusObjectModel) DataAccessMgr.getInstance().queryUnique(op);
	}

	protected RecallBonusObjectModel parseToModel(ResultSet rs) throws SQLException {
		RecallBonusObjectModel model = new RecallBonusObjectModel();
        model.setId(rs.getInt("id"));
        model.setBonus_id(rs.getInt("bonus_id"));
        model.setBonus_name(rs.getString("bonus_name"));
        model.setBonus_cost(rs.getInt("bonus_cost"));
        model.setBonus_rate(rs.getInt("bonus_rate"));
        model.setBonus_count(rs.getInt("bonus_count"));
        model.setBonus_max(rs.getInt("bonus_max"));
        return model;
	}


}
