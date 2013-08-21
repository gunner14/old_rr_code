package com.xiaonei.passport.login;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.passport.configure.ILoginConfigure;
import com.xiaonei.passport.configure.LoginConfigureFactory;
import com.xiaonei.passport.constants.LoginConfigureKey;


public class CommonLoginServiceFactory {
	private static ILoginConfigure loginConfigure=LoginConfigureFactory.getLoginConfigureInstance();
	private static Log logger=LogFactory.getLog(CommonLoginServiceFactory.class);
	
	public static ICommonLoginService getInstance(){
		
		String onlyNewPassport=loginConfigure.getUniqLoginConfigure(1, LoginConfigureKey.ONLY_NEW_PASSPORT);
		
		
		if(logger.isDebugEnabled()){
			logger.debug("loaddata loginconfigure--key: onlyNewPassport, registrationId: 1, result: "+ onlyNewPassport);
		}
		
		if(StringUtils.isEmpty(onlyNewPassport)){
			onlyNewPassport="false";
		}
		if("true".equals(onlyNewPassport)){
			return CommonLoginServiceImpl.getInstance();
		}
		return CompatibleLoginServiceImpl.getInstance();
	}
}
