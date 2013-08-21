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
import com.xiaonei.login.logic.ticket.PassportTicketOprImpl;
import com.xiaonei.platform.core.opt.base.PassportManager;

@AsSuperController
public abstract class WMLConnectController implements InitializingBean {

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
            return this.desc;
        }

        public LoginStatusCode getStatus() {
            return this.status;
        }

        public int getUserId() {
            return this.userId;
        }

    }

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
    private ICLogicImpl icLogicImpl;

    @Autowired
    private IPBindResultChainNode iPBindResultChainNode;

    @Autowired
    private PassportTicketOprImpl passportTicketOprImpl;

    @Autowired
    private SafeCheckResultChainNode safeCheckResultChainNode;

    @Autowired
    private TaskAfterLoginChainNode taskAfterLoginChainNode;

    @Autowired
    private WeakPasswordChainNode weakPasswordChainNode;

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

    @IgnorePassportValidation
    @Get
    public final Object get(final Invocation inv) {
        final Object af = this.afterGet(inv);
        if (null != af) {
            return af;
        }
        return this.icLogicImpl.getJSONObject(LoginStatusCode.FAILNOPOST, "");
    }

    @IgnorePassportValidation
    @Post
    public final Object post(final Invocation inv, @Param("email") final String email,
            @Param("pwd") final String pwd,
            @Param("domain") @DefValue(Globals.RENREN_DOMAIN) final String domain) {
        final Object rt = this.beforePost(inv, email, pwd, domain);
        if (null != rt) {
            return rt;
        }

        final LoginStatusCode status = LoginLogicFactory.getPassportLogin().login(inv.getRequest(),
                inv.getResponse(), false, email, pwd, domain, this.chains);

        final String desc = this.icLogicImpl.getErrorMessage(status);
        Integer userId = (Integer) inv.getRequest().getAttribute(LoginParamDef.ATTRIBUTE_USER_ID);
        if (null == userId) {
            userId = -1;
        }
        final InnerAccount innerAccount = new InnerAccount(status, desc, userId);
        LoginLoggerDef.login.info(String.format(LoginLoggerDef.loginLogPentter, status, email, ""));
        if (innerAccount.getStatus() == LoginStatusCode.LOGIN_OK) {
            final String ticket = PassportManager.getInstance().createTicket(userId,
                    inv.getRequest());
            this.passportTicketOprImpl
                    .setTicket(ticket, false, inv.getRequest(), inv.getResponse());
        }
        final Object rtaf = this.afterPost(inv, email, pwd, domain, innerAccount);
        if (null != rtaf) {
            return rtaf;
        }
        return this.icLogicImpl.getJSONObject(status, email);
    }

    protected abstract Object afterGet(Invocation inv);

    protected abstract Object afterPost(Invocation inv, String inputEmail, String inputPwd,
            String inputDomain, InnerAccount innerAccount);

    protected abstract Object beforePost(Invocation inv, String email, String pwd, String domain);
}
