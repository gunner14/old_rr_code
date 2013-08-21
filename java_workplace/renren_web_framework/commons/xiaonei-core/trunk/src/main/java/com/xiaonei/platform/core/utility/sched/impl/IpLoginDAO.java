package com.xiaonei.platform.core.utility.sched.impl;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;

public class IpLoginDAO  {

	private static IpLoginDAO instance = new IpLoginDAO();

	public static IpLoginDAO getInstance() {
		if (instance == null) {
			synchronized (IpLoginDAO.class) {
				if (instance == null)
					instance = new IpLoginDAO();
			}
		}
		return instance;
	}

	private IpLoginDAO() {
		super();
	}

	private final static String sqlInsertIpLog = "insert into iplog (id, ip, type, time) values (?, ?, ?, now())";

	public int save(final IpLoginObject ilo) throws SQLException {
		OpUpdate op = new OpUpdate(sqlInsertIpLog,DataAccessMgr.BIZ_LOGGING) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, ilo.getUserId());
				ps.setString(2, ilo.getIp());
				ps.setInt(3, ilo.getType());
			}
		};
		return DataAccessMgr.getInstance().update(op);// updateForLog(op);
	}

}
