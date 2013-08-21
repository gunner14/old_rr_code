package com.renren.sns.guide.utils;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Properties;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.WorkplaceInfo;

import org.json.JSONArray;
import org.json.JSONObject;

import com.dodoyo.datum.client.DatumInfoClient;
import com.dodoyo.friends.relationfriend.logic.IRelation;
import com.dodoyo.friends.relationfriend.logic.IRelationFriend;
import com.renren.sns.guide.logic.HotStarRelation;
import com.renren.sns.guide.model.AppHomeView;
import com.renren.sns.guide.model.FriendWithinTime;
import com.renren.sns.guide.model.GuideIRelation;
import com.renren.sns.guide.model.GuideIRelationFriend;
import com.renren.sns.guide.model.GuideSameSchool;
import com.renren.sns.guide.model.GuideUserSomeInfo;
import com.renren.sns.guide.model.SameSchoolPeople;
import com.renren.sns.guide.view.UserTinyView;
import com.xiaonei.page.domain.view.PageSpreadView;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserPassport;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.ice.WUserCacheAdapter;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.photostatefilter.PhotoStateFilterAdapter;

public class GuideUtils {

    private Map<Integer, String> idFilterMap = new HashMap<Integer, String>();

    private void initIdFilter() {
        if (idFilterMap == null || idFilterMap.size() == 0) {
            idFilterMap = loadMap();
        }
    }

