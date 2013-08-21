package com.renren.sns.wiki.delegator.xce;

import java.util.Collections;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.enums.InterestType;
import com.xiaonei.xce.buddyadapter.followrelation.FollowRelationCacheAdapter;
import com.xiaonei.xce.buddyadapter.followrelation.FollowRelationWriterAdapter;
import com.xiaonei.xce.globalid.GlobalId;

/**
 * 
 * 兴趣交互的代理类
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-7
 * 
 */
@Service
public class InterestDelegator {

    private Log logger = LogFactory.getLog(InterestDelegator.class);

    /**
     * 将wiki的各种感兴趣情况转化为global id
     */
    private GlobalId castToGid(int wikiId, String interest) {
        GlobalId gid = null;
        if (InterestType.WISH.getValue().equals(interest)) {
            gid = GlobalId.fromSocialWikiWillDo(wikiId);
            if (logger.isDebugEnabled()) {
                logger.debug("castToGid WISH is true, wikiId-" + wikiId + " interest-" + interest
                        + " gid-" + gid.toString());
            }
        } else if (InterestType.DO.getValue().equals(interest)) {
            gid = GlobalId.fromSocialWikiDoing(wikiId);
            if (logger.isDebugEnabled()) {
                logger.debug("castToGid DO is true, wikiId-" + wikiId + " interest-" + interest
                        + " gid=" + gid.toString());
            }
        } else if (InterestType.COLLECT.getValue().equals(interest)) {
            gid = GlobalId.fromSocialWikiDone(wikiId);
            if (logger.isDebugEnabled()) {
                logger.debug("castToGid COLLECT is true, wikiId-" + wikiId + " interest-"
                        + interest + " gid=" + gid.toString());
            }
        }
        return gid;
    }

    public void interest(int wikiId, int userId, String interest) {
        GlobalId gid = castToGid(wikiId, interest);
        FollowRelationWriterAdapter.getInstance().addFollowRelation(userId, gid);
        if (logger.isDebugEnabled()) {
            logger.debug("begin to interest wikiId-" + wikiId + " userId-" + userId + " interest-"
                    + interest + " gid-" + gid.toString());
        }
    }

    public void uninterest(int wikiId, int userId, String interest) {
        GlobalId gid = castToGid(wikiId, interest);
        FollowRelationWriterAdapter.getInstance().removeFollowRelation(userId, gid);
        if (logger.isDebugEnabled()) {
            logger.debug("begin to uninterest wikiId-" + wikiId + " userId-" + userId
                    + " interest-" + interest + " gid-" + gid.toString());
        }
    }

    public boolean isInterested(int wikiId, int userId, String interest) {
        boolean result = FollowRelationCacheAdapter.getInstance().hasRelation(userId,
                castToGid(wikiId, interest));
        if (logger.isDebugEnabled()) {
            logger.debug("Is User following wikiId-" + wikiId + " userId-" + userId + " result:"
                    + result);
        }
        return result;
    }

    /**
     * 获取wiki的interest列表
     * 
     * @param wikiId wiki id
     * @param start 起始位置
     * @param pageSize 获取一页数据的大小
     * @param 感兴趣的种类,参考InterestType类型
     * @return List<Integer> uid的列表，如果没有获取到结果返回大小为空的list
     */
    public List<Integer> getFollowers(int wikiId, int start, int pageSize, String interest) {
        List<Integer> interestList = FollowRelationCacheAdapter.getInstance().getList(
                castToGid(wikiId, interest), start, pageSize);
        if (null == interestList) {
            interestList = Collections.emptyList();
        }

        if (logger.isDebugEnabled()) {
            logger.debug("get interester wikiId-" + wikiId + " size:" + interestList.size());
        }
        return interestList;
    }

    /**
     * 获取wiki被follow的人数
     * 
     * @param wikiId wikiId
     * @param 感兴趣的种类,参考InterestType类型
     * @return 被follow的总数
     */
    public int getFollowCount(int wikiId, String interest) {
        int count = FollowRelationCacheAdapter.getInstance().getCount(castToGid(wikiId, interest));
        if (logger.isDebugEnabled()) {
            logger.debug("get interest count wikiId-" + wikiId + " size:" + count);
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
     * @param 感兴趣的种类,参考InterestType类型
     * @return List<Integer> uid的列表，如果没有获取到结果返回大小为空的list
     */
    public List<Integer> getFollowerInFriends(int wikiId, int userId, int start, int pageSize,
            String interest) {
        List<Integer> interestList = FollowRelationCacheAdapter.getInstance().getCommonFriend(
                userId, castToGid(wikiId, interest), start, pageSize);
        if (null == interestList) {
            interestList = Collections.emptyList();
        }

        if (logger.isDebugEnabled()) {
            logger.debug("get interesters in friends wikiId-" + wikiId + " userId-" + userId
                    + " size:" + interestList.size());
        }

        return interestList;
    }

    /**
     * 获取用户的好友也interest该wiki的总数
     * 
     * @param userId 用户id
     * @param wikiId wikiId
     * @param 感兴趣的种类,参考InterestType类型
     * @return 总数
     */
    public int getFollowersInFriendsCount(int wikiId, int userId, String interest) {
        int count = FollowRelationCacheAdapter.getInstance().getCommonFriendCount(userId,
                castToGid(wikiId, interest));
        if (logger.isDebugEnabled()) {
            logger.debug("get interesters in friends count wikiId-" + wikiId + " userId-" + userId
                    + " size:" + count);
        }
        return count;
    }

    public static void main(String args[]) {
        InterestDelegator d = new InterestDelegator();
        //        d.interest(610000005, 365323815);
        System.out.println(d.getFollowCount(610000005, "wish"));
        //        System.out.println(d.isInterested(610000005, 365323815, "wish"));
    }

}
