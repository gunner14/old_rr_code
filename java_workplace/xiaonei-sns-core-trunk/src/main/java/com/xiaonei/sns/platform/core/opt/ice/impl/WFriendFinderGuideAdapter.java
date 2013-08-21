package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.util.List;
import java.util.Map;

import mop.hi.oce.domain.factory.FriendFinderFactory;
import mop.hi.oce.domain.model.FriendCommon;
import mop.hi.oce.domain.model.FriendShare;
import mop.hi.oce.domain.model.FriendShares;

import com.xiaonei.sns.platform.core.opt.ice.IFriendFinderGuideReplicaAdapter;
import com.xiaonei.sns.platform.core.opt.ice.model.SnsFriendCommon;
import com.xiaonei.sns.platform.core.opt.ice.model.SnsFriendShare;
import com.xiaonei.xce.friendfinder.FriendFinderGuideReplicaAdapter;

/**
 * 
 * @author Michael(liyong@opi-corp.com)
 * 
 */
public class WFriendFinderGuideAdapter extends
		BaseUsersAdapter<FriendFinderGuideReplicaAdapter> implements
		IFriendFinderGuideReplicaAdapter {

	protected WFriendFinderGuideAdapter() {

	}

	@Override
	public List<FriendCommon> getCommonFriends(int userId, int begin, int limit) {
		return getAdapter().getCommonFriends(userId, begin, limit);
	}

	@Override
	public List<FriendCommon> getCommonFriendsRandom(int userId, int limit) {
		return getAdapter().getCommonFriendsRandom(userId, limit);
	}

	@Override
	public List<FriendShare> getSharedFriends(int userId1, int userId2,
			int begin, int limit) {
		return getAdapter().getSharedFriends(userId1, userId2, begin, limit);
	}

	@Override
	public Map<Integer, FriendShares> getSharedFriends(int userId,
			List<Integer> friendIds, int limit) {
		return getAdapter().getSharedFriends(userId, friendIds, limit);
	}

	@Override
	public int getSharedFriendsCount(int userId1, int userId2) {
		return getAdapter().getSharedFriendsCount(userId1, userId2);
	}

	@Override
	public void removeCommonFriendWithBlock(int userId, int blockId) {
		getAdapter().removeCommonFriendWithBlock(userId, blockId);
	}

	@Override
	public void rebuildCommonFriend(int userId) {
		getAdapter().rebuildCommonFriend(userId);
	}

	@Override
	protected String getServiceName() {
		return "friend finder guide";
	}

	@Override
	protected FriendFinderGuideReplicaAdapter makeObject() {
		return new FriendFinderGuideReplicaAdapter(new FriendFinderFactory() {

			@Override
			public FriendShare createFriendShare() {
				return new SnsFriendShare();
			}

			@Override
			public FriendCommon createFriendCommon() {
				return new SnsFriendCommon();
			}
		});
	}

}
