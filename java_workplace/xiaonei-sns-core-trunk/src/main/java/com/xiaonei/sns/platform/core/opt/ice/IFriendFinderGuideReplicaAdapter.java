package com.xiaonei.sns.platform.core.opt.ice;

import java.util.List;
import java.util.Map;

import mop.hi.oce.domain.model.FriendCommon;
import mop.hi.oce.domain.model.FriendShare;
import mop.hi.oce.domain.model.FriendShares;

/**
 * 为profile用的FriendFinder
 * 
 * @author Michael(liyong@opi-corp.com)
 * 
 */
public interface IFriendFinderGuideReplicaAdapter extends IRegisterable {

    List<FriendCommon> getCommonFriends(int userId, int begin, int limit);

    List<FriendCommon> getCommonFriendsRandom(int userId, int limit);

    void removeCommonFriendWithBlock(int userId, int blockId);

    List<FriendShare> getSharedFriends(int userId1, int userId2, int begin, int limit);

    Map<Integer, FriendShares> getSharedFriends(int userId, List<Integer> friendIds, int limit);
    
    int getSharedFriendsCount(int userId1, int userId2);
    
    void rebuildCommonFriend(int userId);

}
