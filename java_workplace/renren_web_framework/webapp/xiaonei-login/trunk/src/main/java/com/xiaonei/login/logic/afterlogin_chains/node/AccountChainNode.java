/**
 * 
 */
package com.xiaonei.login.logic.afterlogin_chains.node;

import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.afterlogin_chains.AbsLoginResultChainNode;
import com.xiaonei.login.model.LoginModel;
import com.xiaonei.xce.passport.Account;

/**
 * AccountChainNode <br>
 * 
 * @author tai.wang@opi-corp.com Jul 30, 2010 - 5:22:46 PM
 */
@Service
@Scope("prototype")
public class AccountChainNode extends AbsLoginResultChainNode {

    @Override
    public LoginStatusCode doChain(final LoginModel loginModel, final LoginStatusCode perCode) {
        final Account account = loginModel.getAccount();
        /* 判断状态 */
        if (account == null) {
            return LoginStatusCode.FAILUNKNOWN;
        }
        // 输入密码错误
        if (account.getSuccess() == -2) {
            return LoginStatusCode.FAILUSER;
        }
        // 输入用户不存在
        if (account.getSuccess() == -1) {
            return LoginStatusCode.FAILUSERNOTEXIST;
        }
        if (account.getSuccess() < 0) {
            return LoginStatusCode.FAILUSERNOTEXIST;
        }
        if (account.getId() <= 0) {
            return LoginStatusCode.FAILUSERNOTEXIST;
        }
        loginModel.getRequest().setAttribute(LoginParamDef.ATTRIBUTE_USER_ID, account.getId());
        loginModel.getRequest().setAttribute(LoginParamDef.ATTRIBUTE_ACCOUNT, account);
        return this.next.doChain(loginModel, perCode);
    }

}
