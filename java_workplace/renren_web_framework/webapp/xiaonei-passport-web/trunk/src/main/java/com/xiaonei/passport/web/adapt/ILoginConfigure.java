package com.xiaonei.passport.web.adapt;

import java.util.List;
import java.util.Map;

import com.xiaonei.passport.constants.LoginConfigureKey;

public interface ILoginConfigure extends com.renren.passport.service.ILoginConfigure{
	String SPLIT_SIGN="&";
	int PARENT_LOGIN_REGISRATION_ID=0;//对于任何一个key,如果所有的登陆都共享,可以直接配置id=0,key=key, value.则所有的登陆都会共享此value
	int DEFAULT_LOGIN_REGISTRATION_ID=1;
	
	/**
	 *
	 * @return 加载所有的登陆配置信息
	 */
	public Map<String, List<String>> findLoginConfigure();
	
	/** 获取某个应用对应的某种策略的值 */
	List<String> getLoginConfigure(int loginRegistration, String key);
	
	/** 获取某个应用对应的某种策略的值 */
	List<String> getLoginConfigure(int loginRegistration, LoginConfigureKey key);
	
	/** 获取某个应用对应的某种策略的值 */
	String getUniqLoginConfigure(int loginRegistration, String key);
	
	/** 获取某个应用对应的某种策略的值 */
	Integer getUniqLoginConfigureForInt(int loginRegistration, String key);
	

	/** 获取某个应用对应的某种策略的值 */
	String getUniqLoginConfigure(int loginRegistration, LoginConfigureKey key);
	
	/** 获取某个应用对应的某种策略的值 */
	Integer getUniqLoginConfigureForInt(int loginRegistration, LoginConfigureKey key);
	
	/** 获取某个应用对应的某种策略的值 */
	String getUniqLoginConfigure(int loginRegistration, LoginConfigureKey key, String defaultValue);
	
	/** 获取某个应用对应的某种策略的值 */
	Integer getUniqLoginConfigureForInt(int loginRegistration, LoginConfigureKey key, Integer defalueValue);

}
