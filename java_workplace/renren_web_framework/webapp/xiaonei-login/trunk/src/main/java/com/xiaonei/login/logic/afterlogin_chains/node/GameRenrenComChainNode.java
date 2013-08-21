package com.xiaonei.login.logic.afterlogin_chains.node;

import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.Globals;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.afterlogin_chains.AbsLoginResultChainNode;
import com.xiaonei.login.model.LoginModel;
import com.xiaonei.xce.passport.Account;

/**
 * domain是imop的用户, 登录到http://game.renren.com
 * @author wang-tai
 *	
 */
@Service
@Scope("prototype")
public class GameRenrenComChainNode extends AbsLoginResultChainNode {

	@Override
	public LoginStatusCode doChain(LoginModel loginModel,
			LoginStatusCode perCode) {
		Account account = loginModel.getAccount();
		if(Globals.IMOP_DOMAIN.equalsIgnoreCase(account.getDomain())){
			loginModel.getRequest().setAttribute(LoginParamDef.ATTRIBUTE_PUZZLE_ORIGURL, "http://game.renren.com");
		}
		return next.doChain(loginModel, perCode);
	}

}
