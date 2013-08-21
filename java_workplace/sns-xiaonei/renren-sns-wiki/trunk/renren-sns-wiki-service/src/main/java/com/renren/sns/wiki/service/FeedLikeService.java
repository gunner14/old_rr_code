package com.renren.sns.wiki.service;

import java.util.HashMap;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import xce.ilikeit.LikeInfo;

import com.renren.sns.wiki.biz.FeedLikeBiz;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;

/**
 * 喜欢某个内容的封装
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-10
 * 
 */
@Service
public class FeedLikeService {

    private static final Log logger = LogFactory.getLog(FeedLikeService.class);

    @Autowired
    private FeedLikeBiz feedLikeBiz;

    /**
     * 喜欢操作
     * 
     * @param gidStr
     * @param uid
     * @return
     */
    public Map<String, Object> addLike(String gidStr, int userId) {
        feedLikeBiz.addLike(gidStr, userId);
        return this.getLikeInfoMap(gidStr, userId);
    }

    /**
     * 取消喜欢操作
     * 
     * @param gidStr
     * @param uid
     * @return
     */
    public Map<String, Object> removeLike(String gidStr, int userId) {
        feedLikeBiz.removeLike(gidStr, userId);
        return this.getLikeInfoMap(gidStr, userId);
    }

    public Map<String, Object> getLikeInfoMap(String gidStr, int userId) {
        if (logger.isDebugEnabled()) {
            logger.debug("getLikeInfoMap which gidStr=" + gidStr + ", userId=" + userId);
        }
        Map<String, Object> map = new HashMap<String, Object>();
        //获取喜欢的信息
        String[] gidStrs = { gidStr };
        Map<String, LikeInfo> likeDetailMap = feedLikeBiz.getLikeDetailMapBatch(gidStrs, userId);

        if (likeDetailMap != null && likeDetailMap.containsKey(gidStr)) {
            LikeInfo likeInfo = likeDetailMap.get(gidStr);
            int selfCount = likeInfo.selfCount;
            int totalCount = likeInfo.totalCount;
            if (selfCount > 0) {
                map.put("isLiked", true);
            } else {
                map.put("isLiked", false);
            }
            map.put("likedNum", totalCount);
            return map;
        } else {
            logger.warn("feedLikeBiz.getLikeDetailMapBatch(" + gidStrs + ", " + "userId"
                    + ") return null");
            throw new WikiException(WikiExceptionType.LIKE_DETAIL_NOT_FOUND);
        }
    }
}
