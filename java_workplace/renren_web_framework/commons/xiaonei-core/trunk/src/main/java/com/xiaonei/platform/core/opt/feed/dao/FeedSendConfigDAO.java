package com.xiaonei.platform.core.opt.feed.dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.core.opt.feed.model.FeedSendConfig;
import com.xiaonei.platform.framework.dao.XnDAO;

public class FeedSendConfigDAO extends XnDAO{

	private static FeedSendConfigDAO instance = new FeedSendConfigDAO();

	public static FeedSendConfigDAO getInstance() {
		return instance;
	}

	private FeedSendConfigDAO() {
		super();
	}
	private static final String fields = " userid , type ";
	private static final String tableName = "feed_send_config";
	
	private final String RETRIEVE_FEED_CONFIG = "select * from feed_send_config where userid=?";
	@SuppressWarnings("unchecked")
	public List<FeedSendConfig> getFeedSendConfig(final int userID){
		try {
			OpList op = new OpList(RETRIEVE_FEED_CONFIG, DataAccessMgr.BIZ_USER_CONFIG) {
				@Override
				public FeedSendConfig parse(ResultSet rs) throws SQLException {
					return parseBean(rs);
				}
				@Override
				public void setParam(PreparedStatement ps) throws SQLException {
					ps.setInt(1, userID);
				}
			};
			return (List<FeedSendConfig>) DataAccessMgr.getInstance().queryList(op);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return null;
	}

	private FeedSendConfig parseBean(ResultSet rs) throws SQLException{
		FeedSendConfig config = new FeedSendConfig();
		config.setUserid(rs.getInt("userid"));
		config.setType(rs.getInt("type"));
		config.setMinifeed(rs.getInt("mini_feed"));
		config.setNewsfeed(rs.getInt("news_feed"));
		return config;
	}

	/**
	 * 增加一条记录
	 * @param userId
	 * @param appId
	 * @return
	 * @throws SQLException
	 */
	public int save(final int userId , final int appId )throws SQLException{
		StringBuffer sb = new StringBuffer();
		sb.append(" insert ignore into ");
		sb.append( tableName);
		sb.append(" (");
		sb.append(fields);
		sb.append(" )values(? ,? )");
		OpUpdate op = new OpUpdate(sb.toString(),DataAccessMgr.BIZ_USER_CONFIG) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userId);
				ps.setInt(2, appId);
			}
		};
		return DataAccessMgr.getInstance().insertReturnId(op);
	}
	public int[] save(final int userId , final Integer[] ids )throws SQLException{
		if(ids==null || ids.length == 0)
			return null;
		Connection conn = null;
		Statement stat = null;
		try{
			conn = DataAccessMgr.getInstance().getConnection(DataAccessMgr.BIZ_USER_CONFIG, false);
			stat = conn.createStatement();
			for (Integer i :ids) {
				StringBuffer sb = new StringBuffer();
				sb.append(" insert ignore into ");
				sb.append( tableName);
				sb.append(" (");
				sb.append(fields);
				sb.append(" )values(");
				sb.append(userId);
				sb.append(",");
				sb.append(i);
				sb.append(")");
				stat.addBatch(sb.toString());
			}
			return stat.executeBatch();
		}catch(Exception e){
			e.printStackTrace(System.err);
		}finally{
			if(conn!=null){
				try{
					conn.close();
				}catch(Exception e){
					
				}
			}
			if(stat!=null){
				try{
					stat.close();
				}catch(Exception e){
					
				}
			}
		}
		return null;
	}
	/**
	 * 
	 * @param userId
	 * @param appId
	 * @return
	 * @throws SQLException
	 */
	public int delete(final int userId,final int appId)throws SQLException{
		StringBuffer sb = new StringBuffer();
		sb.append(" delete from ");
		sb.append(tableName);
		sb.append(" where userid=? and type=? ");
		
		OpUpdate op = new OpUpdate(sb.toString(),DataAccessMgr.BIZ_USER_CONFIG) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userId);
				ps.setInt(2, appId);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}

	public int delete(final int userId,final String[] ids)throws SQLException{
		if(ids==null || ids.length == 0){
			return 0;
		}
		StringBuffer sb = new StringBuffer();
		StringBuilder builder = new StringBuilder();
		String sIds = "";
		for(String i : ids){
			builder.append(i+",");
		}
		if(builder.length()>0){
			sIds = builder.substring(0,builder.length()-1);
		}
		sb.append(" delete from ");
		sb.append(tableName);
		sb.append(" where userid=? and type in ("+sIds+") ");
		OpUpdate op = new OpUpdate(sb.toString(),DataAccessMgr.BIZ_USER_CONFIG) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userId);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
}
