package com.xiaonei.login.logic.afterlogin_chains.node;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.afterlogin_chains.AbsLoginResultChainNode;
import com.xiaonei.login.logic.ticket.PassportTicketOprImpl;
import com.xiaonei.login.model.LoginModel;
import com.xiaonei.xce.passport.Account;

@Service
@Scope("prototype")
public class SaveTicketChainNode extends AbsLoginResultChainNode {

    @Autowired
    private PassportTicketOprImpl passportTicketOprImpl;
    
	@Override
	public LoginStatusCode doChain(LoginModel loginModel,
			LoginStatusCode perCode) {
		// 保存p票
		Account account = loginModel.getAccount();
		HttpServletRequest request = loginModel.getRequest();
		HttpServletResponse response = loginModel.getResponse();
		if (loginModel.isPer()) {
			// 保存一个月的
		    passportTicketOprImpl.savePassport2Cookie(account.getId(), request,
					response, account.getTicket(), true);
		} else {
			// 保存session域
		    passportTicketOprImpl.savePassport2Cookie(account.getId(), request,
					response, account.getTicket(), false);
		}
		return next.doChain(loginModel, perCode);
	}

}
