package com.xiaonei.reg.activity.dogbonus.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.reg.activity.dogbonus.model.DogBonusAwardModel;


public class DogBonusAwardDao {
	 public static DogBonusAwardDao getInstance() {
	        return instance;
	    }

	    private DogBonusAwardDao() {
	        super();
	    }

	    private static DogBonusAwardDao instance = new DogBonusAwardDao();
	    
	    private static final String sqlInsert = "INSERT INTO dog_bonus_award ( uid, award_id, award_remark) VALUES ( ?, ?, ?)";
	    public int insertRecord(final int uid, final int award_id, final String award_remark) throws SQLException {
	        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {
	            @Override
	            public void setParam(PreparedStatement ps) throws SQLException {
	                ps.setInt(1, uid);
	                ps.setInt(2, award_id);
	                ps.setString(3, award_remark);
	            }
	        };
	        return DataAccessMgr.getInstance().update(op);
	    }
	    
	    private static final String sqlSelectLatest20 = "SELECT id, uid, award_id, award_time, award_remark, award_get_time FROM dog_bonus_award order by id desc limit 20";
	    @SuppressWarnings("unchecked")
		public List<DogBonusAwardModel> selectLatest20() throws SQLException{
	    	OpList op = new OpList(sqlSelectLatest20, DataAccessMgr.BIZ_STAT){
	    		 @Override
		            public DogBonusAwardModel parse(ResultSet rs) throws SQLException {
		            	return parseToModel(rs);
		            }
	    	};
			return(List<DogBonusAwardModel>) DataAccessMgr.getInstance().queryList(op);
	    }
	    
	    private static final String sqlSelectByUid = "SELECT id, uid, award_id, award_time, award_remark, award_get_time FROM dog_bonus_award where uid = ? order by id desc limit 5";
	    @SuppressWarnings("unchecked")
		public List<DogBonusAwardModel> selectByUid(final int uid)throws SQLException {
	    	OpList op = new OpList(sqlSelectByUid, DataAccessMgr.BIZ_STAT) {
	            @Override
	            public void setParam(PreparedStatement ps) throws SQLException {
	            	ps.setInt(1, uid);
	            }
	            
	            @Override
	            public DogBonusAwardModel parse(ResultSet rs) throws SQLException {
	            	return parseToModel(rs);
	            }
	        };
	        return (List<DogBonusAwardModel>)DataAccessMgr.getInstance().queryList(op);
	    }

		protected DogBonusAwardModel parseToModel(ResultSet rs) throws SQLException {
			DogBonusAwardModel model = new DogBonusAwardModel();
			model.setId(rs.getInt("id"));
			model.setUid(rs.getInt("uid"));
			model.setAward_id(rs.getInt("award_id"));
			model.setAward_time(rs.getString("award_time"));
			model.setAward_remark(rs.getString("award_remark"));
	        return model;
		}
	
}
