package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.util.List;

import com.xiaonei.sns.platform.core.opt.ice.IBuddyByOnlineTimeCacheAdapter;
import com.xiaonei.xce.buddybyonlinetimecache.BuddyByOnlineTimeCacheAdapter;

/**
 * 
 * @author michael(liyong@opi-corp.com)
 * 
 */
public class WBuddyByOnlineTimeCacheAdapter extends
		BaseUsersAdapter<BuddyByOnlineTimeCacheAdapter> implements
		IBuddyByOnlineTimeCacheAdapter {

	protected WBuddyByOnlineTimeCacheAdapter() {
	}

	@Override
	public int getFriendCount(int userId) {
		return getAdapter().getFriendCount(userId);
	}

	@Override
	public List<Integer> getFriendListAsc(int userId, int limit) {
		return getAdapter().getFriendList(userId, limit);
	}

	@Override
	public List<Integer> getFriendListAsc(int userId) {
		return getFriendListAsc(userId, -1);
	}

	@Override
	protected String getServiceName() {
		return "buddy by online time cache";
	}

	@Override
	protected BuddyByOnlineTimeCacheAdapter makeObject() {
		return BuddyByOnlineTimeCacheAdapter.getInstance();
	}

}
