/**
 * 
 */
package com.xiaonei.sns.platform.core.opt.network;

import org.json.JSONObject;

/**
 * NetworkDirector <br>
 * 
 * @author tai.wang@opi-corp.com Jul 19, 2010 - 11:49:15 AM
 */
public class NetworkDirector {
	
	private static final NetworkDirector instance = new NetworkDirector();

	public static NetworkDirector getInstance() {
		return instance;
	}

	public NetworkModel construct(int userId) {
		NetworkBuilder builder = new NetworkBuilder(userId);
		builder.buildUnivId();
		builder.buildUnivName();
		return builder.getNetwork();
	}

	public JSONObject constructJson(int userId) {
		NetworkBuilder builder = new NetworkBuilder(userId);
		builder.buildUnivId();
		builder.buildUnivName();
		return builder.getNetworkJson();
	}
}
