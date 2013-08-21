package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.xiaonei.sns.platform.core.opt.ice.IBuddyByIdCacheAdapter;
import com.xiaonei.xce.buddybyidcache.BuddyByIdCacheAdapter;

/**
 * WBuddyByIdCacheAdapter <br>
 * 
 * @author tai.wang@opi-corp.com Sep 25, 2010 - 2:04:02 PM
 * @author michael(liyong@opi-corp.com)
 */
public class WBuddyByIdCacheAdapter extends BaseUsersAdapter<BuddyByIdCacheAdapter> implements
        IBuddyByIdCacheAdapter {

    protected WBuddyByIdCacheAdapter() {
    }

    @Override
    public int getFriendCount(int userId) {
        return getAdapter().getFriendCount(userId);
    }

    @Override
    public List<Integer> getFriendListAsc(int userId, int limit) {
        return getAdapter().getFriendListAsc(userId, limit);
    }

    @Override
    public List<Integer> getFriendListAsc(int userId) {
        return getFriendListAsc(userId, -1);
    }

    @Override
    public Map<Integer, Integer> getFriendCounts(List<Integer> userIds) {
        return getAdapter().getFriendCounts(userIds);
    }

    @Override
    public Map<Integer, List<Integer>> getFriendListsAsc(List<Integer> userIds) {
        return getAdapter().getFriendListsAsc(userIds);
    }

    @Override
    protected String getServiceName() {
        return "buddy by id cache";
    }

    @Override
    protected BuddyByIdCacheAdapter makeObject() {
        return BuddyByIdCacheAdapter.getInstance();
    }

    @Override
    public Set<Integer> getSharedFriends(int userId1, int userId2) {
        if (userId1 == userId2) {
            List<Integer> friendListAsc = getFriendListAsc(userId1);
            if (null != friendListAsc) {
                return new HashSet<Integer>(friendListAsc);
            } else {
                return new HashSet<Integer>(0);
            }
        }
        List<Integer> userids = new ArrayList<Integer>(2);
        userids.add(userId1);
        userids.add(userId2);
        Map<Integer, List<Integer>> friendsMap = getFriendListsAsc(userids);
        if (null == friendsMap.get(userId1) || null == friendsMap.get(userId2)) {
            return new HashSet<Integer>(0);
        }
        friendsMap.get(userId1).retainAll(friendsMap.get(userId2));
        return new HashSet<Integer>(friendsMap.get(userId1));
    }

    @Override
    public Set<Integer> getSharedFriendsLimit2K(int userId1, int userId2) {
        int limit = 2000;
        if (userId1 == userId2) {
            List<Integer> friendListAsc = getFriendListAsc(userId1, limit);
            if (null != friendListAsc) {
                return new HashSet<Integer>(friendListAsc);
            } else {
                return new HashSet<Integer>(0);
            }
        }
        // 后端服务没有提供批量取好友限制个数的方法,所以,单独取
        List<Integer> user1friends = getFriendListAsc(userId1, limit);
        List<Integer> user2friends = getFriendListAsc(userId2, limit);
        if (null == user1friends || null == user2friends) {
            return new HashSet<Integer>(0);
        }
        user1friends.retainAll(user2friends);
        return new HashSet<Integer>(user1friends);
    }

}
