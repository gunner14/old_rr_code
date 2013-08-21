package com.xiaonei.login.controllers.base;

import java.net.URL;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.util.Map;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.AsSuperController;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.xcache.XCache;
import com.xiaonei.aegis.login.client.CheckLoginClient;
import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.login.constants.Globals;
import com.xiaonei.login.constants.LoginCookieDef;
import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginSourceType;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.form.PassportForm;
import com.xiaonei.login.logic.DomainOprImpl;
import com.xiaonei.login.logic.LoginConfigureImpl;
import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.login.logic.PassportLoginImpl;
import com.xiaonei.login.logic.afterlogin_chains.LoginResultChains;
import com.xiaonei.login.logic.afterlogin_chains.LoginResultChainsImpl;
import com.xiaonei.login.logic.captcha.CaptchaAdapterFactory;
import com.xiaonei.login.logic.failswitcher.ILoginFailSwitch;
import com.xiaonei.login.logic.failswitcher.LoginFailSwitchImpl;
import com.xiaonei.login.logic.logger.PpLogger;
import com.xiaonei.login.logic.status.GrayIpAndWarningUserImpl;
import com.xiaonei.login.logic.status.ValidationImpl;
import com.xiaonei.login.logic.ticket.PassportTicketOpr;
import com.xiaonei.login.logic.validator.PpParamValidatorUtil;
import com.xiaonei.login.logic.validator.annotation.FormValidator;
import com.xiaonei.login.logic.validator.enumeration.ValidatorEnum;
import com.xiaonei.login.util.LoginDateTimeUtil;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserPassport;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.utility.DES_Encrypt;
import com.xiaonei.platform.core.utility.Encoder;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.idcache.IdInfoAdapter;
import com.xiaonei.xce.passport.Account;

@AsSuperController
public abstract class LoginController implements InitializingBean {

    private static IdInfoAdapter idInfoAdapter = IdInfoAdapter.getInstance();

    @Autowired
    private DomainOprImpl domainOprImpl;

    private ILoginFailSwitch failSwitcher;

    @Autowired
    private GrayIpAndWarningUserImpl grayIpAndWarningUserImpl;

    @Autowired
    private PassportLoginImpl passportLoginImpl;

    @Autowired
    private PassportTicketOpr passportTicketOpr;

    private LoginResultChains resultChains;

    @Autowired
    private ValidationImpl validationImpl;

    protected Log logger = LogFactory.getLog(this.getClass());

    protected PpLogger remoteLogger = LoginLoggerDef.login;
    
    private static Object showBlackBoardAd = null;
    
    static {
    	ScheduledExecutorService scheduleService = Executors.newScheduledThreadPool(1);
        scheduleService.scheduleAtFixedRate(new Runnable() {
			public void run() {
				showBlackBoardAd = XCache.getCache("jebe.rest", Object.class).get("blackboard_flag");
			}
		}, 0, 90, TimeUnit.SECONDS);
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        this.failSwitcher = new LoginFailSwitchImpl();
        this.resultChains = new LoginResultChainsImpl();
        if (this.failSwitcher.isEmpty()) {
            this.setUpFailSwitcher(this.failSwitcher);
        }
        if (this.resultChains.isEmpty()) {
            this.setUpResultChains(this.resultChains);
        }
    }

