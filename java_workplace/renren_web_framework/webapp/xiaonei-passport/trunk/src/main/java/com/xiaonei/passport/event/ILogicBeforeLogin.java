package com.xiaonei.passport.event;

import java.util.Map;

import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.constants.LoginStatusCode;
public interface ILogicBeforeLogin {
	/**
	 * 登陆前调用一个有configure返回的有序的list。其中的每个item都是一个LoginBeforeLogin接口。
	       分别调用每个接口，如果返回false，登陆失败，返回getLoginStatusCode对应的错误码
	 * @param userName 用户名
	 * @param md5Password 密码
	 * @param icode 验证码
	 * @param captchaType 验证码类型,有安全中心提供
	 * @param ick 和验证码有关,在icode服务器端的key,用来对应产生的icode.默认有安全中心写在cooike中
	 * @param loginType 登陆类型，每个申请登陆的应用都会申请一个
	 * @param context 包含request中相关的数据. 此接口会明确要哪些数据.但是为了扩展,客户端可以把整个request数据映射到对应得Map中.以后如果登陆服务需要新的参数,就不需要改变客户端代码.
	 * @return
	 */
	LoginStatusCode process(ILoginParameter loginParameter);
	
}
