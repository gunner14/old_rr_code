package com.xiaonei.passport.dao;

import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.core.utility.sched.impl.IpLoginObject;

public class IpLoginDAO {

	private final static IpLoginDAO instance = new IpLoginDAO();

	public static IpLoginDAO getInstance() {
		return instance;
	}

	private IpLoginDAO() {
		super();
	}

	private final static String sqlInsertIpLog = "insert into iplog (id, ip, type, time) values ";

	public int saveBatch(final String values) throws SQLException {
		OpUpdate op = new OpUpdate(sqlInsertIpLog + values,
				DataAccessMgr.BIZ_LOGGING) {

		};
		return DataAccessMgr.getInstance().update(op);// updateForLog(op);
	}

	public int save(final IpLoginObject ilo) throws SQLException {
		return com.xiaonei.platform.core.utility.sched.impl.IpLoginDAO
				.getInstance().save(ilo);
	}
}
