package com.xiaonei.passport.web.ticket;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

import com.renren.passport.service.ILoginService;
import com.renren.passport.service.LoginContext;
import com.renren.passport.service.LoginResponse;
import com.renren.passport.service.LoginType;
import com.renren.passport.service.TicketInfo;
import com.renren.passport.service.TransferRequest;
import com.xiaonei.passport.constants.Globals;
import com.xiaonei.passport.web.adapt.PassportAdaptFactory;
import com.xiaonei.passport.web.constants.LoginCookieDef;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;
import com.xiaonei.passport.web.util.LoginContextUtil;
import com.xiaonei.platform.core.utility.Encoder;
import com.xiaonei.sns.platform.core.opt.base.CookieManager;

@Service
public class PassportTicketOprImpl implements PassportTicketOpr {

    //    public static int TAIL_NUM = -1;


    protected Log logger = LogFactory.getLog(this.getClass());
	
	private ILoginService loginService=PassportAdaptFactory.getLoginService();

    @Override
    public String getPassportFromCookie(final HttpServletRequest request) {
        return CookieManager.getInstance().getCookie(request, LoginCookieDef.COOKIE_PASSPORT);
    }

    @Override
    public LoginResult getUserIdByPassport(HttpServletRequest request, HttpServletResponse response, final PassportForm pForm, LoginType loginType){
        final String passport = this.getPassportFromCookie(request);
        // passport无效
        if (passport == null) {
            logger.error("there is no cookie named 'passport' in the request.. so return");
        	return null;
        }  
        final String autoLogin = CookieManager.getInstance().getCookie(request, LoginCookieDef.AUTOLOGIN);
        //TODO 自动登录这里需要处理吗
        
        
        LoginContext context=LoginContextUtil.getLoginContext(request);
        TransferRequest transferRequest=new TransferRequest();
        transferRequest.setLoginRegistrationId(pForm.getLoginRegistrationId());
        transferRequest.setLoginType(loginType);
        transferRequest.setOrigUrl(pForm.getOrigURL());
        transferRequest.setPassport(passport);
        transferRequest.setUserName(pForm.getEmail());
        LoginResponse result=loginService.transfer(transferRequest, context);
        if(result==null){
        	return null;
        }
        return new LoginResult(result);
    }



    @Override
    public void savePassport2Cookie(final int userId, final HttpServletRequest request,
            final HttpServletResponse response, final String passport, final String oldPassport,final boolean isPersistent) {
    	if (isPersistent) {
            CookieManager.getInstance().saveCookie(response, LoginCookieDef.COOKIE_PASSPORT,
            		passport, 30 * 24 * 60 * 60, "/", Globals.RENREN_DOMAIN);
            
            //ap票种在renren.com下(在自动登录过拦截器时才可以取到此票)
            CookieManager.getInstance().saveCookie(response, LoginCookieDef.AUTOLOGIN, "" + userId,
                    30 * 24 * 60 * 60, "/", Globals.RENREN_DOMAIN);
            if(!StringUtils.isEmpty(oldPassport)){
            	CookieManager.getInstance().saveCookie(response, LoginCookieDef.COOKIE_PASSPORT_OLD,
                		oldPassport, 30 * 24 * 60 * 60, "/", Globals.PASSPORT_DOMAIN_OLD);
                CookieManager.getInstance().saveCookie(response, LoginCookieDef.AUTOLOGIN, "" + userId,
                        30 * 24 * 60 * 60, "/", Globals.PASSPORT_3RD_DOMAIN_OLD);
            }
        } else {
            CookieManager.getInstance().saveCookie(response, LoginCookieDef.COOKIE_PASSPORT,
            		passport, -1, "/", Globals.RENREN_DOMAIN);
            
            CookieManager.getInstance().saveCookie(response, LoginCookieDef.AUTOLOGIN, "" + userId,
                    -1, "/", Globals.RENREN_DOMAIN);
            
            if(!StringUtils.isEmpty(oldPassport)){
            	CookieManager.getInstance().saveCookie(response, LoginCookieDef.COOKIE_PASSPORT_OLD,
            			oldPassport, -1, "/", Globals.PASSPORT_DOMAIN_OLD);
            }
        }
        CookieManager.getInstance().saveCookie(response, LoginCookieDef.FIRST_LOGIN_FLAG, "1", -1, "/", Globals.RENREN_DOMAIN);
    }
  

    @Override
    public int setTicket(final String ticket, final String oldTicket,  final boolean autoLogin,
            final HttpServletRequest request, final HttpServletResponse response) {
        try {
            if ((ticket != null) && (ticket.trim().length() > 0)) {
                // Webpager用
                Long id = loginService.verifyTicket(ticket, LoginType.Web);
                int hostid=id.intValue();
                // 无条件写入ticket
                CookieManager.getInstance().saveCookie(response, LoginCookieDef.loginKey_ticket,
                        ticket, -1, "/", "." + Globals.RENREN_DOMAIN);
                CookieManager.getInstance().saveCookie(response, LoginCookieDef.loginKey_ticket,
                        Encoder.encode(ticket), -1, "/xtalk/", "."+Globals.RENREN_DOMAIN);
                
                if(!StringUtils.isEmpty(oldTicket)){
                	CookieManager.getInstance().saveCookie(response,
                            LoginCookieDef.loginKey_old_ticket, oldTicket, -1, "/",
                            "." +Globals.RENREN_DOMAIN);
                	CookieManager.getInstance().saveCookie(response, LoginCookieDef.loginKey_ticket_old,
                            oldTicket, -1, "/", "." + Globals.RENREN_DOMAIN);
                    CookieManager.getInstance().saveCookie(response, LoginCookieDef.loginKey_ticket_old,
                            Encoder.encode(oldTicket), -1, "/xtalk/", "."+Globals.RENREN_DOMAIN);
                }
                // 他们要的(webpager JS FE)
                CookieManager.getInstance().saveCookie(response, LoginCookieDef.ID, "" + hostid);
                // 黄欢要的
                CookieManager.getInstance().saveCookie(response, LoginCookieDef.XNSID,
                        Integer.toHexString((int) (System.currentTimeMillis() ^ -1)));
                return hostid;
            }
        } catch (final Exception e) {
            e.printStackTrace();
        }
        return 0;
    }

    @Override
    public TicketInfo ticket2Id(final String passport) {
        try {
            TicketInfo ticket= loginService.getTicketInfo(passport);
            if(ticket!=null){
            	return ticket;
            }
            return null;
        } catch (final Exception e) {
            e.printStackTrace();
            return null;
        }
    }

}
