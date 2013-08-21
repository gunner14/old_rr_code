package com.renren.sns.wiki.service;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.follow.FollowBiz;
import com.renren.sns.wiki.biz.follow.FollowListBiz;
import com.renren.sns.wiki.delegator.xce.WUserCacheDelegator;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.xiaonei.platform.core.model.WUserCache;

/**
 * @author yi.li@renren-inc.com since 2012-4-5
 * 
 */
@Service
public class FollowService extends BaseService {

    private static final int DEFAULT_PAGE_SIZE = 6;

    @Autowired
    private FollowBiz followBiz;

    @Autowired
    private FollowListBiz followListBiz;

    @Autowired
    private WUserCacheDelegator userCacheDelegator;

    /**
     * 判断用户是否喜欢了该wiki
     * 
     * @param wikiId
     * @param userId
     * @return true如果用户喜欢了该wiki，否则返回false
     */
    public boolean isFollowing(int wikiId, int userId) {
        try {
            return followBiz.isFollowed(wikiId, userId);
        } catch (Exception e) {
            logger.error("", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    public void follow(int wikiId, int userId) {
        try {
            if (!followBiz.isFollowed(wikiId, userId)) {
                if (logger.isDebugEnabled()) {
                    logger.debug("begin to follow wikiId-" + wikiId + " userId-" + userId);
                }
                followBiz.followWiki(wikiId, userId);
                return;
            }
        } catch (Exception e) {
            logger.error("follow wiki error, wikiId-" + wikiId + " userId-" + userId, e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }

        logger.warn("user-" + userId + " already following wiki-" + wikiId);
        throw new WikiException(WikiExceptionType.USER_ALREADY_FOLLOWING);
    }

    public void unfollow(int wikiId, int userId) {
        try {
            followBiz.unfollowWiki(wikiId, userId);
        } catch (Exception e) {
            logger.error("unfollow wiki error: wikiId- " + wikiId + " userId-" + userId, e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 获取喜欢wiki的用户列表
     * 
     * @param wikiId
     * @param startIndex
     * @param pageSize
     * @return List<WUserCache>
     */
    public List<WUserCache> listFollowers(int wikiId, int startIndex, int pageSize) {
        try {
            List<Integer> fids = followListBiz.listFollowers(wikiId, startIndex, pageSize);
            List<WUserCache> followers = userCacheDelegator.getCacheUsers(fids);
            return followers;
        } catch (Exception e) {
            logger.error("", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 获取喜欢wiki的好友列表
     * 
     * @param wikiId
     * @param startIndex
     * @param pageSize
     * @return List<WUserCache>
     */
    public List<WUserCache> listFollowingFriends(int wikiId, int userId, int startIndex, int pageSize) {
        try {
            List<Integer> fids = followListBiz.listFollowingFriends(wikiId, userId, startIndex,
                    pageSize);
            List<WUserCache> followers = userCacheDelegator.getCacheUsers(fids);
            return followers;
        } catch (Exception e) {
            logger.error("", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 获取首页High light喜欢数据
     * 
     * @param wikiId
     * @param userId
     * @return List<WUserCache>
     */
    public List<WUserCache> listFollowersShortcuts(int wikiId, int userId) {
        try {
            List<Integer> fids = new ArrayList<Integer>();
            if (userId > 0) {
                fids = followListBiz.listFollowingFriends(wikiId, userId, 0, DEFAULT_PAGE_SIZE);
            }

            if (fids.size() < DEFAULT_PAGE_SIZE) { // 如果好友喜欢数不足再从别的全局关注者中取
                List<Integer> otherIds = followListBiz.listFollowers(wikiId, 0, DEFAULT_PAGE_SIZE);

                for (Integer id : otherIds) {
                    if (fids.contains(id)) { //这里需要去重
                        continue;
                    }

                    fids.add(id);
                    if (fids.size() >= DEFAULT_PAGE_SIZE) {
                        break;
                    }
                }
            }

            List<WUserCache> followers = userCacheDelegator.getCacheUsers(fids);
            return followers;
        } catch (Exception e) {
            logger.error("", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }

    }

    /**
     * 获取用户好友follow该wiki的人数
     * 
     * @param wikiId
     * @param userId
     */
    public int getFollowingFriendsCount(int wikiId, int userId) {
        try {
            return followListBiz.getFollowingFriendsCount(wikiId, userId);
        } catch (Exception e) {
            logger.error("", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 获取wiki following的人数
     * 
     * @param wikiId
     */
    public int getFollowersCount(int wikiId) {
        try {
            return followListBiz.getFollowingCount(wikiId);
        } catch (Exception e) {
            logger.error("", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 获取用户wiki相关社交化的信息
     * 
     * @param wikiId
     * @param userId
     * @return
     */
    public JSONObject getSocialInfoJsonObject(int wikiId, int userId) {

        JSONObject jsonObject = new JSONObject();

        List<WUserCache> wUserCacheList = listFollowersShortcuts(wikiId, userId);
        JSONArray jsonArray = new JSONArray();
        if (wUserCacheList != null) {
            for (int i = 0; i < 3 && i < wUserCacheList.size(); i++) {
                WUserCache wUserCache = wUserCacheList.get(i);
                JSONObject userJsonObject = new JSONObject();
                userJsonObject.put("id", wUserCache.getId());
                userJsonObject.put("name", wUserCache.getName());
                userJsonObject.put("tinyUrl", wUserCache.getTinyUrl());
                jsonArray.put(userJsonObject);
            }
        }

        jsonObject.put("userCount", getFollowingFriendsCount(wikiId, userId));
        jsonObject.put("userAction", "collect");
        jsonObject.put("userList", jsonArray);

        return jsonObject;
    }

}
