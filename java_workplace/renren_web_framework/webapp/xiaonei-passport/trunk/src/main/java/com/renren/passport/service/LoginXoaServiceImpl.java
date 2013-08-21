package com.renren.passport.service;

import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.constants.TicketType;
import com.xiaonei.passport.login.CommonLoginServiceFactory;
import com.xiaonei.passport.login.ICommonLoginService;
import com.xiaonei.passport.model.LoginContextWrapper;
import com.xiaonei.passport.model.LoginResult;
import com.xiaonei.passport.model.LogoutResult;
import com.xiaonei.passport.ticket.ITicketService;
import com.xiaonei.passport.ticket.TicketServiceFactory;
import com.xiaonei.passport.util.TypeConvertUtil;

/**
 * 此类是针对XOA定义的接口的一个实现。
 * 但实际应用中，当XOA服务初始化失败时，此类也可以做为一个普通的SERVICE对外提供服务
 * 所以：不要只看类的名字来判断其作用
 * @author shuguo.zhang
 */


public class LoginXoaServiceImpl implements ILoginXoaService{
	
	//为了能在DB实在热切换，这里不要直接实例化loginService
	//private com.xiaonei.passport.login.ICommonLoginService loginService=CommonLoginServiceFactory.getInstance();
	//private com.xiaonei.passport.login.ICommonLoginService loginService ;
	private ITicketService ticketService=TicketServiceFactory.getInstance();
	private static Log logger=LogFactory.getLog(LoginXoaServiceImpl.class);
	
	private com.xiaonei.passport.login.ICommonLoginService getLoginService() {
		//每次都在factory里做一次判断（放心，读取配置的数据库操作已经做了定时）
		
		ICommonLoginService CommonLogin = CommonLoginServiceFactory.getInstance();
		
		return CommonLogin;
	}
	
	@Override
	public int getCaptchaGrade(String account, String ip) {
		if(logger.isDebugEnabled()){
			logger.debug("LoginXoaServiceImpl getCaptchaGrade...");
		}
		return getLoginService().getCaptchaGrade(account, ip);
	}

	@Override
	public TicketInfo getPassportInfo(String ticket) {
		if (logger.isDebugEnabled()) {
			logger.debug("LoginXoaServiceImpl getPassportInfo...");
		}
		com.xiaonei.xce.ticket.TicketInfo info = ticketService.getTicketInfo(ticket, TicketType.PASSPORT);
		return TypeConvertUtil.convertTicketInfo(info);
	}

	@Override
	public TicketInfo getTicketInfo(String ticket) {
		if(logger.isDebugEnabled()){
			logger.debug("LoginXoaServiceImpl getTicketInfo...");
		}
		com.xiaonei.xce.ticket.TicketInfo info=ticketService.getTicketInfo(ticket, TicketType.TICKET);
		return TypeConvertUtil.convertTicketInfo(info);
	}

	@Override
	public LoginResponse login(LoginRequest request, 
			LoginContext context) {
		if(logger.isDebugEnabled()){
			logger.debug("LoginXoaServiceImpl login start...");
		}
		
		if(request==null|| context==null){
			/*LoginResponse response=new LoginResponse();
			response.setLoginStatusCode(LoginStatusCode.FAILINPUT.getCode());
			response.setLoginStatusdescription(LoginStatusCode.FAILINPUT.getDescription());*/
			LoginResult result= new LoginResult(LoginStatusCode.FAILINPUT, "", "");
			return result.createLoginResponse();
		}
		LoginContextWrapper loginContext=new LoginContextWrapper(context);
		String userName=request.getUserName();
		String md5Password=request.getMd5Password();
		boolean isAutoLogin=request.isAutoLogin();
		String origUrl=request.getOrigUrl();
		String icode=request.getIcode();
		String captchaType=request.getCaptchaType();
		String ick=request.getIck();
		long expirePeriod=request.getExpirePeriod();
		int loginRegistrationId=request.getLoginRegistrationId();
		boolean createRenRenTicket=request.isCreateRenRenTicket();
		
		if(logger.isDebugEnabled()){
			logger.debug("LoginXoaServiceImpl login use loginService of" + getLoginService().getClass().getName());
		}
		
		long loginTime = System.nanoTime();
		LoginResult result=getLoginService().login(userName, md5Password, isAutoLogin, origUrl, icode, captchaType, ick, expirePeriod, loginRegistrationId, createRenRenTicket, loginContext);
		long estimatedTime = System.nanoTime() - loginTime;
		if(logger.isInfoEnabled()){
		    logger.info(result.getAccount() + " login xoa service time use time is " + estimatedTime);
		}
		if(estimatedTime > (long)350000000 && logger.isWarnEnabled()) {
		    logger.warn("WARN! login xoa service time out! time is " + estimatedTime );
		}
		
		return result.createLoginResponse();
	}

