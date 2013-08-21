package com.xiaonei.passport.configure.impl;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.passport.configure.ILoginConfigureInsertDao;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;


public class LoginConfigureInsertDaoImpl implements ILoginConfigureInsertDao {
	private final static Log logger = LogFactory.getLog(LoginConfigureInsertDaoImpl.class);
	private static LoginConfigureInsertDaoImpl _instance=new LoginConfigureInsertDaoImpl();
	private LoginConfigureInsertDaoImpl(){
		
	}
	public static LoginConfigureInsertDaoImpl getInstance(){
		return _instance;
	}
	@Override
	public void addLoginConfigure(final int registrationid,final String code,final String value,final int order) {		
			OpUpdate op =new OpUpdate(ILoginConfigureInsertDao.addSql, ILoginConfigureInsertDao.DATA_SOURCE){
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, registrationid);
				ps.setString(2, code);
				ps.setString(3, value);
				ps.setInt(4, order);
			}
			
		};
	
		try {
			DataAccessMgr.getInstance().insert(op);
		} catch (SQLException e) {
			System.out.println("Login Sql Exception " + e);
			logger.error("LoginConfigureInsertDaoImpl execute add error", e);
		}
	}
	
	@Override
	public void updateLoginConfigure(final int registrationid,final String code,final String value,final int order, final int id) {		
			OpUpdate op =new OpUpdate(ILoginConfigureInsertDao.updateSql, ILoginConfigureInsertDao.DATA_SOURCE){
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, registrationid);
				ps.setString(2, code);
				ps.setString(3, value);
				ps.setInt(4, order);
				ps.setInt(5, id);
			}
		};
	
		try {
			DataAccessMgr.getInstance().update(op);
		} catch (SQLException e) {
			System.out.println("Update Sql Exception " + e);
			logger.error("LoginConfigureInsertDaoImpl execute add error", e);
		}
	}
	
	public static void main(String[] args) {
		LoginConfigureInsertDaoImpl daoImpl=new LoginConfigureInsertDaoImpl();
		daoImpl.addLoginConfigure(1, "webSwitchSuccessLoginEvent", "switchPassportAndTicketSaveListern", 1);
	}
	
}
