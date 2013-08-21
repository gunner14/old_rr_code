package com.xiaonei.login.http;

import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.aegis.login.client.CheckLoginClient;
import com.xiaonei.icode.verify.CodeUtil;
import com.xiaonei.login.constants.LoginCookieDef;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.login.task.set.FlyerTaskSet;
import com.xiaonei.login.task.set.LogTaskSet;
import com.xiaonei.login.task.task.FeedTask;
import com.xiaonei.login.util.StatUtil;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.utility.DES_Encrypt;
import com.xiaonei.platform.core.utility.Escape;

/**
 * LoginAction 的 Web 专用版本<br>
 * Web 版本的主要逻辑为一下几个部分<br>
 * <ul>
 * <li>判断用户的输入合法性</li>
 * <li>用户登录数据的一些 Log 操作</li>
 * <li>对重定向参数 origURL 的过滤与处理</li>
 * <li><b>登录的必要流程</b></li>
 * <li>登录车成功后的重定向处理</li>
 * <li>登录失败后的重定向</li>
 * </ul>
 * 
 * @author blackwing
 * 
 */
public class WebLoginAction extends AbstractStructAction {

    public WebLoginAction() {
        // web 的自动登录需要我们保留优先使用旧票的
        super(true);
    }

    @Override
    public String getActionName() {
        return "PLogin.do";
    }

    @Override
    protected ActionForward afterLogin(final ActionMapping mapping, final ActionForm form,
            final HttpServletRequest request, final HttpServletResponse response) throws Exception {
        // 可以顺利登录成功后还会涉及用户的帐号是否存在安全问题的部分
        // 此逻辑的具体代码之后会在此处体现
        Integer user_id = null;
        final User user = BaseThreadUtil.currentHost(request);
        if (user == null) {
            user_id = (Integer) request.getAttribute(LoginAction.USER_ID);
            // if (user_id != null) {
            // try {
            // user = WUserAdapter.getInstance().get(user_id);
            // } catch (Exception e) {
            // e.printStackTrace();
            // }
            // }
        } else {
            user_id = user.getId();
        }

        this.setNextStep(request, false);
        if (user_id != null) {
            final Runnable flyerTask = new FlyerTaskSet(user_id,
                    BizFilterUtil.getClientIP(request), request
                            .getParameter(LoginParamDef.PARAM_DOMAIN));
            executor.submit(flyerTask);
            executor.submit(new FeedTask(user_id, BizFilterUtil.getClientIP(request)));
            // 生成T票 并跳转
            request.setAttribute(PARAM_ORIGURL, this.getRedirectUrl(request));
            return this.sendRedirect2Dest(user_id, request, response, this.needNotify());
        } else {
            request.setAttribute(PARAM_ORIGURL, this.getRedirectUrl(request));
            return this.fail(mapping, form, request, response, LoginStatusCode.FAILUNKNOWN
                    .getCode());
        }
    }

    @Override
    protected ActionForward beforeLogin(final ActionMapping mapping, final ActionForm form,
            final HttpServletRequest request, final HttpServletResponse response) throws Exception {
        final StatUtil su = new StatUtil(_Tstat);

        // 从Cookie验票过程
        final ActionForward forward = this.getRequest(mapping, form, request, response);
        if (forward != null) {
            return forward;
        }

        this.setUserName(request, request.getParameter(PARAM_EMAIL));
        this.setPassword(request, request.getParameter(PARAM_PASSWORD));

        // 记住用户名，用于下次在输入框自动显示
        final String userName = Escape.trim(request.getParameter(PARAM_EMAIL));
        if (userName != null && userName.length() > 0) {
            final Cookie deUserName = new Cookie("_de", DES_Encrypt.encrypt(userName));
            deUserName.setDomain("." + OpiConstants.domain);
            deUserName.setMaxAge(60 * 60 * 24 * 30 * 12);// one year
            response.addCookie(deUserName);
        }

        final boolean isGrayIp = false;
        // if (false) {
        // // ip灰名单策略
        // MemCacheManager manager =
        // MemCacheFactory.getInstance().getManagerByPoolName(
        // MemCacheKeys.pool_user_space);
        // if (manager.get("GrayIp_" + BizFilterUtil.getClientIP(request)) !=
        // null) {
        // isGrayIp = true;
        // }
        // su.StepTimeCost("POST:WebLoginAction:isGrayIp");
        // }

        su.stepTimeCost("POST:WebLoginAction:init");
        // 根据用户的输入用户名来判断用户是否为警告用户
        if (isGrayIp
                || LoginLogicFactory.getValidation().isWarningUser(
                        request.getParameter(PARAM_EMAIL), request.getParameter(PARAM_PASSWORD))) {
            su.stepTimeCost("POST:WebLoginAction:isWarning");

            request.setAttribute("needValidate", true);
            // 验证码检验逻辑
            final String ick = CookieManager.getInstance().getCookie(request, LoginCookieDef.ICODE);
            final String icode = request.getParameter("icode");
            if (ick != null && icode != null) {
                // 用户提交了验证码
                System.err.println(request.getParameter(PARAM_EMAIL) + " UserGray:" + icode);
                if (!CodeUtil.getInstance().validCode(icode, "vlcode", ick)) {
                    // 验证码输入错误
                    this.setNextStep(request, false);
                    return this.fail(mapping, form, request, response,
                            LoginStatusCode.FAILVALIDATELOGIN.getCode());
                }
                su.stepTimeCost("POST:WebLoginAction:checkIcode");
                // 验证码输入正确
            } else {
                // 灰名单用户登录
                final String msg = request.getParameter(PARAM_EMAIL) + " user need validate "
                        + BizFilterUtil.getClientIP(request);
                _log.error(msg);
                System.err.println(msg);
                this.setNextStep(request, false);
                return this.fail(mapping, form, request, response,// NL
                        LoginStatusCode.FAILNOTIFYGRAYUSER.getCode());
            }
        }
        // 验证密码前还会涉及用户帐号安全登录的部分
        // 此逻辑的具体代码之后会在此处体现

        return null;
    }

