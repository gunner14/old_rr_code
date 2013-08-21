package com.xiaonei.reg.activity.recallbonus.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.reg.activity.recallbonus.model.RecallBonusAwardModel;


public class RecallBonusAwardDao {
	 public static RecallBonusAwardDao getInstance() {
	        return instance;
	    }

	    private RecallBonusAwardDao() {
	        super();
	    }

	    private static RecallBonusAwardDao instance = new RecallBonusAwardDao();
	    
	    private static final String sqlInsert = "INSERT INTO recall_bonus_award ( uid, award_id, award_remark) VALUES ( ?, ?, ?)";
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
	    
	    private static final String sqlSelectLatest20 = "SELECT id, uid, award_id, award_time, award_remark, award_get_time FROM recall_bonus_award order by id desc limit 20";
	    @SuppressWarnings("unchecked")
		public List<RecallBonusAwardModel> selectLatest20() throws SQLException{
	    	OpList op = new OpList(sqlSelectLatest20, DataAccessMgr.BIZ_STAT){
	    		 @Override
		            public RecallBonusAwardModel parse(ResultSet rs) throws SQLException {
		            	return parseToModel(rs);
		            }
	    	};
			return(List<RecallBonusAwardModel>) DataAccessMgr.getInstance().queryList(op);
	    }
	    
	    private static final String sqlSelectByUid = "SELECT id, uid, award_id, award_time, award_remark, award_get_time FROM recall_bonus_award where uid = ? order by id desc limit 5";
	    @SuppressWarnings("unchecked")
		public List<RecallBonusAwardModel> selectByUid(final int uid)throws SQLException {
	    	OpList op = new OpList(sqlSelectByUid, DataAccessMgr.BIZ_STAT) {
	            @Override
	            public void setParam(PreparedStatement ps) throws SQLException {
	            	ps.setInt(1, uid);
	            }
	            
	            @Override
	            public RecallBonusAwardModel parse(ResultSet rs) throws SQLException {
	            	return parseToModel(rs);
	            }
	        };
	        return (List<RecallBonusAwardModel>)DataAccessMgr.getInstance().queryList(op);
	    }

	    private static final String sqlSelectYesterdayRewards = "select * from recall_bonus_award where datediff(now(), award_time) = ?";
	    @SuppressWarnings("unchecked")
		public List<RecallBonusAwardModel> selectYesterdayRewards(final int yesterjiday)throws SQLException {
	    	OpList op = new OpList(sqlSelectYesterdayRewards, DataAccessMgr.BIZ_STAT) {
	            @Override
	            public void setParam(PreparedStatement ps) throws SQLException {
	            	int para = yesterjiday;
	            	if(yesterjiday <= 0){
	            		para = 0;
	            	}
	            	ps.setInt(1, para);
	            }
	            
	            @Override
	            public RecallBonusAwardModel parse(ResultSet rs) throws SQLException {
	            	return parseToModel(rs);
	            }
	        };
	        return (List<RecallBonusAwardModel>)DataAccessMgr.getInstance().queryList(op);
	    }
	    
		protected RecallBonusAwardModel parseToModel(ResultSet rs) throws SQLException {
			RecallBonusAwardModel model = new RecallBonusAwardModel();
			model.setId(rs.getInt("id"));
			model.setUid(rs.getInt("uid"));
			model.setAward_id(rs.getInt("award_id"));
			model.setAward_time(rs.getString("award_time"));
			model.setAward_remark(rs.getString("award_remark"));
			model.setAward_get_time(rs.getString("award_get_time"));
	        return model;
		}
		
		private static final String sqlSelectYesterdayRewardsWillSend = "select * from recall_bonus_award where datediff(now(), award_time) = ? and award_get_time is null";
	    @SuppressWarnings("unchecked")
		public List<RecallBonusAwardModel> selectYesterdayRewardsWillSend(final int yesterjiday)throws SQLException {
	    	OpList op = new OpList(sqlSelectYesterdayRewardsWillSend, DataAccessMgr.BIZ_STAT) {
	            @Override
	            public void setParam(PreparedStatement ps) throws SQLException {
	            	int para = yesterjiday;
	            	if(yesterjiday <= 0){
	            		para = 0;
	            	}
	            	ps.setInt(1, para);
	            }
	            
	            @Override
	            public RecallBonusAwardModel parse(ResultSet rs) throws SQLException {
	            	return parseToModel(rs);
	            }
	        };
	        return (List<RecallBonusAwardModel>)DataAccessMgr.getInstance().queryList(op);
	    }
	    
	    private static final String sqlSelectYesterdayRewardsWillSendLimit = "select id, uid, award_id, award_time, award_remark, award_get_time from recall_bonus_award where datediff(now(), award_time) = ? and award_get_time is null limit ?";
	    @SuppressWarnings("unchecked")
		public List<RecallBonusAwardModel> selectYesterdayRewardsWillSendWithLimit(final int yesterjiday,final int limit)throws SQLException {
	    	OpList op = new OpList(sqlSelectYesterdayRewardsWillSendLimit, DataAccessMgr.BIZ_STAT) {
	            @Override
	            public void setParam(PreparedStatement ps) throws SQLException {
	            	int para = yesterjiday;
	            	if(yesterjiday <= 0){
	            		para = 0;
	            	}
	            	ps.setInt(1, para);
	            	ps.setInt(2, limit);
	            }
	            
	            @Override
	            public RecallBonusAwardModel parse(ResultSet rs) throws SQLException {
	            	return parseToModel(rs);
	            }
	        };
	        return (List<RecallBonusAwardModel>)DataAccessMgr.getInstance().queryList(op);
	    }
		
		public static final String sqlSendAward = "update recall_bonus_award set award_get_time = now() where id = ? and award_get_time is null";
		public int sendAward(final int id) throws SQLException {
	        OpUpdate op = new OpUpdate(sqlSendAward, DataAccessMgr.BIZ_STAT) {
	            @Override
	            public void setParam(PreparedStatement ps) throws SQLException {
	                ps.setInt(1, id);
	            }
	        };
	        return DataAccessMgr.getInstance().update(op);
	    }
		
		public String getToday0Dian(){
			SimpleDateFormat fmt=new SimpleDateFormat("yyyy-MM-dd 00:00:00");
			Date dt = new Date();
			String str_dt_today0dian = fmt.format(dt);
			return str_dt_today0dian;
		}
		
		public static void main(String[] args) {
			System.out.println(123);
			SimpleDateFormat fmt=new SimpleDateFormat("yyyy-MM-dd 00:00:00");
			Date dt = new Date();
			String str_dt_today0dian = fmt.format(dt);
			System.out.println(str_dt_today0dian);
			
		}
	
}
