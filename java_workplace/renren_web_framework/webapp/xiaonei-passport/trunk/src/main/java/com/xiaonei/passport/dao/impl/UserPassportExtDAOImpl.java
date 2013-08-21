package com.xiaonei.passport.dao.impl;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.dao.UserPassportExtDAO;
import com.xiaonei.passport.model.UserPassportExt;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.xce.XceAdapter;
@Service
public class UserPassportExtDAOImpl implements UserPassportExtDAO {
	private final static Log logger = LogFactory.getLog(UserPassportExtDAOImpl.class);
	@Override
	public UserPassportExt select(final int id) throws SQLException {
		OpUniq op =new OpUniq(UserPassportExtDAO.selectSql, XceAdapter.DB_USER_PASSPORT_EXT){

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, id);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				UserPassportExt up=new UserPassportExt();
				up.setEmail_verify(rs.getString("email_verify"));
				up.setId(rs.getInt("id"));
				up.setVerify_code(rs.getString("verify_code"));
				up.setNew_email(rs.getString("new_email"));
				return up;
			}
			
		};
		try {
			return (UserPassportExt) DataAccessMgr.getInstance().queryUnique(op);
		} catch (SQLException e) {
			logger.error("UserPassportExtDAOImpl execute select error", e);
			return null;
		}
	}

	@Override
	public void updateVerifyCodeById(final String verifyCode, final int id) {
		OpUpdate op =new OpUpdate(UserPassportExtDAO.updateVerifyCodeByIdSql, XceAdapter.DB_USER_PASSPORT_EXT){

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, verifyCode);
				ps.setString(2, verifyCode);
				ps.setInt(3, id);
				ps.setString(4, verifyCode);
			}
			
		};
		try {
			DataAccessMgr.getInstance().update(op);
		} catch (SQLException e) {
			logger.error("UserPassportExtDAOImpl execute updateVerifyCodeById error", e);
		}

	}

	@Override
	public void updateVerifyNewEmail(final String newEmail, final String emailVerify, final int id) {
		OpUpdate op =new OpUpdate(UserPassportExtDAO.updateVerifyNewEmailSql, XceAdapter.DB_USER_PASSPORT_EXT){

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, newEmail);
				ps.setString(2, emailVerify);
				ps.setInt(3, id);
			}
			
		};
		try {
			DataAccessMgr.getInstance().update(op);
		} catch (SQLException e) {
			logger.error("UserPassportExtDAOImpl execute updateVerifyNewEmail error", e);
		}
	}

}
