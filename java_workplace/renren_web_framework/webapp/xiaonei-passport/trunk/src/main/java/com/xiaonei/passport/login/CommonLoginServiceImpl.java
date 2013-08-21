package com.xiaonei.passport.login;


import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.passport.service.LoginType;
import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.configure.ILoginConfigure;
import com.xiaonei.passport.configure.LoginConfigureFactory;
import com.xiaonei.passport.constants.CommonVariable;
import com.xiaonei.passport.constants.LoginConfigureKey;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.constants.LogoutStatusCode;
import com.xiaonei.passport.constants.TicketType;
import com.xiaonei.passport.dbwrite.AccountServiceFactory;
import com.xiaonei.passport.event.LogicBeanProcessor;
import com.xiaonei.passport.logger.LoginLoggerDef;
import com.xiaonei.passport.logger.PpLogger;
import com.xiaonei.passport.logic.captcha.CaptchaAdapterFactory;
import com.xiaonei.passport.model.Account;
import com.xiaonei.passport.model.LoginContextWrapper;
import com.xiaonei.passport.model.LoginParameter;
import com.xiaonei.passport.model.LoginResult;
import com.xiaonei.passport.model.LogoutResult;
import com.xiaonei.passport.passport.IPassportService;
import com.xiaonei.passport.passport.PassportServiceFactory;
import com.xiaonei.passport.ticket.ITicketService;
import com.xiaonei.passport.ticket.TicketServiceFactory;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.idcache.IdInfoAdapter;
import com.xiaonei.xce.passport.Login;
import com.xiaonei.xce.passport.PassportAdapterFactory;
import com.xiaonei.xce.ticket.TicketInfo;

public class CommonLoginServiceImpl implements ICommonLoginService {
	protected PpLogger remoteLogger = LoginLoggerDef.login;
	protected Log logger = LogFactory.getLog(this.getClass());
	private IPassportService passportService = PassportServiceFactory
			.getInstance();
	private ITicketService ticketService = TicketServiceFactory.getInstance();
	private ILoginConfigure loginConfigure = LoginConfigureFactory
			.getLoginConfigureInstance();
	private static ICommonLoginService _instance = new CommonLoginServiceImpl();

	private CommonLoginServiceImpl() {
	};

	public static ICommonLoginService getInstance() {
		return _instance;
	}
	


	@Override
	public LoginResult login(String userName, String md5Password,
			boolean isAutoLogin, String origUrl, String icode,
			String captchaType, String ick, long expirePeriod,
			int loginRegistrationId, boolean createRenRenTicket,
			final ILoginContext context) {
		String domain = getDomain(loginRegistrationId);
		if (!checkParameter(userName, loginRegistrationId, context)) {
			return new LoginResult(LoginStatusCode.FAILNOLOGINREGISTRATION, userName, domain);
		}
		ILoginParameter loginParameter = new LoginParameter(userName,
				md5Password, isAutoLogin, icode, captchaType, ick, domain,
				expirePeriod, loginRegistrationId, createRenRenTicket, context);

		try {
			/**
			 * 登陆前验证，如验证码
			 */

			LoginStatusCode code = LogicBeanProcessor.processLogicBeforeLogin(loginParameter, LoginConfigureKey.LOGIC_BEFORE_LOGIN.getKey());
			if (LoginStatusCode.LOGIN_OK != code) {
				return LogicBeanProcessor.processLogicAfterError(code, loginParameter,
						LoginConfigureKey.LOGIC_FAIL_LOGIN.getKey());
			}

			/**
			 * 验证用户名密码，对于符合条件的多个id，返回上次登录的id。对于page管理员 ，返回pageid和管理员id
			 */
			long beforelogin=System.nanoTime();
			Account account = passportService.login(userName, md5Password);// 帐号检查在登陆里面处理
			long estimatedTime = System.nanoTime() - beforelogin;
			if(estimatedTime > 200000000 && logger.isWarnEnabled()) {
			    logger.warn("WARN! passport service login use time too long " + estimatedTime);
			}
			LoginResult loginResult=createTicketAndExecuteLogic(account, loginParameter,
					loginRegistrationId, expirePeriod, createRenRenTicket,
					context, LoginConfigureKey.LOGIC_AFTER_LOGIN.getKey(), LoginConfigureKey.LOGIC_FAIL_LOGIN.getKey());
			return loginResult;
		} catch (Exception e) {
			logger.error(userName+" login error", e);
			return LogicBeanProcessor.processLogicAfterError(LoginStatusCode.FAILUNKNOWN,
					loginParameter, LoginConfigureKey.LOGIC_FAIL_LOGIN.getKey());
		}

	}

