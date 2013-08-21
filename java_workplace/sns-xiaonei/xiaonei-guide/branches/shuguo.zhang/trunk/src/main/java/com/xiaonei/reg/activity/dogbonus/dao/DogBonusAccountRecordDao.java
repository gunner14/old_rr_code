package com.xiaonei.reg.activity.dogbonus.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.reg.activity.dogbonus.model.DogBonusAccountRecordModel;


public class DogBonusAccountRecordDao {
	 public static DogBonusAccountRecordDao getInstance() {
	        return instance;
	    }

	    private DogBonusAccountRecordDao() {
	        super();
	    }

	    private static DogBonusAccountRecordDao instance = new DogBonusAccountRecordDao();
	    
	    private static final String sqlInsert = "INSERT INTO dog_bonus_account_record ( uid, act_type, act_amount) VALUES ( ?, ?, ?)";
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
	    
	    private static final String sqlSelectById = "SELECT id, uid, act_type, act_amount, act_time from dog_bonus_account_record where uid = ?";
	    @SuppressWarnings("unchecked")
		public List<DogBonusAccountRecordModel> selecById(final int uid)throws SQLException {
	    	 OpList op = new OpList(sqlSelectById, DataAccessMgr.BIZ_STAT) {
	            @Override
	            public void setParam(PreparedStatement ps) throws SQLException {
	            	ps.setInt(1, uid);
	            }
	            
	            @Override
	            public DogBonusAccountRecordModel parse(ResultSet rs) throws SQLException {
	            	return parseToModel(rs);
	            }
	        };
	        return (List<DogBonusAccountRecordModel>)DataAccessMgr.getInstance().queryList(op);
	    }
	    
	    private static final String sqlSelectDailyIncreasement = "SELECT id, uid, act_type, act_amount, act_time from dog_bonus_account_record where uid = ? and act_type = 1 and datediff(now(), act_time) = 0 limit 1";
	    public DogBonusAccountRecordModel selectDailyRecord(final int uid)throws SQLException {
	    	 OpUniq op = new OpUniq(sqlSelectDailyIncreasement, DataAccessMgr.BIZ_STAT) {
	            @Override
	            public void setParam(PreparedStatement ps) throws SQLException {
	            	ps.setInt(1, uid);
	            }
	            
	            @Override
	            public DogBonusAccountRecordModel parse(ResultSet rs) throws SQLException {
	            	return parseToModel(rs);
	            }
	        };
	        return (DogBonusAccountRecordModel)DataAccessMgr.getInstance().queryUnique(op);
	    }

		protected DogBonusAccountRecordModel parseToModel(ResultSet rs) throws SQLException {
			DogBonusAccountRecordModel model = new DogBonusAccountRecordModel();
	        model.setId(rs.getInt("id"));
	        model.setUid(rs.getInt("uid"));
	        model.setAct_type(rs.getInt("act_type"));
	        model.setAct_time(rs.getString("act_time"));
	        model.setAct_amount(rs.getInt("act_amount"));
	        return model;
		}
	
}
