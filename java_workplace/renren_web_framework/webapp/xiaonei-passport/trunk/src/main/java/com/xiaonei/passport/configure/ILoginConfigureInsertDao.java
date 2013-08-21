package com.xiaonei.passport.configure;


public interface ILoginConfigureInsertDao {
	String addSql="INSERT INTO login_configure(login_registration_id,code,value,login_configure.order) VALUES (?,?,?,?)";
	String updateSql="UPDATE login_configure SET login_registration_id=?, code=?, value=?, login_configure.order=? where id=?";
	String DATA_SOURCE="user_accounts";
	
	/**
	 *
	 * @return 添加RL登陆流程的实现
	 */
	public void addLoginConfigure( final int registrationid,final String code,final String value,final int order);
	
	public void updateLoginConfigure(final int registrationid,final String code,final String value,final int order, final int id);
	
}