	@Override
	public LoginResult transfer(String userName, String passport, String origUrl,
			int type, int loginRegistrationId,
			final ILoginContext context) {
		String domain =getDomain(loginRegistrationId);
		ILoginParameter loginParameter = new LoginParameter(userName, null,
				false, null, null, null, domain, ITicketService.DEFAULT_TICKET_TIME, loginRegistrationId, true,
				context);
		try{
			if (StringUtils.isEmpty(passport)) {
				return LogicBeanProcessor.processLogicAfterError(LoginStatusCode.FAILNOPOST,
						loginParameter, LoginConfigureKey.TRANSFER_LOGIC_FAIL_LOGIN
								.getKey());
			}
			if (type==-1) {
				if(logger.isWarnEnabled()){
					logger.warn(userName+"'s transfer has not loginType");
				}
				type = LoginType.Web.getValue();
			}
			if (loginRegistrationId <= 0) {
				if(logger.isWarnEnabled()){
					logger.warn(userName+"'s transfer has not loginRegistrationId");
				}
				loginRegistrationId = ILoginConfigure.DEFAULT_LOGIN_REGISTRATION_ID;
			}
			
			TicketInfo info = ticketService.getTicketInfo(passport,
					TicketType.PASSPORT);
			
			if (info == null || info.get_id() <= 0) {
				return LogicBeanProcessor.processLogicAfterError(LoginStatusCode.FAILNOPOST,
						loginParameter, LoginConfigureKey.TRANSFER_LOGIC_FAIL_LOGIN
								.getKey());
			}

			long createPassportTime = System.nanoTime();
			String ticket = ticketService.createTicket(info.get_id(), info
                    .get_realId(), context.getIp(), type, CommonVariable.T_TICKET_MAX_LIEFTIME,
                    TicketType.TICKET);
            long estimatedTime = System.nanoTime() - createPassportTime;
            if(estimatedTime > 100000000 && logger.isWarnEnabled()) {
                logger.warn("WARN! transfer create ticket use time " + estimatedTime);
            }
            
            long getAccountTime = System.nanoTime();
            Account account = passportService.getAccount(info.get_id());
            estimatedTime = System.nanoTime() - getAccountTime;
            if(estimatedTime > 100000000 && logger.isWarnEnabled()) {
                logger.warn("WARN! transfer get account  use time " + estimatedTime);
            }
			
			int userId = Long.valueOf(info.get_id()).intValue();
			int realId = Long.valueOf(info.get_realId()).intValue();
			LoginResult loginResult = new LoginResult(LoginStatusCode.LOGIN_OK,
					account.getAccount(), userId, realId, account.getStatus(),
					passport, ticket, account.getDomain());

			//过了在过渡期保证其它子域下可以正常验票访问，这里有必要创建老的t票，将来common-interceptor类全站推广后，就可以去掉这段代码了
			//createOldTicketForOtherDomainValidation(context, loginResult);
			
			LoginStatusCode code = LogicBeanProcessor.processLogicAfterLogin(loginResult,
					loginParameter, LoginConfigureKey.TRANSFER_LOGIC_AFTER_LOGIN
							.getKey());
			if (LoginStatusCode.LOGIN_OK != code) {
				loginResult.setLoginStatusCode(code);
				return LogicBeanProcessor.processLogicAfterError(loginResult, loginParameter,
						LoginConfigureKey.TRANSFER_LOGIC_FAIL_LOGIN.getKey());
			}
			return loginResult;
		}catch(Exception e){
			logger.error(userName+" transfer error", e);
			return LogicBeanProcessor.processLogicAfterError(LoginStatusCode.FAILUNKNOWN,
					loginParameter, LoginConfigureKey.TRANSFER_LOGIC_FAIL_LOGIN.getKey());
		}
		
	}

