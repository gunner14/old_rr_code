package com.xiaonei.login.logic.status;

import net.paoding.rose.web.Invocation;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
 
import com.xiaonei.login.constants.LoginCookieDef;
import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;

@Service
public class GrayIpAndWarningUserImpl implements GrayIpAndWarningUser,
		LoginLoggerDef {
    
    @Autowired
    ValidationImpl validationImpl;

	@Override
	public LoginStatusCode getRtAddrForGrayIpOrWaringUser(final Invocation inv, String email, String pwd, String icode) {
		boolean isGrayIp = false;
		// 根据用户的输入用户名来判断用户是否为警告用户
		if (!isGrayIpOrWaringUser(email, pwd, isGrayIp)) {
			return LoginStatusCode.LOGIN_OK;
		}

		setNeedVlidateTagInAttribute(inv);

		// 验证码检验逻辑
		final String ick = CookieManager.getInstance().getCookie(
				inv.getRequest(), LoginCookieDef.ICODE);
		if (ick == null || icode == null) {
			// 灰名单用户登录
			return getRtAddrForGrayIp(inv, icode, isGrayIp);
		}


		// 验证码输入正确
		return LoginStatusCode.LOGIN_OK;
	}

	private LoginStatusCode getRtAddrForGrayIp(final Invocation inv,
			final String email, boolean isGrayIp) {
		final String msg = email
				+ (isGrayIp ? " ip need validate " : " user need validate ")
				+ BizFilterUtil.getClientIP(inv.getRequest());
		loginGrayList.info(msg);
		return isGrayIp ? LoginStatusCode.FAILNOTIFYGRAYIP
				: LoginStatusCode.FAILNOTIFYGRAYUSER;
	}

	private boolean isGrayIpOrWaringUser(final String email, final String pwd,
			boolean isGrayIp) {
		return isGrayIp || isWarningUser(email, pwd);
	}

	private boolean isWarningUser(final String username, final String password) {
		return validationImpl.isWarningUser(username, password);
	}

	private void setNeedVlidateTagInAttribute(final Invocation inv) {
		inv.addModel(LoginParamDef.ATTRIBUTE_NEED_VALIDATE, true);
	}
}
