package com.xiaonei.login.client;

import java.util.HashMap;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.login.client.base.AbstractStructAction;
import com.xiaonei.login.client.base.LoginAction;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.platform.component.application.home.AppBasicHome;
import com.xiaonei.platform.component.application.home.SpecialAppListHome;
import com.xiaonei.platform.component.application.home.UserAppHome;
import com.xiaonei.platform.component.application.model.AppActiveUserLog;
import com.xiaonei.platform.component.application.model.AppBasic;
import com.xiaonei.platform.component.application.model.SpecialApp;
import com.xiaonei.platform.component.application.sk.mgr.SessionKeyManager;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.platform.service.PlatformServiceHome;
import com.xiaonei.xce.passport.PassportAdapterFactory;

/**
 * 此Action专门为手机客户端登录所提供<br>
 * 1、必须传入的参数:api_key,v,user,password(md5加密后的密文) user可能是email，手机号等<br>
 * 2、可选传入的参数:format<br>
 * 3、如果必须传入的参数没有传入，则给出错误信息<br>
 * 4.返回值为session_key uid ticket
 * 
 * @author zhicai.li@opi-corp.com 2008-10-14 13:49
 */
public class ClientLoginAction extends AbstractStructAction {

    protected Logger logger = Logger.getLogger("debug_logger");

    protected final static int LENGTH_OF_PHONE_NUM = 11;// 手机号长度