    /**
     * index<br>
     * 这个一个get,做什么用的呢?<br>
     * 换票,主要工作
     * 
     * @param inv
     * @param domain
     * @param origURL
     * @param p3p
     * @param autoLogin
     * @return
     * 
     * @author tai.wang@opi-corp.com Jan 11, 2010 - 5:27:54 PM
     */
    @Get
    public String get(
            final Invocation inv,// 负责登录的主要方法
            @FormValidator(params = { ValidatorEnum.domain, ValidatorEnum.origURL,
                    ValidatorEnum.errURL, ValidatorEnum.p3p, ValidatorEnum.autologin }) final PassportForm pForm) {
        final HttpServletRequest request = inv.getRequest();
        this.setUp(inv);
        final String ip = this.getClientIpFromRequest(inv);
        inv.getRequest().setAttribute("CLIENT_IP", ip);
        final String origUrl = this.passportLoginImpl.getOrigUrl(pForm.getDomain(),
                pForm.getOrigURL(), this.needNotify(),
                request.getAttribute(LoginParamDef.ATTRIBUTE_WEAK_PASSWORD_WARNING));
        pForm.setOrigURL(origUrl);
        if (PpParamValidatorUtil.isCheckNotOk(inv)) {
            // 基本的参数提交异常
            return this.failSwitcher.failSwitch(pForm.getDomain(), inv,
                    PpParamValidatorUtil.getLoginStatusOfValidation(inv));
        }

        // 不是passport domain ,便跳转过去
        if (this.currentDomainIsNotPassportDomain(request)) {
            return "r:" + this.getPpDomAddrFrCurAddr(inv, pForm);
        }

        // 用户id,判断是否来换票
        final int hostid = this.passportTicketOpr.getUserIdByPassport(inv.getRequest(),
                inv.getResponse());
        // pageAdmin登陆的情况
        int hostid_real = 0;
        if (idInfoAdapter.isVirtualId(hostid)) {
            hostid_real = idInfoAdapter.getRelatedTransId(hostid);
        } else {
            hostid_real = hostid;
        }
        // 换t票加上状态判断
        final UserPassport userPassport = SnsAdapterFactory.getUserPassportAdapter()
                .getUserPassport(hostid_real);
        final Account account = new Account(hostid, 1, userPassport.getAccount(),
                userPassport.getDomain(), userPassport.getStatus(), "");
        final LoginStatusCode loginStatusCode = this.validationImpl.checkUserStatus(account);
        

        // 不是换票
        if ((hostid <= 0) && (loginStatusCode == LoginStatusCode.LOGIN_OK)) {
            if (StringUtils.isNotEmpty(pForm.getErrURL())) {
                try {
                    final String url = URLDecoder.decode(pForm.getErrURL(), "UTF-8");
                    new URL(url);
                    return "r:" + url;
                } catch (final Exception e) {
                    e.printStackTrace();
                }
            }
            return this.failSwitcher.failSwitch(pForm.getDomain(), inv, LoginStatusCode.FAILNOPOST);
        } else {
        	int type= -1;
            try{
                type=inv.getRequest().getAttribute(LoginParamDef.ATTRIBUTE_LOGIN_SOURCE)==null?-1:((LoginSourceType)inv.getRequest().getAttribute(LoginParamDef.ATTRIBUTE_LOGIN_SOURCE)).getValue();
            }catch(final Exception e){
                this.logger.error("", e);
            }
            this.remoteLogger.info(LoginLoggerDef.loginLogPentterNew, "autoOk", hostid, inv
                    .getRequest().getHeader("Referer"),pForm.getEmail(),ip, type);
        }

        // 自动登录触发
        this.theTasksToRunAfterExchangeTicket(inv, hostid);

        // after
        return this.succAndForward(inv, hostid, pForm,userPassport.getAccount());
    }

