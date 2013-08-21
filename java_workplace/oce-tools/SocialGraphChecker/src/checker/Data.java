package checker;

import mop.hi.oce.domain.factory.FriendFinderFactory;
import mop.hi.oce.domain.model.FriendCommon;
import mop.hi.oce.domain.model.FriendShare;

import com.xiaonei.xce.buddyapplycache.BuddyApplyCacheData;
import com.xiaonei.xce.buddyapplycache.BuddyApplyCacheFactory;
import com.xiaonei.xce.buddyapplycache.BuddyApplyCacheWithTotal;

import com.xiaonei.xce.usercache.UserCache;
import com.xiaonei.xce.usercache.UserCacheFactory;

class MyFriendCommon extends FriendCommon {
}
class MyFriendShare extends FriendShare {
}
class MyFriendFinderFactory implements FriendFinderFactory {
	public FriendCommon createFriendCommon() {
		return new MyFriendCommon();
	}

	public FriendShare createFriendShare() {
		return new MyFriendShare();
	}
}


class MyBuddyApplyCacheFactory
		implements
		BuddyApplyCacheFactory<MyBuddyApplyCacheData, MyBuddyApplyCacheDataWithTotal> {
	public MyBuddyApplyCacheData createBuddyApplyCacheData() {
		return new MyBuddyApplyCacheData();
	}

	public MyBuddyApplyCacheDataWithTotal createBuddyApplyCacheWithTotal() {
		return new MyBuddyApplyCacheDataWithTotal();
	}
}
class MyBuddyApplyCacheData extends BuddyApplyCacheData {
}
class MyBuddyApplyCacheDataWithTotal extends
		BuddyApplyCacheWithTotal<MyBuddyApplyCacheData> {
}

class MyUserCache extends UserCache {
}
class MyUserCacheFactory implements UserCacheFactory<MyUserCache> {
	public MyUserCache createUserCache() {
		return new MyUserCache();
	}
}
