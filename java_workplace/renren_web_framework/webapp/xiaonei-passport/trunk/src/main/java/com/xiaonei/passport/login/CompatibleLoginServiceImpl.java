package com.xiaonei.passport.login;


import java.util.HashMap;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.configure.ILoginConfigure;
import com.xiaonei.passport.configure.LoginConfigureFactory;
import com.xiaonei.passport.constants.Globals;
import com.xiaonei.passport.constants.LoginConfigureKey;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.model.LoginParameter;
import com.xiaonei.passport.model.LoginResult;
import com.xiaonei.passport.model.LogoutResult;
import com.xiaonei.passport.passport.IPassportService;
import com.xiaonei.passport.passport.PassportServiceFactory;
import com.xiaonei.passport.ticket.ITicketAdapter;
import com.xiaonei.passport.ticket.ITicketService;
import com.xiaonei.xce.idcache.IdInfoAdapter;
import com.xiaonei.xce.passport.Account;
import com.xiaonei.xce.passport.Login;
import com.xiaonei.xce.passport.PassportAdapterFactory;
import com.xiaonei.xce.passport.TicketWebImpl;
import com.xiaonei.xce.ticket.Ticket;

/**
 * 兼容新旧两种票机制及数据结构的登录
 * 当新的登录方式失败时，尝试用老的票数据再次登录
 * @author shuguo.zhang
 */
public class CompatibleLoginServiceImpl implements ICommonLoginService {
	
	private ICommonLoginService commonLoginService = CommonLoginServiceImpl.getInstance();
	private static ICommonLoginService _instance =new CompatibleLoginServiceImpl();
	
	private ILoginConfigure loginConfigure = LoginConfigureFactory.getLoginConfigureInstance();
	
	private TicketWebImpl ticketAdapter=TicketWebImpl.getInstance();
	private IPassportService passportService=PassportServiceFactory.getInstance();
	private static Log logger = LogFactory.getLog(CompatibleLoginServiceImpl.class);
	private static IdInfoAdapter idInfoAdapter = new IdInfoAdapter();
	private CompatibleLoginServiceImpl(){}
	public static ICommonLoginService getInstance(){
		return _instance;
	}
	@Override
	public LoginResult login(String userName, String md5Password,
			boolean isAutoLogin, String origUrl, String icode,
			String captchaType, String ick, long expirePeriod,
			int loginRegistrationId, boolean createRenRenTicket,
			ILoginContext context) {


		LoginResult result = commonLoginService.login(userName, md5Password,
				isAutoLogin, origUrl, icode, captchaType, ick, expirePeriod,
				loginRegistrationId, createRenRenTicket, context);
		
		if (LoginStatusCode.LOGIN_OK != result.getCode()) {
			if(logger.isDebugEnabled()){
				logger.debug("CompatibleLoginServiceImpl use new-login failed...[userName]" + userName);
			}
			if(LoginStatusCode.FAILUSERNOTEXIST==result.getCode()){
				if(logger.isDebugEnabled()){
					logger.debug("CompatibleLoginServiceImpl use new-login failed...[user not exist in table user_accounts]" + userName);
				}	
				/**
				 * 可能数据没有正确同步，用老的数据登陆
				 */
				result=loginUserOldPassportData(userName, md5Password, isAutoLogin, origUrl, icode, captchaType, ick, expirePeriod, loginRegistrationId, createRenRenTicket, context, result);
				
				
			}
			return result;
		} else {
			if(logger.isDebugEnabled()){
				logger.debug("CompatibleLoginServiceImpl use new-login successfully...[userName]" + userName);
			}
			createOldTicket(loginRegistrationId, userName, md5Password, isAutoLogin, createRenRenTicket, expirePeriod, result, context);
		}
		return result;
	}
	@Override
	public LoginResult createTicketAndExecuteLogic(
			com.xiaonei.passport.model.Account account,
			ILoginParameter loginParameter, int loginRegistrationId,
			long expirePeriod, boolean createRenRenTicket,
			ILoginContext context, String afterLoginKey, String failLoginKey) {
		return commonLoginService.createTicketAndExecuteLogic(account, loginParameter, loginRegistrationId, expirePeriod, createRenRenTicket, context, afterLoginKey, failLoginKey);
	}
	@Override
	public LogoutResult logout(String passportTicket, String ticket, ILoginContext context) {
		LogoutResult result=commonLoginService.logout(passportTicket, ticket,  context);
		clearOldTicket(0, context.getCooikes());
		return result;
	}

