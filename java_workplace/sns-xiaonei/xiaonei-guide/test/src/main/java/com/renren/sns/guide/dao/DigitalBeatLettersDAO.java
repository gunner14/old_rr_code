package com.renren.sns.guide.dao;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;

public class DigitalBeatLettersDAO {

	private DigitalBeatLettersDAO() {
	}

	private static DigitalBeatLettersDAO instance = null;

	public static synchronized DigitalBeatLettersDAO getInstance() {
		if (instance == null) {
			instance = new DigitalBeatLettersDAO();
		}
		return instance;
	}

	private static String TABLE_NAME = "digital_beat_letters";

	public void insertOrUpdateDigital(final int userId, final int digital) {

		String sql = "INSERT INTO " + TABLE_NAME + " (user_id,digital,letters) VALUES (?,?,0) " +
				"ON DUPLICATE KEY " + "UPDATE digital = if(digital<?,?,digital)";

		OpUpdate op = new OpUpdate(sql, DataAccessMgr.BIZ_ACTIVITY) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userId);
				ps.setInt(2, digital);
				ps.setInt(3, digital);
				ps.setInt(4, digital);
			}
		};
		try {
			DataAccessMgr.getInstance().update(op);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void insertOrUpdateLetters(final int userId, final int letters) {
		String sql = "INSERT INTO " + TABLE_NAME + " (user_id,digital,letters) VALUES (?,0,?) " +
				"ON DUPLICATE KEY " + "UPDATE letters = if(letters<?,?,letters)";
		OpUpdate op = new OpUpdate(sql, DataAccessMgr.BIZ_ACTIVITY) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userId);
				ps.setInt(2, letters);
				ps.setInt(3, letters);
				ps.setInt(4, letters);
			}
		};
		try {
			DataAccessMgr.getInstance().update(op);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
