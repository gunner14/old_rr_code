package com.xiaonei.login.logic;

import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.afterlogin_chains.LoginResultChains;
import com.xiaonei.xce.passport.Account;

public interface PassportLogin {

	/**
	 * login<br>
	 * 
	 * @param request
	 *            {@link HttpServletRequest}
	 * @param response
	 *            {@link HttpServletResponse}
	 * @param isAutoLogin
	 *            {@link Boolean}
	 * @param email
	 *            {@link String}
	 * @param pwd
	 *            {@link String}
	 * @param domain
	 *            {@link String}
	 * @param chains
	 *            {@link LoginResultChains}
	 * @return {@link LoginStatusCode}
	 * 
	 * @author tai.wang@opi-corp.com Jan 14, 2010 - 10:54:04 AM
	 */
	public LoginStatusCode login(final HttpServletRequest request,
			final HttpServletResponse response, final boolean isAutoLogin,
			String email, String pwd, String domain,
			final LoginResultChains chains);
    /**
     * 调用新的login登陆接口<br>
     * 
     * @param request
     *            {@link HttpServletRequest}
     * @param response
     *            {@link HttpServletResponse}
     * @param isAutoLogin
     *            {@link Boolean}
     * @param email
     *            {@link String}
     * @param pwd
     *            {@link String}
     * @param domain
     *            {@link String}
     * @param chains
     *            {@link LoginResultChains}
     * @return {@link LoginStatusCode}
     * 
     * @author tai.wang@opi-corp.com Jan 14, 2010 - 10:54:04 AM
     */
    public LoginStatusCode login(final HttpServletRequest request,
            final HttpServletResponse response, final boolean isAutoLogin,
            String email, String pwd, String domain,
            final LoginResultChains chains,boolean hasVirtual);
	/**
	 * getOrigUrl<br>
	 * 过滤不合适的origUrl
	 * 
	 * @param domain
	 * @param origUrl
	 * @param needNotify
	 * @param weakPassportTag
	 *            request.getAttribute(LoginParamDef.ATTRIBUTE_WEAK_PASSWORD_WARNING
	 *            )
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jan 13, 2010 - 10:43:36 AM
	 */
	public String getOrigUrl(String domain, String origUrl, boolean needNotify,
			Object weakPassportTag);

	/**
	 * getOrigUrlEncode<br>
	 * 获得url encode 后的 Url
	 * 
	 * @param domain
	 * @param origUrl
	 * @param needNotify
	 * @param weakPassportTag
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jan 13, 2010 - 11:05:56 AM
	 */
	public String getOrigUrlEncode(String domain, String origUrl,
			boolean needNotify, Object weakPassportTag);

	public void doTaskAfterLogin(HttpServletRequest request, int userId);
	
	/**
	 * getAccountsByEmail<br>
	 * 使用<code>email</code>得到所有帐号
	 *
	 * @param email {@link String}
	 * @return {@link List} {@link Account}
	 *
	 * @author tai.wang@opi-corp.com Sep 25, 2010 - 2:59:49 PM
	 */
	public List<Account> getAccountsByEmail(String email);
	/**
     * 通过ticketId来获取，因为有page虚拟账号情况
     * @param ticketId
     * @return
     */
	public List<Account> getAccountsById(int ticketId);

}
