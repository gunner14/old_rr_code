package com.xiaonei.login.daqin.node;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.afterlogin_chains.AbsLoginResultChainNode;
import com.xiaonei.login.model.LoginModel;

public class GetAccountChainNode extends AbsLoginResultChainNode {

    @Override
    public LoginStatusCode doChain(final LoginModel loginModel, final LoginStatusCode perCode) {
        loginModel.getRequest().setAttribute("login_account", loginModel.getAccount());
        return this.next.doChain(loginModel, perCode);
    }

}
