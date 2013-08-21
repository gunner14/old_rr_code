package com.xiaonei.sns.platform.core.opt.ice;

import java.util.List;


/**
 * 
 * @author michael(liyong@opi-corp.com)
 *
 */
public interface IBuddyByNameCacheAdapter extends IRegisterable {
    
    /**
     * 返回用户好友数量 
     * 
     * @param userId
     * @return
     */
    int getFriendCount(int userId);
    
    /**
     * 返回用户好友的id数组，按名字排序
     * limit=-1或大于实现用户的好友数量的时 返回所有好友的id
     * 其它情况且返回limit个id
     * 
     * @param userId
     * @param limit
     * @return
     */
    List<Integer> getFriendList(int userId,int limit);
    
    
    /**
     * 返回所有用户好友的id数组，按名字排序
     * @param userId
     * @return
     */
    List<Integer> getFriendList(int userId);
    
}
