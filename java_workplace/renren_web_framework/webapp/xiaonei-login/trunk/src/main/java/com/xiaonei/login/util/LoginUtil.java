package com.xiaonei.login.util;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.xce.passport.Account;

/**
 * LoginUtil
 * 
 * @deprecated 消灭上帝类
 * @see com.xiaonei.login.constants.LoginStatusCode
 * @see com.xiaonei.login.task.GrayListLoadingTask
 * @see com.xiaonei.login.logic.DomainOpr
 * @see com.xiaonei.login.logic.LoginStatusOpr
 * @see com.xiaonei.login.logic.ticket.PassportTicketOpr
 * @see com.xiaonei.login.logic.status.Validation
 * 
 * @author tai.wang@opi-corp.com Dec 11, 2009 - 3:26:34 PM
 */
@Deprecated
public class LoginUtil {

    /**
     * 检查用户的安全状态
     * 
     * @param user
     * @return
     */
    public static boolean checkUserSafeStatus(final User user) {
        return LoginLogicFactory.getValidation().checkUserSafeStatus(user);
    }

    /**
     * 检查当前用户的状态，返回对应的状态码<br>
     * 返回的状态码大于零，表示用户处于不正常的状态应该禁止登录<br>
     * 状态码等于零表示正常状态
     * 
     * @param user
     * @return
     */
    public static int checkUserStatus(final Account account) {
        return LoginLogicFactory.getValidation().checkUserStatus(account).getCode();
    }

    public static String forwardSetTicket(final HttpServletRequest request, final String ticket,
            final String domain, final boolean needNotify) {
        return LoginLogicFactory.getPassportTicketOpr().forwardSetTicket(request, ticket, domain,
                request.getParameter(LoginParamDef.PARAM_ORIGURL), request.getParameter("p3p"),
                Boolean.valueOf(request.getParameter(LoginParamDef.PARAM_AUTOLOGIN)), needNotify);
    }

    public static String getDomainParam(final HttpServletRequest request) {
        return LoginLogicFactory.getDomainOpr().getDomainParam(
                request.getParameter(LoginParamDef.PARAM_DOMAIN), request);
    }

    /**
     * 根据状态码返回对应的状态码描述信息
     * 
     * @param status
     * @return
     */
    public static String getErrorMessage(final int status) {
        for (final LoginStatusCode value : LoginStatusCode.values()) {
            if (status == value.getCode()) {
                return LoginLogicFactory.getLoginStatusOprImpl().getErrorMessage(value);
            }
        }
        return LoginLogicFactory.getLoginStatusOprImpl().getErrorMessage(
                LoginStatusCode.FAILUNKNOWN);
    }

    public static String getPassportFromCookie(final HttpServletRequest request) {
        return LoginLogicFactory.getPassportTicketOpr().getPassportFromCookie(request);
    }

    /**
     * 通过票获取用户id
     * 
     * @param request
     * @param response
     * @param passportCookieName
     * @return
     */
    public static int getUserIdByPassport(final HttpServletRequest request,
            final HttpServletResponse response) {
        return LoginLogicFactory.getPassportTicketOpr().getUserIdByPassport(request, response);
    }

    /**
     * 判断ip地址是否被封禁
     * 
     * @param ip
     * @return
     */
    public static boolean isBanIP(final String ip) {
        return LoginLogicFactory.getValidation().isBanIP(ip);
    }

    /**
     * 判断是否是有效的email地址
     * 
     * @param mailAddr
     * @return
     */
    public static boolean isValidEmailAddr(final String mailAddr) {
        return LoginLogicFactory.getValidation().isValidEmailAddr(mailAddr);
    }

    /**
     * 判断是否是合法的手机号码
     * 
     * @param mobile
     * @return
     */
    public static boolean isValidMobile(final String mobile) {
        return LoginLogicFactory.getValidation().isValidMobile(mobile);
    }

    public static boolean isWarningUser(final String username, final String password) {
        return LoginLogicFactory.getValidation().isWarningUser(username, password);
    }

    public static void savePassport2Cookie(final HttpServletRequest request,
            final HttpServletResponse response, final String passport, final boolean isPersistent) {
        LoginLogicFactory.getPassportTicketOpr().savePassport2Cookie(0, request, response,
                passport, isPersistent);
    }

    public static void setDomainEnv(final HttpServletRequest request) {
        LoginLogicFactory.getDomainOpr().setDomainEnv(request);
    }

    public static void setDomainEnv(final HttpServletRequest request, final String domain) {
        LoginLogicFactory.getDomainOpr().setDomainEnv(request, domain);
    }

    public static int setTicket(final String ticket, final boolean autoLogin,
            final HttpServletRequest request, final HttpServletResponse response) {
        return LoginLogicFactory.getPassportTicketOpr().setTicket(ticket, autoLogin, request,
                response);
    }

    public static void warningWeakPassword(final int id, final HttpServletRequest request) {
        LoginLogicFactory.getValidation().setWarningTagOfWeakPasswordInAttribut(request);
    }

}
