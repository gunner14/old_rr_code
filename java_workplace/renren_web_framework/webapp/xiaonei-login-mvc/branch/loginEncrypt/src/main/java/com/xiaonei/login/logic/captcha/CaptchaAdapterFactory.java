package com.xiaonei.login.logic.captcha;
/**
 * 
 * @author xiaodong.yuan
 * 返回ICaptchaAdapter接口实例
 *
 */
public class CaptchaAdapterFactory {
	public static ICaptchaAdapter getCaptchaAdapterInstance(){
		return CaptchaAdapter.getInstance();
	}
	public static IWeakPassword getWeakPasswordInstance(){
		return CaptchaAdapter.getInstance();
	}
}
