package com.xiaonei.passport.web.passport;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

import com.renren.passport.service.ILoginService;
import com.renren.passport.service.LoginContext;
import com.renren.passport.service.LoginRequest;
import com.renren.passport.service.LoginResponse;
import com.xiaonei.passport.constants.Globals;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.logger.LoginLoggerDef;
import com.xiaonei.passport.logger.PpLogger;
import com.xiaonei.passport.web.adapt.PassportAdaptFactory;
import com.xiaonei.passport.web.constants.LoginCookieDef;
import com.xiaonei.passport.web.constants.LoginParamDef;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;
import com.xiaonei.passport.web.util.LoginContextUtil;
import com.xiaonei.platform.core.utility.Encoder;
import com.xiaonei.sns.platform.core.opt.base.CookieManager;

@Service
public class PassportLoginImpl implements PassportLogin {

    private ILoginService loginService=PassportAdaptFactory.getLoginService();

    protected final Log logger = LogFactory.getLog(this.getClass());

    protected PpLogger remoteLogger = LoginLoggerDef.login;

    private static final Pattern p = Pattern.compile("\\w+");
   

	@Override
	public LoginResult login(PassportForm pForm, boolean createRenRenTicket,
			HttpServletRequest request) {
		final String pwd = pForm.getPassword();
        final boolean isPasswordEncode = request.getAttribute(LoginParamDef.PARAM_ENPWD) != null ? true
                : false;
        final String password = isPasswordEncode ? pwd : Encoder.encode(pwd);
        request.setAttribute(LoginParamDef.ATTRIBUTE_ENCODE_PASSWORD, password);
        String ick=CookieManager.getInstance().getCookie(request, LoginCookieDef.ICODE);
		return login(pForm.getEmail(), password, pForm.isAutoLogin(), pForm.getOrigURL(), pForm.getIcode(), pForm.getCaptchaType(), ick, pForm.getDomain(),Long.valueOf(pForm.isAutoLogin() ? -1 : 6 * 3600).longValue(), pForm.getLoginRegistrationId(), true, request);
	}
	
    @Override
	public LoginResult login(String userName, String md5Password,
			boolean isAutoLogin, String origUrl, String icode,
			String captchaType, String ick, String domain, long expirePeriod,
			int loginRegistrationId, boolean createRenRenTicket,
			HttpServletRequest request) {
    	LoginContext context=LoginContextUtil.getLoginContext(request);
    	LoginResult loginResult=null;
    	 if (Globals.C56_DOMAIN.equals(domain)) { //56登陆的先验证56，否则先验证人人
    		 loginResult = this.process56User(loginResult, userName, md5Password, isAutoLogin, origUrl, icode, captchaType, ick, domain, expirePeriod, loginRegistrationId, createRenRenTicket, request, context);
    		 loginResult = this.processNon56User(loginResult, userName, md5Password, isAutoLogin, origUrl, icode, captchaType, ick, domain, expirePeriod, loginRegistrationId, createRenRenTicket, request, context);
         } else {
        	 loginResult = this.processNon56User(loginResult, userName, md5Password, isAutoLogin, origUrl, icode, captchaType, ick, domain, expirePeriod, loginRegistrationId, createRenRenTicket, request, context);
             loginResult = this.process56User(loginResult, userName, md5Password, isAutoLogin, origUrl, icode, captchaType, ick, domain, expirePeriod, loginRegistrationId, createRenRenTicket, request, context);
         }
    	 return loginResult;
	}
    /**
     * 处理56冲突的用户
     * 
     * @author Wang Tai (cn.wang.tai@gmail.com)
     * @date Dec 13, 2011 2:05:21 PM
     * 
     * @param account
     * @param hasVirtual
     * @param domain
     * @param isAutoLogin
     * @param passwordEncode
     * @param email
     * @return
     */
    private LoginResult process56User(LoginResult result, String userName, String md5Password,
			boolean isAutoLogin, String origUrl, String icode,
			String captchaType, String ick, String domain,long expirePeriod,
			int loginRegistrationId, boolean createRenRenTicket,
			HttpServletRequest request, LoginContext context) {
        if ((result != null) && LoginStatusCode.LOGIN_OK.getCode()==result.getLoginStatusCode()) {
            return result;
        }
        final Matcher m = PassportLoginImpl.p.matcher(userName);
        if (!m.matches()) {
            return result;
        }
        return this.login(userName, md5Password, isAutoLogin, origUrl, icode, captchaType, ick, expirePeriod, loginRegistrationId, createRenRenTicket, context);
    }

    /**
     * 处理非56帐号
     * 
     * @author Wang Tai (cn.wang.tai@gmail.com)
     * @date Dec 13, 2011 4:29:31 PM
     * 
     * @param account
     * @param hasVirtual
     * @param domain
     * @param isAutoLogin
     * @param passwordEncode
     * @param email
     * @return
     */
    private LoginResult processNon56User(LoginResult result, String userName, String md5Password,
			boolean isAutoLogin, String origUrl, String icode,
			String captchaType, String ick, String domain,long expirePeriod,
			int loginRegistrationId, boolean createRenRenTicket,
			HttpServletRequest request, LoginContext context) {
    	 if ((result != null) &&  LoginStatusCode.LOGIN_OK.getCode()==result.getLoginStatusCode()) {
             return result;
         }
        return this.login(userName, md5Password, isAutoLogin, origUrl, icode, captchaType, ick, expirePeriod, loginRegistrationId, createRenRenTicket, context);

    }
 
    /**
     * 包装接口
     * @param userName
     * @param md5Password
     * @param isAutoLogin
     * @param origUrl
     * @param icode
     * @param captchaType
     * @param ick
     * @param expirePeriod
     * @param loginRegistrationId
     * @param createRenRenTicket
     * @param context
     * @return
     */
    private LoginResult login(String userName, String md5Password,
			boolean isAutoLogin, String origUrl, String icode,
			String captchaType, String ick,long expirePeriod,
			int loginRegistrationId, boolean createRenRenTicket,LoginContext context){
    	LoginRequest request=new LoginRequest();
    	request.setAutoLogin(isAutoLogin);
    	request.setCaptchaType(captchaType);
    	request.setCreateRenRenTicket(createRenRenTicket);
    	request.setExpirePeriod(expirePeriod);
    	request.setIck(ick);
    	request.setIcode(icode);
    	request.setLoginRegistrationId(loginRegistrationId);
    	request.setMd5Password(md5Password);
    	request.setOrigUrl(origUrl);
    	request.setUserName(userName);
    	if(ick == null && logger.isErrorEnabled()) {
    	    logger.error("ick param is null cause error");
    	}
    	LoginResponse response=loginService.login(request, context);
    	LoginResult loginResult=new LoginResult(response);
    	return loginResult;
    }
}
