/**
 * 
 */
package com.xiaonei.login.logic.failswitcher.nodes;

import net.paoding.rose.web.Invocation;

import com.xiaonei.login.constants.LoginStatusCode;

/**
 * ILoginFailSwitchNode <br>
 * 
 * @author tai.wang@opi-corp.com Jun 28, 2010 - 11:48:09 AM
 */
public interface ILoginFailSwitchNode {
	public String failUri ="r:http://www.renren.com/SysHome.do" ;//"/views/login.jsp";
	/**
	 * 
	 * 
	 * @return
	 */
	LoginStatusCode getTag();

	/**
	 * 
	 * 
	 * @param domain
	 * @param inv
	 * @param status
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jul 8, 2010 - 6:34:28 PM
	 */
	String fail(String domain, Invocation inv, LoginStatusCode status);
}
