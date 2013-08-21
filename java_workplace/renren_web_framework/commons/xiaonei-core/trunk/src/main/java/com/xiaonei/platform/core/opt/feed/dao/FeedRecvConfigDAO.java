package com.xiaonei.platform.core.opt.feed.dao;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;

public class FeedRecvConfigDAO extends XnDAO{

	private static FeedRecvConfigDAO instance = new FeedRecvConfigDAO();

	public static FeedRecvConfigDAO getInstance() {
		if (instance == null) {
			synchronized (FeedRecvConfigDAO.class) {
				if (instance == null)
					instance = new FeedRecvConfigDAO();
			}
		}
		return instance;
	}

	private FeedRecvConfigDAO() {
		super();
	}
	
	public static String fields = " userid , type ";
	public static String tableName = "feed_recv_config";
	


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
}
