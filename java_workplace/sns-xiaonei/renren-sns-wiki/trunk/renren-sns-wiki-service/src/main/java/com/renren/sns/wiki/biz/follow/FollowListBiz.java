package com.renren.sns.wiki.biz.follow;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.BaseBiz;
import com.renren.sns.wiki.delegator.xce.FollowDelegator;

/**
 * 查询wiki follow列表服务
 * 
 * @author yi.li@renren-inc.com since 2012-4-5
 * 
 */
@Service
public class FollowListBiz extends BaseBiz {

    @Autowired
    private FollowDelegator followAdpater;

    public List<Integer> listFollowers(int wikiId, int startIndex, int pageSize) {
        return followAdpater.getFollowers(wikiId, startIndex, pageSize);
    }

    public List<Integer> listFollowingFriends(int wikiId, int userId, int startIndex, int pageSize) {
        return followAdpater.getFollowerInFriends(wikiId, userId, startIndex, pageSize);
    }

    public int getFollowingCount(int wikiId) {
        return followAdpater.getFollowCount(wikiId);
    }

    public int getFollowingFriendsCount(int wikiId, int userId) {
        return followAdpater.getFollowersInFriendsCount(wikiId, userId);
    }
}
