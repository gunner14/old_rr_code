/**
 * 
 */
package com.xiaonei.sns.platform.core.opt.network;

import org.json.JSONObject;

/**
 * INetwrokBuilder <br>
 * 构建network对象
 * 
 * @author tai.wang@opi-corp.com Jul 19, 2010 - 11:02:14 AM
 */
public interface INetwrokBuilder {

	/**
	 * buildUnivId<br>
	 * 构建网络id
	 * 
	 * @author tai.wang@opi-corp.com Jul 19, 2010 - 11:06:26 AM
	 */
	void buildUnivId();

	/**
	 * buildUnivName<br>
	 * 构建网络姓名
	 * 
	 * @author tai.wang@opi-corp.com Jul 19, 2010 - 11:07:06 AM
	 */
	void buildUnivName();

	/**
	 * getNetwork<br>
	 * 返回网络对象
	 * 
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jul 19, 2010 - 11:07:28 AM
	 */
	NetworkModel getNetwork();
	
	/**
	 * getNetworkJson<br>
	 * 返回一个network 的json
	 *
	 * @return
	 *
	 * @author tai.wang@opi-corp.com Jul 19, 2010 - 11:28:04 AM
	 */
	JSONObject getNetworkJson();
}
