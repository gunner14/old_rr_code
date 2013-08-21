package com.xiaonei.login.logic.afterlogin_chains.node;

import javax.servlet.http.HttpServletRequest;

import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import com.xiaonei.icode.verify.CodeUtil;
import com.xiaonei.login.constants.LoginCookieDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.afterlogin_chains.AbsLoginResultChainNode;
import com.xiaonei.login.model.LoginModel;
import com.xiaonei.platform.core.opt.base.CookieManager;

/**
 * 检验验证码
 * 
 * @author Shunlong.jin @@version
 * 
 */
@Service
@Scope("prototype")
public class ICodeChainNode extends AbsLoginResultChainNode {

    @Override
    public LoginStatusCode doChain(LoginModel loginModel, LoginStatusCode perCode) {
        HttpServletRequest request = loginModel.getRequest();
        final String ick = CookieManager.getInstance().getCookie(request, LoginCookieDef.ICODE);
        final String icode = request.getParameter("icode");
        String key_id = request.getParameter("key_id");
        if (key_id != null && ick != null && (icode == null || !CodeUtil.getInstance().validCode(icode, "vlcode", ick))) {
            // 没输，或者输入错误，都是不对的
            return LoginStatusCode.FAILVALIDATELOGIN;
        }
        return next.doChain(loginModel, perCode);
    }

}
