package com.xiaonei.platform.core.opt.dao;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;


/**
 * 用途是保存一些内网负载均衡设备的IP，以便在获取用户IP的时候做过滤
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-9-1 下午06:56:45
 */
public class NativeIpDAO {

	private static NativeIpDAO instance = new NativeIpDAO();

	private NativeIpDAO() {}

	public static NativeIpDAO getInstance() {
		return instance;
	}

	private static final String TABLE_NAME = "native_ip";
	private static final String FIELDS = "ip";
	
	private static final String SELECT_SQL = "select " + FIELDS + " from "
			+ TABLE_NAME + " limit 1000";

	@SuppressWarnings("unchecked")
	public List<String> getIps() throws SQLException {
		OpList op = new OpList(SELECT_SQL, DataAccessMgr.BIZ_COMMON) {
			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return rs.getString("ip");
			}
		};
		return (List) DataAccessMgr.getInstance().queryList(op);
	}

}
