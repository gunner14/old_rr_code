/**
 * 
 */
package com.xiaonei.platform.core.model.factory;

import com.xiaonei.platform.core.model.WFriendDistCount;
import com.xiaonei.platform.core.model.WFriendDistWithTotal;
import com.xiaonei.xce.frienddist.FriendDistFactory;

/**
 * @author lookis
 * 
 */
public class WFriendDistFactory implements
		FriendDistFactory<WFriendDistCount, WFriendDistWithTotal> {

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.xiaonei.xce.frienddist.FriendDistFactory#createFriendDistCount()
	 */
	public WFriendDistCount createFriendDistCount() {
		return new WFriendDistCount();
	}

	public WFriendDistWithTotal createFriendDistWithTotal() {
		return new WFriendDistWithTotal();
	}

}
