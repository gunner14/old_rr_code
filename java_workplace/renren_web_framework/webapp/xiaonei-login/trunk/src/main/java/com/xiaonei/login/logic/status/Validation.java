package com.xiaonei.login.logic.status;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.xce.passport.Account;

public interface Validation {

	/**
	 * 判断用户是否为灰名单内
	 * 
	 * @param id
	 * @return
	 */
	public boolean isWarningUser(String username, String password);

	/**
	 * 判断ip地址是否被封禁
	 * 
	 * @param ip
	 * @return
	 */
	public boolean isBanIP(String ip);

	/**
	 * 判断是否是合法的手机号码
	 * 
	 * @param mobile
	 * @return
	 */
	public boolean isValidMobile(String mobile);

	/**
	 * 判断是否是有效的email地址
	 * 
	 * @param mailAddr
	 * @return
	 */
	public boolean isValidEmailAddr(String mailAddr);

	/**
	 * 检查当前用户的状态，返回对应的状态码<br>
	 * 返回的状态码大于零，表示用户处于不正常的状态应该禁止登录<br>
	 * 状态码等于零表示正常状态
	 * 
	 * @param user
	 * @return
	 */
	public LoginStatusCode checkUserStatus(Account account);

	/**
	 * 检查用户的安全状态
	 * 
	 * @param user
	 * @return
	 */
	public boolean checkUserSafeStatus(User user);

	public void setWarningTagOfWeakPasswordInAttribut(HttpServletRequest request);

	public Boolean getWarningTagOfWeakPwd(HttpServletRequest request);

	public boolean isWeekPasswordInMd5Format(String md5Password);

	/**
	 * isBindedIPDoNotBind<br>
	 * 绑定表中的帐号没有在对应ip传入,返回true
	 * 
	 * @param ip
	 * @param id
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Mar 3, 2010 - 11:57:18 AM
	 */
	public boolean isBindedIPDoNotBind(String ip, int id);
}
