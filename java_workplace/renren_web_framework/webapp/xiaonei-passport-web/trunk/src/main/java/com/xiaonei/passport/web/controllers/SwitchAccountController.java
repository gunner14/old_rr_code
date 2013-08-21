package com.xiaonei.passport.web.controllers;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.sf.json.JSONObject;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;

import com.renren.passport.service.ILoginService;
import com.renren.passport.service.LoginContext;
import com.renren.passport.service.LoginResponse;
import com.renren.passport.service.SwitchRequest;
import com.renren.passport.service.TicketInfo;
import com.xiaonei.passport.constants.Globals;
import com.xiaonei.passport.constants.LoginConfigureKey;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.constants.MultiAccountStatus;
import com.xiaonei.passport.constants.TicketType;
import com.xiaonei.passport.ticket.TicketServiceImpl;
import com.xiaonei.passport.util.UrlBuilder;
import com.xiaonei.passport.web.account.SwitchAccount;
import com.xiaonei.passport.web.adapt.PassportAdaptFactory;
import com.xiaonei.passport.web.constants.LoginCookieDef;
import com.xiaonei.passport.web.constants.LoginParamDef;
import com.xiaonei.passport.web.event.EventProcess;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;
import com.xiaonei.passport.web.util.AjaxWriter;
import com.xiaonei.passport.web.util.LoginContextUtil;
import com.xiaonei.passport.web.util.OrigUrlUtil;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.passport.PassportAdapterFactory;


@Controller
public class SwitchAccountController {

    protected Log logger = LogFactory.getLog(this.getClass());
    
    private ILoginService loginService=PassportAdaptFactory.getLoginService();
    
    @Autowired
    private SwitchAccount switchAccount;
    
    @RequestMapping(value = { "/switchAccount" }, method = RequestMethod.POST)
    public String post(@RequestParam(value = "origUrl", required = false) String origUrl,
            @RequestParam(value = "destId", required = true) Integer destId,
            final HttpServletRequest request,
            final HttpServletResponse response,//
            final PassportForm pForm) {
        JSONObject jsonObject = new JSONObject();
        
        int ticketId = getTicketId(request);
        
        final String passportTicket = CookieManager.getInstance().getCookie(request,
                LoginCookieDef.COOKIE_PASSPORT);
        
        TicketInfo info=loginService.getPassportInfo(passportTicket);
        
        int realUserId = info.getRealUserId();
        if(realUserId <= 0 ){
            realUserId = info.getUserId();
            if(realUserId <= 0) {
                if(logger.isErrorEnabled()) {
                    logger.error("real user id can not less than 0, error!");
                }
                return null; 
            }
        }
        User realUser = SnsAdapterFactory.getUserAdapter().get(realUserId);
        if(realUser == null){
            if(logger.isErrorEnabled()) {
                logger.error("real user is null, error!");
            }
            return null;
        }
        MultiAccountStatus multiStatus = switchAccount.getMultiAccountStatus(realUser);
        //如果没有多账号
        if (multiStatus.is(MultiAccountStatus.SINGLE)) {
            if(logger.isErrorEnabled()) {
                logger.error("user account is single account!");
            }
            return null;
        }
        
        User destUser = switchAccount.getReallyUser(destId);
        Integer destRealId = switchAccount.getTransUserId(destId);
        
        //获得成功跳转url
        origUrl = OrigUrlUtil.changeOrigUrl(origUrl, destRealId);
        
        //1、判断这个账号是否存在
        if (destUser == null) {
            if(logger.isErrorEnabled()) {
                logger.error("dest user no exist!");
            }
            return null;
        }
        //2、判断这个账号是否是关联账号,其实你也看到了，这里没判断他俩是否是有关系:(
        if(realUser.getId() == destUser.getId() || destUser.getPassword_md5().equals(realUser.getPassword_md5())){
            //一个爹,或者双账号还密码相同，
            jsonObject.put("isJump", true);
            jsonObject.put("isShow", false);
            
            if (!StringUtils.equals(destUser.getPassword_md5(), realUser.getPassword_md5())) {
                UrlBuilder ub = new UrlBuilder(origUrl);
                jsonObject.put("url", ub.toString());
                String resultUrl = jsonObject.toString();
                AjaxWriter.write(resultUrl, response);
                return null;
            }
            
            LoginResult loginResult = switchPath(pForm, destId, ticketId, destUser.getPassword_md5(), true, request);
            UrlBuilder ub = new UrlBuilder(origUrl);
            if(loginResult.getLoginStatusCode() == LoginStatusCode.LOGIN_OK.getCode()) {
                EventProcess.getInstance().fireLoginSuccessEvent(request, response, pForm, loginResult, LoginConfigureKey.WEB_SWITCH_SUCCESS_LOGIN_EVENT);
                String successString=createSuccessString(loginResult, new UrlBuilder(origUrl));
                jsonObject.put("url", successString);
                String resultUrl = jsonObject.toString();
                AjaxWriter.write(resultUrl, response);
                return null;
            } else {
                if(logger.isWarnEnabled()) {
                    logger.warn("switch account service error!");
                }
                jsonObject.put("url", ub.getUrl());
                String resultUrl = jsonObject.toString();
                AjaxWriter.write(resultUrl, response);
                return null;
            }
            
        }else{
            jsonObject.put("isJump", false);
            jsonObject.put("isShow", true);
            //判断是否需要验证码
            MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(
                    MemCacheKeys.pool_home);
            Object o = mem.get("switch_pwd_" + ticketId);
            if (o == null) {
                jsonObject.put("showIC", false);
            } else {
                jsonObject.put("showIC", true);
            }
            jsonObject.put("domain", destUser.getDomain());
            jsonObject.put("name", destUser.getName());
            jsonObject.put("account", getMaskAccount(destUser));
            
            String resultUrl=jsonObject.toString();            
            
            AjaxWriter.write(resultUrl, response);
            return null;
        }
    }
    
