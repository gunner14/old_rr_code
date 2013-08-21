package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.util.List;

import com.xiaonei.sns.platform.core.opt.ice.IFriendFinderByCommonAdapter;
import com.xiaonei.xce.friendfinderbycommon.CommonFriendData;
import com.xiaonei.xce.friendfinderbycommon.FriendFinderByCommonAdapter;

public class WFriendFinderByCommonAdapter extends
		BaseUsersAdapter<FriendFinderByCommonAdapter> implements
		IFriendFinderByCommonAdapter {

	protected WFriendFinderByCommonAdapter() {

	}

	@Override
	public List<CommonFriendData> getCommonFriendItem(int userId, int limit) {
		return getAdapter().getCommonFriendItem(userId, limit);
	}

	@Override
	public List<Integer> getFriendsByCommon(int guestId, int hostId, int limit) {
		return getAdapter().getFriendsByCommon(guestId, hostId, limit);
	}

	@Override
	protected String getServiceName() {
		return "friend finder by common";
	}

	@Override
	protected FriendFinderByCommonAdapter makeObject() {
		return FriendFinderByCommonAdapter.getInstance();
	}

}
