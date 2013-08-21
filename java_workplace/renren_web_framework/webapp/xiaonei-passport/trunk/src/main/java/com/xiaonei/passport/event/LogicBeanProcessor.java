package com.xiaonei.passport.event;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.LoginLogicFactory;
import com.xiaonei.passport.configure.ILoginConfigure;
import com.xiaonei.passport.configure.LoginConfigureFactory;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.constants.LogoutStatusCode;
import com.xiaonei.passport.logger.LoginLoggerDef;
import com.xiaonei.passport.logger.PpLogger;
import com.xiaonei.passport.model.LoginResult;
import com.xiaonei.passport.model.LogoutResult;

public class LogicBeanProcessor {
	protected static Log logger = LogFactory.getLog(LogicBeanProcessor.class);
	private static LogicBeanProcessor _instance=new LogicBeanProcessor();
	private static ILoginConfigure loginConfigure = LoginConfigureFactory
	.getLoginConfigureInstance();
	protected static PpLogger remoteLogger = LoginLoggerDef.login;
	private LogicBeanProcessor(){}
	public static LogicBeanProcessor getInstance(){
		return _instance;
	}
	
	
	/**
	 * 登陆前执行逻辑，如验证码验证等。如果出错返回错误码，否则返回正确码
	 * 
	 * @param account
	 *            用户名
	 * @param md5Password
	 *            密码
	 * @param site
	 *            域名
	 * @param loginRegistrationId
	 *            登陆类型
	 * @param ip
	 *            用户最终ip
	 * @param context
	 *            上下文环境
	 * @return
	 */
	public static LoginStatusCode processLogicBeforeLogin(
			ILoginParameter loginParameter, String key) {
		List<String> beanNames = loginConfigure.getLoginConfigure(
				loginParameter.getLoginRegistrationId(),
				key);
		if ((beanNames == null || beanNames.size() == 0)) {
			if(logger.isWarnEnabled()){
				StringBuffer sb = new StringBuffer();
				sb.append("the user which  loginRegistrationId is ");
				sb.append(loginParameter.getLoginRegistrationId());
				sb.append(" and key is ").append(key);
				sb.append(" does not config logic before login bean. System has skipt this step");
				logger.warn(sb.toString());
			}
			return LoginStatusCode.LOGIN_OK;
		}
		Class<ILogicBeforeLogin> clazz = ILogicBeforeLogin.class;
		for (String beanName : beanNames) {
			try{
				Object object = LoginLogicFactory.getBean(beanName);
				if (object != null && clazz.isInstance(object)) {
					ILogicBeforeLogin logic = (ILogicBeforeLogin) object;
					LoginStatusCode rt = logic.process(loginParameter);
					if (rt != LoginStatusCode.LOGIN_OK) {
						return rt;
					}
				}
			}catch(Exception e){
				logger.error("execute logice before login error, the bean name is "+beanName, e);
			}
			
		}
		return LoginStatusCode.LOGIN_OK;
	}
	
	/**
	 * 登陆后执行逻辑，如验证码验证等。如果出错返回错误码，否则返回正确码
	 * 
	 * @param account
	 *            用户名
	 * @param md5Password
	 *            密码
	 * @param site
	 *            域名
	 * @param loginRegistrationId
	 *            登陆类型
	 * @param ip
	 *            用户最终ip
	 * @param context
	 *            上下文环境
	 * @return
	 */
	public static LoginStatusCode processLogicAfterLogin(LoginResult result,
			ILoginParameter loginParameter, String loginAfterKey) {
		List<String> beanNames = loginConfigure.getLoginConfigure(
				loginParameter.getLoginRegistrationId(), loginAfterKey);
		Class<ILogicAfterLogin> clazz = ILogicAfterLogin.class;
		if ((beanNames == null || beanNames.size() == 0)) {
			if(logger.isWarnEnabled()){
				StringBuffer sb = new StringBuffer();
				sb.append("the user which  loginRegistrationId is ");
				sb.append(loginParameter.getLoginRegistrationId());
				sb.append(" does not config logic after login success bean. System has skipt this step");
				logger.warn(sb.toString());
			}
			beanNames=new ArrayList<String>();
		}
		
		for (String beanName : beanNames) {
			try{
				
				Object object = LoginLogicFactory.getBean(beanName);
				
				
				if (object != null && clazz.isInstance(object)) {
					ILogicAfterLogin logic = (ILogicAfterLogin) object;
					
					LoginStatusCode rt = logic.process(result, loginParameter);
					
					if (LoginStatusCode.LOGIN_OK != rt) {
						logger.error("bean logice after login error, the bean name is "+beanName);
						return rt;
					}
				}
			}catch(Exception e){
				logger.error("execute logice after login success, the bean name is "+beanName, e);
			}
			
		}
		ILoginContext context= loginParameter.getLogincontext();	
		if(remoteLogger.isInfoEnabled()){
			remoteLogger.info(LoginLoggerDef.loginLogPentter, "loginOk", result.getUserId(), context==null?null:context.getReferer(), loginParameter.getUserName(),context==null?null:context.getIp(), loginParameter.getLoginRegistrationId());
		}
		return LoginStatusCode.LOGIN_OK;
	}