    /**
     * add at 2008-12-18 20:03 加入了升级验证的功能
     * */
    private String getSuccessfulInfo(final int hostId, final int appId,
            final HttpServletRequest request, final boolean isJson) {
        final int sessionKeyType = SessionKeyManager.SESSION_KEY_CANVASDESKTOP;
        boolean needTicket = false;
        String sessionKey = "", ticket = "";
        // if (Config.phoneclientMap.containsKey(appId)) {
        if (SpecialAppListHome.getInstance().isSpecial(SpecialApp.TYPE_NEED_TICKET, appId)) {
            // sessionKeyType = SessionKeyManager.SESSION_KEY_CANVASDESKTOP;
            needTicket = true;
        }
        try {
            sessionKey = SessionKeyManager.getInstance()
                    .getSessionKey(sessionKeyType, hostId, appId, false).getSessionKey();
            if (needTicket) {
                // 小白提供的接口，根据id获取ticket，如果没有创建，2条过期
                ticket = PassportAdapterFactory.getWapLogin().createTicket(hostId, 48 * 3600, true);
            }
            if (isJson) {
                final JSONObject obj = new JSONObject();
                try {
                    obj.put("session_key", sessionKey);
                    if (needTicket) {
                        obj.put("ticket", ticket);
                    }
                    obj.put("uid", hostId);

                    // 为了兼容老版本的客户端而添加的难看代码
                    final String clientName = request.getParameter("clientName");
                    final String from = request.getParameter("from");
                    if (("xiaonei".equals(clientName)) && ((from == null) || "0".equals(from))) {
                        final JSONObject object = new JSONObject();
                        object.put("is_enforce_update", false);
                        object.put("download_url", "http://m.renren.com/dl.do");
                        object.put("info", "手机人人2009全新推出，建议您马上安装。现在安装吗？");
                        final JSONArray array = new JSONArray();
                        array.add(object);
                        obj.put("update_info", array);
                    }

                } catch (final Exception e) {
                    e.printStackTrace();
                }
                if (this.logger.isInfoEnabled()) {
                    this.logger.info("-----登录成功----" + hostId + ":" + System.currentTimeMillis());
                }
                return obj.toString();
            } else {
                final StringBuffer xml = new StringBuffer(
                        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
                xml.append("<login_response>");
                xml.append("<session_key>").append(sessionKey).append("</session_key>");
                if (needTicket) {
                    xml.append("<ticket>").append(ticket).append("</ticket>");
                }
                xml.append("<uid>").append(hostId).append("</uid>");
                xml.append("</login_response>");
                if (this.logger.isInfoEnabled()) {
                    this.logger.info("---登录成功----" + hostId + ":" + System.currentTimeMillis());
                }

                return xml.toString();
            }
        } catch (final Exception e) {
            e.printStackTrace();
        }
        // System.err.println(xml);
        return null;
    }

    @Override
    protected ActionForward afterLogin(final ActionMapping mapping, final ActionForm form,
            final HttpServletRequest request, final HttpServletResponse response) throws Exception {
        final boolean isJson = ApiLoginUtil.isJsonFormat(request);// 数据格式
        String result = "";
        final AppBasic ab = (AppBasic) request.getAttribute("app_basic_obj");
        final int appId = ab.getAppId();
        // User host = BaseThreadUtil.currentHost(request);
        // int userId = host.getId();
        final Object object = request.getAttribute(LoginAction.USER_ID);

        Integer userId = null;
        try {
            userId = (Integer) object;
        } catch (final ClassCastException e) {
            e.printStackTrace();
        } catch (final Exception e) {
            e.printStackTrace();
        }

        if (userId == null) {
            return this.fail(mapping, form, request, response, ApiLoginErrorCode.LOGIN_BUSY);
        }

        // if(appId!=69574&&appId!=69579&&appId!=23137&&appId!=53414&&"kaixin.com".equals(host.getDomain())){
        // 客户端要求开心用户暂时无法登陆
        // 为什么？开心跟校内其实是一样的哈，没啥区别，开心的用户登录就当作互联互通呗，连UI都不需要改
        // result=ApiLoginUtil.getErrorInfo(ApiLoginErrorCode.USER_UNKNOW,
        // "当前登录用户不存在", isJson);
        // ApiLoginUtil.sendResultResponse(response, result, isJson);
        // this.setNextStep(request, false);
        // return null;
        // }
        PassportAdapterFactory.getWebTicket().createTicket(userId, new HashMap<String, String>());
        result = this.getSuccessfulInfo(userId, appId, request, isJson);
        ApiLoginUtil.sendResultResponse(response, result, isJson);
        ApiLoginUtil.updateActiveUserStat(userId, appId, AppActiveUserLog.TYPE_WAP, request,
                response);// 加入统计信息

        /**
         * 用户输入用户名和密码登录，其实就是同意app知道他是谁并获取他的信息，这就是基本授权，所以应该走基本授权的流程
         * 由于这些app跟校内有合作关系，并不一定是通过审核的，所以用后台授权的接口 2009-11-23 11：20
         */
        final boolean isAppUser = UserAppHome.getInstance().isAppUser(appId, userId);
        if (!isAppUser) {
            try {
                PlatformServiceHome.getInstance().installApp(userId, appId);
            } catch (final Exception e) {
                e.printStackTrace();
            }
        }
        /*******************************/

        this.setNextStep(request, false);
        return null;
    }

    @Override
    protected ActionForward beforeLogin(final ActionMapping mapping, final ActionForm form,
            final HttpServletRequest request, final HttpServletResponse response) throws Exception {
        final boolean isJson = ApiLoginUtil.isJsonFormat(request);// 数据格式
        if (this.logger.isInfoEnabled()) {
            this.logger.info("--登录响应-------" + request.getParameter("user") + ":"
                    + System.currentTimeMillis());
        }
        String result = "";
        // 如果不是post提交来的，返回错误信息
        if (!"post".equalsIgnoreCase(request.getMethod())) {
            result = ApiLoginUtil.getErrorInfo(ApiLoginErrorCode.REQUEIRE_POST, "提交必须是Post方式",
                    isJson);
            ApiLoginUtil.sendResultResponse(response, result, isJson);
            // 由于要返回null，没有下一步
            this.setNextStep(request, false);
            return null;
        }
        final String ak = Escape.trim(request.getParameter("api_key"));
        String domain = request.getParameter("domain");
        if (StringUtils.isEmpty(domain)) {
            domain = "renren.com";
        }
        // appBasic已本地缓存
        final AppBasic ab = AppBasicHome.getInstance().getByApiKey(ak);
        if (ab == null) {
            this.logger.error("api_key wrong   " + ak);
            result = ApiLoginUtil.getErrorInfo(ApiLoginErrorCode.API_EC_PARAM_API_KEY,
                    "请检查传入的api_key参数是否正确", isJson);
            ApiLoginUtil.sendResultResponse(response, result, isJson);
            // 由于要返回null，没有下一步
            this.setNextStep(request, false);
            return null;
        } else {
            // 将appBasic对象放到request上下文中，供其他方法使用
            request.setAttribute("app_basic_obj", ab);
        }

        final String passWord = request.getParameter("password");
        this.setUserName(request, Escape.trim(request.getParameter("user")));
        this.setPassword(request, Escape.trim((passWord == null) ? null : passWord.toLowerCase()),
                true);
        this.setDomain(request, domain);// 判断当前的登录来源，如果是kaixin.com则使用开心帐号登录
        return null;
    }

    @Override
    protected ActionForward fail(final ActionMapping mapping, final ActionForm form,
            final HttpServletRequest request, final HttpServletResponse response, final int failCode)
                    throws Exception {
        final boolean isJson = ApiLoginUtil.isJsonFormat(request);// 数据格式
        String result = "";

        if (failCode == LoginStatusCode.FAILUSER.getCode()) {// 账号、密码 输入验证失败
            result = ApiLoginUtil.getErrorInfo(ApiLoginErrorCode.USER_PASSWORD_INVALID,
                    "输入的帐号或者密码不对", isJson);
            ApiLoginUtil.sendResultResponse(response, result, isJson);
            return null;
        } else if (failCode == LoginStatusCode.FAILBANIP.getCode()) {// 地址封禁过滤

            result = ApiLoginUtil.getErrorInfo(ApiLoginErrorCode.USER_IP_FORBIDDEN,
                    "当前登录的ip地址处于封禁状态", isJson);
            ApiLoginUtil.sendResultResponse(response, result, isJson);
            return null;
        } else if (failCode == LoginStatusCode.FAILUSERNOTEXIST.getCode()) {// 没有此用户

            result = ApiLoginUtil.getErrorInfo(ApiLoginErrorCode.USER_UNKNOW, "用户不存在", isJson);
            ApiLoginUtil.sendResultResponse(response, result, isJson);
            return null;
        } else if (failCode == LoginStatusCode.FAILUSERACTIVE.getCode()) {// 帐号未激活

            result = ApiLoginUtil.getErrorInfo(ApiLoginErrorCode.ACCOUNT_INACTIVE, "帐号处于未激活的状态",
                    isJson);
            ApiLoginUtil.sendResultResponse(response, result, isJson);
            return null;
        } else if (failCode == LoginStatusCode.FAILUSERBAN.getCode()) {// 帐号被封禁

            result = ApiLoginUtil.getErrorInfo(ApiLoginErrorCode.ACCOUNT_FORBIDDEN, "帐号处于被封禁的状态",
                    isJson);
            ApiLoginUtil.sendResultResponse(response, result, isJson);
            return null;
        } else if (failCode == LoginStatusCode.FAILUSERSUICIDE.getCode()) {// 帐号已经注销

            result = ApiLoginUtil.getErrorInfo(ApiLoginErrorCode.ACCOUNT_LOGINOUT, "帐号处于注销的状态",
                    isJson);
            ApiLoginUtil.sendResultResponse(response, result, isJson);
            return null;
        } else if (failCode == LoginStatusCode.FAILNOSAFEUSER.getCode()) {// 帐号已经注销

            result = ApiLoginUtil
                    .getErrorInfo(ApiLoginErrorCode.ACCOUNT_UNSAFE, "帐号存在安全隐患", isJson);
            ApiLoginUtil.sendResultResponse(response, result, isJson);
            return null;
        } else if (failCode == LoginStatusCode.FAILINPUT.getCode()) {// 帐号已经注销

            result = ApiLoginUtil.getErrorInfo(ApiLoginErrorCode.FAILINPUT, "用户输入不合法", isJson);
            ApiLoginUtil.sendResultResponse(response, result, isJson);
            return null;
        } else if (failCode == 0) {
            result = ApiLoginUtil.getErrorInfo(ApiLoginErrorCode.LOGIN_BUSY, "登录服务暂时不可用，请重试",
                    isJson);
            ApiLoginUtil.sendResultResponse(response, result, isJson);
            return null;
        }
        return null;
    }

    @Override
    protected String getIdentify() {
        return null;
    }
}
