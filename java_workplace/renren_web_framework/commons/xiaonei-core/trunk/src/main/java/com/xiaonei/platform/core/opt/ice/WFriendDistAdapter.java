package com.xiaonei.platform.core.opt.ice;

import java.util.List;

import com.xiaonei.platform.core.model.WFriendDistCount;
import com.xiaonei.platform.core.model.WFriendDistWithTotal;
import com.xiaonei.platform.core.model.factory.WFriendDistFactory;
import com.xiaonei.xce.frienddist.FriendDistAdapter;

public class WFriendDistAdapter {
	/**
	 * @param factory
	 */

	private final FriendDistAdapter<WFriendDistCount, WFriendDistWithTotal> adapter = new FriendDistAdapter<WFriendDistCount, WFriendDistWithTotal>(
			new WFriendDistFactory());

	private WFriendDistAdapter() {

	}

	private static WFriendDistAdapter instance = new WFriendDistAdapter();

	public static WFriendDistAdapter getInstance() {
		return WFriendDistAdapter.instance;
	}

	public WFriendDistWithTotal getFriendDistWithTotal(int userId) {
		return adapter.getFriendDistWithTotal(userId);

	}

	public List<WFriendDistCount> getFriendDistListAll(int userId) {
		return adapter.getFriendDistListAll(userId);
	}

	public int getFriendDistByUniversity(int userId, int universityId) {
		return adapter.getFriendDistByUniversity(userId, universityId);
	}

	public void rebuildFriendDist(int userId) {
		adapter.rebuildFriendDist(userId);
	}

}
