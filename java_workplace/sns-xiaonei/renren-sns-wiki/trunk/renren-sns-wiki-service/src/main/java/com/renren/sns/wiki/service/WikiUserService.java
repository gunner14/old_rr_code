package com.renren.sns.wiki.service;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.enums.InterestType;
import com.renren.sns.wiki.model.UserActionRecord;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-24
 * 
 */
@Service
public class WikiUserService {

    @Autowired
    private UserActionRecordService userActionRecordService;

    @Autowired
    private FollowService followService;

    /**
     * 获取userInfo
     * 
     * @param userCache
     * @return
     */
    public Map<String, Object> getUserInfo(WUserCache userCache) {
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("id", userCache.getId());
        map.put("name", userCache.getName());
        map.put("headTinyUrl", userCache.getTiny_Url());
        return map;
    }

    /**
     * 用户获取wiki用户的用户列表
     * 
     * @param wikiId
     * @param userList
     * @return
     */
    public List<Map<String, Object>> getWikiUserList(int wikiId, List<WUserCache> userList) {
        List<Map<String, Object>> list = new ArrayList<Map<String, Object>>();

        List<Integer> userIdList = new ArrayList<Integer>();
        for (WUserCache userCache : userList) {
            userIdList.add(userCache.getId());
        }

        Map<Integer, UserActionRecord> userActionRecordMap = userActionRecordService
                .getUserActionRecordMap(wikiId, userIdList);
        //将获取用户的信息加到list中去
        for (WUserCache userCache : userList) {
            Map<String, Object> userMap = new HashMap<String, Object>();

            //瀑布流相关的信息
            userMap.put("itemType", "user");
            userMap.put("feedId", userCache.getId());

            Map<String, Object> userInfoMap = this.getUserInfo(userCache);
            userMap.put("userInfo", userInfoMap);

            UserActionRecord userActionRecord = userActionRecordMap.get(userCache.getId());
            if (userActionRecord != null) {
                Map<String, Object> interestInfoMap = userActionRecordService
                        .getSimpleInterestInfoMap(userActionRecord);
                userMap.put("interestInfo", interestInfoMap);
            } else {
                userMap.put("interestInfo", "");
            }

            list.add(userMap);
        }
        return list;
    }

    /**
     * wiki用户返回的json格式 userList会比pageSize大的值传过来
     * 
     * @param wikiId
     * @param user 用于返回好友数
     * @param page
     * @param pageSize
     * @param type
     * @return
     */
    public Map<String, Object> getAllWikiUserMap(int wikiId, User user, int page, int pageSize,
            String type) {

        int extra = 5;//每次会多返回5条数据,用于判断hasMore
        int startIndex = page * pageSize;
        List<WUserCache> userList = null;
        if (InterestType.COLLECT.getValue().equals(type) || InterestType.DO.getValue().equals(type)
                || InterestType.WISH.getValue().equals(type)) {
            userList = userActionRecordService.listFollowers(wikiId, startIndex, pageSize + extra,
                    type);
        } else {
            userList = followService.listFollowers(wikiId, startIndex, pageSize + extra);
        }

        //多少人操作,多少好友操作的数据
        Map<String, Object> countInfoMap = getCountInfoMap(wikiId, user, type);

        Map<String, Object> map = getWikiUserMap(wikiId, userList, pageSize);
        map.put("countInfo", countInfoMap);

        return map;
    }

    /**
     * wiki用户返回的json格式 userList会比pageSize大的值传过来
     * 
     * @param wikiId
     * @param user 用于返回好友数
     * @param page
     * @param pageSize
     * @param type
     * @return
     */
    public Map<String, Object> getFriendWikiUserMap(int wikiId, User user, int page, int pageSize,
            String type) {
        int extra = 5;//每次会多返回5条数据,用于判断hasMore
        int startIndex = page * pageSize;
        List<WUserCache> userList = null;
        if (InterestType.COLLECT.getValue().equals(type) || InterestType.DO.getValue().equals(type)
                || InterestType.WISH.getValue().equals(type)) {
            userList = userActionRecordService.listFollowingFriends(wikiId, user.getId(),
                    startIndex, pageSize + extra, type);
        } else {
            userList = followService.listFollowingFriends(wikiId, user.getId(), startIndex,
                    pageSize + extra);
        }

        Map<String, Object> countInfoMap = getCountInfoMap(wikiId, user, type);

        Map<String, Object> map = getWikiUserMap(wikiId, userList, pageSize);
        map.put("countInfo", countInfoMap);

        return map;
    }

    /**
     * 返回wikiUser给前端的Map,不包含countInfo信息,配合getCountInfoMap方法使用
     * 
     * @param wikiId
     * @param userList
     * @param pageSize
     * @return
     */
    private Map<String, Object> getWikiUserMap(int wikiId, List<WUserCache> userList, int pageSize) {
        if (userList == null) {
            return new HashMap<String, Object>();
        }

        boolean hasMore = false;
        if (userList.size() > pageSize) {
            userList = userList.subList(0, pageSize);
            hasMore = true;
        }

        List<Map<String, Object>> list = this.getWikiUserList(wikiId, userList);

        Map<String, Object> map = new HashMap<String, Object>();
        map.put("hasMore", hasMore);
        map.put("feeds", list);
        return map;
    }

    /**
     * 获取人数信息,返回的wikiUser的接口使用
     * 
     * @param wikiId
     * @param user
     * @param type
     * @return
     */
    public Map<String, Object> getCountInfoMap(int wikiId, User user, String type) {
        String action = null;
        int totalCount = 0;
        int friendCount = 0;
        if (InterestType.COLLECT.getValue().equals(type) || InterestType.DO.getValue().equals(type)
                || InterestType.WISH.getValue().equals(type)) {
            action = type;
            totalCount = userActionRecordService.getFollowersCount(wikiId, type);
            if (user != null) {
                friendCount = userActionRecordService.getFollowingFriendsCount(wikiId,
                        user.getId(), type);
            }
        } else {
            action = "follow";
            totalCount = followService.getFollowersCount(wikiId);
            if (user != null) {
                friendCount = followService.getFollowingFriendsCount(wikiId, user.getId());
            }
        }
        //countInfo
        Map<String, Object> countInfoMap = new HashMap<String, Object>();
        countInfoMap.put("action", action);
        countInfoMap.put("totalCount", totalCount);
        if (user != null) {
            countInfoMap.put("friendCount", friendCount);
        }
        return countInfoMap;
    }

}