	/**
	 * 当登录失败时，返回错误码，且记录日志
	 * 
	 * @param code
	 * @param account
	 *            用户名
	 * @param md5Password
	 *            密码
	 * @param site
	 *            域名
	 * @param loginRegistrationId
	 *            登陆类型
	 * @param ip
	 *            用户最终ip
	 * @param context
	 *            上下文环境
	 * @return
	 */
	public static LoginResult processLogicAfterError(LoginStatusCode code,
			ILoginParameter loginParameter, String loginLogicKey) {
		return processLogicAfterError(new LoginResult(code, loginParameter
				.getUserName(),loginParameter.getDomain()), loginParameter, loginLogicKey);
	}

	/**
	 * 当登录失败时，返回错误码，且记录日志
	 * 
	 * @param code
	 * @param account
	 *            用户名
	 * @param md5Password
	 *            密码
	 * @param site
	 *            域名
	 * @param loginRegistrationId
	 *            登陆类型
	 * @param ip
	 *            用户最终ip
	 * @param context
	 *            上下文环境
	 * @return
	 */
	public static LoginResult processLogicAfterError(LoginResult result,
			ILoginParameter loginParameter, String loginFailKey) {
		ILoginContext context = loginParameter.getLogincontext();
		if (remoteLogger.isEnable()) {
			remoteLogger.error(String.format(LoginLoggerDef.loginLogPentter,
					result.getCode().name(), loginParameter.getUserName(), context
							.getReferer(), loginParameter.getUserName(),
					context.getIp(), loginParameter.getLoginRegistrationId()));
		}
		List<String> beanNames = loginConfigure.getLoginConfigure(
				loginParameter.getLoginRegistrationId(), loginFailKey);
		if ((beanNames == null || beanNames.size() == 0)) {
			if(logger.isWarnEnabled()){
				StringBuffer sb = new StringBuffer();
				sb.append("the user which  loginRegistrationId is ");
				sb.append(loginParameter.getLoginRegistrationId());
				sb.append(" does not config logic after login error bean. System has skipt this step");
				logger.warn(sb.toString());
			}
			return result;
		}
		Class<ILoginAfterLoginFail> clazz = ILoginAfterLoginFail.class;
		for (String beanName : beanNames) {
			try{
				Object object = LoginLogicFactory.getBean(beanName);
				if (object != null && clazz.isInstance(object)) {
					ILoginAfterLoginFail logic = (ILoginAfterLoginFail) object;
					if (logic.canProcessLoginStatusCode(result.getCode())) {
						logic.process(result, loginParameter);
					}
				}
			}catch(Exception e){
				logger.error("execute logice before login error, the bean name is "+beanName, e);
			}
			
		}

		return result;
	}

	/**
	 * 返回注销时的结果
	 * 
	 * @param code
	 *            注销代码
	 * @return
	 */
	public static LogoutResult processLogicAfterLogout(LogoutStatusCode code, int userId) {
		if (remoteLogger.isEnable()) {
			if(LogoutStatusCode.LOGOUT_OK==code){
				LoginLoggerDef.logout.info(LoginLoggerDef.loginLogPentter, "Logout Succ!!!",
	                    userId, "", "", "", "");
			}else{
				LoginLoggerDef.logout.info(LoginLoggerDef.loginLogPentter, "Logout fail!!!",
	                    userId, "", "", "", "");
			}
			
		}
		return new LogoutResult(code);
	}
}
