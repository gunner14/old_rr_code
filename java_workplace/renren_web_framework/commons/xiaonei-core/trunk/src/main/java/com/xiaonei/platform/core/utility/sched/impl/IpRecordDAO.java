package com.xiaonei.platform.core.utility.sched.impl;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;

public class IpRecordDAO {

	private static IpRecordDAO instance = new IpRecordDAO();

	public static IpRecordDAO getInstance() {
		if (instance == null) {
			synchronized (IpRecordDAO.class) {
				if (instance == null)
					instance = new IpRecordDAO();
			}
		}
		return instance;
	}

	private IpRecordDAO() {
		super();
	}

	private static final String sqlInsert = "insert into ip_record(user_id, ip, type, event_id, rec_time) values (?, ?, ?, ?, now())";

	public void save(final IpRecordObject iro) throws SQLException {
		OpUpdate op = new OpUpdate(sqlInsert,DataAccessMgr.BIZ_LOGGING) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, iro.getUserId());
				ps.setString(2, iro.getIp());
				ps.setInt(3, iro.getType());
				ps.setLong(4, iro.getEventId());
			}

		};
		DataAccessMgr.getInstance().update(op);// updateForLog(op);
	}

}
