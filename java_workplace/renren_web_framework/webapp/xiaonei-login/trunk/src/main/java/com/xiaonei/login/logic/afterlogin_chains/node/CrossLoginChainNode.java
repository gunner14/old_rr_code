package com.xiaonei.login.logic.afterlogin_chains.node;

import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.afterlogin_chains.AbsLoginResultChainNode;
import com.xiaonei.login.model.LoginModel;

@Service
@Scope("prototype")
public class CrossLoginChainNode extends AbsLoginResultChainNode {

    @Override
    public LoginStatusCode doChain(LoginModel loginModel, LoginStatusCode perCode) {
        // 判断用户是否有交叉登录的情况发生
        //        if (Globals.IMOP_DOMAIN.equalsIgnoreCase(loginModel.getAccount().getDomain())) {
        //            return next.doChain(loginModel, perCode);
        //        }
        //        if (!loginModel.getDomain().equalsIgnoreCase(loginModel.getAccount().getDomain())) {
        //            loginModel.getRequest().setAttribute(LoginParamDef.ATTRIBUTE_LOGIN_CROSS,
        //                    Boolean.toString(true));
        //        }
        // 不见查跨站登录
        return next.doChain(loginModel, perCode);
    }

}
