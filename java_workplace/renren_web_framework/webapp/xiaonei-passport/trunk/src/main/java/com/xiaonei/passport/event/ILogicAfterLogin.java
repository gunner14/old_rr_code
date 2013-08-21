package com.xiaonei.passport.event;

import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.model.LoginResult;

/**
 * 
 * @author 登陆成功后需要做的逻辑
 *
 */
public interface ILogicAfterLogin {
	/**
	 * 登陆成功后调用一个有configure返回的有序的list。其中的每个item都是一个LoginAfterLogin接口。
	       分别调用每个接口，如果返回false，登陆失败，返回getLoginStatusCode对应的错误码

	 * @param result 到目前为止已经完成登陆的结果
	 * @param loginType 登陆类型，每个申请登陆的应用都会申请一个
	 * @param userName 用户名
	 * @param md5Password 密码
	 * @param context 包含request中相关的数据. 此接口会明确要哪些数据.但是为了扩展,客户端可以把整个request数据映射到对应得Map中.以后如果登陆服务需要新的参数,就不需要改变客户端代码.
	 * @return
	 */
	LoginStatusCode process(LoginResult result, ILoginParameter loginParameter);

}
