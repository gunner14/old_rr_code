package com.xiaonei.reg.register.dao;

import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;

public class NetbarIpDAO {
	private static NetbarIpDAO instance = new NetbarIpDAO();

	private NetbarIpDAO() {

	}

	public static NetbarIpDAO getInstance() {
		return instance;
	}

	public int getIp(final String ip) throws SQLException {
		final String sqlGetIp = "select count(ip) from netbar_ip where ip = inet_aton('"
				+ ip + "')";
		OpUniq op = new OpUniq(sqlGetIp, DataAccessMgr.BIZ_STAT) {
		};
		return DataAccessMgr.getInstance().queryInt(op);
	}
}