	@Override
	public LogoutResponse logout(String passportTicket, String ticket, LoginContext context) {
		if(logger.isDebugEnabled()){
			logger.debug("LoginXoaServiceImpl logout start...");
		}
		LoginContextWrapper loginContext=new LoginContextWrapper(context);
		
		if(logger.isDebugEnabled()){
			logger.debug("LoginXoaServiceImpl logout use loginService of" + getLoginService().getClass().getName());
		}
		LogoutResult result=getLoginService().logout(passportTicket, ticket, loginContext);
		return result.createLogoutResponse();
	}
	@Override
	public LoginResponse transfer(TransferRequest request,
			LoginContext context){
		if(logger.isDebugEnabled()){
			logger.debug("LoginXoaServiceImpl transfer start...");
		}
		if(request==null|| context==null){
			LoginResponse response=new LoginResponse();
			response.setLoginStatusCode(LoginStatusCode.FAILINPUT.getCode());
			response.setLoginStatusdescription(LoginStatusCode.FAILINPUT.getDescription());
			return response;
		}
		String userName=request.getUserName();
		String passport=request.getPassport();
		String origUrl=request.getOrigUrl();
		LoginType loginType=request.getLoginType();
		int loginRegistrationId=request.getLoginRegistrationId();
		LoginContextWrapper loginContext=new LoginContextWrapper(context);
		long transferTime = System.nanoTime();
		LoginResult result=getLoginService().transfer(userName, passport, origUrl, TypeConvertUtil.loginType2Type(loginType), loginRegistrationId, loginContext);
        long estimatedTime = System.nanoTime() - transferTime;
        if(logger.isInfoEnabled()){
            logger.info(result.getAccount() + " transferTime xoa service use time is " + estimatedTime);
        }
        if(estimatedTime > (long)150000000 && logger.isWarnEnabled()) {
            logger.warn("WARN! transfer xoa service time out! time is " + estimatedTime );
        }
        
		
		if(logger.isDebugEnabled()){
			logger.debug("LoginXoaServiceImpl transfer use loginService of" + getLoginService().getClass().getName() + "[data] userName"
					+ userName + ", passport: "+passport +"loginType: "+loginType +"loginRegistrationId: "+loginRegistrationId
					+ "[result] result: "+result.toString());
		}
		return result.createLoginResponse();
	}
	@Override
	public LoginResponse switchAccount(SwitchRequest request, LoginContext context) {
		if(logger.isDebugEnabled()){
			logger.debug("LoginXoaServiceImpl switchAccount start...");
		}
		if(request==null|| context==null){
			LoginResponse response=new LoginResponse();
			response.setLoginStatusCode(LoginStatusCode.FAILINPUT.getCode());
			response.setLoginStatusdescription(LoginStatusCode.FAILINPUT.getDescription());
			return response;
		}
		LoginContextWrapper loginContext=new LoginContextWrapper(context);
		String passport=request.getPassport();
		String ticket=request.getTicket();
		int destId=request.getDestId();
		int realUserId=request.getRealUserId();
		String md5Password=request.getMd5Password();
		String icode=request.getIcode();
		String captchaType=request.getCaptchaType();
		String ick=request.getIck();
		boolean createRenRenTicket=request.isCreateRenRenTicket();
		int loginRegistrationId=request.getLoginRegistrationId();
		LoginResult result=getLoginService().switchAccount(passport, ticket, destId, realUserId, md5Password, loginRegistrationId, icode, captchaType, ick,  createRenRenTicket, loginContext);
		if(logger.isDebugEnabled()){
			logger.debug("LoginXoaServiceImpl switchAccount use loginService of" + getLoginService().getClass().getName() + "[data] passport"
					+ passport + ", destId: "+destId +"realUserId: "+realUserId +"loginRegistrationId: "+loginRegistrationId
					+ "[result] result: "+result.toString());
		}
		
		return result.createLoginResponse();
	}

	@Override
	public long verifyPassport(String ticket, LoginType loginType) {
		if(logger.isDebugEnabled()){
			logger.debug("LoginXoaServiceImpl verifyPassport start...[data] ticket" + ticket +", loginType" + loginType);
		}
		return ticketService.verifyTicket(ticket, TypeConvertUtil.loginType2Type(loginType), TicketType.PASSPORT);
	}

	@Override
	public long verifyTicket(String ticket, LoginType loginType) {
		if(logger.isDebugEnabled()){
			logger.debug("LoginXoaServiceImpl verifyPassport start...[data] ticket" + ticket +", loginType" + loginType 
					+ ", ticketType: "+TicketType.TICKET);
		}
		return ticketService.verifyTicket(ticket, TypeConvertUtil.loginType2Type(loginType), TicketType.TICKET);
	}

	@Override
	public long verifyPassportForTypes(String ticket, List<LoginType> loginTypes) {
		List<Integer> types=TypeConvertUtil.loginType2Type(loginTypes);
		if(logger.isDebugEnabled()){
			logger.debug("LoginXoaServiceImpl verifyPassport start...[data] ticket" + ticket +", types" + types);
		}
		return ticketService.verifyTicket(ticket, types, TicketType.PASSPORT);
	}

	@Override
	public long verifyTicketForTypes(String ticket, List<LoginType> loginTypes) {
		List<Integer> types=TypeConvertUtil.loginType2Type(loginTypes);
		if(logger.isDebugEnabled()){
			logger.debug("LoginXoaServiceImpl verifyPassport start...[data] ticket" + ticket +", types" + types
					+", ticketType: "+TicketType.TICKET);
		}
		return ticketService.verifyTicket(ticket, types, TicketType.TICKET);
	}

}
