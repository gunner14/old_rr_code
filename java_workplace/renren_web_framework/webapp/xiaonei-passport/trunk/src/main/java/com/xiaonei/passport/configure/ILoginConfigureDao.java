package com.xiaonei.passport.configure;

import java.util.List;
import java.util.Map;

import com.xiaonei.passport.model.LoginRegistration;

public interface ILoginConfigureDao {
	String selectLoginRegistrationSQL="select registration.id as id , registration.name as name,  registration.create_time as create_time from login_registration registration";
	String selectLoginConfigureSQL="select configure.login_registration_id as login_registration_id , configure.code as configure_key, configure.value as configure_value,configure.order as configure_order from login_configure configure";
	String DATA_SOURCE="user_accounts";
	
	/**
	 *
	 * @return 加载所有的登陆注册信息
	 */
	public List<LoginRegistration> findLoginRegistration();
	
	/**
	 *
	 * @return 加载所有的登陆配置信息
	 */
	public Map<String, List<String>> findLoginConfigure();
}
