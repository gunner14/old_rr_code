package com.xiaonei.platform.core.head;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;

public class HeadMonitorDAO {
	private static HeadMonitorDAO instance = new HeadMonitorDAO();

	public static HeadMonitorDAO getInstance() {
		return instance;
	}

	private HeadMonitorDAO() {
	}

	private final static String sqlInsert = "insert into head_monitor (id, url, pic) values (?, ?, ?)";

	public int insert(final int id, final String url, final byte[] pic)
			throws SQLException {
		OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_ALBUM_INFO) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, id);
				ps.setString(2, url);
				ps.setBytes(3, pic);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}

}