    private LoginResult switchPath(final PassportForm pForm, Integer destId, int userId, String md5Password, boolean createRenRenTicket, final HttpServletRequest request) {
        final String ticket = CookieManager.getInstance().getCookie(request,
                LoginCookieDef.loginKey_ticket);
        final String passportTicket = CookieManager.getInstance().getCookie(request,
                LoginCookieDef.COOKIE_PASSPORT);
       
        request.setAttribute(LoginParamDef.ATTRIBUTE_ENCODE_PASSWORD, md5Password);
        String ick=CookieManager.getInstance().getCookie(request, LoginCookieDef.ICODE);
       
        return switchPath(passportTicket, ticket, destId, userId, md5Password, pForm.getLoginRegistrationId(), pForm.getIcode(), pForm.getCaptchaType(), ick, createRenRenTicket,request);
    }
    
    private LoginResult switchPath(String passport, String ticket,
            int destId, int realUserId, String md5Password,
            int registrationId,String iCode, String captchaType, 
            String ick, boolean createRenRenTicket,
            HttpServletRequest request) {
        
        SwitchRequest switchRequest=new SwitchRequest();
        switchRequest.setPassport(passport);
        switchRequest.setTicket(ticket);
        switchRequest.setDestId(destId);
        switchRequest.setRealUserId(realUserId);
        switchRequest.setMd5Password(md5Password);
        switchRequest.setLoginRegistrationId(registrationId);
        switchRequest.setIcode(iCode);
        switchRequest.setCaptchaType(captchaType);
        switchRequest.setIck(ick);
        switchRequest.setCreateRenRenTicket(createRenRenTicket);
        
        LoginContext context=LoginContextUtil.getLoginContext(request);
        LoginResponse response = loginService.switchAccount(switchRequest, context);
        LoginResult loginResult=new LoginResult(response);
        return loginResult;
    
    }
    
    /**
     * 用于显示在输入密码框前，给个提示，要转到的账号长啥样
     * @param user
     * @return
     */
    private String getMaskAccount(User user) {
        String account = user.getAccount();
        if (account.length() > 6) {
            account = account.substring(0, 2) + "******" + account.substring(account.length() - 3);
        } else {
            account = account.substring(0, 1) + "******" + account.substring(account.length() - 2);
        }
        return account;
    }
    
    
    private String createSuccessString(LoginResult loginResult, UrlBuilder urlBuilder) {
        final String oldPassportTicket = loginResult.getLoginResponse().getOldPassport();
        final String oldTicket = loginResult.getLoginResponse().getOldTicket();
        StringBuilder sb = new StringBuilder("http://www." + Globals.RENREN_DOMAIN + "/cp/j1/" + oldPassportTicket + "/" + oldTicket
                + "/?origUrl=" + urlBuilder.getEncode());
       
        return sb.toString();
    }
    
    
    private int getTicketId(HttpServletRequest request) {
        int ticketId = 0;
        final String passport = CookieManager.getInstance().getCookie(request, LoginCookieDef.COOKIE_PASSPORT_OLD);
        if (null == passport) {
            //p票没有
            return ticketId;
        }
        ticketId = PassportAdapterFactory.getWebLogin().ticket2Id(passport);
        if (ticketId <= 0) {
            //没有这个用户
            return 0;
        }
        return ticketId;
    }
}
