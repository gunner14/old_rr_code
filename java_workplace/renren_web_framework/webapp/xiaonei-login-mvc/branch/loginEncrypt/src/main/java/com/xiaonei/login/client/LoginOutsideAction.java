package com.xiaonei.login.client;

import java.util.HashMap;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.login.client.base.AbstractStructAction;
import com.xiaonei.login.client.base.LoginAction;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.platform.component.application.home.AppBasicHome;
import com.xiaonei.platform.component.application.model.AppBasic;
import com.xiaonei.platform.component.application.sk.mgr.SessionKeyManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.xce.passport.PassportAdapterFactory;

/**
 * 一些合作伙伴，他们拥有登录自己的页面，此action专为这种情况考虑<br>
 * 1、必须传入的参数:api_key,v,email,password <br>
 * 2、可选传入的参数:format<br>
 * 3、如果必须传入的参数没有传入，则给出错误信息<br>
 * 4、如果api_key或v验证错误，则给出错误信息<br>
 * 5、如果email或password验证错误，则给出错误信息<br>
 * 
 * @author lifs
 * @deprecated 最好不要提供对外 传入明文密码的登录 @modify haobo.cui@opi-corp.com
 */
@Deprecated
public class LoginOutsideAction extends AbstractStructAction {

    private String getSuccessfulInfo(final int hostId, final AppBasic ab/* ,String auth */) {
        String sessionKey;
        String xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
        try {
            // sessionKey = SessionKeyMgr.createSession(hostId);
            sessionKey = SessionKeyManager
                    .getInstance()
                    .getSessionKey(SessionKeyManager.SESSION_KEY_CANVASDESKTOP, hostId,
                            ab.getAppId(), false).getSessionKey();
            xml += "<auth_getSession_response>";
            xml += "<session_key>" + sessionKey + "</session_key>";
            xml += "<uid>" + hostId + "</uid>";
            xml += "</auth_getSession_response>";
            this.logger.info("xml=" + xml);
        } catch (final Exception e) {
            e.printStackTrace();
            this.logger.error("--------Exception------e=" + e);
        }
        return xml;
    }

    private String getYinKeXmlInfo(final String result) {
        String xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
        xml += "<login_validate_response>";
        xml += "<result>" + result + "</result>";
        xml += "</login_validate_response>";
        this.logger.error("【来自印客的登录】-----------xml=" + xml);
        return xml;
    }

    @Override
    protected ActionForward afterLogin(final ActionMapping mapping, final ActionForm form,
            final HttpServletRequest request, final HttpServletResponse response) throws Exception {
        final boolean isJson = ApiLoginUtil.isJsonFormat(request);// 数据格式
        String result = "";
        // 针对印客网/天网的特殊情况，它只要判断用户帐号是否是校内帐号
        final String apiKey = Escape.trim(request.getParameter("api_key"));
        if ((apiKey != null)
                && ("c9sx65r7s57puwmn635mwyhcj895f523sy7p".equals(apiKey) || "rmmnwu5mfd42r4nsw9ceyfd5cn95hpxhc9pe"
                        .equals(apiKey))) {
            result = this.getYinKeXmlInfo("1");
            ApiLoginUtil.sendResultResponse(response, result, isJson);
            // 由于要返回null，没有下一步
            this.setNextStep(request, false);
            return null;
        }
        Integer user_id = null;
        final User user = BaseThreadUtil.currentHost(request);
        if (user == null) {
            user_id = (Integer) request.getAttribute(LoginAction.USER_ID);
        } else {
            user_id = user.getId();
        }
        if (user_id == null) {
            return this.fail(mapping, form, request, response,
                    LoginStatusCode.FAILUNKNOWN.getCode());
        }
        PassportAdapterFactory.getWebTicket().createTicket(user_id, new HashMap<String, String>());
        final AppBasic ab = AppBasicHome.getInstance().getByApiKey(apiKey);
        result = this.getSuccessfulInfo(user_id, ab/* , authToken */);
        ApiLoginUtil.sendResultResponse(response, result, isJson);
        // 由于要返回null，没有下一步
        this.setNextStep(request, false);
        return null;
    }

    @Override
    protected ActionForward beforeLogin(final ActionMapping mapping, final ActionForm form,
            final HttpServletRequest request, final HttpServletResponse response) throws Exception {
        final boolean isJson = ApiLoginUtil.isJsonFormat(request);// 数据格式
        String result = "";
        if (!"post".equalsIgnoreCase(request.getMethod())) {
            result = ApiLoginUtil.getErrorInfo(1000, "提交必须是Post方式", isJson);
            ApiLoginUtil.sendResultResponse(response, result, isJson);
            this.logger.error("提交必须是Post方式");
            // 由于要返回null，没有下一步
            this.setNextStep(request, false);
            return null;
        }
        //
        final String ak = Escape.trim(request.getParameter("api_key"));
        // 这里的判断调用会比较低效，要解决
        final AppBasic ab = AppBasicHome.getInstance().getByApiKey(ak);
        if (ab == null) {
            result = ApiLoginUtil.getErrorInfo(101, "提交的api_key不属于任何已知的应用", isJson);
            ApiLoginUtil.sendResultResponse(response, result, isJson);
            // 由于要返回null，没有下一步
            this.setNextStep(request, false);
            return null;
        }
        this.setUserName(request, Escape.trim(request.getParameter("email")));
        this.setPassword(request, Escape.trim(request.getParameter("password")));
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
            result = ApiLoginUtil.getErrorInfo(ApiLoginErrorCode.LOGIN_BUSY, LoginLogicFactory
                    .getLoginStatusOprImpl().getErrorMessage(LoginStatusCode.FAILUNKNOWN), isJson);
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