	@Override
	public LoginResult switchAccount(String passport, String ticket, int desId, int realUserId,
			String pwd, int loginRegistrationId,  String icode,
			String captchaType, String ick,  boolean createRenRenTicket,
			ILoginContext context) {
		LoginResult result = commonLoginService.switchAccount(passport, ticket, desId, realUserId,pwd, loginRegistrationId,icode, captchaType, ick, createRenRenTicket, context);
		clearOldTicket(loginRegistrationId, context.getCooikes());
		//createOldTicket(loginRegistrationId, result.getAccount(), pwd, false, createRenRenTicket,false, ITicketService.DEFAULT_TICKET_TIME, result, context);
		return result;
	}

	@Override
	public LoginResult transfer(String userName, String passport, String origUrl,
			int loginType, int loginRegistrationId,
			final ILoginContext context) {
		LoginResult result =  commonLoginService.transfer(userName, passport, origUrl, loginType, loginRegistrationId, context);
		Map<String, String> cookie=context.getCooikes();
		String oldPassport=cookie.get("p");
		 
		String oldPassportAdapt=loginConfigure.getUniqLoginConfigure(loginRegistrationId, LoginConfigureKey.OLD_PASSPORT_ADAPT);
		Login login=getLogin(oldPassportAdapt);
		if(login==null){
		    result.setLoginStatusCode(LoginStatusCode.FAILNOLOGINREGISTRATION);
			return result;
		}
		if(StringUtils.isEmpty(oldPassport)){
			Account account=login.id2Account(result.getUserId());
			result.setOldPassport(account.getTicket());
		}else{
			result.setOldPassport(oldPassport);
		}
		//String ticket=login.createTicket(result.getUserId(), Long.valueOf(ITicketService.DEFAULT_TICKET_TIME).intValue(), true);
		Map<String,String> ticketInfoMap = new HashMap<String, String>();
		ticketInfoMap.put("User-Agent", context.getUserAgent());
		ticketInfoMap.put("IP", context.getIp());
		String oldTicket = PassportAdapterFactory.getWebTicket().createTicket(result.getUserId(), ticketInfoMap);
		result.setOldTicket(oldTicket);
		return result;
	}
	
	@Override
	public int getCaptchaGrade(String account, String ip) {
		return commonLoginService.getCaptchaGrade(account, ip);
	}
	/**
	 * 清除老的票
	 * @param loginRegistrationId
	 * @param cookies
	 */
	
	private void clearOldTicket(int loginRegistrationId, Map<String, String> cookies){
        if(cookies==null){
            logger.error("clear old ticket cookies is null");
            return ;
        }
        try{
            String oldPassportAdapt=loginConfigure.getUniqLoginConfigure(loginRegistrationId, LoginConfigureKey.OLD_PASSPORT_ADAPT);
            String oldTicket=cookies.get("t");
            String oldPassport=cookies.get("p");
            Login login=getLogin(oldPassportAdapt);
            if(login==null){
                logger.error("can not get Login type for loginRegistrationId's "+loginRegistrationId);
                return ;
            }
            if(!StringUtils.isEmpty(oldTicket)){
                if(ticketAdapter.destroyTicket(oldTicket) == false) {
                    logger.error("destroy old ticket error! old ticket is " + oldTicket);
                }
            }
            if(!StringUtils.isEmpty(oldPassport)){
                if(login.destroyByTicket(oldPassport) == false) {
                    logger.error("destroy old passport error! old ticket is " + oldPassport);
                }
            }
        }catch(Exception e){
            logger.error("clear old ticket error", e);
        }
        
    }
	
	/**
	 * 创建老的票
	 * @param loginRegistrationId
	 * @param userName
	 * @param md5Password
	 * @param isAutoLogin
	 * @param createRenRenTicket
	 * @param expirePeriod
	 * @param result
	 */
	private void createOldTicket(int loginRegistrationId, String userName, String md5Password, boolean isAutoLogin, boolean createRenRenTicket, long expirePeriod,LoginResult result, ILoginContext context){
	    LoginStatusCode resultStatusCode = LoginStatusCode.LOGIN_OK;
	    try{
			String oldPassportAdapt=loginConfigure.getUniqLoginConfigure(loginRegistrationId, LoginConfigureKey.OLD_PASSPORT_ADAPT);
			Login login=getLogin(oldPassportAdapt);
			if(login==null){
				logger.error("can not get Login type for loginRegistrationId's "+loginRegistrationId);
				return ;
			}
			Account account;
			
    			account = login
    					.loginVirtual(//
    							Globals.defaultLogin,//
    							userName,//
    							md5Password,//
    							result.getDomain(), //
    							isAutoLogin ? -1 : 6 * 3600,//
    							true//
    					);
    			
			    createOldTicket(account, result, createRenRenTicket, expirePeriod, login, context);
			    resultStatusCode = createCode(account.getSuccess(), account.getId(), account.getId());
			
			
		} catch(Exception e) {
		    if(logger.isErrorEnabled()) {
		        logger.error("create old ticket error", e);
		        result.setLoginStatusCode(LoginStatusCode.FAILUNKNOWN);
		    }
		}
		result.setLoginStatusCode(resultStatusCode);
	}
	
