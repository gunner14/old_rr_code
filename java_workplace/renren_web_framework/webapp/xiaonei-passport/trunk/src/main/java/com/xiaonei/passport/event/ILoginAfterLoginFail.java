package com.xiaonei.passport.event;

import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.model.LoginResult;
/**
 * 当出现错误时，需要做的处理
 * @author xiaodong.yuan
 *
 */
public interface ILoginAfterLoginFail {
	/**
	 * 当登陆失败时，需要做的处理
	 * @param result
	 * @param userName 用户名
	 * @param md5Password 密码
	 * @param loginType 登陆类型，每个申请登陆的应用都会申请一个
	 * @param context 包含request中相关的数据. 此接口会明确要哪些数据.但是为了扩展,客户端可以把整个request数据映射到对应得Map中.以后如果登陆服务需要新的参数,就不需要改变客户端代码.
	 */
	public void process(LoginResult result, ILoginParameter loginParameter);
	
	
	/**
	 * 是否可以处理这类错误
	 * @param code 错误码
	 * @return ture or false
	 */
	public boolean canProcessLoginStatusCode(LoginStatusCode code);
}
