package com.renren.sns.guide.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import com.renren.sns.guide.model.DigitalAndLetters;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;

public class DigitalLettersSumDAO {
	
	private DigitalLettersSumDAO() {
	}

	private static DigitalLettersSumDAO instance = null;

	public static synchronized DigitalLettersSumDAO getInstance() {
		if (instance == null) {
			instance = new DigitalLettersSumDAO();
		}
		return instance;
	}
	
	private static String TABLE_NAME = "digital_letters_sum";

	public DigitalAndLetters getDigitalAndLetters() throws SQLException {
		String sqlSelect = "select * from " + TABLE_NAME + " where id =1";
		OpUniq op = new OpUniq(sqlSelect, DataAccessMgr.BIZ_ACTIVITY) {
            public Object parse(ResultSet rs) throws SQLException {
            	DigitalAndLetters dal = new DigitalAndLetters(rs.getInt("digital_sum"), rs.getInt("letters_sum"));
            	dal.setId(rs.getInt("id"));
                return dal;
            }
        };
        return (DigitalAndLetters)DataAccessMgr.getInstance().queryUnique(op);
	}
	
	public void updateDigital(final int digital) {
		String sqlUpdate = "update " + TABLE_NAME + " set digital_sum=digital_sum+? where id=1";
		OpUpdate op = new OpUpdate(sqlUpdate, DataAccessMgr.BIZ_ACTIVITY) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, digital);								
			}
		};
		try {
			DataAccessMgr.getInstance().update(op);
		} catch (SQLException e) {			
			e.printStackTrace();
		}
	}
	
	public void updateLetters(final int letters) {
		String sqlUpdate = "update " + TABLE_NAME + " set letters_sum=letters_sum+? where id=1";
		OpUpdate op = new OpUpdate(sqlUpdate, DataAccessMgr.BIZ_ACTIVITY) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {				
				ps.setInt(1, letters);				
			}
		};
		try {
			DataAccessMgr.getInstance().update(op);
		} catch (SQLException e) {			
			e.printStackTrace();
		}
	}
}