    @Override
    protected ActionForward fail(final ActionMapping mapping, final ActionForm form,
            final HttpServletRequest request, final HttpServletResponse response, final int failCode)
            throws Exception {
        final StatUtil su = new StatUtil();
        final String fail = "fail";
        LoginLogicFactory.getDomainOpr().setDomainEnv(request);
        if (failCode == LoginStatusCode.FAILBANIP.getCode()) {
            // IP被封禁可能会有独立页面显示的需求
            this.addErrorMessage(request, "您的IP处于被禁用状态");
        } else if (failCode == LoginStatusCode.FAILUSERNOTEXIST.getCode()) {
            if (OpiConstants.isKaixin()) {
                // 开心用户不存在失败逻辑
                try {
                    // 保存用户输入的东西
                    final MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(
                            MemCacheKeys.pool_default);
                    final String sessionid = UUID.randomUUID().toString();
                    final String key = "loginfail_usernofound_" + sessionid;
                    final Map<String, String> params = new HashMap<String, String>();
                    for (final Object param : request.getParameterMap().keySet()) {
                        params.put(param.toString(), request.getParameter(param.toString()));
                    }
                    mem.set(key, params, 60);
                    su.stepTimeCost("KXUserNotExistFailSetUserInfo", this.getActionName());
                    // 跳转到注册的公用页面
                    return new ActionForward(OpiConstants.urlRegJump + "/kx.do?ss=10122&rt=72&f="
                            + sessionid, true);
                } catch (final Exception e) {
                    e.printStackTrace();
                }
            }
            this.addErrorMessage(request, "登录帐号或密码错误，请重试");
        } else if (failCode == LoginStatusCode.FAILINPUT.getCode()) {
            this.addErrorMessage(request, "登录帐号或密码错误，请重试");
        } else if (failCode == LoginStatusCode.FAILNOPOST.getCode()) {
            // 以Get方式获取
            this.addWarningMessage(request, "为保护用户信息需要登录才能执行此操作，请先登录");
        } else if (failCode == LoginStatusCode.FAILUSER.getCode()) {
            // 原代码中此处是从资源文件获取该信息的
            this.addErrorMessage(request, "登录帐号或密码错误，请重试");
        } else if (failCode == LoginStatusCode.FAILUSERACTIVE.getCode()) {
            final String email = Escape.trim(request.getParameter(PARAM_EMAIL));
            final StringBuffer ermsg = new StringBuffer();
            ermsg.append("帐号尚未激活，激活后才可登录。<a href='" + OpiConstants.urlReg
                    + "/reactivate.do'>重发激活信</a>");
            ermsg.append("或用");
            ermsg.append(email);
            ermsg.append("向<strong>hi@" + OpiConstants.domainMain + "</strong> 发封任意邮件");
            this.addErrorMessage(request, ermsg.toString());
        } else if (failCode == LoginStatusCode.FAILUSERBAN.getCode()) {
            // 原代码中此处是从资源文件获取该信息的
            this.addErrorMessage(request, "你的帐号因不当行为被管理员封禁，" + "要申诉或解除封禁请" + "<a href=\""
                    + OpiConstants.urlSupport + "/GetGuestbookList.do\">" + "给管理员留言" + "</a>");
        } else if (failCode == LoginStatusCode.FAILUSERSUICIDE.getCode()) {
            // 有独立的错误信息格式
            // addErrorMessage(request, "您的帐号已被冻结，" + "请先" + "<a href=\"" +
            // OpiConstants.urlLogin
            // + "/relive.do\">" + "解锁帐号" + "</a>。");
            request.setAttribute("SERSUICIDE", true);
        } else if (failCode == LoginStatusCode.FAILNOSAFEUSER.getCode()) {
            // 存在安全问题则跳转至安全中心处理
            return new ActionForward(OpiConstants.urlSafeCenter + "/Home.do", true);
        } else if (failCode == LoginStatusCode.FAILVALIDATELOGIN.getCode()) {
            this.addErrorMessage(request, LoginLogicFactory.getLoginStatusOprImpl()
                    .getErrorMessage(LoginStatusCode.FAILVALIDATELOGIN));
        } else if (failCode == LoginStatusCode.FAILNOTIFYGRAYUSER.getCode()) {
            this.addWarningMessage(request, LoginLogicFactory.getLoginStatusOprImpl()
                    .getErrorMessage(LoginStatusCode.FAILNOTIFYGRAYUSER));
        } else if (failCode == LoginStatusCode.FAILNOTIFYGRAYIP.getCode()) {
            this.addWarningMessage(request, LoginLogicFactory.getLoginStatusOprImpl()
                    .getErrorMessage(LoginStatusCode.FAILNOTIFYGRAYUSER));
        } else if (failCode == LoginStatusCode.FAILUNKNOWN.getCode()) {
            this.addWarningMessage(request, "系统异常,请稍候重试!!!");
        } else {
            this.addWarningMessage(request, "系统异常,请稍候重试!!!");
        }
        final String _de = CookieManager.getInstance().getCookie(request, "_de");
        final String email = (String) request.getAttribute("email");
        if (failCode != LoginStatusCode.FAILNOPOST.getCode() && email != null && email.length() > 0) {
            // 优先显示上次登录失败的emial
            request.setAttribute("_de", email);
        } else if (_de != null && _de.trim().length() != 0) {
            // 如果cookie中有上次登录失败的Email
            request.setAttribute("_de", DES_Encrypt.decrypt(_de));
        }
        request.setAttribute(PARAM_ORIGURL, this.getRedirectUrl(request));
        request.setAttribute(PARAM_FROM, request.getParameter(PARAM_FROM));
        return mapping.findForward(fail);
    }