	private LoginStatusCode createCode(int success, int id, int realId) {
        // 输入密码错误
        if (success == -2) {
            return LoginStatusCode.FAILUSER;
        }
        // 输入用户不存在
        if (success == -1) {
            return LoginStatusCode.FAILUSERNOTEXIST;
        }
        if (success < 0) {
            return LoginStatusCode.FAILUSERNOTEXIST;
        }
        if (id < 0|| realId<0) {
            return LoginStatusCode.FAILUSERNOTEXIST;
        }
        return LoginStatusCode.LOGIN_OK;
    }
	
	private void createOldTicket(Account account, LoginResult result, boolean createRenRenTicket, long expirePeriod, Login login, 
            ILoginContext context){
        //TODO
        result.setOldPassport(account.getTicket());
        if(createRenRenTicket){
            //String ticket=login.createTicket(result.getUserId(), Long.valueOf(expirePeriod).intValue(), true);
            //result.setOldTicket(ticket);
            Map<String,String> ticketInfoMap = new HashMap<String, String>();
            ticketInfoMap.put("User-Agent", context.getUserAgent());
            ticketInfoMap.put("IP", context.getIp());
            String oldTicket = PassportAdapterFactory.getWebTicket().createTicket(account.getId(), ticketInfoMap);
            result.setOldTicket(oldTicket);
        }
    }
	
	/**
	 * 获取旧版用的Login实例
	 * @param oldPassportAdapt
	 * @return
	 */
	private Login getLogin(String oldPassportAdapt){
		if(StringUtils.isEmpty(oldPassportAdapt)){
			return  null;
		}
		if("web".equals(oldPassportAdapt)){
			return  PassportAdapterFactory.getWebLogin();
		}else if("wap".equals(oldPassportAdapt)){
			return PassportAdapterFactory.getWapLogin();
		}else if ("mobileClient".equals(oldPassportAdapt)){
			return PassportAdapterFactory.getMobileClientLogin();
		}
		return  null;
	}

	private LoginResult loginUserOldPassportData(String userName, String md5Password,
			boolean isAutoLogin, String origUrl, String icode,
			String captchaType, String ick, long expirePeriod,
			int loginRegistrationId, boolean createRenRenTicket,
			ILoginContext context, LoginResult result){
		String oldPassportAdapt=loginConfigure.getUniqLoginConfigure(loginRegistrationId, LoginConfigureKey.OLD_PASSPORT_ADAPT);
		Login login=getLogin(oldPassportAdapt);
		if(login==null){
			logger.error("can not get Login type for loginRegistrationId's "+loginRegistrationId);
			return result;
		}
		Account account = login
				.loginVirtual(//
						Globals.defaultLogin,//
						userName,//
						md5Password,//
						result.getDomain(), //
						isAutoLogin ? -1 : 6 * 3600,//
						true//
				);
		if(account.getSuccess()==0){
			int userId=account.getId();
			int realUserId=userId;
			if(idInfoAdapter.isVirtualId(userId)){
				userId=idInfoAdapter.getRelatedUserId(userId);
				realUserId=idInfoAdapter.getRelatedTransId(userId);
			}
			ILoginParameter loginParameter = new LoginParameter(userName,
					md5Password, isAutoLogin, icode, captchaType, ick, result.getDomain(),
					expirePeriod, loginRegistrationId, createRenRenTicket, context);
			com.xiaonei.passport.model.Account loginAccount=passportService.login(userId, realUserId, md5Password);
			result=commonLoginService.createTicketAndExecuteLogic(loginAccount, loginParameter, loginRegistrationId, expirePeriod, createRenRenTicket, context ,LoginConfigureKey.LOGIC_AFTER_LOGIN.getKey(), LoginConfigureKey.LOGIC_FAIL_LOGIN.getKey());
			createOldTicket(account, result, createRenRenTicket,expirePeriod, login, context);
			return result;
		}
		return result;
	}
}
