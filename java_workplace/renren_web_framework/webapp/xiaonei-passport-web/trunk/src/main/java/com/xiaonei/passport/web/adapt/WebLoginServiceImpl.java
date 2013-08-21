package com.xiaonei.passport.web.adapt;

import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.passport.service.ILoginService;
import com.renren.passport.service.ILoginXoaService;
import com.renren.passport.service.LoginContext;
import com.renren.passport.service.LoginRequest;
import com.renren.passport.service.LoginResponse;
import com.renren.passport.service.LoginType;
import com.renren.passport.service.LogoutResponse;
import com.renren.passport.service.SwitchRequest;
import com.renren.passport.service.TicketInfo;
import com.renren.passport.service.TransferRequest;
import com.renren.xoa2.client.ServiceFactory;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.constants.LogoutStatusCode;

/**
 * WEB端对登录接口的一个实现，其使用了保险模式（即XOA服务不可用时，直接调用本地依赖JAR包）
 * @author shuguo.zhang
 *
 */
public class WebLoginServiceImpl implements ILoginService{
	private ILoginService xoaLoginServce = null;
	private static ILoginService _instance = new WebLoginServiceImpl();
	protected Log logger = LogFactory.getLog(this.getClass());

	private WebLoginServiceImpl() {
	}

	public static ILoginService getInstance() {
		return _instance;
	}
	@Override
	 public int getCaptchaGrade( String account, final String ip){
		try{
			int captchaGrade= getLoginService().getCaptchaGrade(account, ip);	
			return captchaGrade;
		}catch(Exception e){
			logger.error("get captcha grade service error", e);
			return 0;
		}
		
	}
	@Override
	public TicketInfo getPassportInfo(String ticket) {
		try{
			return getLoginService().getPassportInfo(ticket);
		}catch(Exception e){
			logger.error("get passport info service error", e);
			return null;
		}
		
	}
	@Override
	public TicketInfo getTicketInfo(String ticket) {
		try{
			return getLoginService().getTicketInfo(ticket);
		}catch(Exception e){
			logger.error("get ticket info service error", e);
			return null;
		}
		
	}
	@Override
	public LoginResponse login(LoginRequest request,
			LoginContext context) {
		try{
			return getLoginService().login(request, context);
		}catch(Exception e){
			logger.error("login service error", e);
			LoginResponse response= new LoginResponse();
			response.setLoginStatusCode(LoginStatusCode.FAILUNKNOWN.getCode());
			response.setLoginStatusdescription(LoginStatusCode.FAILUNKNOWN.getDescription());
			//response.setLoginStatusdescription(e.getCause().getCause().getMessage());
			return response;
		}
		
	}
	@Override
	public LogoutResponse logout(String passportTicket, String ticket,
			LoginContext context) {
		try{
			return getLoginService().logout(passportTicket, ticket, context);
		}catch(Exception e){
			logger.error("logout service error", e);
			LogoutResponse response=new LogoutResponse();
			response.setLogoutStatusCode(LogoutStatusCode.SYSTEM_EXCEPTION.getCode());
			response.setLogoutStatusdescription(LogoutStatusCode.SYSTEM_EXCEPTION.getDescription());
			return null;
		}
		
	}
	@Override
	public LoginResponse switchAccount(SwitchRequest request, LoginContext context) {
		try{
			return getLoginService().switchAccount(request, context);
		}catch(Exception e){
			logger.error("switchAccount service error", e);
			LoginResponse response= new LoginResponse();
			response.setLoginStatusCode(LoginStatusCode.FAILUNKNOWN.getCode());
			response.setLoginStatusdescription(LoginStatusCode.FAILUNKNOWN.getDescription());
			return response;
		}
		
	}
	
	@Override
	public LoginResponse transfer(TransferRequest request,
			LoginContext context) {
		try{
			return getLoginService().transfer(request, context);
		}catch(Exception e){
			logger.error("transfer service error", e);
			LoginResponse response= new LoginResponse();
			response.setLoginStatusCode(LoginStatusCode.FAILUNKNOWN.getCode());
			response.setLoginStatusdescription(LoginStatusCode.FAILUNKNOWN.getDescription());
			return response;
		}
	}
	
	@Override
	public long verifyPassport(String ticket, LoginType loginLoginType) {
		try{
			return getLoginService().verifyPassport(ticket, loginLoginType);
		}catch(Exception e){
			logger.error("verify passport service error", e);
			return -1;
		}
		
	}
	
	@Override
	public long verifyTicket(String ticket, LoginType loginLoginType) {
		try{
			return getLoginService().verifyTicket(ticket, loginLoginType);
		}catch(Exception e){
			logger.error("verify ticket service error", e);
			return -1;
		}
		
	}
	
	/**
	 * 获取xoa服务，由于xoa2.0也是测试阶段，所以做了一个开关，允许暂时使用jar包的方式调用登陆服务。
	 * 如果xoa服务初始化失败，也将使用jar包的方式保证服务可以使用。
	 * 
	 * 用三个变量，更好控制   by shuguo.zhang
	 */
	private ILoginService getLoginService() {
		//String serviceMode = loginConfigure.getUniqLoginConfigure(1, LoginConfigureKey.USE_VM_SERVICE);
		//String serviceMode = LoginConfigureFactory.getLoginConfigureInstance().getUniqLoginConfigure(1, LoginConfigureKey.USE_VM_SERVICE);
		if(this.xoaLoginServce==null || true){
			try {
				xoaLoginServce=ServiceFactory.getService(ILoginXoaService.class,1000);
			} catch (Exception e) {
				logger.error("init xoaLoginService with ONLY_XOA_MODE failed...", e);
			} 
		}
		return xoaLoginServce;
	}
	
	@Override
	public long verifyPassportForTypes(String ticket,
			List<LoginType> loginLoginType) {
		try{
			return getLoginService().verifyPassportForTypes(ticket, loginLoginType);
		}catch(Exception e){
			logger.error("verify passport service error", e);
			return -1;
		}
	}
	@Override
	public long verifyTicketForTypes(String ticket,
			List<LoginType> loginLoginType) {
		try{
			return getLoginService().verifyTicketForTypes(ticket, loginLoginType);
		}catch(Exception e){
			logger.error("verify ticket service error", e);
			return -1;
		}
	}
	
}
