/**
 * 
 */
package com.xiaonei.platform.core.model;

import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.xce.frienddist.FriendDistCount;

/**
 * @author lookis
 * 
 */
public class WFriendDistCount extends FriendDistCount {
	public String getNetworkName() {
		return NetworkManager.getInstance().getNetById(getNetworkId())
				.getName();
	}
}
