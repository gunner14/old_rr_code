/**
 * 
 */
package com.xiaonei.login.logic.failforreg;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;

/**
 * LoginFailedForRegistrationProcess <br>
 * 注册的登录失败流程
 * 
 * @author tai.wang@opi-corp.com Jan 26, 2010 - 11:13:17 AM
 */
public interface LoginFailedForRegistrationProcess {

	/**
	 * isLoginFailForReg<br>
	 * 确定为登录失败流程
	 * 
	 * @param email
	 * @param request
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jan 26, 2010 - 10:55:11 AM
	 */
	boolean isLoginFailForReg(String email, HttpServletRequest request);

	/**
	 * LoginFailForReg<br>
	 * 登录失败去注册
	 * 
	 * @param inv
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jan 26, 2010 - 10:57:10 AM
	 */
	String loginFailForReg(final Invocation inv);

	/**
	 * LoginFailForRegSaveUserInfoIntoMemcache<br>
	 * 登录失败流程中,把用户输入的东西保存到memcache 中
	 * 
	 * @param inv
	 * @return 返回memcache的一个key
	 * 
	 * @author tai.wang@opi-corp.com Jan 26, 2010 - 10:58:51 AM
	 */
	String loginFailForRegSaveUserInfoIntoMemcache(final Invocation inv);
}
