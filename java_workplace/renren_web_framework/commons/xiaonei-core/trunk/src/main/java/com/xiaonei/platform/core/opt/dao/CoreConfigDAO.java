package com.xiaonei.platform.core.opt.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;

/**
 * core保存的一些重要数据
 * @author rongjin.zheng
 *
 */
public class CoreConfigDAO {

	private static CoreConfigDAO instance = new CoreConfigDAO();

	private CoreConfigDAO() {}

	public static CoreConfigDAO getInstance() {
		return instance;
	}

	private static final String TABLE_NAME = "core_config";
	private static final String FIELDS = "ckey, cvalue";
	
	private static final String SELECT_SQL = "select " + FIELDS + " from " + TABLE_NAME + " where ckey = ? ";
	
	/**
	 * 获取core_config表里的key-value值
	 * @param key
	 * @return value
	 * @throws SQLException
	 */
	public String getCoreConfigValue(final String key) throws SQLException {
		OpUniq op = new OpUniq(SELECT_SQL, DataAccessMgr.BIZ_COMMON) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, key);
			}
			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return rs.getString("cvalue");
			}
		};
		return (String)DataAccessMgr.getInstance().queryUnique(op);
	}

}