	@Override
	public LogoutResult logout(String passportTicket, String ticket, ILoginContext context) {
		int userId=-1;
		try {
			TicketInfo info=ticketService.getTicketInfo(passportTicket, TicketType.PASSPORT);
			if(info!=null){
				userId=Long.valueOf(info.get_id()).intValue();
			}
			boolean sucess = ticketService.destroyTicket(passportTicket,
					TicketType.PASSPORT);
			if (!sucess) {
			    if(logger.isErrorEnabled()) {
			        logger.error(passportTicket + " passport Ticket destroy error");
			    }
				return LogicBeanProcessor.processLogicAfterLogout(LogoutStatusCode.PASSPORT_DESTROY_FAIL, userId);
			}
			if (!StringUtils.isEmpty(ticket)) {
				sucess = ticketService.destroyTicket(ticket, TicketType.TICKET);
				if (!sucess) {
				    if(logger.isErrorEnabled()) {
	                    logger.error(ticket + " Ticket destroy error");
	                }
					return LogicBeanProcessor.processLogicAfterLogout(LogoutStatusCode.TICKET__DESTROY_FAIL, userId);
				}
			}
			return LogicBeanProcessor.processLogicAfterLogout(LogoutStatusCode.LOGOUT_OK, userId);
		} catch (Exception e) {
			logger.error("logout error", e);
			return LogicBeanProcessor.processLogicAfterLogout(LogoutStatusCode.SYSTEM_EXCEPTION, userId);
		}

	}