    /**
     * post<br>
     * 负责登录的方法
     * 
     * @param inv
     * @param pForm
     * @return
     * 
     * @author tai.wang@opi-corp.com Jan 13, 2010 - 11:55:51 AM
     */
    @IgnorePassportValidation
    @Post
    public String post(final Invocation inv,//
            @FormValidator(validateAllParams = true) final PassportForm pForm) {
        final String account = pForm.getEmail();
        this.setUp(inv);
        final String ip = this.getClientIpFromRequest(inv);
        inv.getRequest().setAttribute("CLIENT_IP", ip);
        if(StringUtils.isEmpty(account)){
            return this.failSwitcher.failSwitch(pForm.getDomain(), inv,
                    LoginStatusCode.FAILEMPTYACOUNT);
        }
        CaptchaAdapterFactory.getCaptchaAdapterInstance().getCaptchaGrade(inv.getRequest(),
                account, ip);
        this.logger.debug("login posted datas are: " + pForm.toString());


        // 基本的参数提交异常
        if (PpParamValidatorUtil.isCheckNotOk(inv)) {
            return this.failSwitcher.failSwitch(pForm.getDomain(), inv,
                    PpParamValidatorUtil.getLoginStatusOfValidation(inv));
        }

        // 记住用户名，用于下次在输入框自动显示
        CookieManager.getInstance().saveCookie(inv.getResponse(), LoginCookieDef.DE,
                DES_Encrypt.encrypt(pForm.getEmail()), 60 * 60 * 24 * 30 * 12,// one year
                "/");

        this.setAutoLoginTagInAttribute(inv, pForm);

        final String pwd = pForm.getPassword();
        final boolean isPasswordEncode = inv.getRequest().getAttribute(LoginParamDef.PARAM_ENPWD) != null ? true
                : false;
        final String password = isPasswordEncode ? pwd : Encoder.encode(pwd);
        inv.getRequest().setAttribute(LoginParamDef.ATTRIBUTE_ENCODE_PASSWORD, password);

        /**
         * 检验验证码
         */
        final boolean captchaValidationResult = CaptchaAdapterFactory.getCaptchaAdapterInstance()
                .validateCatcha(inv.getRequest(), account, ip);
        if (!captchaValidationResult) {
            CaptchaAdapterFactory.getCaptchaAdapterInstance().processLoginCountAfterLoginFail(
                    inv.getRequest(), account, LoginStatusCode.FAILVALIDATELOGIN, ip);
            return this.failSwitcher.failSwitch(pForm.getDomain(), inv,
                    LoginStatusCode.FAILVALIDATELOGIN);
        }
        final LoginStatusCode loginStatus = this.getRtAddrForGrayIpOrWaringUser(inv, pForm);
        if (loginStatus != LoginStatusCode.LOGIN_OK) {
            return this.failSwitcher.failSwitch(pForm.getDomain(), inv, loginStatus);
        }

        // 登录的主体
        final LoginStatusCode rt = this.passportLoginImpl.login(inv.getRequest(),
                inv.getResponse(), pForm.isAutoLogin(), pForm.getEmail(), pForm.getPassword(),
                pForm.getDomain(), this.resultChains, true);
        // 登陆后
        if (rt == LoginStatusCode.LOGIN_OK) {
            inv.getRequest().setAttribute(LoginParamDef.PARAM_ORIGURL,
                    this.getRedirectUrl(inv.getRequest()));

            final Integer userId = this.getUserId(inv);
            if (userId == null) {
                return this.failSwitcher.failSwitch(pForm.getDomain(), inv,
                        LoginStatusCode.FAILUNKNOWN);
            }

            if (this.remoteLogger.isEnable()) {
                int type= -1;
                String https=inv.getParameter("https");
                if(StringUtils.isEmpty(https)){
                	https="";
                }
                try{
                    type=inv.getRequest().getAttribute(LoginParamDef.ATTRIBUTE_LOGIN_SOURCE)==null?-1:((LoginSourceType)inv.getRequest().getAttribute(LoginParamDef.ATTRIBUTE_LOGIN_SOURCE)).getValue();
                }catch(final Exception e){
                    this.logger.error("", e);
                }
                if (pForm.isAutoLogin()) {
                    this.remoteLogger.info(LoginLoggerDef.loginLogPentterNewSsl, "loginAutoOk", userId,
                            inv.getRequest().getHeader("Referer"), pForm.getEmail(),ip, type, https);
                }
                this.remoteLogger.info(LoginLoggerDef.loginLogPentterNewSsl, "loginOk", userId, inv
                        .getRequest().getHeader("Referer"), pForm.getEmail(),ip, type, https);
            }
            // 生成T票 并跳转
            inv.addModel(LoginParamDef.ATTRIBUTE_LOGIN_STATUS_CODE, LoginStatusCode.LOGIN_OK);
            CaptchaAdapterFactory.getCaptchaAdapterInstance().processLoginCountAfterLoginSuccess(
                    inv.getRequest(), account, userId, ip);
            return this.succAndForward(inv, userId, pForm);
        } else {
            CaptchaAdapterFactory.getCaptchaAdapterInstance().processLoginCountAfterLoginFail(
                    inv.getRequest(), account, rt, ip);
            return this.failSwitcher.failSwitch(pForm.getDomain(), inv, rt);
        }
    }
    /**
     * currentDomainIsNotPassportDomain<br>
     * 
     * @param request
     * @return
     * 
     * @author tai.wang@opi-corp.com Jan 25, 2010 - 10:59:32 PM
     */
    private boolean currentDomainIsNotPassportDomain(final HttpServletRequest request) {
        return this.domainOprImpl.currentDomainIsNotPassportDomain(request);
    }
    /**
     * getPassportDomainAddrFromCurrentAddr<br>
     * 
     * @param inv
     * @param pForm
     * @return
     * 
     * @author tai.wang@opi-corp.com Jan 25, 2010 - 11:03:24 PM
     */
    private String getPpDomAddrFrCurAddr(final Invocation inv, final PassportForm pForm) {
        return this.domainOprImpl.getPassportDomainAddrFromCurrentAddr(inv, pForm.getOrigURL());
    }
    /**
     * getRtAddrForGrayIpOrWaringUser<br>
     * 
     * @param inv
     * @param pForm
     * 
     * @author tai.wang@opi-corp.com Jan 25, 2010 - 7:08:07 PM
     * @return
     */
    private LoginStatusCode getRtAddrForGrayIpOrWaringUser(final Invocation inv,
            final PassportForm pForm) {
        return this.grayIpAndWarningUserImpl.getRtAddrForGrayIpOrWaringUser(inv, pForm.getEmail(),
                pForm.getPassword(), pForm.getIcode());
    }

