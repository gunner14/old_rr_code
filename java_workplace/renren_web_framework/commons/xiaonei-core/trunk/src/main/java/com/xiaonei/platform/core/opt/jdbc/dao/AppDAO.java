package com.xiaonei.platform.core.opt.jdbc.dao;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.framework.dao.XnDAO;

/**
 * 此DAO
 * @author fusong.li@dudu-inc.com
 * 2008-5-24 上午01:12:33
 */
public class AppDAO extends XnDAO{

	private static AppDAO instance = new AppDAO();

	public static AppDAO getInstance() {
		if (instance == null) {
			synchronized (AppDAO.class) {
				if (instance == null)
					instance = new AppDAO();
			}
		}
		return instance;
	}

	private AppDAO() {
		super();
	}

	
	/**
	 * 判断user_app_config是否有用户记录，是判断用户是否安装过应用的其中一个条件
	 * @param userId
	 * @return
	 * @throws SQLException
	 */
	public boolean hasUserAppConfig(final int userId)throws SQLException{
		StringBuffer sb = new StringBuffer();
		sb.append("select user_id from user_app_config where user_id=?");
		OpUniq op = new OpUniq(sb.toString(),DataAccessMgr.BIZ_APP) {
		
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userId);
			}
		
		};
		return DataAccessMgr.getInstance().queryExist(op);
	}
	
	/**
	 * 判断app_user是否有用户记录，是判断用户是否安装过应用的其中一个条件
	 * @return
	 * @throws SQLException
	 */
	public boolean hasAppUser(final int userId)throws SQLException{
		StringBuffer sb = new StringBuffer();
		sb.append("select app_id from app_user where user_id=?");
		OpUniq op = new OpUniq(sb.toString(),DataAccessMgr.BIZ_APP) {
			
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userId);
			}
		
		};
		return DataAccessMgr.getInstance().queryExist(op);
	}
}
