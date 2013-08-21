/**
 * 
 */
package com.xiaonei.login.logic.afterlogin_chains;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.model.LoginModel;

/**
 * AbsLoginResultChainNode <br>
 * 
 * @author tai.wang@opi-corp.com Jul 30, 2010 - 3:39:11 PM
 */
public abstract class AbsLoginResultChainNode implements LoginResultChainNode,
Cloneable {

    private static final LoginResultChainNode END_NODE = new AbsLoginResultChainNode() {
        @Override
        public LoginStatusCode doChain(final LoginModel loginModel,
                final LoginStatusCode perCode) {
            return LoginStatusCode.LOGIN_OK;
        }

        @Override
        public boolean isEnd() {
            return true;
        };
    };

    protected Log logger = LogFactory.getLog(this.getClass());

    protected LoginResultChainNode next = END_NODE;

    @Override
    public AbsLoginResultChainNode clone() throws CloneNotSupportedException {
        return (AbsLoginResultChainNode) super.clone();
    }

    @Override
    public boolean equals(final Object obj) {
        if (obj == null) {
            return false;
        }
        final String objn = obj.getClass().getName();
        final String thisn = this.getClass().getName();
        return objn.equals(thisn);
    }

    @Override
    public LoginResultChainNode getNext() {
        return this.next;
    }

    @Override
    public int hashCode() {
        return this.getClass().getName().hashCode();
    }

    @Override
    public boolean isEnd() {
        return false;
    }

    @Override
    public void setNext(final LoginResultChainNode node) {
        this.next = node;
    }
}
