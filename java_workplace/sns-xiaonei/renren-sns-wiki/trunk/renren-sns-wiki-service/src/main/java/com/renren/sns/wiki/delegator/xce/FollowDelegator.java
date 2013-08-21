package com.renren.sns.wiki.delegator.xce;

import java.util.Collections;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

import com.xiaonei.xce.buddyadapter.followrelation.FollowRelationCacheAdapter;
import com.xiaonei.xce.buddyadapter.followrelation.FollowRelationWriterAdapter;
import com.xiaonei.xce.globalid.GlobalId;

/**
 * follow服务的代理类
 * 
 * @author yi.li@renren-inc.com since 2012-4-9
 * 
 */
@Service
public class FollowDelegator {

    private Log logger = LogFactory.getLog(FollowDelegator.class);

    /**
     * 将wiki id转化为global id
     */
    private GlobalId castToGid(int wikiId) {
        GlobalId gid = GlobalId.fromSocialWiki(wikiId);
        return gid;
    }

    public void follow(int wikiId, int userId) {
        if (logger.isDebugEnabled()) {
            logger.debug("begin to follow wikiId-" + wikiId + " userId-" + userId);
        }
        FollowRelationWriterAdapter.getInstance().addFollowRelation(userId, castToGid(wikiId));
    }

    public void unfollow(int wikiId, int userId) {
        if (logger.isDebugEnabled()) {
            logger.debug("begin to unfollow wikiId-" + wikiId + " userId-" + userId);
        }
        FollowRelationWriterAdapter.getInstance().removeFollowRelation(userId, castToGid(wikiId));
    }

    public boolean isFollowed(int wikiId, int userId) {
        boolean result = FollowRelationCacheAdapter.getInstance().hasRelation(userId,
                castToGid(wikiId));
        if (logger.isDebugEnabled()) {
            logger.debug("Is User following wikiId-" + wikiId + " userId-" + userId + " result:"
                    + result);
        }

        return result;
    }

    /**
     * 获取wiki的follow列表
     * 
     * @param wikiId wiki id
     * @param start 起始位置
     * @param pageSize 获取一页数据的大小
     * @return List<Integer> uid的列表，如果没有获取到结果返回大小为空的list
     */
    public List<Integer> getFollowers(int wikiId, int start, int pageSize) {
        List<Integer> followList = FollowRelationCacheAdapter.getInstance().getList(
                castToGid(wikiId), start, pageSize);
        if (null == followList) {
            followList = Collections.emptyList();
        }

        if (logger.isDebugEnabled()) {
            logger.debug("get followers wikiId-" + wikiId + " size:" + followList.size());
        }

        return followList;
    }

    /**
     * 获取wiki被follow的人数
     * 
     * @param wikiId wikiId
     * @return 被follow的总数
     */
    public int getFollowCount(int wikiId) {
        int count = FollowRelationCacheAdapter.getInstance().getCount(castToGid(wikiId));
        if (logger.isDebugEnabled()) {
            logger.debug("get followers count wikiId-" + wikiId + " size:" + count);
        }

        return count;
    }

    /**
     * 获取用户的好友也follow该wiki的列表
     * 
     * @param wikiId wikiId
     * @param userId 用户id
     * @param start 起始位置
     * @param pageSize 页大小
     * @return List<Integer> uid的列表，如果没有获取到结果返回大小为空的list
     */
    public List<Integer> getFollowerInFriends(int wikiId, int userId, int start, int pageSize) {
        List<Integer> followList = FollowRelationCacheAdapter.getInstance().getCommonFriend(userId,
                castToGid(wikiId), start, pageSize);
        if (null == followList) {
            followList = Collections.emptyList();
        }

        if (logger.isDebugEnabled()) {
            logger.debug("get followers in friends wikiId-" + wikiId + " userId-" + userId
                    + " size:" + followList.size());
        }

        return followList;
    }

    /**
     * 获取用户的好友也follow该wiki的总数
     * 
     * @param userId 用户id
     * @param wikiId wikiId
     * @return 总数
     */
    public int getFollowersInFriendsCount(int wikiId, int userId) {
        int count = FollowRelationCacheAdapter.getInstance().getCommonFriendCount(userId,
                castToGid(wikiId));
        if (logger.isDebugEnabled()) {
            logger.debug("get followers in friends count wikiId-" + wikiId + " userId-" + userId
                    + " size:" + count);
        }
        return count;
    }

    public static void main(String args[]) {
        FollowDelegator d = new FollowDelegator();
        //        d.follow(610000005, 365323815);
        System.out.println(d.getFollowCount(610000005));
        System.out.println(d.isFollowed(610000005, 365323815));
    }

}
