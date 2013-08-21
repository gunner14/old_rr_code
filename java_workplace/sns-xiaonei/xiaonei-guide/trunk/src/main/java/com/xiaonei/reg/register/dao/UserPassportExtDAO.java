package com.xiaonei.reg.register.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.reg.register.model.UserPassportExt;
import com.xiaonei.xce.XceAdapter;

public class UserPassportExtDAO {
	public static UserPassportExtDAO getInstance() {
		return instance;
	}

	private UserPassportExtDAO() {
		super();
	}

	private static UserPassportExtDAO instance = new UserPassportExtDAO();

	public UserPassportExt select(final UserPassportExt m) throws SQLException {

		String sql = "SELECT user_passport_ext.id, user_passport_ext.verify_code, user_passport_ext.new_email, user_passport_ext.email_verify FROM  user_passport_ext WHERE user_passport_ext.id = ?";
		OpUniq op = new OpUniq(sql, XceAdapter.DB_USER_PASSPORT_EXT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, m.getId());
			}

			@Override
			public UserPassportExt parse(ResultSet rs) throws SQLException {
				return parseUserPassportExt(rs);
			}
		};
		return (UserPassportExt) DataAccessMgr.getInstance().queryUnique(op);
	}

	protected UserPassportExt parseUserPassportExt(ResultSet rs)
			throws SQLException {
		UserPassportExt up = new UserPassportExt();
		up.setId(rs.getInt("id"));
		up.setVerify_code(rs.getString("verify_code"));
		up.setNew_email(rs.getString("new_email"));
		up.setEmail_verify(rs.getString("email_verify"));
		return up;
	}

	public int updateVerifyCodeById(final UserPassportExt upe)
			throws SQLException {
		String sql = "insert into user_passport_ext set verify_code = ?,email_verify=?, id=? on DUPLICATE KEY update verify_code = ? ";
		OpUpdate op = new OpUpdate(sql, XceAdapter.DB_USER_PASSPORT_EXT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, upe.getVerify_code());
				ps.setString(2, upe.getVerify_code());
				ps.setInt(3, upe.getId());
				ps.setString(4, upe.getVerify_code());
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}

	public int updateVerifyNewEmail(final UserPassportExt upe)
			throws SQLException {
		String sql = "UPDATE user_passport_ext SET user_passport_ext.new_email = ?, user_passport_ext.email_verify = ? where user_passport_ext.id=?";
		OpUpdate op = new OpUpdate(sql, XceAdapter.DB_USER_PASSPORT_EXT) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, upe.getNew_email());
				ps.setString(2, upe.getEmail_verify());
				ps.setInt(3, upe.getId());
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}

}