    /**
     * setAutoLoginTagInAttribute<br>
     * 
     * @param inv
     * @param pForm
     * 
     * @author tai.wang@opi-corp.com Jul 7, 2010 - 5:59:26 PM
     */
    private void setAutoLoginTagInAttribute(final Invocation inv, final PassportForm pForm) {
        if (pForm.isAutoLogin()) {
            inv.addModel(LoginParamDef.ATTRIBUTE_AUTOLOGIN, pForm.isAutoLogin());
        }
    }

    /**
     * setUp<br>
     * 在Get 和Post之前执行
     * 
     * @param inv
     * 
     * @author tai.wang@opi-corp.com Jul 27, 2010 - 12:21:14 PM
     */
    private void setUp(final Invocation inv) {
        this.ahead(inv.getRequest(), inv.getResponse());
    }
    
    /**
     * 登录成功,自动登陆有相关的处理，并跳转
     * 
     * @param inv {@link Invocation}
     * @param userid {@link Integer}
     * @param pForm {@link PassportForm}
     * @return
     */
    private String succAndForward(final Invocation inv, final int userid, final PassportForm pForm,final String account) {
        try {
            // 生成一个T票
            final String ticket = PassportManager.getInstance().createTicket(userid,
                    inv.getRequest());
            String origURL=pForm.getOrigURL();
            if(inv.getRequest().getAttribute("saftOrigURL")!=null){
                origURL=(String) inv.getRequest().getAttribute("saftOrigURL");
            }
            final String forward = this.passportTicketOpr.forwardSetTicket(inv.getRequest(),
                    ticket, pForm.getDomain(), origURL, pForm.getP3p(),
                    pForm.isAutoLogin(), this.needNotify());
            
            // 用户userId是否是当天第一次登陆，如果是就跳到登陆广告首页停留四秒，否则就跳到主工程首页
            // loginConfMap是从db读取过来的，有一个定时任务5分钟自动加载至loginConfMap中,
            // key:advertiseStartTime或advertiseEndTime,value:时间。
            // 当前时间是否在广告开始时间和结束时间之间，如果在就跳到登陆广告首页停留四秒，否则跳到主工程首页
            Map<String,String> loginConfMap = LoginConfigureImpl.getInstance().getLoginConfigure();
            if (OpiConstants.domainPassport.toString().equalsIgnoreCase(inv.getRequest().getHeader("host")) && loginConfMap != null && loginConfMap.size() != 0) {
            	
            	String startTime = loginConfMap.get(Globals.advertiseStartTime);
            	String endTime   = loginConfMap.get(Globals.advertiseEndTime);
            	if (StringUtils.isNotEmpty(startTime) && StringUtils.isNotEmpty(endTime)) {
	        		if (LoginDateTimeUtil.isBetweenDate(startTime,endTime) && !passportTicketOpr.isAutoLoginByDay(userid)) {
	        			
			        	StringBuilder strRedirect = new StringBuilder("r:");
			        	strRedirect.append(OpiConstants.urlWww)
						.append("/pages/autoLogin-ads.jsp")
			        		.append("?")
			        		.append("email=").append(account)
			        		.append("&mainPage=").append(URLEncoder.encode(forward, "UTF-8"))
			        		.append("&showBlackBoardAd=");
			        	
			        	/*
			        	 * 此值由广告部门每隔一分钟读DB写入XCache一次，已确保其持续可用。
			        	 * 在极端情况下，如果此值未能取到，默认显示小广告。
			        	 * 已与广告部门王丹确认，即使在投放大广告的时候，也会有小广告兜底。
			        	 */
			        	if (showBlackBoardAd == null) {
			        		showBlackBoardAd = 0;
			        	}
			        	strRedirect.append(showBlackBoardAd);
			        	
			        	return strRedirect.toString();
		        	}
            	}
            }
            
            return "r:" + forward;
        } catch (final Exception e) {
            e.printStackTrace();
            return "r:" + pForm.getDomain() + "/";
        }
        
        
    }

