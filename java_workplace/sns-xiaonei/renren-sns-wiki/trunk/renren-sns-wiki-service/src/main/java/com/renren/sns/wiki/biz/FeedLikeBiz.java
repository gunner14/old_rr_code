package com.renren.sns.wiki.biz;

import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import xce.ilikeit.LikeInfo;

import com.renren.sns.wiki.delegator.xce.LikeLogicDelegator;


/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-4
 *
 */
@Service
public class FeedLikeBiz {
    
    @Autowired
    private LikeLogicDelegator likeLogicDelegator;
    
    /**
     * 喜欢操作
     * @param gidStr
     * @param uid
     */
    public void addLike(String gidStr, int userId) {
        likeLogicDelegator.addLike(gidStr, userId);
    }

    /**
     * 取消喜欢操作
     * @param gidStr
     * @param uid
     */
    public void removeLike(String gidStr, int userId) {
        likeLogicDelegator.removeLike(gidStr, userId);
    }
    
    /**
     * 根据gidStrs和userId,获取喜欢的具体信息
     * @param gidStrs
     * @param userid
     * @param limit
     * @param detailNum
     * @return
     */
    public Map<String, LikeInfo> getLikeDetailMapBatch(String[] gidStrs, int userId) {
        return likeLogicDelegator.getLikeDetailMapBatch(gidStrs, userId);
    }

}