	@Override
	public LoginResult switchAccount(String passport, String ticket, int destId, int realUserId,
			String md5Password, int loginRegistrationId, String icode, String captchaType, String ick,
			boolean createRenRenTicket, ILoginContext context) {
	    com.xiaonei.xce.passport.Account account = PassportAdapterFactory.getWebLogin().switchAccountIncVirtualId(realUserId,
	            destId, md5Password, true);
	    
	    if(!idInfoAdapter.isVirtualId(account.getId())) {
	        realUserId = account.getId();
	    }
	    Account realAccount = new Account(account.getId(), account.getAccount(), account.getDomain(), account.getStatus(), realUserId, 0);
		//Account account = passportService.login(destId, realUserId,md5Password);
		ILoginParameter loginParameter = new LoginParameter(account
				.getAccount(), md5Password, false, null, null, null, account.getDomain(),
				ITicketService.DEFAULT_TICKET_TIME, loginRegistrationId,
				createRenRenTicket, context);
		
		/**
		 * 登陆前验证，如验证码
		 */
		LoginStatusCode code = LogicBeanProcessor.processLogicBeforeLogin(loginParameter, LoginConfigureKey.SWITCH_LOGIC_BEFORE_LOGIN.getKey());
		if (LoginStatusCode.LOGIN_OK != code) {
			return LogicBeanProcessor.processLogicAfterError(code, loginParameter,
					LoginConfigureKey.SWITCH_LOGIC_FAIL_LOGIN.getKey());
		}
		this.logout(passport, ticket, context);
		
		dbwriteUserTransfer(realUserId, destId);
		
//		if(destId==realUserId){
//			passportService.updateUserAccounts(destId, account.getAccount());
//		}else{
//			passportService.updateUserTransfers(destId, realUserId);
//		}
		
		
		LoginResult result= createTicketAndExecuteLogic(realAccount, loginParameter,
				loginRegistrationId, ITicketService.DEFAULT_TICKET_TIME,
				createRenRenTicket, context,
				LoginConfigureKey.SWITCH_LOGIC_AFTER_LOGIN.getKey(), null);
		
		String oldPassportAdapt=loginConfigure.getUniqLoginConfigure(loginRegistrationId, LoginConfigureKey.OLD_PASSPORT_ADAPT);
        Login login=getLogin(oldPassportAdapt);
        
		createOldTicket(account, result, createRenRenTicket, ITicketService.DEFAULT_TICKET_TIME,login,context);
//		
		return result;
	}
	/**
	 * 登陆成功后，进行票的创建，以及登陆成功后逻辑的处理，如安全检查
	 * @param account 登陆相关信息
	 * @param loginParameter 登陆参数
	 * @param loginRegistrationId 登陆注册id
	 * @param expirePeriod p票时间
	 * @param createRenRenTicket 是否创建t票
	 * @param context 登陆上下文环境
	 * @param key 获取逻辑的key
	 * @return
	 */
	@Override
	public LoginResult createTicketAndExecuteLogic(final Account account,
			final ILoginParameter loginParameter, int loginRegistrationId,
			long expirePeriod, boolean createRenRenTicket,
			ILoginContext context, String afterLoginKey, String failLoginKey) {
		try{
			LoginStatusCode code = account.getCode();
			if (LoginStatusCode.LOGIN_OK != code) {
				return LogicBeanProcessor.processLogicAfterError(code, loginParameter, failLoginKey);
			}
			int userId = account.getId();
			int realUserId = account.getRealId();
			/**
			 * 产生P票
			 */
			
			Integer loginType = loginConfigure.getUniqLoginConfigureForInt(
					loginRegistrationId, LoginConfigureKey.LOGIN_TYPE.getKey());
			
			if (loginType == null) {
				return LogicBeanProcessor.processLogicAfterError(LoginStatusCode.FAILNOLOGINTYPE,
						loginParameter, failLoginKey);
			}
			long createPassportTime = System.nanoTime();
			String passortTicket = ticketService.createTicket(userId,
					realUserId, context.getIp(), loginType, CommonVariable.T_TICKET_MAX_LIEFTIME,
					TicketType.PASSPORT);
			long estimatedTime = System.nanoTime() - createPassportTime;
			
            if(estimatedTime > 100000000 && logger.isWarnEnabled()) {
                logger.warn("WARN! create passport use time " + estimatedTime);
            }
			
			
			String ticket = null;
			
			/**
			 * 根据需求产生T票
			 */
			if (createRenRenTicket) {
				
				long createTicketTime = System.nanoTime();
				//过期时间设置为0（而不是-1）表示此值永远不过期
				ticket = ticketService.createTicket(userId, realUserId, context
						.getIp(), loginType, CommonVariable.T_TICKET_MAX_LIEFTIME, TicketType.TICKET);
				estimatedTime = System.nanoTime() - createTicketTime;
				if(estimatedTime > 100000000 && logger.isWarnEnabled()) {
				    logger.warn("WARN! create Ticket use time " + estimatedTime);
	            }
				
				
				
				
			}
			LoginResult result = new LoginResult(LoginStatusCode.LOGIN_OK, account
					.getAccount(), userId, realUserId, account.getStatus(),
					passortTicket, ticket, account.getDomain());
			
			
			//过了在过渡期保证其它子域下可以正常验票访问，这里有必要创建老的t票，将来common-interceptor类全站推广后，就可以去掉这段代码了
			//createOldTicketForOtherDomainValidation(context, result);
			
			long processLogicTime = System.nanoTime();
			code = LogicBeanProcessor.processLogicAfterLogin(result, loginParameter, afterLoginKey);
			estimatedTime = System.nanoTime() - processLogicTime;
			
			if(estimatedTime > 150000000 && logger.isWarnEnabled()) {
			    logger.warn("WARN! process Logic use time " + estimatedTime);
			}
			
			if (LoginStatusCode.LOGIN_OK != code) {
				result.setLoginStatusCode(code);
				return LogicBeanProcessor.processLogicAfterError(result, loginParameter, failLoginKey);
			}
			
			return result;
		}catch(Exception e){
			logger.error(loginParameter.getUserName()+" createTicketAndExecuteLogic error", e);
			return LogicBeanProcessor.processLogicAfterError(LoginStatusCode.FAILUNKNOWN,
					loginParameter, failLoginKey);
		}
	
	}

//	private void createOldTicketForOtherDomainValidation(ILoginContext context, LoginResult result){
//		Map<String,String> ticketInfoMap = new HashMap<String, String>();
//		ticketInfoMap.put("User-Agent", context.getUserAgent());
//		ticketInfoMap.put("IP", context.getIp());
//		String oldTicket = PassportAdapterFactory.getWebTicket().createTicket(result.getUserId(), ticketInfoMap);
//		result.setOldTicket(oldTicket);
//	}

	

