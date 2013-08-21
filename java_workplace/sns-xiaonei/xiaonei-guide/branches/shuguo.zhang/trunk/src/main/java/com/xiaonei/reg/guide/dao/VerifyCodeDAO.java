package com.xiaonei.reg.guide.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;

public class VerifyCodeDAO {
	

	public static String getVerifyCodeByUserId(final int id){
		String ret = "";
		try {
			ret = _getVerifyCodeByUserId(id);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return ret;
	}

	/**
	 * 验证用户的验证码是否有效
	 */
	private static String _getVerifyCodeByUserId(final int id) throws Exception {
		
		final String sqlGetByIdNVc = "select verify_code from user_passport_ext where id = ? limit 1";
		OpUniq op = new OpUniq(sqlGetByIdNVc, DataAccessMgr.BIZ_USER_COMMON) {
			
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, id);
			}
			
			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return rs.getString("verify_code");
			}
			
		};
		
		return (String) DataAccessMgr.getInstance().queryUnique(op);
	}
	/**
	 * 验证用户的验证码是否有效
	 */
	public static boolean getByIdAndVerifyCode(final int id, final String verifyCode) throws Exception {

		final String sqlGetByIdNVc = "select id from user_passport_ext where id = ? and verify_code = ?";
		OpUniq op = new OpUniq(sqlGetByIdNVc, DataAccessMgr.BIZ_USER_COMMON) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, id);
				ps.setString(2, verifyCode);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return Boolean.TRUE;
			}

		};

		if (DataAccessMgr.getInstance().queryUnique(op, true) == Boolean.TRUE) {
			return true;
		}

		return false;
	}
	
	public static boolean updateUserVerifyKey(final String key,final int uid) {
        
        final String sqlUpdateUserPassportExt = "update user_passport_ext set verify_code=? where id = ?";
        OpUpdate op = new OpUpdate(sqlUpdateUserPassportExt,DataAccessMgr.BIZ_USER_COMMON) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, key);
                ps.setInt(2, uid);
            }
            
        };

        try {
           return DataAccessMgr.getInstance().update(op)>0;
        } catch (Exception e) {
            e.printStackTrace();
        }finally {

        }
        return false;
    }
}
