package com.xiaonei.login.controllers.base;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.AsSuperController;
import net.paoding.rose.web.annotation.DefValue;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.login.constants.Globals;
import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.LoginLogicFactory;
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
import com.xiaonei.login.logic.iphoneconnect.ICLogicImpl;

@AsSuperController
public abstract class IphoneConnectController implements InitializingBean {

    protected final static class InnerAccount {

        private final String desc;

        private final LoginStatusCode status;

        private final int userId;

        public InnerAccount(final LoginStatusCode status, final String desc, final int userId) {
            super();
            this.status = status;
            this.desc = desc;
            this.userId = userId;
        }

        public String getDesc() {
            return desc;
        }

        public LoginStatusCode getStatus() {
            return status;
        }

        public int getUserId() {
            return userId;
        }

    }

    @Autowired
    private SafeCheckResultChainNode safeCheckResultChainNode;

    @Autowired
    private AccountChainNode accountChainNode;

    @Autowired
    private CheckUserStatusResultChainNode checkUserStatusResultChainNode;

    @Autowired
    private IPBindResultChainNode iPBindResultChainNode;

    @Autowired
    private AdminGroupResultChainNode adminGroupResultChainNode;

    @Autowired
    private TaskAfterLoginChainNode taskAfterLoginChainNode;

    @Autowired
    private CrossLoginChainNode crossLoginChainNode;

    @Autowired
    private WeakPasswordChainNode weakPasswordChainNode;

    @Autowired
    private ICLogicImpl icLogicImpl;

    @IgnorePassportValidation
    @Get
    public final Object get(final Invocation inv) {
        final Object af = afterGet(inv);
        if (null != af) {
            return af;
        }
        return icLogicImpl.getJSONObject(LoginStatusCode.FAILNOPOST, "");
    }

    @IgnorePassportValidation
    @Post
    public final Object post(final Invocation inv, @Param("email") final String email,
            @Param("pwd") final String pwd,
            @Param("domain") @DefValue(Globals.RENREN_DOMAIN) final String domain) {
        final Object rt = beforePost(inv, email, pwd, domain);
        if (null != rt) {
            return rt;
        }

        final LoginStatusCode status = LoginLogicFactory.getPassportLogin().login(inv.getRequest(),
                inv.getResponse(), false, email, pwd, domain, chains);

        final String desc = icLogicImpl.getErrorMessage(status);
        Integer userId = (Integer) inv.getRequest().getAttribute(LoginParamDef.ATTRIBUTE_USER_ID);
        if (null == userId) {
            userId = -1;
        }
        final InnerAccount innerAccount = new InnerAccount(status, desc, userId);
        LoginLoggerDef.login.info(String.format(LoginLoggerDef.loginLogPentter, status, email, ""));
        final Object rtaf = afterPost(inv, email, pwd, domain, innerAccount);
        if (null != rtaf) {
            return rtaf;
        }
        return icLogicImpl.getJSONObject(status, email);
    }

    protected abstract Object afterGet(Invocation inv);

    protected abstract Object afterPost(Invocation inv, String inputEmail, String inputPwd,
            String inputDomain, InnerAccount innerAccount);

    protected abstract Object beforePost(Invocation inv, String email, String pwd, String domain);

    private LoginResultChains chains = new LoginResultChainsImpl();

    @Override
    public final void afterPropertiesSet() throws Exception {
        if (chains.isEmpty()) {
            chains.setNode(safeCheckResultChainNode);
            chains.setNode(accountChainNode);
            chains.setNode(checkUserStatusResultChainNode);
            chains.setNode(iPBindResultChainNode);
            chains.setNode(adminGroupResultChainNode);
            chains.setNode(taskAfterLoginChainNode);
            chains.setNode(crossLoginChainNode);
            chains.setNode(weakPasswordChainNode);
        }
    }
}
