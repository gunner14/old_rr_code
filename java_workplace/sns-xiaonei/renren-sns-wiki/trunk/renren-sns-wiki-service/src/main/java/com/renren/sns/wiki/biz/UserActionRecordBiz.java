package com.renren.sns.wiki.biz;

import java.util.Collection;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.dao.UserActionRecordDAO;
import com.renren.sns.wiki.delegator.xce.InterestDelegator;
import com.renren.sns.wiki.model.UserActionRecord;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-4-5
 */
@Service
public class UserActionRecordBiz extends BaseBiz {

    @Autowired
    private UserActionRecordDAO userActionRecordDAO;

    @Autowired
    private InterestDelegator interestDelegator;

    /**
     * 根据wikiId和userId 获取用户对这个本书的收藏 评分信息
     * 
     * @param wikiId
     * @param userId
     * @return
     */
    public UserActionRecord getUserActionRecord(int wikiId, int userId) {
        return userActionRecordDAO.getUserActionRecord(wikiId, userId);
    }

    /**
     * 获取userId为key, UserActionRecord为value的Map
     * 
     * @param wikiId
     * @param userIds
     * @return
     */
    public Map<Integer, UserActionRecord> getUserActionRecordMap(int wikiId,
            Collection<Integer> userIds) {
        return userActionRecordDAO.getUserActionRecord(wikiId, userIds);
    }

    /**
     * 添加一条UserActionRecord
     * 
     * @param wikiId
     * @param userId
     * @param interest
     */
    public void addUserActionRecord(int wikiId, int wikiType, int userId, String userName,
            String userHeadurl, String interest) {
        userActionRecordDAO.addUserActionRecord(wikiId, wikiType, userId, userName, userHeadurl,
                interest);
        interestDelegator.interest(wikiId, userId, interest);
    }

    /**
     * 删除某条记录
     * 
     * @param wikiId
     * @param userId
     */
    public int deleteUserActionRecord(int wikiId, int userId) {
        return userActionRecordDAO.deleteUserActionRecord(wikiId, userId);
    }

    /**
     * 修改interest
     * 
     * @param wikiId
     * @param userId
     * @param interest
     */
    public void updateInterest(int wikiId, int userId, String interest) {
        userActionRecordDAO.updateInterest(wikiId, userId, interest);
        interestDelegator.interest(wikiId, userId, interest);
    }

    /**
     * 从中间件服务取消某个兴趣
     * 
     * @param wikiId
     * @param userId
     * @param interest
     */
    public void removeFollowRelation(int wikiId, int userId, String interest) {
        interestDelegator.uninterest(wikiId, userId, interest);
    }

    /**
     * 修改评分
     * 
     * @param wikiId
     * @param userId
     * @param rating
     */
    public int updateRating(int wikiId, int userId, int rating) {
        return userActionRecordDAO.updateRating(wikiId, userId, rating);
    }

    /**
     * 修改收藏的描述
     * 
     * @param wikiId
     * @param userId
     * @param comment
     */
    public int updateComment(int wikiId, int userId, String comment) {
        return userActionRecordDAO.updateComment(wikiId, userId, comment);
    }

    /**
     * 获取某个wiki的评分的和
     * 
     * @param wikiId
     * @return
     */
    public int getTotalRating(int wikiId) {
        Integer total = userActionRecordDAO.getTotalRating(wikiId);
        if (total == null) {
            return 0;
        } else {
            return total;
        }
    }

    /**
     * 获取评分的总人数
     * 
     * @param wikiId
     * @return
     */
    public int getRatingCount(int wikiId) {
        return userActionRecordDAO.getRatingCount(wikiId);
    }

    public List<Integer> listFollowers(int wikiId, int startIndex, int pageSize, String interest) {
        return interestDelegator.getFollowers(wikiId, startIndex, pageSize, interest);
    }

    public List<Integer> listFollowingFriends(int wikiId, int userId, String interest,
            int startIndex, int pageSize) {
        return interestDelegator.getFollowerInFriends(wikiId, userId, startIndex, pageSize,
                interest);
    }

    public int getFollowingCount(int wikiId, String interest) {
        return interestDelegator.getFollowCount(wikiId, interest);
    }

    public int getFollowingFriendsCount(int wikiId, int userId, String interest) {
        return interestDelegator.getFollowersInFriendsCount(wikiId, userId, interest);
    }

    public List<Integer> getWikiIdList(int userId, int type, String interest) {
        return userActionRecordDAO.getWikiIdList(userId, type, interest);
    }

    public int getCount(int userId, int wikiType, String interest) {
        return userActionRecordDAO.getCount(userId, wikiType, interest);
    }

    public List<Integer> getWikiIdList(int userId, int wikiType, String interest, int offset,
            int limit) {
        return userActionRecordDAO.getWikiIdList(userId, wikiType, interest, offset, limit);
    }

}
