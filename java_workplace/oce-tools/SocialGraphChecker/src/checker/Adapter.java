package checker;

import java.util.List;
import java.util.Map;

import mop.hi.oce.adapter.impl.BuddyCoreAdapterImpl;
import mop.hi.oce.domain.buddy.BuddyApplyWithInfo;
import mop.hi.oce.domain.buddy.BuddyRelation;

import com.xiaonei.xce.buddyapplycache.BuddyApplyCacheAdapter;

import com.xiaonei.xce.buddybyaddtimecache.BuddyByAddTimeCacheAdapter;
import com.xiaonei.xce.buddybyidcache.BuddyByIdCacheAdapter;
import com.xiaonei.xce.buddybynamecache.BuddyByNameCacheAdapter;
import com.xiaonei.xce.buddybyonlinetimecache.BuddyByOnlineTimeCacheAdapter;
import com.xiaonei.xce.usercache.UserCacheAdapter;
import com.xiaonei.xce.usercount.UserCountAdapter;

public class Adapter {
	public static UserCountAdapter _adapter_usercount = UserCountAdapter
			.getInstance();
	public static BuddyApplyCacheAdapter<MyBuddyApplyCacheData, MyBuddyApplyCacheDataWithTotal> _adapter_buddyapply = new BuddyApplyCacheAdapter<MyBuddyApplyCacheData, MyBuddyApplyCacheDataWithTotal>(
			new MyBuddyApplyCacheFactory());
	public static BuddyByIdCacheAdapter _adapter_buddybyidcache = BuddyByIdCacheAdapter
			.getInstance();
	public static BuddyByNameCacheAdapter _adapter_buddybynamecache = BuddyByNameCacheAdapter
			.getInstance();
	public static BuddyByAddTimeCacheAdapter _adapter_buddybyaddtimecache = BuddyByAddTimeCacheAdapter
			.getInstance();
	public static BuddyByOnlineTimeCacheAdapter _adapter_buddybyonlinetimecache = BuddyByOnlineTimeCacheAdapter
			.getInstance();
	public static BuddyCoreAdapterImpl _adapter_buddycore = new BuddyCoreAdapterImpl();
	public static UserCacheAdapter<MyUserCache> _adapter_usercache = new UserCacheAdapter<MyUserCache>(
			new MyUserCacheFactory());

	public static int getRequestCount(int userId) {
		return _adapter_usercount.get(userId, UserCountAdapter.GUEST_REQUEST);
	}

	public static int getBuddyCoreAppliedCount(int userId) {
		return _adapter_buddycore.getAppliedCount(userId);
	}

	public static MyBuddyApplyCacheDataWithTotal getBuddyApplyCacheWithTotal(
			int userId, int begin, int limit) {
		return _adapter_buddyapply.getBuddyApplyCacheWithTotal(userId, begin,
				limit);
	}

	public static List<BuddyRelation> getBuddyCoreFriendList(int userId,
			int begin, int limit) {
		return _adapter_buddycore.getFriendList(userId, begin, limit);
	}

	public static List<Integer> getBuddyByIdCache(int userId) {
		return _adapter_buddybyidcache.getFriendListAsc(userId, -1);
	}

	public static Map<Integer, List<Integer>> getBuddyByIdCaches(List<Integer> userIds) {
		return _adapter_buddybyidcache.getFriendListsAsc(userIds);
	}

	public static List<Integer> getBuddyByNameCache(int userId) {
		return _adapter_buddybynamecache.getFriendList(userId, -1);
	}

	public static List<Integer> getBuddyByAddTimeCache(int userId) {
		return _adapter_buddybyaddtimecache.getFriendList(userId, -1);
	}

	public static List<Integer> getBuddyByOnlineTimeCache(int userId) {
		return _adapter_buddybyonlinetimecache.getFriendList(userId, -1);
	}

	public static BuddyRelation getRelation(int userId, int otherId) {
		return _adapter_buddycore.getRelation(userId, otherId);
	}

	public static void reloadBuddyApplyCache(int userId) {
		BuddyApplyCacheAdapter.rebuildBuddyApplyCache(userId);
	}

	public static void reloadBuddyCache(int userId) {
		_adapter_buddybyidcache.reload(userId);
	}

	public static void addApply(int userId, int otherId) {
		BuddyApplyWithInfo apply = new BuddyApplyWithInfo(userId, otherId);
		_adapter_buddycore.addApply(apply);
	}

	public static void acceptApply(int userId, int otherId) {
		BuddyApplyWithInfo apply = new BuddyApplyWithInfo(userId, otherId);
		_adapter_buddycore.acceptApply(apply);
	}

	public static void removeFriend(int userId, int otherId) {
		BuddyRelation relation = new BuddyRelation(userId, otherId,
				BuddyRelation.BuddyDesc.Friend);
		_adapter_buddycore.removeFriend(relation);
	}

	public static void addFriend(int userId, int otherId) {
		_adapter_buddycore.addFriend(userId, otherId);
	}
}
