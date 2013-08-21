package com.renren.sns.wiki.delegator.xce;

import java.util.List;

import org.springframework.stereotype.Service;

import com.xiaonei.xce.buddyadapter.buddybyonlinetimecache.BuddyByOnlineTimeCacheAdapter;

/**
 * 获取好友id列表接口的代理类
 * 
 * @author yi.li@renren-inc.com since 2012-5-2
 * 
 */
@Service
public class FriendDelegator {

    public List<Integer> getFriendList(int userId, int limit) {
        return BuddyByOnlineTimeCacheAdapter.getInstance().getFriendList(userId, limit);
    }

}
