package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.util.List;

import com.xiaonei.sns.platform.core.opt.ice.IBuddyByAddTimeCacheAdapter;
import com.xiaonei.xce.buddybyaddtimecache.BuddyByAddTimeCacheAdapter;

/**
 * 
 * @author michael(liyong@opi-corp.com)
 * 
 */
public class WBuddyByAddTimeCacheAdapter extends
		BaseUsersAdapter<BuddyByAddTimeCacheAdapter<?>> implements
		IBuddyByAddTimeCacheAdapter {

	protected WBuddyByAddTimeCacheAdapter() {
	}

	@Override
	public int getFriendCount(int userId) {
		return getAdapter().getFriendCount(userId);
	}

	@Override
	public List<Integer> getFriendList(int userId, int limit) {
		return getAdapter().getFriendList(userId, limit);
	}

	@Override
	public List<Integer> getFriendList(int userId) {
		return getFriendList(userId, -1);
	}

	@Override
	protected String getServiceName() {
		return "buddy by add time cache";
	}

	@Override
	protected BuddyByAddTimeCacheAdapter<?> makeObject() {
		return BuddyByAddTimeCacheAdapter.getInstance();
	}

}