    /**
     * 登录成功并跳转
     * 
     * @param inv {@link Invocation}
     * @param userid {@link Integer}
     * @param pForm {@link PassportForm}
     * @return
     */
    private String succAndForward(final Invocation inv, final int userid, final PassportForm pForm) {
        try {
            // 生成一个T票
            final String ticket = PassportManager.getInstance().createTicket(userid,
                    inv.getRequest());
            String origURL=pForm.getOrigURL();
            if(inv.getRequest().getAttribute("saftOrigURL")!=null){
                origURL=(String) inv.getRequest().getAttribute("saftOrigURL");
            }
            final String forward = this.passportTicketOpr.forwardSetTicket(inv.getRequest(),
                    ticket, pForm.getDomain(), origURL, pForm.getP3p(),
                    pForm.isAutoLogin(), this.needNotify());
            
            return "r:" + forward;
        } catch (final Exception e) {
            e.printStackTrace();
            return "r:" + pForm.getDomain() + "/";
        }
        
        
    }

    /**
     * theTasksToRunAfterExchangeTicket<br>
     * 在换票后执行的任务
     * 
     * @param inv
     * @param hostid
     * 
     * @author tai.wang@opi-corp.com Jan 25, 2010 - 11:10:53 PM
     */
    private void theTasksToRunAfterExchangeTicket(final Invocation inv, final int hostid) {
        final String ip = BizFilterUtil.getClientIP(inv.getRequest());
        CheckLoginClient.getInstance().autologinCheck(hostid, "", "", ip, true,
                System.currentTimeMillis(), inv.getRequest());
        LoginLogicFactory.getPassportLogin().doTaskAfterLogin(inv.getRequest(), hostid);
    }

    /**
     * ahead<br>
     * 
     * @param request
     * @param response
     * 
     * @author tai.wang@opi-corp.com Jul 7, 2010 - 6:01:10 PM
     */
    protected abstract void ahead(final HttpServletRequest request,
            final HttpServletResponse response);

    /**
     * 获取客户端的原始ip
     * @param inv
     * @return
     */
    protected String getClientIpFromRequest(final Invocation inv){
        final RemoteAddrFetherImpl addrFetherImpl = new RemoteAddrFetherImpl();
        final String ip = addrFetherImpl.getRemoteAddr(inv.getRequest());
        return ip;
    }

    /**
     * getRedirectUrl<br>
     * 
     * @param request
     * @return
     * 
     * @author tai.wang@opi-corp.com Jul 7, 2010 - 6:01:16 PM
     */
    protected String getRedirectUrl(final HttpServletRequest request) {
        final String homeURL = OpiConstants.urlHome + "/Home.do";
        String origURL = request.getParameter(LoginParamDef.PARAM_ORIGURL);
        origURL = (origURL != null) && (origURL.length() > 0) ? origURL : homeURL;
        if (origURL.contains(OpiConstants.urlLogin + "/GLogin.do")
                // 暂时删了 || origURL.contains("SysHome.do") // 不跳回主页
                || origURL.contains(OpiConstants.urlPassport + "/PLogin.do")) {
            origURL = homeURL;
        }
        return origURL;
    }

    /**
     * 获取用户id从request中
     * 
     * @param inv
     * @return userId
     */
    protected Integer getUserId(final Invocation inv) {
        Integer userId = (Integer) inv.getRequest().getAttribute(LoginParamDef.ATTRIBUTE_USER_ID);
        if (userId == null) {
            final User user = BaseThreadUtil.currentHost(inv.getRequest());
            if (null != user) {
                userId = user.getId();
            }
        }
        return userId;
    }

    /**
     * needNotify<br>
     * 
     * @return
     * 
     * @author tai.wang@opi-corp.com Jul 7, 2010 - 6:01:21 PM
     */
    protected abstract boolean needNotify();

    /**
     * setUpFailSwitcher<br>
     * 登陆失败链
     * 
     * @param failSwitcher {@link ILoginFailSwitch} input
     * 
     * @author tai.wang@opi-corp.com Jul 27, 2010 - 12:16:01 PM
     */
    protected abstract void setUpFailSwitcher(final ILoginFailSwitch failSwither);

    /**
     * setUpResultChains<br>
     * 
     * @param resultChains
     * 
     * @author tai.wang@opi-corp.com Aug 2, 2010 - 11:50:39 AM
     */
    protected abstract void setUpResultChains(final LoginResultChains resultChains);

}
