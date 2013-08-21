package com.renren.sns.wiki.biz;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.delegator.xce.FriendDelegator;


/**
 * 好友相关的接口
 * @author weiwei.wang@renren-inc.com since 2012-5-14
 *
 */
@Service
public class FriendBiz {
    
    @Autowired
    private FriendDelegator friendDelegator;
    
   
    /**
     * 获取某个用户的好友,按照在线时间排序
     * @param userId
     * @param limit
     * @return
     */
    public List<Integer> getFeedSetByIds(int userId, int limit){
        return friendDelegator.getFriendList(userId, limit);
    }

}
