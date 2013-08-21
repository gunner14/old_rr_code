package com.xiaonei.reg.activity.recallbonus.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.reg.activity.recallbonus.model.RecallBonusAccountRecordModel;


public class RecallBonusAccountRecordDao {
	 public static RecallBonusAccountRecordDao getInstance() {
	        return instance;
	    }

	    private RecallBonusAccountRecordDao() {
	        super();
	    }

	    private static RecallBonusAccountRecordDao instance = new RecallBonusAccountRecordDao();
	    
	    private static final String sqlInsert = "INSERT INTO recall_bonus_account_record ( uid, act_type, act_amount) VALUES ( ?, ?, ?)";
	    public int insertRecord(final int uid, final int act_type, final int act_amount) throws SQLException {
	        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {
	            @Override
	            public void setParam(PreparedStatement ps) throws SQLException {
	                ps.setInt(1, uid);
	                ps.setInt(2, act_type);
	                ps.setInt(3, act_amount);
	            }
	        };
	        return DataAccessMgr.getInstance().update(op);
	    }
	    
	    private static final String sqlSelectById = "SELECT id, uid, act_type, act_amount, act_time from recall_bonus_account_record where uid = ?";
	    @SuppressWarnings("unchecked")
		public List<RecallBonusAccountRecordModel> selecById(final int uid)throws SQLException {
	    	 OpList op = new OpList(sqlSelectById, DataAccessMgr.BIZ_STAT) {
	            @Override
	            public void setParam(PreparedStatement ps) throws SQLException {
	            	ps.setInt(1, uid);
	            }
	            
	            @Override
	            public RecallBonusAccountRecordModel parse(ResultSet rs) throws SQLException {
	            	return parseToModel(rs);
	            }
	        };
	        return (List<RecallBonusAccountRecordModel>)DataAccessMgr.getInstance().queryList(op);
	    }
	    
	    private static final String sqlSelectDailyRecordByFlag = "SELECT id, uid, act_type, act_amount, act_time from recall_bonus_account_record where uid = ? and act_type = ? and datediff(now(), act_time) = 0 limit 1";
	    public RecallBonusAccountRecordModel selectDailyRecordByType(final int uid, final int type)throws SQLException {
	    	 OpUniq op = new OpUniq(sqlSelectDailyRecordByFlag, DataAccessMgr.BIZ_STAT) {
	            @Override
	            public void setParam(PreparedStatement ps) throws SQLException {
	            	ps.setInt(1, uid);
	            	ps.setInt(2, type);
	            }
	            
	            @Override
	            public RecallBonusAccountRecordModel parse(ResultSet rs) throws SQLException {
	            	return parseToModel(rs);
	            }
	        };
	        return (RecallBonusAccountRecordModel)DataAccessMgr.getInstance().queryUnique(op);
	    }

	    
	    private static final String sqlSelectDailyIncreasement = "SELECT id, uid, act_type, act_amount, act_time from recall_bonus_account_record where uid = ? and act_type = 1 and datediff(now(), act_time) = 0 limit 1";
	    public RecallBonusAccountRecordModel selectDailyRecord(final int uid)throws SQLException {
	    	 OpUniq op = new OpUniq(sqlSelectDailyIncreasement, DataAccessMgr.BIZ_STAT) {
	            @Override
	            public void setParam(PreparedStatement ps) throws SQLException {
	            	ps.setInt(1, uid);
	            }
	            
	            @Override
	            public RecallBonusAccountRecordModel parse(ResultSet rs) throws SQLException {
	            	return parseToModel(rs);
	            }
	        };
	        return (RecallBonusAccountRecordModel)DataAccessMgr.getInstance().queryUnique(op);
	    }

		protected RecallBonusAccountRecordModel parseToModel(ResultSet rs) throws SQLException {
			RecallBonusAccountRecordModel model = new RecallBonusAccountRecordModel();
	        model.setId(rs.getInt("id"));
	        model.setUid(rs.getInt("uid"));
	        model.setAct_type(rs.getInt("act_type"));
	        model.setAct_time(rs.getString("act_time"));
	        model.setAct_amount(rs.getInt("act_amount"));
	        return model;
		}
	
}
