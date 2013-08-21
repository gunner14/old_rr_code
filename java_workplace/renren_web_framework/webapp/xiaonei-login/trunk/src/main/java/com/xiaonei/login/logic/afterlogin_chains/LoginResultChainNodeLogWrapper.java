package com.xiaonei.login.logic.afterlogin_chains;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.model.LoginModel;

public class LoginResultChainNodeLogWrapper implements LoginResultChainNode {

    protected Log logger = LogFactory.getLog(this.getClass());

    private LoginResultChainNode target;

    public LoginResultChainNodeLogWrapper(LoginResultChainNode target) {
        this.target = target;
    }

    public void setTarget(LoginResultChainNode target) {
        this.target = target;
    }

    @Override
    public LoginStatusCode doChain(LoginModel loginModel, LoginStatusCode perCode) {
        if (logger.isDebugEnabled()) {
            logger.debug(loginModel.toString() + ", perCode:" + perCode + ", this node:"
                    + target.toString());
        }
        return target.doChain(loginModel, perCode);
    }

    @Override
    public LoginResultChainNode getNext() {
        return target.getNext();
    }

    @Override
    public boolean isEnd() {
        return target.isEnd();
    }

    @Override
    public void setNext(LoginResultChainNode node) {
        target.getNext();
    }

    @Override
    public boolean equals(Object obj) {
        return target.equals(obj);
    }

    @Override
    protected Object clone() throws CloneNotSupportedException {
        if (target instanceof AbsLoginResultChainNode) {
            return ((AbsLoginResultChainNode) target).clone();
        } else {
            return super.clone();
        }
    }

    @Override
    public int hashCode() {
        return target.hashCode();
    }

}
