package com.xiaonei.login.logic;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;

public interface DomainOpr {

	public void setDomainEnv(HttpServletRequest request);

	public void setDomainEnv(HttpServletRequest request, String domain);

	public String getDomainParam(String domain, HttpServletRequest request);

	/**
	 * isRenKaiDomain<br>
	 * 判断是否是Kaixin 或Renren 的 domain
	 * 
	 * @param domain
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jan 12, 2010 - 11:36:30 AM
	 */
	public boolean isRenKaiDomain(String domain);

	/**
	 * CurrentDomainIsNotPassportDomain<br>
	 * 
	 * @param request
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jan 25, 2010 - 10:59:32 PM
	 */
	public boolean currentDomainIsNotPassportDomain(
			final HttpServletRequest request);

	/**
	 * getPassportDomainAddrFromCurrentAddr<br>
	 * 
	 * @param inv
	 * @param pForm
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jan 25, 2010 - 11:03:24 PM
	 */
	public String getPassportDomainAddrFromCurrentAddr(final Invocation inv,
			final String origURL);

	/**
	 * getDomainAsParameterFromCurUrl<br>
	 * 从当前请求passport的地址中,找到适当的domain参数,example:(domain=kaixin.com)
	 * 
	 * @param dm
	 * @param request
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jan 25, 2010 - 11:20:44 PM
	 */
	public String getDomainAsParameterFromCurUrl(HttpServletRequest request);

}