    private Map<Integer, String> loadMap() {
        Map<Integer, String> retMap = new HashMap<Integer, String>();
        try {
            final Properties p = new Properties();
            p.load(this.getClass().getClassLoader().getResourceAsStream("useridfilter.properties"));
            for (Object k : p.keySet()) {
                String key = (String) k;
                String value = p.getProperty(key);
                try {
                    retMap.put(Integer.parseInt(key), value);
                } catch (NumberFormatException e) {
                    e.printStackTrace();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return retMap;
    }

    private GuideUtils() {
        initIdFilter();
    }

    private static GuideUtils instance = null;

    public static synchronized GuideUtils getInstance() {
        if (instance == null) {
            instance = new GuideUtils();
        }
        return instance;
    }

    public static String AJAX_GUIDE_USER = "Ajax_Guide_User";
    
    public static String FIRST_TIME_GUIDE = "first_time_guide";
    //v6新版引导用户
    public static String GUIDE_V6_USER = "guide_v6_user";
    
    /**
     * 按照资料推人和按照IP推人
     * 
     * @param list
     * @return
     */
    public String getJSONPeopleMayKnowForGuide(List<GuideIRelation> list, List<Integer> sameIpIds) {
        JSONArray jsonArray = new JSONArray();
        if (list != null) {
            for (GuideIRelation gp : list) {
                JSONObject json = new JSONObject();
                json.put("typeId", gp.getTypeId());
                json.put("typeName", gp.getTypeName());
                json.put("moreLink", gp.getMoreLink());
                json.put("type", gp.getType());
                String type = gp.getType();
                json.put("count", gp.getCount());
                List<GuideIRelationFriend> peopleList = gp.getList();
                JSONArray people = new JSONArray();
                if (peopleList != null && peopleList.size() != 0) {
                    List<Integer> userIds = new ArrayList<Integer>();
                    for (int i = 0; i < peopleList.size(); i++) {
                        Integer userId = peopleList.get(i).getId();
                        if ("location".equals(type)) {
                            if (idFilterMap == null) {
                                userIds.add(userId);
                            } else if (!idFilterMap.containsKey(userId)) {
                                userIds.add(userId);
                            }
                        } else {
                            userIds.add(userId);
                        }
                    }
                    Map<Integer, WUserCache> userCacheMap = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(userIds);
                    //peopleList过滤掉了自己和好友
                    //json.put("count", peopleList.size());
                    for (GuideIRelationFriend su : peopleList) {
                        JSONObject person = new JSONObject();
                        person.put("id", su.getId());
                        person.put("fullName", su.getName());
                        if (userCacheMap.get(su.getId()) == null) {
                            continue;
                        }
                        person.put("isOnline", userCacheMap.get(su.getId()).isOnline());
                        if (su.getName() != null) {
                            person.put("name", GuideUtils.getInstance().interceptWord(su.getName(), 6));
                        }
                        person.put("head_url", su.getHead_Url());
                        person.put("tiny_url", su.getTiny_Url());
                        if (su.getTiny_Url() == null || su.getTiny_Url().length() == 0 || "".equals(su.getTiny_Url())) {
                            if ("男生".equals(su.getGender())) {
                                person.put("tiny_url", "http://head.xiaonei.com/photos/0/0/men_tiny.gif");
                            } else {
                                person.put("tiny_url", "http://head.xiaonei.com/photos/0/0/women_tiny.gif");
                            }
                        }
                        people.put(person);
                    }
                    json.put("people", people);
                }
                jsonArray.put(json);
            }
            if (sameIpIds != null) {
                int[] arrayUserIds = new int[sameIpIds.size()];
                for (int i = 0; i < sameIpIds.size(); i++) {
                    arrayUserIds[i] = sameIpIds.get(i);
                }
                List<Integer> filtedUserIds = PhotoStateFilterAdapter.getInstance().getPhotoIdsFrom(arrayUserIds);//过滤头像 
                List<WUserCache> userCacheList = WUserCacheAdapter.getInstance().getUsersCacheList(filtedUserIds);
                JSONObject json = new JSONObject();
                json.put("typeId", -3);
                json.put("typeName", "附近上网的人");
                json.put("type", "sameIp");
                JSONArray people = new JSONArray();
                if (userCacheList != null && userCacheList.size() != 0) {
                    json.put("count", userCacheList.size());
                    for (WUserCache wuc : userCacheList) {
                        JSONObject person = new JSONObject();
                        if (wuc == null) {
                            continue;
                        }
                        person.put("id", wuc.getId());
                        person.put("fullName", wuc.getName());
                        person.put("isOnline", wuc.isOnline());
                        if (wuc.getName() != null) {
                            person.put("name", GuideUtils.getInstance().interceptWord(wuc.getName(), 6));
                        }
                        person.put("tiny_url", wuc.getTiny_Url());
                        if (wuc.getTiny_Url() == null || wuc.getTiny_Url().length() == 0 || "".equals(wuc.getTiny_Url())) {
                            if ("男生".equals(wuc.getGender())) {
                                person.put("tiny_url","http://head.xiaonei.com/photos/0/0/men_tiny.gif");
                            } else {
                                person.put("tiny_url","http://head.xiaonei.com/photos/0/0/women_tiny.gif");
                            }
                        }
                        people.put(person);
                    }
                    json.put("people", people);
                }
                jsonArray.put(json);
            }
        }
        return jsonArray.toString();
    }

    /**
     * 通过type值得来判断拼的json串是干啥的
     * 
     * @param cacheList
     * @param type
     * @param limit
     * @return
     */
    public String getJSONPeopleForDefType(List<WUserCache> cacheList, String type,int limit,int count) {
        JSONObject json = new JSONObject();
        //json的类型:例如二度好友啊，同学校啊啥的
		json.put("type", type);
		JSONArray people = new JSONArray();
		if (cacheList != null) {
			int loop = 0;
			GuideLogger.printLog(type + ":cacheList.size():" + cacheList.size());
			//如果limit==-1,就全要
			if (limit == -1) {
				loop = cacheList.size();
			} else {
				//如果limit小于给定的list.size,就取limit，否则取list.size
				loop = (limit < cacheList.size()) ? limit : cacheList.size();
			}
			GuideLogger.printLog(type + ":loop:" + loop);
			for (int i = 0; i < loop; i++) {
				JSONObject person = new JSONObject();
				WUserCache wuc = cacheList.get(i);
				if (wuc == null) {
					continue;
				}
				person.put("id", wuc.getId());
				person.put("fullName", wuc.getName());
				person.put("name", GuideUtils.getInstance().interceptWord(wuc.getName(), 6));
				person.put("tiny_url", wuc.getTiny_Url());
				person.put("isOnline", wuc.isOnline());
				String fullUnivName = wuc.getUnivName();
				String univName = fullUnivName;
				if (fullUnivName != null && fullUnivName.length() > 14) {
					univName = fullUnivName.substring(0, 14);
				}
				person.put("univName", univName);
				person.put("fullUnivName", fullUnivName);
				// 获取头像失败时给TA一个默认的头像
				if (cacheList.get(i).getTiny_Url() == null || cacheList.get(i).getTiny_Url().length() == 0 || "".equals(cacheList.get(i).getTiny_Url())) {
					if ("男生".equals(cacheList.get(i).getGender())) {
						person.put("tiny_url","http://head.xiaonei.com/photos/0/0/men_tiny.gif");
					} else {
						person.put("tiny_url","http://head.xiaonei.com/photos/0/0/women_tiny.gif");
					}
				}
				people.put(person);
			}
		}
		if(count == -1){
			if (cacheList != null) {
				json.put("count", cacheList.size());
			} else {
				json.put("count", 0);
			}
		}
		else{
			json.put("count", count);
		}
		json.put("people", people);
		return json.toString();
    }

    /**
     * 截取字符串的工具类
     * 
     * @param source
     * @param length
     * @return
     */
    public String interceptWord(String source, int length) {
        int sum = 0;
        if (source == null) {
            return "";
        } else {
            int strLength = source.length();
            for (int i = 0; i < source.length(); i++) {
                char c = source.charAt(i);
                //判断字符类型，数字和字母长度加1，其余加2
                if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
                    sum += 1;
                } else {
                    sum += 2;
                }
                if (sum > length) {
                    strLength = i;
                    break;
                }
            }
            return source.substring(0, strLength);
        }
    }

    /**
     * 热门page
     * 
     * @param hotSpreadPage
     * @return
     */
    public String getJSONForHotPage(List<PageSpreadView> hotSpreadPage) {
        JSONObject json = new JSONObject();
        JSONArray jsonArray = new JSONArray();
        if (hotSpreadPage != null) {
            for (int i = 0; i < hotSpreadPage.size(); i++) {
                JSONObject jsonPage = new JSONObject();
                jsonPage.put("pageId", hotSpreadPage.get(i).getPageId());
                String pageName = hotSpreadPage.get(i).getPageName();
                jsonPage.put("fullPageName", pageName);
                if (pageName != null) {
                    jsonPage.put("pageName", GuideUtils.getInstance().interceptWord(pageName, 6));
                }
                //这个是80*80的头像，暂时取不到小头像的数据
                jsonPage.put("pagePicUrl", hotSpreadPage.get(i).getPicUrl());
                jsonArray.put(jsonPage);
            }
        }
        json.put("pageJsonArray", jsonArray);
        return json.toString();
    }

    /**
     * 推荐app
     * 
     * @param recApp
     * @return
     */
    public String getJSONForRecomRightApp(List<AppHomeView> recApp) {
        JSONObject json = new JSONObject();
        JSONArray jsonArray = new JSONArray();
        if (recApp != null) {
            for (int i = 0; i < recApp.size(); i++) {
                JSONObject jsonApp = new JSONObject();
                jsonApp.put("appId", recApp.get(i).getAppId());
                jsonApp.put("appLogo", recApp.get(i).getAppLogo());
                jsonApp.put("appName", recApp.get(i).getAppName());
                jsonApp.put("appUrl", recApp.get(i).getAppUrl());
                jsonApp.put("appTotalCount", recApp.get(i).getAppTotalCount());
                jsonApp.put("appFriendCount", recApp.get(i).getAppFriendCount());
                jsonApp.put("appDailyActive", recApp.get(i).getAppDailyActive());
                jsonArray.put(jsonApp);
            }
        }
        json.put("appJsonArray", jsonArray);
        return json.toString();
    }

    /**
     * 1. 若最近玩过的时间在6个小时内，显示为XXX分钟前 2. 若最近使用的时间大等于6小时小等于24小时，显示为XX小时前 3.
     * 若最近使用时间大于1天，显示为X天前。
     * 
     * @param date
     * @return
     */
    public String getLastAppTimeString(Date date) {
        Date now = new Date();
        long l = now.getTime() - date.getTime();
        long difMin = l / (60 * 1000);
        long day = l / (24 * 60 * 60 * 1000);
        long hour = (l / (60 * 60 * 1000) - day * 24);
        if (difMin < 6 * 60) {
            return difMin + "分钟前";
        } else if (difMin >= 60 * 6 && difMin <= 60 * 24) {
            return hour + "小时前";
        } else {
            return day + "天前";
        }
    }

    /**
     * 为人气之星拼json
     * 
     * @param starList
     * @return
     */
    public String getJSONForHotStar(List<HotStarRelation> starList) {
        JSONArray jsonArray = new JSONArray();
        if (starList != null) {
            for (HotStarRelation hsr : starList) {
                JSONObject json = new JSONObject();
                json.put("univId", hsr.getUnivId());
                json.put("univName", hsr.getUnivName());
                String type = hsr.getType();
                json.put("type", type);
                List<UserTinyView> peopleList = hsr.getList();
                JSONArray people = new JSONArray();
                if (peopleList != null && peopleList.size() != 0) {
                    List<Integer> userIds = new ArrayList<Integer>();
                    for (int i = 0; i < peopleList.size(); i++) {
                        Integer userId = peopleList.get(i).getId();
                        if ("location".equals(type)) {
                            if (idFilterMap == null) {
                                userIds.add(userId);
                            } else if (!idFilterMap.containsKey(userId)) {
                                userIds.add(userId);
                            }
                        } else {
                            userIds.add(userId);
                        }
                    }
                    Map<Integer, WUserCache> userCacheMap = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(userIds);
                    json.put("count", peopleList.size());
                    for (UserTinyView utv : peopleList) {
                        JSONObject person = new JSONObject();
                        person.put("id", utv.getId());
                        person.put("fullName", utv.getName());
                        WUserCache wuc = userCacheMap.get(utv.getId());
                        if (wuc == null) {
                            continue;
                        }
                        person.put("isOnline", wuc.isOnline());
                        if (utv.getName() != null) {
                            person.put("name", GuideUtils.getInstance().interceptWord(utv.getName(), 6));
                        }
                        person.put("tiny_url", utv.getTiny_Url());
                        if (utv.getTiny_Url() == null || utv.getTiny_Url().length() == 0 || "".equals(utv.getTiny_Url())) {
                            if ("男生".equals(wuc.getGender())) {
                                person.put("tiny_url","http://head.xiaonei.com/photos/0/0/men_tiny.gif");
                            } else {
                                person.put("tiny_url","http://head.xiaonei.com/photos/0/0/women_tiny.gif");
                            }
                        }
                        people.put(person);
                    }
                    json.put("people", people);
                }
                jsonArray.put(json);
            }
        }
        return jsonArray.toString();
    }

    public String getJSONForGuideOfferFriends(List<WUserCache> userList) {
        JSONObject json = new JSONObject();
        JSONArray jsonArray = new JSONArray();
        if (userList != null) {
            for (int i = 0; i < userList.size(); i++) {
                JSONObject person = new JSONObject();
                person.put("id", userList.get(i).getId());
                person.put("netName", userList.get(i).getUnivName());
                person.put("name", userList.get(i).getName());
                person.put("head", userList.get(i).getTiny_Url());
                person.put("isOnLine", userList.get(i).isOnline());
                //获取头像失败时给TA一个默认的头像
                if (userList.get(i).getTiny_Url() == null || userList.get(i).getTiny_Url().length() == 0 || "".equals(userList.get(i).getTiny_Url())) {
                    if ("男生".equals(userList.get(i).getGender())) {
                        person.put("tiny_url", "http://head.xiaonei.com/photos/0/0/men_tiny.gif");
                    } else {
                        person.put("tiny_url", "http://head.xiaonei.com/photos/0/0/women_tiny.gif");
                    }
                }
                jsonArray.put(person);
            }
        }
        json.put("candidate", jsonArray);
        json.put("qkey", "friend");
        json.put("groups", new JSONArray());
        return json.toString();
    }

    public static String getTable(String tableName, int id) {
        return tableName + id % 100;
    }

    /**
     * 按照资料推人
     * 
     * @param list
     * @return
     */
    public String getJSONForGuideSearchInfo(List<GuideIRelation> list) {
        JSONArray jsonArray = new JSONArray();
        if (list != null) {
            for (GuideIRelation gp : list) {
                JSONObject json = new JSONObject();
                json.put("typeId", gp.getTypeId());
                json.put("typeName", gp.getTypeName());
                json.put("moreLink", gp.getMoreLink());
                json.put("type", gp.getType());
                String type = gp.getType();
                json.put("count", 0);
                List<GuideIRelationFriend> peopleList = gp.getList();
                JSONArray people = new JSONArray();
                if (peopleList != null && peopleList.size() != 0) {
                    List<Integer> userIds = new ArrayList<Integer>();
                    for (int i = 0; i < peopleList.size(); i++) {
                        Integer userId = peopleList.get(i).getId();
                        if ("location".equals(type)) {
                            if (idFilterMap == null) {
                                userIds.add(userId);
                            } else if (!idFilterMap.containsKey(userId)) {
                                userIds.add(userId);
                            }
                        } else {
                            userIds.add(userId);
                        }
                    }
                    Map<Integer, WUserCache> userCacheMap = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(userIds);
                    //peopleList过滤掉了自己和好友
                    json.put("count", peopleList.size());
                    for (GuideIRelationFriend su : peopleList) {
                        JSONObject person = new JSONObject();
                        person.put("id", su.getId());
                        person.put("fullName", su.getName());
                        if (userCacheMap.get(su.getId()) == null) {
                            continue;
                        }
                        person.put("isOnline", userCacheMap.get(su.getId()).isOnline());
                        if (su.getName() != null) {
                            person.put("name", GuideUtils.getInstance().interceptWord(su.getName(),6));
                        }
                        person.put("head_url", su.getHead_Url());
                        person.put("tiny_url", su.getTiny_Url());
                        if (su.getTiny_Url() == null || su.getTiny_Url().length() == 0 || "".equals(su.getTiny_Url())) {
                            if ("男生".equals(su.getGender())) {
                                person.put("tiny_url","http://head.xiaonei.com/photos/0/0/men_tiny.gif");
                            } else {
                                person.put("tiny_url","http://head.xiaonei.com/photos/0/0/women_tiny.gif");
                            }
                        }
                        people.put(person);
                    }
                    json.put("people", people);
                }
                jsonArray.put(json);
            }
        }
        return jsonArray.toString();
    }

    /**
     * 这是取上次登录时间以及用这个时间取这段时间内的新加好友
     * 
     * @param list
     * @return
     */
    public String getJSONForGetFriendListWithinTime(List<Integer> list) {
        JSONArray jsonArray = new JSONArray();
        if (list != null) {
            Map<Integer, WUserCache> userCacheMap = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(list);
            for (int i = 0; i < list.size(); i++) {
                WUserCache wuc = userCacheMap.get(list.get(i));
                if (wuc == null) {
                    continue;
                }
                JSONObject json = new JSONObject();
                json.put("userId", wuc.getId());
                String fullUserName = wuc.getName();
                String userName = fullUserName;
                if (fullUserName != null && fullUserName.length() > 3) {
                    userName = fullUserName.substring(0, 3);
                }
                json.put("userName", userName);
                json.put("fullUserName", fullUserName);
                jsonArray.put(json);
            }
        }
        return jsonArray.toString();
    }

    /**
     * 我的人人需求中间的一个模块，取各种信息给前端
     * 
     * @return
     */
    public String getJSONForGetFriendWithinTime(FriendWithinTime fwt) {
        JSONObject json = new JSONObject();
        json.put("userId", fwt.getUserId());
        json.put("userName", fwt.getUserName());
        json.put("userTinyUrl", fwt.getUserTinyUrl());
        json.put("userBlogCount", fwt.getBlogCount());
        json.put("userDoingCount", fwt.getDoingCount());
        json.put("userFriendCount", fwt.getFriendCount());
        json.put("userAlbumCount", fwt.getAlbumCount());
        json.put("userDoingId", fwt.getDoingId());
        json.put("userDoingTime", fwt.getDoingTime());
        json.put("userNewestDoing", fwt.getNewestDoing());
        json.put("isFriend", fwt.isFriend());
        json.put("isShowProfile", fwt.isShowProfile());
        json.put("isShowGossipBoard", fwt.isShowGossipBoard());
        return json.toString();
    }

    public boolean isFullInfo(User user) {
        boolean flag = false;
        ElementarySchoolInfo eleInfo = AdapterFactory.getNetworkAdapter().getElementarySchoolInfo(user.getId()); // 小学资料信息
        JuniorHighSchoolInfo junInfo = AdapterFactory.getNetworkAdapter().getJuniorHighSchoolInfo(user.getId()); //初中资料信息
        List<HighSchoolInfo> hsInfoList = AdapterFactory.getNetworkAdapter().getHighSchoolInfoList(user.getId()); // 高中资料信息
        List<CollegeInfo> colleges = DatumInfoClient.getInstance().getCollegeInfos(user);//中专技校信息
        List<UniversityInfo> univInfoList = AdapterFactory.getNetworkAdapter().getUniversityInfoList(user.getId()); // 大学资料信息
        List<WorkplaceInfo> workInfoList = AdapterFactory.getNetworkAdapter().getWorkplaceInfoList(user.getId()); // 工作资料信息
        if (user.getStage() == Stage.STAGE_HEIGHSCHOOL) {
            if (eleInfo != null && !"".equalsIgnoreCase(eleInfo.getElementarySchoolName())
                    && junInfo != null
                    && !"".equalsIgnoreCase(junInfo.getJunHighentarySchoolName())) {
                flag = true;
            }
        } else if (user.getStage() == Stage.STAGE_UNIV) {
            if (eleInfo != null && !"".equalsIgnoreCase(eleInfo.getElementarySchoolName())
                    && junInfo != null
                    && !"".equalsIgnoreCase(junInfo.getJunHighentarySchoolName())
                    && hsInfoList != null && hsInfoList.size() != 0 && colleges != null
                    && colleges.size() != 0 && univInfoList != null && univInfoList.size() != 0) {
                flag = true;
            }
        } else if (user.getStage() == Stage.STAGE_MAJOR) {
            if (((eleInfo != null && !"".equalsIgnoreCase(eleInfo.getElementarySchoolName()))
                    || (junInfo != null && !"".equalsIgnoreCase(junInfo.getJunHighentarySchoolName()))
                    || (hsInfoList != null && hsInfoList.size() != 0)
                    || (colleges != null && colleges.size() != 0) || (univInfoList != null && univInfoList.size() != 0))
                    && (workInfoList != null && workInfoList.size() != 0)) {
                flag = true;
            }
        } else {
            if (((eleInfo != null && !"".equalsIgnoreCase(eleInfo.getElementarySchoolName()))
                    || (junInfo != null && !"".equalsIgnoreCase(junInfo.getJunHighentarySchoolName()))
                    || (hsInfoList != null && hsInfoList.size() != 0)
                    || (colleges != null && colleges.size() != 0) || (univInfoList != null && univInfoList.size() != 0))
                    || (workInfoList != null && workInfoList.size() != 0)) {
                flag = true;
            }
        }
        return flag;
    }

    /**
     * 我申请过谁和好友的ID
     * 
     * @param fwt
     * @return
     */
    public String getJSONForTwoWayFriendsIds(List<Integer> list) {
        JSONArray jsonArray = new JSONArray();
        if (list != null) {
            for (Integer i : list) {
                JSONObject json = new JSONObject();
                json.put("userId", i);
                jsonArray.put(json);
            }
        }
        return jsonArray.toString();
    }
    
    public int []  getIntArray(List<Integer> list){
		if(list == null || list.size() == 0)return new int [0];
		int [] ints = new int [list.size()];
		for (int i = 0; i < ints.length; i++) {
			ints[i] = list.get(i);
		}
		return ints;
	}
    
    /**
     * 传进来的是List<Integer>，返回JSONArray
     * @param list
     * @return
     */
    public String getJSONArrayForIds(List<Integer> list) {
    	JSONArray jsonArray = new JSONArray();
        if (list != null) {
            Map<Integer, WUserCache> userCacheMap = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(list);
            for (int i = 0; i < list.size(); i++) {
                WUserCache wuc = userCacheMap.get(list.get(i));
                if (wuc == null) {
                    continue;
                }
                JSONObject json = new JSONObject();
                json.put("userId", wuc.getId());                
                json.put("userName", wuc.getName());
                json.put("userTinyUrl", wuc.getTiny_Url());
                jsonArray.put(json);
            }
        }
        return jsonArray.toString();
    }
    
    /**
     * 传进来的是List<WUserCache>，返回JSONArray
     * @param list
     * @return
     */
    public String getJSONArrayForCacheList(List<WUserCache> list,Integer limit) {
    	JSONArray jsonArray = new JSONArray();
    	Integer count=0;
        if (list != null) {            
            for (int i = 0; i < list.size() && count<limit; i++) {   
            	WUserCache wuc=list.get(i);
                if (wuc == null) {
                    continue;
                }
                if(wuc.isSelected()){
                	JSONObject json = new JSONObject();
                    json.put("userId", wuc.getId());                
                    json.put("userName", wuc.getName());
                    json.put("userTinyUrl", wuc.getTiny_Url());
                    json.put("userGender", wuc.getGender());
                    jsonArray.put(json);
                    count++;
                }                
            }
        }
        return jsonArray.toString();
    }
    
    /**
     * 按照传进来的userIds的顺序返回相应的userCacheList
     * @param userIds
     * @return
     */
    public List<WUserCache> getUserCacheListOrderByOriginal(List<Integer> userIds){    	
    	Map<Integer,WUserCache> userCacheMap = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(userIds);
    	List<WUserCache> userCacheList = new ArrayList<WUserCache>();
    	for(int userId:userIds){
    		WUserCache wuc = userCacheMap.get(userId);
    		if(wuc==null){
    			continue;
    		}
    		userCacheList.add(wuc);
    	}
    	return userCacheList;
    }
    
    public String getJSONForGuideUserSomeInfo(GuideUserSomeInfo gusi) {
        JSONObject json = new JSONObject();
        json.put("albumCount", gusi.getAlbumCount());
        json.put("blogCount", gusi.getBlogCount());
        json.put("friendCount", gusi.getFriendCount());
        json.put("userId", gusi.getUserId());
        json.put("doingContent", gusi.getDoingContent());
        json.put("htmlContentForPubLisher", gusi.getHtmlContentForPubLisher());
        json.put("middleSchoolName", gusi.getMiddleSchoolName());
        json.put("middleSchoolYear", gusi.getMiddleSchoolYear());        
        return json.toString();
    }
    
    /**
     * 把IRelation的list转换成GuideSameSchool的list
     * @param list
     * @return
     */
    public List<GuideSameSchool> parseGuideIRelationList(List<IRelation> list) {
		List<GuideSameSchool> gssList = new ArrayList<GuideSameSchool>();
		int count = 0;
		if (list != null) {
			for (IRelation gp : list) {
				GuideSameSchool gss = new GuideSameSchool();
				List<Integer> userIds = new ArrayList<Integer>();
				if (gp == null) {
					continue;
				}
				List<IRelationFriend> peopleList = gp.getList();
				List<SameSchoolPeople> sspList = new ArrayList<SameSchoolPeople>();
				if (peopleList != null && peopleList.size() != 0) {
					for (int i = 0; i < peopleList.size(); i++) {
						IRelationFriend irf = peopleList.get(i);
						if(irf == null){
							continue;
						}
						int userId = irf.getId();
						userIds.add(userId);
						SameSchoolPeople ssp =new SameSchoolPeople();
						ssp.setId(userId);
						ssp.setCollegeEnrollYear(irf.getCollegeEnrollYear());
						ssp.setHighSchoolEnrollEnrollYear(irf.getHighSchoolEnrollYear());
						ssp.setJuniorSchoolEnrollYear(irf.getJuniorSchoolEnrollYear());
						sspList.add(ssp);
					}
				}
				count = gp.getCount();
				GuideLogger.printLog("count:" + count);
				GuideLogger.printLog("gp.getType():" + gp.getType());
				GuideLogger.printLog("gp.getTypeId():" + gp.getTypeId());
				GuideLogger.printLog("gp.getTypeName():" + gp.getTypeName());
				gss.setCount(count);
				gss.setMoreLink(gp.getMoreLink());
				gss.setSchoolId(gp.getTypeId());
				gss.setSchoolName(gp.getTypeName());
				gss.setSspList(sspList);
				gss.setType(gp.getType());
				gssList.add(gss);
			}
		}
		return gssList;
	}
    
    /**
     * 按学校资料推人
     * @param newGssList
     * @param limit
     * @return
     */
    public String sameSchoolJson(List<GuideSameSchool> newGssList, int limit) {
		JSONArray jsonArray = new JSONArray();
		GuideLogger.printLog("newGssList == null:"+(newGssList==null));
		if (newGssList != null) {
			GuideLogger.printLog("newGssList.size():"+newGssList.size());
			for (GuideSameSchool gss : newGssList) {
				if (gss == null) {
					continue;
				}
				GuideLogger.printLog(gss.getSchoolId()+":"+gss.getCount());
				JSONObject json = new JSONObject();
				json.put("count", gss.getCount());
				json.put("moreLink", gss.getMoreLink());
				json.put("schoolId", gss.getSchoolId());
				json.put("schoolname", gss.getSchoolName());
				json.put("type", gss.getType());
				JSONArray people = new JSONArray();
				List<SameSchoolPeople> sspList = gss.getSspList();
				GuideLogger.printLog("sspList == null:"+(sspList==null));
				if (sspList != null && sspList.size() != 0) {
					//如果给定的limit小于idList.size就用limit
					int loop = (limit < sspList.size()) ? limit : sspList.size();
					for (int i = 0; i < loop; i++) {
						SameSchoolPeople ssp = sspList.get(i);
						if (ssp == null) {
							continue;
						}
						JSONObject person = new JSONObject();
						person.put("id", ssp.getId());
						String name = ssp.getName();
						//保证每个json都有name这个key
						if(name==null){
							person.put("name", "");
						}
						else{
							person.put("name", ssp.getName());
						}
						String tinyUrl = ssp.getTiny_url();
						if (tinyUrl == null || tinyUrl.length() == 0 || "".equals(tinyUrl)) {
							if ("女生".equals(ssp.getGender())) {
								tinyUrl = "http://head.xiaonei.com/photos/0/0/women_tiny.gif";
							} else {
								tinyUrl = "http://head.xiaonei.com/photos/0/0/men_tiny.gif";
							}
						}
						person.put("tiny_url", tinyUrl);
						person.put("collegeEnrollYear", ssp.getCollegeEnrollYear());
						person.put("highSchoolEnrollEnrollYear", ssp.getHighSchoolEnrollEnrollYear());
						person.put("juniorSchoolEnrollYear", ssp.getJuniorSchoolEnrollYear());
						people.put(person);
					}
					json.put("people", people);
				}
				jsonArray.put(json);
			}
		}
		return jsonArray.toString();
	}
   
    public void setPassportStatusCutDown(User host) {
    	if(host == null){
    		GuideLogger.printLog("host null");
    		return;
    	}
        UserPassport passport = null;
		try {
			passport = host.getUserPassportInfo();
		} catch (Exception e) {
			GuideLogger.printLog("host:" + host.getId() +" get_passport_null e:"+ e.toString(), GuideLogger.ERROR);
		}
		
		StringBuilder sb = new StringBuilder();
		if(passport != null){
			int oldstatus = passport.getStatus();
			sb.append(" old:"+oldstatus);
			if(oldstatus >= 10){
				oldstatus = oldstatus % 10;
				passport.setStatus(oldstatus);
				sb.append(" new:"+oldstatus+" domain:"+host.getDomain());
				try {
					SnsAdapterFactory.getUserPassportAdapter().setUserPassport(host.getId(),passport);
				} catch (Exception e) {
					GuideLogger.printLog("host:" + host.getId() +" update_passport_err e:"+ e.toString(), GuideLogger.ERROR);
				}
			}
		}
		GuideLogger.printLog(" host:"+host.getId()+ sb.toString() +" ");
    }
}