    @Override
    protected String getIdentify() {
        return "Login-Web";
    }

    protected ActionForward getRequest(final ActionMapping mapping, final ActionForm form,
            final HttpServletRequest request, final HttpServletResponse response) throws Exception {
        // 暂时不支持那些通过参数来登录的方案
        final StatUtil su = new StatUtil();
        final String loginType = request.getParameter("loginType");
        if (loginType != null) {
            // 保留登录前缀
            request.setAttribute(LOGINTYPE_PRE, loginType);
        }

        int hostid = 0;
        if (!request.getMethod().equalsIgnoreCase("post")) {
            // 从cookie取票
            hostid = LoginLogicFactory.getPassportTicketOpr()
                    .getUserIdByPassport(request, response);
            su.stepTimeCost("GET:WebLoginAction:validateByCookie");

            if (hostid > 0) { // 验票成功，得到这个票是什么
                // User user = WUserAdapter.getInstance().get(hostid);
                su.stepTimeCost("GET:WebLoginAction:getUser");
                // boolean autoLogin =
                // request.getAttribute(LoginAction.AUTOLOGIN) != null;
                if (true) {
                    final String ip = BizFilterUtil.getClientIP(request);
                    CheckLoginClient.getInstance().autologinCheck(hostid, "", "", ip, true,
                            System.currentTimeMillis(), request);
                    final Runnable logTask = new LogTaskSet(hostid, ip, request.getRequestURL()
                            .toString());
                    final Runnable flyTask = new FlyerTaskSet(hostid, ip, request
                            .getParameter(LoginParamDef.PARAM_DOMAIN));
                    executor.submit(logTask);
                    executor.submit(flyTask);
                }
                su.stepTimeCost("GET:WebLoginAction:setUser");
                this.setNextStep(request, false);
                return this.sendRedirect2Dest(hostid, request, response, this.needNotify());
            } else {
                // 当验证不过时则清理这个P票 防止出现循环
                CookieManager.getInstance().saveCookie(response, COOKIE_PASSPORT, null, 0, "/",
                        PASSPORT_DOMAIN);
            }
            this.setNextStep(request, false);
            return this
                    .fail(mapping, form, request, response, LoginStatusCode.FAILNOPOST.getCode());
        }
        return null;
    }

    protected boolean needNotify() {
        return true;
    }

    protected ActionForward sendRedirect2Dest(final int userid, final HttpServletRequest request,
            final HttpServletResponse response, final boolean needNotify) {
        final StatUtil su = new StatUtil(_Tstat);
        final String domain = (String) request.getAttribute(PARAM_DOMAIN);
        LoginLogicFactory.getDomainOpr().setDomainEnv(request, domain);
        // 生成一个T票
        final String ticket = PassportManager.getInstance().createTicket(userid, request);
        su.stepTimeCost("Last:WebLoginAction:createTicket");
        final String forward = LoginLogicFactory.getPassportTicketOpr().forwardSetTicket(request,
                ticket, domain, request.getParameter(LoginParamDef.PARAM_ORIGURL),
                request.getParameter("p3p"),
                Boolean.valueOf(request.getParameter(LoginParamDef.PARAM_AUTOLOGIN)), needNotify);
        su.stepTimeCost("Last:WebLoginAction:forwardSetTicket");
        return new ActionForward(forward, true);
    }
}
