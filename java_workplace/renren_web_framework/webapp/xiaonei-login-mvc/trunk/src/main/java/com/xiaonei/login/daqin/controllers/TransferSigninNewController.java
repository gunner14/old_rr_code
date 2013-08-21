package com.xiaonei.login.daqin.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.antispam.validator.HtmlPure;
import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.origurl.DontPutOrigURL;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.annotation.Singleton;
import com.xiaonei.login.logic.captcha.CaptchaAdapterFactory;
import com.xiaonei.xce.passport.Account;

@Path("t-signin2")
@DontPutOrigURL
@IgnoreUserStatusImportedValidation
@IgnorePassportValidation
@Singleton
public class TransferSigninNewController extends TransferSigninController {
	@Override
	protected LoginStatusCode md5login(Invocation inv, String email,
			String md5Password, String domain) {
	     final String ip = addrFetherImpl.getRemoteAddr(inv.getRequest());
		final boolean captchaValidationResult = CaptchaAdapterFactory
				.getCaptchaAdapterInstance().validateCatcha(inv.getRequest(),
						email, ip);
		if (!captchaValidationResult) {
			CaptchaAdapterFactory.getCaptchaAdapterInstance()
					.processLoginCountAfterLoginFail(inv.getRequest(), email,
							LoginStatusCode.FAILVALIDATELOGIN, ip);
			return LoginStatusCode.FAILVALIDATELOGIN;
		}
		

		LoginStatusCode rt= super.md5login(inv, email, md5Password, domain);
		if(LoginStatusCode.LOGIN_OK==rt){
			  final Account mainAccount = (Account) inv.getRequest().getAttribute(
		                LoginParamDef.ATTRIBUTE_ACCOUNT);
			CaptchaAdapterFactory.getCaptchaAdapterInstance().processLoginCountAfterLoginSuccess(inv.getRequest(), email, mainAccount.getId(), ip);
		}else{
			CaptchaAdapterFactory.getCaptchaAdapterInstance()
			.processLoginCountAfterLoginFail(inv.getRequest(), email,
					rt, ip);
		}
		return rt;
	}

	@Override
	@Post
	 public String post(final Invocation inv, @HtmlPure @Param("e") final String email,
	            @Param("p") final String password, @HtmlPure @Param("d") final String domain,
	            @HtmlPure @Param("u") final String url) {
		return super.post(inv, email, password, domain, url);
	}
	

}
