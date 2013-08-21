package com.xiaonei.login.controllers.ajax;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.commons.lang3.StringUtils;

import com.xiaonei.antispam.validator.HtmlPure;
import com.xiaonei.commons.interceptors.Ajax;
import com.xiaonei.login.controllers.base.RemoteAddrFetherImpl;
import com.xiaonei.login.form.PassportForm;
import com.xiaonei.login.logic.annotation.Singleton;
import com.xiaonei.login.logic.captcha.CaptchaAdapterFactory;
import com.xiaonei.login.logic.captcha.ICaptchaAdapter;
import com.xiaonei.login.logic.validator.annotation.FormValidator;

/**
 * 
 * @author yong.dou
 *         提供ajax接口，当浏览器输入帐户时，异步请求本接口获取是否要显示验证码的结果（56专用，为支持跨域请求，采用jsonp方式）
 * 
 */
@Path(value = { "show56Captcha" })
@Singleton
@Ajax
public class Captcha56Controller {
	private final ICaptchaAdapter captchaAdapter = CaptchaAdapterFactory.getCaptchaAdapterInstance();

	/**
	 * 是否显示验证码
	 * 
	 * @param account
	 *            帐号
	 * @return 回调函数（isSafe） isSafe:true不需要验证码 false需要验证码
	 */
	@Get
	public String isShowCaptcha(final Invocation inv,
			@FormValidator(validateAllParams = true) final PassportForm pForm,
			@Param(value = "callback") @HtmlPure String callback) {

		// 验证referer
		// 56的域名现在有56.com和59xiu.com，这两种referer都是合法的
		String referer = inv.getRequest().getHeader("Referer");
		if (StringUtils.isBlank(referer)
				|| (!referer.matches("^http://[A-Za-z0-9\\.]+.56.com.*") 
						&& !referer.matches("^http://[A-Za-z0-9\\.]+.59xiu.com.*"))) {
			return "@false";
		}

		final RemoteAddrFetherImpl addrFetherImpl = new RemoteAddrFetherImpl();
		final String ip = addrFetherImpl.getRemoteAddr(inv.getRequest());
		int value = this.captchaAdapter.getCaptchaGrade(inv.getRequest(), pForm.getEmail(), ip);
		return "@" + callback + "(" + String.valueOf(value == 0) + ")";
	}
}
