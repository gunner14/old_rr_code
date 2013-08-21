package com.xiaonei.login.logic.afterlogin_chains;

/**
 * LoginResultChains <br>
 * 
 * @author tai.wang@opi-corp.com Jul 30, 2010 - 5:49:58 PM
 */
public interface LoginResultChains {

	/**
	 * setNode<br>
	 * set node to the end of the chain, except for the chain already has the
	 * same node;
	 * 
	 * @param node
	 *            {@link LoginResultChainNode}
	 * 
	 * @author tai.wang@opi-corp.com Jul 30, 2010 - 5:50:06 PM
	 */
	void setNode(LoginResultChainNode node);

	/**
	 * getFristNode<br>
	 * 
	 * @return {@link LoginResultChainNode}
	 * 
	 * @author tai.wang@opi-corp.com Jul 30, 2010 - 5:51:41 PM
	 */
	LoginResultChainNode getFristNode();

	/**
	 * isEmpty<br>
	 * 
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jul 30, 2010 - 5:53:11 PM
	 */
	boolean isEmpty();
}
