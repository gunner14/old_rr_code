/**
 * 
 */
package com.xiaonei.login.logic.afterlogin_chains.node;

import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import com.xiaonei.aegis.login.client.CheckLoginClient;
import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginSourceType;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.afterlogin_chains.AbsLoginResultChainNode;
import com.xiaonei.login.model.LoginModel;

/**
 * SafeCheckResultChainNode <br>
 * 
 * @author tai.wang@opi-corp.com Jul 30, 2010 - 3:38:06 PM
 */
@Service
@Scope("prototype")
public class SafeCheckResultChainNode extends AbsLoginResultChainNode {

	@Override
	public LoginStatusCode doChain(final LoginModel loginModel,
			final LoginStatusCode perCode) {
		/* 安全登录检查 */
		this.safeCheckLogin(loginModel);
		return super.next.doChain(loginModel, perCode);
	}

	/**
	 * safeCheckLogin<br>
	 * 安全登录检查
	 * 
	 * @param request
	 * @param userName
	 * @param password
	 * @param ip
	 * @param account
	 * 
	 * @author tai.wang@opi-corp.com Jan 14, 2010 - 10:44:51 AM
	 */
	private void safeCheckLogin(final LoginModel model) {
		if (!LoginLoggerDef.loginSafe.isEnable()) {
			return;
		}
		try {
			final Object loginSourceTypeObject=model.getRequest().getAttribute(LoginParamDef.ATTRIBUTE_LOGIN_SOURCE);
			final LoginSourceType sourceType=loginSourceTypeObject==null?LoginSourceType.Web:(LoginSourceType) loginSourceTypeObject;
			if (model.getAccount().getSuccess() != -1) {
				CheckLoginClient.getInstance().loginCheckTest(
						model.getAccount().getId(), model.getUserName(),
						model.getPwd(), model.getIp(),
						model.getAccount().getSuccess() == 0,
						System.currentTimeMillis(),sourceType.getValue(), model.getRequest());
			} else {
				// 用户不存在的情况
				CheckLoginClient.getInstance().loginCheckTest(-1,
						model.getUserName(), model.getPwd(), model.getIp(),
						false, System.currentTimeMillis(),sourceType.getValue(), model.getRequest());
			}
		} catch (final Exception e) {
			LoginLoggerDef.loginSafe.error("", e);
		}
	}
}
