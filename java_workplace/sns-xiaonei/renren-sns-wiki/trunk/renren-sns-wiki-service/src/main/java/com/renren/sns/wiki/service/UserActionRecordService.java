package com.renren.sns.wiki.service;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.doingubb.tool.DoingUbbReplace;
import com.renren.sns.wiki.biz.UserActionRecordBiz;
import com.renren.sns.wiki.biz.follow.FollowBiz;
import com.renren.sns.wiki.biz.follow.FollowListBiz;
import com.renren.sns.wiki.delegator.xce.WUserCacheDelegator;
import com.renren.sns.wiki.enums.InterestType;
import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.UserActionRecord;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.service.info.WikiInfoService;
import com.renren.sns.wiki.service.security.WikiAuditService;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-4-5
 * 
 */
@Service
public class UserActionRecordService extends BaseService {

    private static final int DEFAULT_PAGE_SIZE = 3;

    @Autowired
    private UserActionRecordBiz userActionRecordBiz;

    @Autowired
    private WUserCacheDelegator userCacheDelegator;

    @Autowired
    private WikiService wikiService;

    @Autowired
    private WikiInfoService wikiInfoService;

    @Autowired
    private FollowBiz followBiz;

    @Autowired
    private FollowListBiz followListBiz;

    @Autowired
    private WikiAuditService wikiAuditService;

    /**
     * 根据wikiId和userId 获取用户对这个本书的收藏 评分信息
     * 
     * @param wikiId
     * @param id
     * @return
     */
    public UserActionRecord getUserActionRecord(int wikiId, int userId) {
        return userActionRecordBiz.getUserActionRecord(wikiId, userId);
    }

    public Map<Integer, UserActionRecord> getUserActionRecordMap(int wikiId,
            Collection<Integer> userIds) {
        return userActionRecordBiz.getUserActionRecordMap(wikiId, userIds);
    }

    /**
     * 获取只和交互相关的Map
     * 
     * @param userActionRecord
     * @return
     */
    public Map<String, Object> getSimpleInterestInfoMap(int wikiId, int userId) {
        UserActionRecord userActionRecord = getUserActionRecord(wikiId, userId);
        return getSimpleInterestInfoMap(userActionRecord);

    }

    public Map<String, Object> getSimpleInterestInfoMap(UserActionRecord userActionRecord) {
        Map<String, Object> map = new HashMap<String, Object>();
        if (userActionRecord != null) {

            //解析表情
            String orginComment = userActionRecord.getComment();
            String comment = "";
            if (!StringUtils.isBlank(orginComment)) {
                comment = DoingUbbReplace.getInstance().replaceUBB(orginComment);
            }

            map.put("interest", userActionRecord.getInterest());
            map.put("rating", userActionRecord.getRating());
            map.put("orginComment", orginComment);
            map.put("comment", comment);
            map.put("interestTime", userActionRecord.getInterestTime().getTime());
        }
        return map;
    }

    /**
     * 获取用户交互的信息,多了这个wiki的总评分,json形式返回
     * 
     * @param wikiId
     * @param id
     * @return
     */
    public Map<String, Object> getInterestInfoMap(int wikiId, int userId) {
        Map<String, Object> map = getSimpleInterestInfoMap(wikiId, userId);
        //data中的数据是平均分 总分 评分人数
        Map<String, Object> wikiValuationMap = wikiService.getWikiValuationMap(wikiId);
        map.put("wikiValuation", wikiValuationMap);

        return map;
    }

