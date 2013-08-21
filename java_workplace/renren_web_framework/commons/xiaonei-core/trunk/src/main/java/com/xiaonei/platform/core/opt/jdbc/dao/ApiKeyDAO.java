package com.xiaonei.platform.core.opt.jdbc.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import com.xiaonei.platform.core.model.ApiKey;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.framework.dao.XnDAO;

public class ApiKeyDAO extends XnDAO{

	private static ApiKeyDAO instance = new ApiKeyDAO();

	public static ApiKeyDAO getInstance() {
		if (instance == null) {
			synchronized (ApiKeyDAO.class) {
				if (instance == null)
					instance = new ApiKeyDAO();
			}
		}
		return instance;
	}

	private ApiKeyDAO() {
		super();
	}

	
	/**
	 * 
	 * @deprecated
	 * @param appId
	 * @return
	 * @throws SQLException
	 */
	public ApiKey get(final int appId)throws SQLException{
		StringBuffer sb = new StringBuffer();
		sb.append("select app_id , api_key from app_basic where app_id =?");
		OpUniq op = new OpUniq(sb.toString(),DataAccessMgr.BIZ_APP) {
		
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, appId);
			}
		
			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseApiKey(rs);
			}
		
		};
		return (ApiKey) DataAccessMgr.getInstance().queryUnique(op);
	}
	
	/**
	 * 
	 * @return
	 * @throws SQLException
	 */
	public List<ApiKey> getAll()throws SQLException{
		StringBuffer sb = new StringBuffer();
		sb.append("select app_id , api_key from app_basic ");
		OpList op = new OpList(sb.toString(),DataAccessMgr.BIZ_APP) {
			
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
			}
		
			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseApiKey(rs);
			}
		
		};
		return (ArrayList<ApiKey>)DataAccessMgr.getInstance().queryList(op);
	}
	
	private ApiKey parseApiKey(ResultSet rs) throws SQLException {
		ApiKey ab = new ApiKey();
		ab.setAppId(rs.getInt("app_id"));
		ab.setApiKey(rs.getString("api_key"));
		return ab;
	}
}
