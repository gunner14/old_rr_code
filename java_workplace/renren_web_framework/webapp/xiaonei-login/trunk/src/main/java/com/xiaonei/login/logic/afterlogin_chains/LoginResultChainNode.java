package com.xiaonei.login.logic.afterlogin_chains;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.model.LoginModel;

/**
 * LoginResultChainNode <br>
 * 
 * @author tai.wang@opi-corp.com Jul 30, 2010 - 2:30:29 PM
 */
public interface LoginResultChainNode {

	/**
	 * setNext<br>
	 * set后面的节点
	 * 
	 * @param node
	 *            {@link LoginResultChainNode}
	 * 
	 * @author tai.wang@opi-corp.com Jul 30, 2010 - 3:10:15 PM
	 */
	void setNext(final LoginResultChainNode node);

	/**
	 * getNext<br>
	 * 
	 * @return {@link LoginResultChainNode}
	 * 
	 * @author tai.wang@opi-corp.com Jul 30, 2010 - 6:00:51 PM
	 */
	LoginResultChainNode getNext();

	/**
	 * isEnd<br>
	 * 
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jul 30, 2010 - 5:59:03 PM
	 */
	boolean isEnd();

	/**
	 * doChain<br>
	 * 
	 * @param loginModel
	 *            {@link LoginModel}
	 * @param perCode
	 *            {@link LoginStatusCode}
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jul 30, 2010 - 3:22:36 PM
	 */
	LoginStatusCode doChain(final LoginModel loginModel, LoginStatusCode perCode);
	
}
