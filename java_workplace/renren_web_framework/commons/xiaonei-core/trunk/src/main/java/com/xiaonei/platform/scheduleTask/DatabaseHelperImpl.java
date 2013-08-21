package com.xiaonei.platform.scheduleTask;

import java.sql.Connection;

import com.xiaonei.platform.core.opt.DataAccessMgr;

public class DatabaseHelperImpl extends DatabaseHelper {

	public DatabaseHelperImpl() {

	}

	public Connection getConnection() {
		try {
			 return	 DataAccessMgr.getInstance().getConnection(DataAccessMgr.BIZ_STAT,true);
		} catch (Exception e) {
			e.printStackTrace();
		} finally {

		}
		return null;
	}

	public void closeConnection(Connection con) {
		try {
			DataAccessMgr.getInstance().closeConnection(con);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
	}
}
