package com.xiaonei.login.controllers.ajax;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.Ajax;
import com.xiaonei.login.controllers.base.RemoteAddrFetherImpl;
import com.xiaonei.login.form.PassportForm;
import com.xiaonei.login.logic.annotation.Singleton;
import com.xiaonei.login.logic.captcha.CaptchaAdapterFactory;
import com.xiaonei.login.logic.captcha.ICaptchaAdapter;
import com.xiaonei.login.logic.validator.annotation.FormValidator;
/**
 * 
 * @author xiaodong.yuan
 * 提供ajax接口，当浏览器输入帐户时，异步请求本接口获取是否要显示验证码的结果
 *
 */
@Path(value = { "ShowCaptcha.do", "ShowCaptcha", "showCaptcha" })
@Singleton
@Ajax
public class CaptchaController {
    private final ICaptchaAdapter captchaAdapter = CaptchaAdapterFactory.getCaptchaAdapterInstance();

    /**
     * 是否显示验证码
     * 
     * @param account
     *            帐号
     * @param ip
     *            登录IP
     * @return 验证码级别 0为不显示
     */
    @Post
    public int isShowCaptcha(final Invocation inv,//
            @FormValidator(validateAllParams = true) final PassportForm pForm) {
    	 final RemoteAddrFetherImpl addrFetherImpl = new RemoteAddrFetherImpl();
         final String ip = addrFetherImpl.getRemoteAddr(inv.getRequest());
        return this.captchaAdapter.getCaptchaGrade(inv.getRequest(), pForm.getEmail(), ip);
    }

}
