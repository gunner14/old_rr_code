package com.xiaonei.sns.platform.core.opt.ice;

import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * 
 * @author michael(liyong@opi-corp.com)
 * @author tai.wang@opi-corp.com Sep 14, 2010 - 11:03:09 AM
 */
public interface IBuddyByIdCacheAdapter extends IRegisterable {

    /**
     * 返回用户好友数量
     * 
     * @param userId
     * @return
     */
    int getFriendCount(int userId);

    /**
     * 返回好友数
     * 
     * @param userIds 用户ID列表
     * @return 用户ID对应的好友数
     * @since 20091203
     */
    Map<Integer, Integer> getFriendCounts(List<Integer> userIds);

    /**
     * 返回用户好友的id数组，按id增序 limit=-1或大于实现用户的好友数量的时 返回所有好友的id 其它情况且返回limit个id
     * 
     * @param userId
     * @param limit
     * @return
     */
    List<Integer> getFriendListAsc(int userId, int limit);

    /**
     * 返回所有用户好友的id数组，按id增序
     * 
     * @param userId
     * @return
     */
    List<Integer> getFriendListAsc(int userId);

    /**
     * 返回用户好友的id数组，按id增序
     * 
     * @param userIds
     * @return
     * @since 20091203
     */
    Map<Integer, List<Integer>> getFriendListsAsc(List<Integer> userIds);

    /**
     * getSharedFriends<br>
     * 得到<code>userId1</code>,<code>userId2</code>的共同好友id set.
     * 先得到两个人的好友列表,然后取交集
     * 
     * @see IBuddyByIdCacheAdapter#getSharedFriendsLimit2K(int, int)
     * @param userId1 {@link Integer}
     * @param userId2 {@link Integer}
     * @return {@link Set} {@link Integer}
     * 
     * @author tai.wang@opi-corp.com Sep 14, 2010 - 11:04:06 AM
     */
    Set<Integer> getSharedFriends(int userId1, int userId2);

    /**
     * getSharedFriendsLimit2K<br>
     * 得到前2000个好友中的共同好友, 默认好友上限是2k所以可以满足大部分情况.
     * 
     * @see IBuddyByIdCacheAdapter#getSharedFriends(int, int)
     * @param userId1 {@link Integer}
     * @param userId2 {@link Integer}
     * @return {@link Set} {@link Integer}
     * 
     * @author tai.wang@opi-corp.com Sep 25, 2010 - 2:01:44 PM
     */
    Set<Integer> getSharedFriendsLimit2K(int userId1, int userId2);
}
