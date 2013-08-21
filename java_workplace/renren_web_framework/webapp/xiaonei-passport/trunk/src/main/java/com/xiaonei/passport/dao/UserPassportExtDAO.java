package com.xiaonei.passport.dao;

import java.sql.SQLException;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;
import com.xiaonei.xce.XceAdapter;

@DAO(catalog = XceAdapter.DB_USER_PASSPORT_EXT)
public interface UserPassportExtDAO {
	String selectSql="SELECT id, verify_code, new_email, email_verify FROM  user_passport_ext WHERE id =?";
	String updateVerifyCodeByIdSql="insert into user_passport_ext set verify_code =?, email_verify=?, id=? on DUPLICATE KEY update verify_code =?";
	String updateVerifyNewEmailSql="UPDATE user_passport_ext SET user_passport_ext.new_email =?, user_passport_ext.email_verify =? where user_passport_ext.id=?";
	
	@SQL("SELECT id, verify_code, new_email, email_verify FROM  user_passport_ext WHERE id = :id")
	public com.xiaonei.passport.model.UserPassportExt select(@SQLParam("id")final int id) throws SQLException;

	@SQL("insert into user_passport_ext set verify_code =:verifyCode, email_verify=:verifyCode, id=:id on DUPLICATE KEY update verify_code = :verifyCode ")
	public void updateVerifyCodeById(@SQLParam("verifyCode")final String verifyCode, @SQLParam("id")final int id);

	@SQL("UPDATE user_passport_ext SET user_passport_ext.new_email = :1, user_passport_ext.email_verify = :2 where user_passport_ext.id=:id")
	public void updateVerifyNewEmail(final String newEmail, final String emailVerify, @SQLParam("id")final int id);
}