	/**
	 * 获取domain，因为可能在某些业务会根据domain做处理。默认domain配置在configure中。如果没有配置，则产生默认值。
	 * 
	 * @param loginRegistrationId
	 *            登陆注册号
	 * @return
	 */
	private String getDomain(int loginRegistrationId) {
		String domain = loginConfigure.getUniqLoginConfigure(
				loginRegistrationId, LoginConfigureKey.DOMAIN);
		if (StringUtils.isEmpty(domain) ) {
			if(logger.isWarnEnabled()){
				StringBuffer sb = new StringBuffer();
				sb.append("the user which  loginRegistrationId is ");
				sb.append(loginRegistrationId);
				sb
						.append(" does not config domain. System has used a default value");
				logger.warn(sb.toString());
			}
			return "renren.com";
		}
		return domain;
	}

	/**
	 * 初步参数检查，如果参数有问题，直接拒绝掉
	 * 
	 * @param userName
	 *            用户名
	 * @param loginRegistrationId
	 *            登陆注册号
	 * @param context
	 *            登陆上下文
	 * @return
	 */
	private boolean checkParameter(String userName, int loginRegistrationId,
			ILoginContext context) {
		if (loginRegistrationId <= 0 || context == null) {
			remoteLogger.info(String.format(LoginLoggerDef.loginLogPentter,
					LoginStatusCode.FAILNOLOGINREGISTRATION.name(), userName,
					context == null ? null : context.getReferer(), userName,
					context == null ? null : context.getIp(),
					loginRegistrationId));
			return false;
		}
		return true;
	}

	@Override
	public int getCaptchaGrade( String account, String ip) {
		return CaptchaAdapterFactory.getCaptchaAdapterInstance().getCaptchaGrade(LoginContextWrapper.newLoginLocalVar(), account, ip);
	}

	
	private static IdInfoAdapter idInfoAdapter = new IdInfoAdapter();
	
    private int getTransUserId(int accountId){
        if (idInfoAdapter.isVirtualId(accountId)) {
            return idInfoAdapter.getRelatedTransId(accountId);
        }else{
            return accountId;
        }
    }
    
    /**
     * 当切换身份时,同时双写新的数据库表,保证登陆顺序
     * @param sourceId 切换原id
     * @param destId 切换到id
     */
    private void dbwriteUserTransfer(int sourceId, int  destId){
        try{
            User realUser=getReallyUser(sourceId);
            if(realUser==null){
                return ;
            }
            User relationUser=getReallyUser(destId);
            if(relationUser==null){
                return ;
            }
            if(idInfoAdapter.isVirtualId(destId)){
                AccountServiceFactory.getInstance().updateUserTransfersLastLoginTime(realUser.getId(), idInfoAdapter.getRelatedTransId(destId));
            }else{
                AccountServiceFactory.getInstance().updateUserAccountsLastLoginTime(realUser.getId(), relationUser.getAccount());
            }
        }catch(Exception e){
            e.printStackTrace();
        }
    }
    
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
    
    private void createOldTicket(com.xiaonei.xce.passport.Account account, LoginResult result, boolean createRenRenTicket, long expirePeriod, Login login, 
            ILoginContext context){
        //TODO
        result.setOldPassport(account.getTicket());
        if(createRenRenTicket){
            //String ticket=login.createTicket(result.getUserId(), Long.valueOf(expirePeriod).intValue(), true);
            //result.setOldTicket(ticket);
            Map<String,String> ticketInfoMap = new HashMap<String, String>();
            ticketInfoMap.put("User-Agent", context.getUserAgent());
            ticketInfoMap.put("IP", context.getIp());
            String oldTicket = PassportAdapterFactory.getWebTicket().createTicket(result.getUserId(), ticketInfoMap);
            result.setOldTicket(oldTicket);
        }
    }
    
    private User getReallyUser(int accountId) {
        User realUser = null;
        if (idInfoAdapter.isVirtualId(accountId)) {
            int transId = idInfoAdapter.getRelatedUserId(accountId);
            try {
                realUser = SnsAdapterFactory.getUserAdapter().get(transId);
            } catch (Exception e) {
                this.logger.error(
                        "getReallyUser-SnsAdapterFactory.getUserAdapter().get Error,transId="
                                + transId, e);
            }
        } else {
            try {
                realUser = SnsAdapterFactory.getUserAdapter().get(accountId);
            } catch (Exception e) {
                this.logger.error(
                        "getReallyUser-SnsAdapterFactory.getUserAdapter().get Error,accountId="
                                + accountId, e);
            }
        }
        return realUser;
    }
}
