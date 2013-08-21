package com.xiaonei.login.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONObject;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.origurl.DontPutOrigURL;
import com.xiaonei.login.constants.Globals;
import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginSourceType;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.PassportLoginImpl;
import com.xiaonei.login.logic.afterlogin_chains.LoginResultChains;
import com.xiaonei.login.logic.afterlogin_chains.LoginResultChainsImpl;
import com.xiaonei.login.logic.afterlogin_chains.node.AccountChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.AdminGroupResultChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.CheckUserStatusResultChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.CrossLoginChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.IPBindResultChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.SafeCheckResultChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.TaskAfterLoginChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.WeakPasswordChainNode;
import com.xiaonei.login.logic.logger.PpLogger;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.PassportManager;

@DontPutOrigURL
public class Md5LoginController implements InitializingBean {

    @Autowired
    private AccountChainNode accountChainNode;

    @Autowired
    private AdminGroupResultChainNode adminGroupResultChainNode;

    private final LoginResultChains chains = new LoginResultChainsImpl();

    @Autowired
    private CheckUserStatusResultChainNode checkUserStatusResultChainNode;

    @Autowired
    private CrossLoginChainNode crossLoginChainNode;

    @Autowired
    private IPBindResultChainNode iPBindResultChainNode;

    @Autowired
    private PassportLoginImpl passportLoginImpl;

    @Autowired
    private SafeCheckResultChainNode safeCheckResultChainNode;

    @Autowired
    private TaskAfterLoginChainNode taskAfterLoginChainNode;

    @Autowired
    private WeakPasswordChainNode weakPasswordChainNode;

    protected Log logger = LogFactory.getLog(this.getClass());

    protected PpLogger remoteLogger = LoginLoggerDef.login;

    @Override
    public final void afterPropertiesSet() throws Exception {
        if (this.chains.isEmpty()) {
            this.chains.setNode(this.safeCheckResultChainNode);
            this.chains.setNode(this.accountChainNode);
            this.chains.setNode(this.checkUserStatusResultChainNode);
            this.chains.setNode(this.iPBindResultChainNode);
            this.chains.setNode(this.adminGroupResultChainNode);
            this.chains.setNode(this.taskAfterLoginChainNode);
            this.chains.setNode(this.crossLoginChainNode);
            this.chains.setNode(this.weakPasswordChainNode);
        }
    }

    @Post
    public Object index(final Invocation inv, @Param("e") final String account,
            @Param("p") final String md5pwd) {
        inv.getRequest().setAttribute(LoginParamDef.PARAM_ENPWD, Boolean.TRUE);
        inv.getRequest().setAttribute(LoginParamDef.ATTRIBUTE_LOGIN_SOURCE, LoginSourceType.Client);
        final LoginStatusCode rt = this.passportLoginImpl.login(inv.getRequest(),
                inv.getResponse(), false, account, md5pwd, Globals.RENREN_DOMAIN, this.chains);
        // 登陆后
        final JSONObject jo = new JSONObject();
        if (rt == LoginStatusCode.LOGIN_OK) {
            Integer userId = null;
            final User user = BaseThreadUtil.currentHost(inv.getRequest());
            if (user == null) {
                userId = (Integer) inv.getRequest().getAttribute(LoginParamDef.ATTRIBUTE_USER_ID);
            } else {
                userId = user.getId();
            }

            if (userId == null) {
                jo.put("status", "fail");
                jo.put("code", LoginStatusCode.FAILUNKNOWN.getCode());
                jo.put("desc", LoginStatusCode.FAILUNKNOWN.name());
                return jo;
            }

            if (this.remoteLogger.isEnable()) {
                this.remoteLogger.info(LoginLoggerDef.loginLogPentter, "loginOkMd5", userId, inv
                        .getRequest().getHeader("Referer"));
            }
            // 生成T票 并跳转
            final String ticket = PassportManager.getInstance().createTicket(userId,
                    inv.getRequest());
            jo.put("status", "ok");
            jo.put("code", LoginStatusCode.LOGIN_OK.getCode());
            jo.put("desc", LoginStatusCode.LOGIN_OK.name());
            jo.put("t", ticket);
            return jo;
        } else {
            jo.put("status", "fail");
            jo.put("code", rt.getCode());
            jo.put("desc", rt.name());
            return jo;
        }
    }
}
