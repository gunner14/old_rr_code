package com.renren.sns.wiki.biz.follow;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.BaseBiz;
import com.renren.sns.wiki.delegator.xce.FollowDelegator;

/**
 * @author yi.li@renren-inc.com since 2012-4-5
 */
@Service
public class FollowBiz extends BaseBiz {

    @Autowired
    private FollowDelegator followAdapter;

    /**
     * follow a wiki
     * 
     * @param wikiId
     * @param userId
     */
    public void followWiki(int wikiId, int userId) {
        followAdapter.follow(wikiId, userId);
    }

    /**
     * unfollow a wiki
     * 
     * @param wikiId
     * @param userId
     */
    public void unfollowWiki(int wikiId, int userId) {
        followAdapter.unfollow(wikiId, userId);
    }

    /**
     * 判断用户是否follow了该wiki
     * 
     * @param wikiId
     * @param userId
     * @return true if followed, otherwise false
     */
    public boolean isFollowed(int wikiId, int userId) {
        return followAdapter.isFollowed(wikiId, userId);
    }
}