    /**
     * 记录用户的操作
     * 
     * @param wikiId
     * @param user
     * @param userActionRecord
     * @param userId 什么需要
     */
    public void interest(int wikiId, User user, UserActionRecord userActionRecord, boolean audit) {
        if (userActionRecord == null) {
            return;
        }

        int userId = user.getId();
        //检查是否包含一条记录
        UserActionRecord bean = userActionRecordBiz.getUserActionRecord(wikiId, userId);
        String interest = userActionRecord.getInterest();
        if (logger.isDebugEnabled()) {
            logger.debug("wikiId=" + wikiId + ", userId=" + userId + ", interest=" + interest);
        }
        if (InterestType.WISH.getValue().equals(interest)
                || InterestType.DO.getValue().equals(interest)
                || InterestType.COLLECT.getValue().equals(interest)) {} else {
            if (bean != null) {
                interest = bean.getInterest();
            } else {
                interest = InterestType.COLLECT.getValue();
            }
        }
        if (bean == null) {
            String userName = user.getName();
            String userHeadurl = user.getTinyFullUrl();
            //数据库不包含user的操作,insert一条记录
            Wiki wiki = wikiService.getWikiById(wikiId);
            if (wiki == null) {
                logger.warn("wiki not exist, wikiId=" + wikiId);
                throw new WikiException(WikiExceptionType.WIKI_NOT_EXISTS);
            }
            userActionRecordBiz.addUserActionRecord(wikiId, wiki.getType(), userId, userName,
                    userHeadurl, interest);
        } else if (!interest.equals(bean.getInterest())) {
            //interest状态有修改,从中间件服务取消掉原来的服务,然后修改
            userActionRecordBiz.removeFollowRelation(wikiId, userId, bean.getInterest());
            userActionRecordBiz.updateInterest(wikiId, userId, interest);
        }
        int rating = userActionRecord.getRating();
        int beanRating = bean == null ? 0 : bean.getRating();
        if (rating != beanRating && rating >= 1 && rating <= 5) {
            int updateCount = userActionRecordBiz.updateRating(wikiId, userId, rating);
            //更新评分
            if (updateCount > 0) {
                int changeRating = rating - beanRating;
                int changeUserCount = 0;
                if (beanRating == 0) {
                    changeUserCount = 1;
                } else if (rating == 0) {
                    changeUserCount = 1;
                }
                wikiService.updateWikiValuationByChange(wikiId, changeRating, changeUserCount);
            }
        }

        String comment = userActionRecord.getComment();
        String beanComment = bean == null ? "" : bean.getComment();
        if (comment != null && !comment.equals(beanComment)) {
            if (logger.isDebugEnabled()) {
                logger.debug("wikiComment from " + beanComment + " to " + comment);
            }
            if (audit) {
                wikiAuditService.auditSimpleComment(wikiId, userId, user.getName(), comment);
            }
            int updateCount = userActionRecordBiz.updateComment(wikiId, userId, comment);
            if (updateCount == 0) {
                throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, "修改简评失败");
            }
        }
    }

    /**
     * 为安全提供接口,清空用户的简评
     * 
     * @param wikiId
     * @param userId
     */
    public void deleteComment(int wikiId, int userId) {
        logger.warn("delete Comment where wikiId=" + wikiId + ", userId=" + userId);
        userActionRecordBiz.updateComment(wikiId, userId, "");
    }

    /**
     * 删除用户的交互
     * 
     * @param wikiId
     * @param host
     */
    public void romove(int wikiId, User user) {
        int userId = user.getId();
        UserActionRecord bean = userActionRecordBiz.getUserActionRecord(wikiId, userId);
        if (bean != null) {
            String interest = bean.getInterest();
            //清除中间件数据
            userActionRecordBiz.removeFollowRelation(wikiId, userId, interest);

            //获取评分
            int rating = bean.getRating();

            //从数据库中删除交互记录
            int deleteCount = userActionRecordBiz.deleteUserActionRecord(wikiId, userId);

            //更新wiki的评分
            if (deleteCount > 0 && rating > 0) {
                wikiService.updateWikiValuationByChange(wikiId, 0 - rating, -1);
            }
        }
    }

    /**
     *获取共同兴趣的好友
     * 
     * @param wikiId
     * @param userId
     * @return List<WUserCache>
     */
    public List<WUserCache> listFollowingFriends(int wikiId, int userId, String interest) {
        try {
            List<Integer> fids = new ArrayList<Integer>();
            if (userId > 0) {
                fids = userActionRecordBiz.listFollowingFriends(wikiId, userId, interest, 0,
                        DEFAULT_PAGE_SIZE);
            }

            List<WUserCache> followers = userCacheDelegator.getCacheUsers(fids);
            return followers;
        } catch (Exception e) {
            logger.error("", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }

    }

    /**
     * 获取对某个wiki兴趣的好友
     * 
     * @param wikiId
     * @param userId
     * @return List<WUserCache>
     */
    public List<WUserCache> listFollowingFriends(int wikiId, int userId, int startIndex,
            int pageSize, String interest) {
        try {
            List<Integer> fids = userActionRecordBiz.listFollowingFriends(wikiId, userId, interest,
                    startIndex, pageSize);
            List<WUserCache> followers = null;
            if (fids != null && fids.size() >= 0) {
                followers = userCacheDelegator.getCacheUsers(fids);
            } else {
                followers = new ArrayList<WUserCache>();
            }
            return followers;
        } catch (Exception e) {
            logger.error("", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 获取对某个wiki兴趣的用户
     * 
     * @param wikiId
     * @param userId
     * @return List<WUserCache>
     */
    public List<WUserCache> listFollowers(int wikiId, int startIndex, int pageSize, String interest) {
        try {
            List<Integer> fids = userActionRecordBiz.listFollowers(wikiId, startIndex, pageSize,
                    interest);
            List<WUserCache> followers = null;
            if (fids != null && fids.size() >= 0) {
                followers = userCacheDelegator.getCacheUsers(fids);
            } else {
                followers = new ArrayList<WUserCache>();
            }
            return followers;
        } catch (Exception e) {
            logger.error("", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 获取用户wiki相关社交化的信息, 会根据collect do wish follow的顺序给出
     * 
     * @param wikiId
     * @param userId
     * @return
     */
    public Map<String, Object> getSocialInfoMap(int wikiId, int userId) {
        Map<String, Object> map = null;
        int maxCount = 3;

        String interest = null;
        int userCount = 0;

        interest = InterestType.COLLECT.getValue();
        map = getSocialInfoMap(wikiId, userId, interest, maxCount);

        userCount = (Integer) map.get("userCount");

        if (userCount <= 0) {
            interest = InterestType.DO.getValue();
            map = getSocialInfoMap(wikiId, userId, interest, maxCount);
            userCount = (Integer) map.get("userCount");
        }

        if (userCount <= 0) {
            interest = InterestType.WISH.getValue();
            map = getSocialInfoMap(wikiId, userId, interest, maxCount);
            userCount = (Integer) map.get("userCount");
        }

        //好友的喜欢相关 start,如果不需要喜欢的信息,注释掉这一块就可以了
        if (userCount <= 0) {
            interest = "follow";
            map = getSocialInfoMap(wikiId, userId, interest, maxCount);
            userCount = (Integer) map.get("userCount");
        }

        return map;
    }

    /**
     * 
     * @param wikiId
     * @param userId
     * @param interest 是collect do wish follow四个值
     * @return
     */
    public Map<String, Object> getSocialInfoMap(int wikiId, int userId, String interest,
            int maxCount) {
        Map<String, Object> map = new HashMap<String, Object>();

        int userCount = 0;
        List<Integer> userIdList = null;

        if ("follow".equals(interest)) {
            userCount = followListBiz.getFollowingFriendsCount(wikiId, userId);
            if (userCount > 0) {
                userIdList = followListBiz.listFollowingFriends(wikiId, userId, 0, maxCount);
            }
        } else if (InterestType.COLLECT.getValue().equals(interest)
                || InterestType.DO.getValue().equals(interest)
                || InterestType.WISH.getValue().equals(interest)) {
            userCount = userActionRecordBiz.getFollowingFriendsCount(wikiId, userId, interest);
            if (userCount > 0) {
                userIdList = userActionRecordBiz.listFollowingFriends(wikiId, userId, interest, 0,
                        maxCount);
            }
        }

        if (userCount <= 0) {
            interest = "";
        }

        List<Map<String, Object>> mapList = new ArrayList<Map<String, Object>>();
        List<WUserCache> wUserCacheList = userCacheDelegator.getCacheUsers(userIdList);
        if (wUserCacheList != null) {
            for (int i = 0; i < maxCount && i < wUserCacheList.size(); i++) {
                WUserCache wUserCache = wUserCacheList.get(i);
                Map<String, Object> userMap = new HashMap<String, Object>();
                userMap.put("id", wUserCache.getId());
                userMap.put("name", wUserCache.getName());
                userMap.put("tinyUrl", wUserCache.getTiny_Url());
                mapList.add(userMap);
            }
        }

        map.put("userCount", userCount);
        map.put("userAction", interest);
        map.put("userList", mapList);

        return map;
    }

    /**
     * 好友的collect,do,wish,follow列表
     * 
     * @param wiki
     * @param user
     * @return
     */
    @SuppressWarnings("unchecked")
    public List<Map<String, Object>> getFriendsInfoMap(Wiki wiki, User user) {
        List<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
        int maxCount = 24;
        int wikiId = wiki.getId();
        int userId = user.getId();

        String[] interests = { InterestType.COLLECT.getValue(), InterestType.DO.getValue(),
                InterestType.WISH.getValue(), "follow" };
        for (String interest : interests) {
            //获取好友信息
            Map<String, Object> socialInfoMap = getSocialInfoMap(wikiId, userId, interest, maxCount);

            int userCount = (Integer) socialInfoMap.get("userCount");
            if (userCount <= 0) {
                continue;
            }

            //获取好友的交互信息
            List<Map<String, Object>> mapList = (List<Map<String, Object>>) socialInfoMap
                    .get("userList");
            for (Map<String, Object> userMap : mapList) {
                int fid = (Integer) userMap.get("id");
                Map<String, Object> friendInterestInfoMap = getInterestInfoMap(wikiId, fid);
                userMap.put("interestInfo", friendInterestInfoMap);
            }
            list.add(socialInfoMap);
        }
        return list;
    }

    /**
     * 获取wiki,用户等相关的信息,用于wikiCard以及返回wiki页需要
     * 
     * @param wikiId
     * @param userId
     * @return
     */
    public Map<String, Object> getFullInfoMap(Wiki wiki, User user) {

        Map<String, Object> map = new HashMap<String, Object>();

        int wikiId = wiki.getId();

        //wiki相关的数据
        Map<String, Object> wikiInfoMap = wikiInfoService.getWikiInfoMap(wiki);
        //social相关
        Map<String, Object> socialInfoMap = new HashMap<String, Object>();
        //交互相关的数据
        Map<String, Object> userActionRecordMap = new HashMap<String, Object>();

        if (user != null) {
            socialInfoMap = this.getSocialInfoMap(wikiId, user.getId());
            userActionRecordMap = this.getInterestInfoMap(wikiId, user.getId());
            //isFollow加到userActionRecordMap中去
            boolean isFollow = followBiz.isFollowed(wikiId, user.getId());
            userActionRecordMap.put("follow", isFollow);
        }

        map.put("wikiInfo", wikiInfoMap);
        map.put("socialInfo", socialInfoMap);
        map.put("userActionRecordInfo", userActionRecordMap);

        return map;
    }

    public int getFollowersCount(int wikiId, String type) {
        try {
            return userActionRecordBiz.getFollowingCount(wikiId, type);
        } catch (Exception e) {
            logger.error("", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }

    }

    public int getFollowingFriendsCount(int wikiId, int userId, String type) {
        try {
            return userActionRecordBiz.getFollowingFriendsCount(wikiId, userId, type);
        } catch (Exception e) {
            logger.error("", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    public List<Integer> getWikiIdList(int userId, WikiType wikiType, InterestType interestType) {
        return userActionRecordBiz.getWikiIdList(userId, wikiType.getWikiType(), interestType
                .getValue());
    }

    /**
     * 获取某个兴趣的数目
     * 
     * @param userId
     * @param wikiType
     * @param interestType
     * @return
     */
    public int getCount(int userId, WikiType wikiType, InterestType interestType) {
        return userActionRecordBiz
                .getCount(userId, wikiType.getWikiType(), interestType.getValue());
    }

    /**
     * 获取一个列表
     * 
     * @param id
     * @param wikiType
     * @param wish
     * @param offset
     * @param limit
     * @return
     */
    public List<Integer> getWikiIdList(int userId, WikiType wikiType, InterestType interestType,
            int offset, int limit) {
        return userActionRecordBiz.getWikiIdList(userId, wikiType.getWikiType(),
                interestType.getValue(), offset, limit);
    }

}
