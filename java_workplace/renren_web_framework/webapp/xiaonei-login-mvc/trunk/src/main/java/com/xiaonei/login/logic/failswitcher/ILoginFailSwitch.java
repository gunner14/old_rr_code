package com.xiaonei.login.logic.failswitcher;

import net.paoding.rose.web.Invocation;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.failswitcher.nodes.ILoginFailSwitchNode;

/**
 * 
 * 
 * @author Wang Tai (tai.wang@opi-corp.com)
 */
public interface ILoginFailSwitch {

	/**
	 * failSwitch<br>
	 * 
	 * @param domain
	 * @param inv
	 * @param failCode
	 *            {@link LoginStatusCode}
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jul 30, 2010 - 11:55:55 AM
	 */
	String failSwitch(String domain, Invocation inv, LoginStatusCode failCode);

	void setUpSwitchNode(ILoginFailSwitchNode node);

	void removeSwitchNode(ILoginFailSwitchNode node);

	boolean isEmpty();
}
