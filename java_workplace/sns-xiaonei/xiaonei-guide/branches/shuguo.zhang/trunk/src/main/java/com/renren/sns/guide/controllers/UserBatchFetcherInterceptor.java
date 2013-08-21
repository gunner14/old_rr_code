package com.renren.sns.guide.controllers;

import static com.renren.sns.guide.controllers.window.GuideWindowsController.GUIDE_BUDDY_APPLY;
import static com.renren.sns.guide.controllers.window.GuideWindowsController.GUIDE_COMMON_FRIEND;
import static com.renren.sns.guide.controllers.window.GuideWindowsController.GUIDE_CONFIRMING_FRIENDS;
import static com.renren.sns.guide.controllers.window.GuideWindowsController.GUIDE_FRIDBIRTHDAY_KEY;
import static com.renren.sns.guide.controllers.window.GuideWindowsController.GUIDE_FRIENDS_LIST;
import static com.renren.sns.guide.controllers.window.GuideWindowsController.GUIDE_HOTSTAR_KEY;
import static com.renren.sns.guide.controllers.window.GuideWindowsController.GUIDE_NOT_ENOUGH_COMPLETION;
import static com.renren.sns.guide.controllers.window.GuideWindowsController.GUIDE_SAME_AREA_IDS;
import static com.renren.sns.guide.controllers.window.GuideWindowsController.GUIDE_SAME_IP;
import static com.renren.sns.guide.controllers.window.GuideWindowsController.GUIDE_TWO_MORE_DEGREES;
import static com.renren.sns.guide.controllers.window.GuideWindowsController.GUIDE_VISIT_IDS;
import static com.renren.sns.guide.controllers.window.GuideWindowsController.SAME_SCHOOL_LIST;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.portal.Portal;
import net.paoding.rose.web.portal.PortalUtils;

import com.renren.sns.guide.model.GuideSameSchool;
import com.renren.sns.guide.model.SameSchoolPeople;
import com.renren.sns.guide.model.WUserBirthdayView;
import com.renren.sns.guide.utils.GuideUtils;
import com.renren.sns.guide.view.UserTinyView;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.LogTimeUtils;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * 合并获取UserCache
 * 
 * @author wei.xiang (wei.xiang@opi-corp.com)
 */

public class UserBatchFetcherInterceptor extends ControllerInterceptorAdapter {

	public UserBatchFetcherInterceptor() {
		setPriority(100);
	}

	@SuppressWarnings("unchecked")
	@Override
	public Object after(Invocation inv, Object instruction) throws Exception {
		if (!GuideController.class.isAssignableFrom(inv.getControllerClass())
				|| !GuidexfController.class.isAssignableFrom(inv.getControllerClass())
				|| !GuideBarController.class.isAssignableFrom(inv.getControllerClass())) {
			Portal portal = PortalUtils.getPortal(inv);
			if (portal == null) {
				return instruction;
			}
		}
		long begin = LogTimeUtils.logBegin();
		List<Integer> guideBuddyApplyIds = new ArrayList<Integer>();
		try {
			guideBuddyApplyIds = (List<Integer>) inv.getModel().get(GUIDE_BUDDY_APPLY);
		} catch (Exception e) {
			e.printStackTrace();
		}
		List<Integer> guideCommonFriendIds = new ArrayList<Integer>();
		try {
			guideCommonFriendIds = (List<Integer>) inv.getModel().get(GUIDE_COMMON_FRIEND);
		} catch (Exception e) {
			e.printStackTrace();
		}
		List<Integer> guideConfirmingFriendsIds = new ArrayList<Integer>();
		try {
			guideConfirmingFriendsIds = (List<Integer>) inv.getModel().get(GUIDE_CONFIRMING_FRIENDS);
		} catch (Exception e) {
			e.printStackTrace();
		}
		List<Integer> guideFriendsListIds = new ArrayList<Integer>();
		try {
			guideFriendsListIds = (List<Integer>) inv.getModel().get(GUIDE_FRIENDS_LIST);
		} catch (Exception e) {
			e.printStackTrace();
		}
		List<Integer> guideSameIpIds = new ArrayList<Integer>();
		try {
			guideSameIpIds = (List<Integer>) inv.getModel().get(GUIDE_SAME_IP);
		} catch (Exception e) {
			e.printStackTrace();
		}
		List<UserTinyView> hostStarList = new ArrayList<UserTinyView>();
		try {
			hostStarList = (List<UserTinyView>) inv.getModel().get(GUIDE_HOTSTAR_KEY);
		} catch (Exception e) {
			e.printStackTrace();
		}
		List<Integer> guideVisitIds = new ArrayList<Integer>();
		try {
			guideVisitIds = (List<Integer>) inv.getModel().get(GUIDE_VISIT_IDS);
		} catch (Exception e) {
			e.printStackTrace();
		}
		List<WUserBirthdayView> birthDayList = new ArrayList<WUserBirthdayView>();
		try {
			birthDayList = (List<WUserBirthdayView>) inv.getModel().get(GUIDE_FRIDBIRTHDAY_KEY);
		} catch (Exception e) {
			e.printStackTrace();
		}
		List<Integer> twoMoreDegreesIds = new ArrayList<Integer>();
		try {
			twoMoreDegreesIds = (List<Integer>) inv.getModel().get(GUIDE_TWO_MORE_DEGREES);
		} catch (Exception e) {
			e.printStackTrace();
		}
		List<Integer> guideNotEnoughCompletionIds = new ArrayList<Integer>();
		try {
			guideNotEnoughCompletionIds = (List<Integer>) inv.getModel().get(GUIDE_NOT_ENOUGH_COMPLETION);
		} catch (Exception e) {
			e.printStackTrace();
		}
		List<Integer> guideSameAreaIds = new ArrayList<Integer>();
		try {
			guideSameAreaIds = (List<Integer>) inv.getModel().get(GUIDE_SAME_AREA_IDS);
		} catch (Exception e) {
			e.printStackTrace();
		}
		List<GuideSameSchool> gssList = new ArrayList<GuideSameSchool>();
		try {
			gssList = (List<GuideSameSchool>) inv.getModel().get(SAME_SCHOOL_LIST);
		} catch (Exception e) {
			e.printStackTrace();
		}
		try {
			Set<Integer> ids = new HashSet<Integer>();
			if (guideBuddyApplyIds != null && guideBuddyApplyIds.size() > 0) {
				ids.addAll(guideBuddyApplyIds);
			}
			if (guideCommonFriendIds != null && guideCommonFriendIds.size() > 0) {
				ids.addAll(guideCommonFriendIds);
			}
			if (guideConfirmingFriendsIds != null
					&& guideConfirmingFriendsIds.size() > 0) {
				ids.addAll(guideConfirmingFriendsIds);
			}
			if (guideFriendsListIds != null && guideFriendsListIds.size() > 0) {
				ids.addAll(guideFriendsListIds);
			}
			if (guideSameIpIds != null && guideSameIpIds.size() > 0) {
				ids.addAll(guideSameIpIds);
			}
			if (hostStarList != null && hostStarList.size() > 0) {
				for (UserTinyView tv : hostStarList) {
					ids.add(tv.getId());
				}
			}
			if (guideVisitIds != null && guideVisitIds.size() > 0) {
				ids.addAll(guideVisitIds);
			}
			if (birthDayList != null && birthDayList.size() > 0) {
				for (WUserBirthdayView tv : birthDayList) {
					ids.add(tv.getId());
				}
			}
			if (twoMoreDegreesIds != null && twoMoreDegreesIds.size() > 0) {
				ids.addAll(twoMoreDegreesIds);
			}
			if (guideNotEnoughCompletionIds != null
					&& guideNotEnoughCompletionIds.size() > 0) {
				ids.addAll(guideNotEnoughCompletionIds);
			}
			if (guideSameAreaIds != null && guideSameAreaIds.size() > 0) {
				ids.addAll(guideSameAreaIds);
			}
			// 过滤掉好友和我申请过谁
			List<Integer> twoFriendsFilter = new ArrayList<Integer>();
			if (guideConfirmingFriendsIds != null && guideConfirmingFriendsIds.size() > 0) {
				twoFriendsFilter.addAll(guideConfirmingFriendsIds);
			}
			GuideLogger.printLog("before ids.size():" + ids.size());
			if (gssList != null && gssList.size() > 0) {
				for (GuideSameSchool gss : gssList) {
					if (gss == null) {
						continue;
					}
					List<SameSchoolPeople> sspList = gss.getSspList();
					if (sspList != null && sspList.size() > 0) {
						for (SameSchoolPeople ssp : sspList) {
							if (ssp == null) {
								continue;
							}
							ids.add(ssp.getId());
						}
					}
				}
			}
			GuideLogger.printLog("after ids.size():" + ids.size());
			if (guideFriendsListIds != null && guideFriendsListIds.size() > 0) {
				twoFriendsFilter.addAll(guideConfirmingFriendsIds);
			}
			List<Integer> userIds = new ArrayList<Integer>(ids);
			if (userIds != null) {
				GuideLogger.printLog("userIds.size()" + userIds.size());
			}
			Map<Integer, WUserCache> results = null;
			try {
				results = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(userIds);
			} catch (Exception e) {// 超时或者出错？这个服务太重要的，再取一次？
				System.err.println("GET_USER_CACHE_FOR_HOME_ERROR");
				try {
					results = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(userIds);
				} catch (Exception e2) {
					System.err.println("GET_USER_CACHE_FOR_HOME_ERROR AGAIN!!!");
					e2.printStackTrace();
				}
			}
			LogTimeUtils.logEnd(begin, "UserBatchFetcher:usercache");
			if (results == null || results.size() == 0)
				return instruction;// 没有取出结果
			GuideLogger.printLog("results.size():" + results.size());
			if (guideCommonFriendIds != null && guideCommonFriendIds.size() > 0) {
				List<WUserCache> guideCommonFriend = new ArrayList<WUserCache>();
				for (Integer id : guideCommonFriendIds) {
					WUserCache c = results.get(id);
					if (c != null) {
						guideCommonFriend.add(c);
					}
				}
				String jsonTwoDegreesFriends = GuideUtils.getInstance().getJSONPeopleForDefType(guideCommonFriend, "TwoDegreesFriends", -1, -1);
				String jaMaybeKnowPeople = GuideUtils.getInstance().getJSONArrayForCacheList(guideCommonFriend, 4);
				inv.addModel("jaMaybeKnowPeople", jaMaybeKnowPeople);
				inv.addModel("TwoDegreesFriends", jsonTwoDegreesFriends);
				inv.addModel("commonFriend", guideCommonFriend);
				if (guideCommonFriend != null) {
					inv.addModel("commonFriendSize", guideCommonFriend.size());
				} else {
					inv.addModel("commonFriendSize", 0);
				}
			}
			if (guideBuddyApplyIds != null && guideBuddyApplyIds.size() > 0) {
				List<WUserCache> guideBuddyApplies = new ArrayList<WUserCache>();
				for (Integer id : guideBuddyApplyIds) {
					WUserCache c = results.get(id);
					if (c != null) {
						guideBuddyApplies.add(c);
					}
				}
				inv.addModel("guideBuddyApplies", guideBuddyApplies);
			}
			if (guideConfirmingFriendsIds != null && guideConfirmingFriendsIds.size() > 0) {
				List<WUserCache> guideConfirmingFriends = new ArrayList<WUserCache>();
				for (Integer id : guideConfirmingFriendsIds) {
					WUserCache c = results.get(id);
					if (c != null) {
						guideConfirmingFriends.add(c);
					}
				}
				String jaConfirmingFriends = GuideUtils.getInstance().getJSONArrayForCacheList(guideConfirmingFriends, 4);
				inv.addModel("jaConfirmingFriends", jaConfirmingFriends);
				inv.addModel("myConfirmingFriends", guideConfirmingFriends);
			}
			if (guideFriendsListIds != null && guideFriendsListIds.size() > 0) {
				List<WUserCache> guideFriendsList = new ArrayList<WUserCache>();
				for (Integer id : guideFriendsListIds) {
					WUserCache c = results.get(id);
					if (c != null) {
						guideFriendsList.add(c);
					}
				}
				inv.addModel("myFriendsList", guideFriendsList);
			}
			if (guideSameIpIds != null && guideSameIpIds.size() > 0) {
				Collections.shuffle(guideSameIpIds);
				List<WUserCache> guideSameIp = new ArrayList<WUserCache>();
				for (Integer id : guideSameIpIds) {
					WUserCache c = results.get(id);
					if (c != null) {
						guideSameIp.add(c);
					}
				}
				String jsonSameIP = GuideUtils.getInstance().getJSONPeopleForDefType(guideSameIp, "SameIP", -1, -1);
				inv.addModel("sameIP", jsonSameIP);
			}
			if (hostStarList != null && hostStarList.size() > 0) {
				for (UserTinyView v : hostStarList) {
					if (v == null)
						continue;
					WUserCache c = results.get(v.getId());
					if (c != null) {
						v.setOnline(c.isOnline());
						v.setVipMember(c.isVipMember());
					}
				}
				inv.addModel("starList", hostStarList);
			}
			if (guideVisitIds != null && guideVisitIds.size() > 0) {
				List<WUserCache> guideVisitList = new ArrayList<WUserCache>();
				for (Integer id : guideVisitIds) {
					WUserCache c = results.get(id);
					if (c != null) {
						guideVisitList.add(c);
					}
				}
				inv.addModel("guideVisitList", guideVisitList);
			}
			if (birthDayList != null && birthDayList.size() > 0) {
				List<UserTinyView> friendBirthList = new ArrayList<UserTinyView>();
				for (WUserBirthdayView v : birthDayList) {
					WUserCache c = results.get(v.getId());
					if (c != null) {
						UserTinyView u = new UserTinyView();
						u.setVipMember(c.isVipMember());
						u.setBirth_day(v.getDay());
						u.setBirth_month(v.getMonth());
						u.setName(c.getName());
						u.setTinyUrl(c.getTinyUrl());
						u.setId(v.getId());
						friendBirthList.add(u);
					}
				}
				java.util.Calendar today = java.util.Calendar.getInstance();
				inv.addModel("THIS_MONTH", today.get(java.util.Calendar.MONTH) + 1);
				inv.addModel("THIS_DATE", today.get(java.util.Calendar.DATE));
				List<UserTinyView> list = new ArrayList<UserTinyView>();
				try{
					list = (List<UserTinyView>) friendBirthList;
				}catch(Exception e){
					e.printStackTrace();
				}
				if (list != null) {
					java.util.Collections.sort(list,
							new java.util.Comparator<UserTinyView>() {
								public int compare(UserTinyView o1, UserTinyView o2) {
									if (o1 == null || o2 == null)
										return 1;
									if (o1.getBirth_month() == 1 && o2.getBirth_month() == 12)
										return 1;
									if (o1.getBirth_month() == 12 && o2.getBirth_month() == 1)
										return -1;
									if (o1.getBirth_month() == o2.getBirth_month())
										return o1.getBirth_day() - o2.getBirth_day();
									return o1.getBirth_month() - o2.getBirth_month();
								}
							});
					inv.addModel("friendBirthdayList", list);
				}
			}
			if (twoMoreDegreesIds != null && twoMoreDegreesIds.size() > 0) {
				List<WUserCache> guideTwoMoreDegreesList = new ArrayList<WUserCache>();
				for (Integer id : twoMoreDegreesIds) {
					WUserCache c = results.get(id);
					if (c != null) {
						guideTwoMoreDegreesList.add(c);
					}
				}
				String jaTwoMoreDegrees = GuideUtils.getInstance().getJSONArrayForCacheList(guideTwoMoreDegreesList, 4);
				inv.addModel("jaTwoMoreDegrees", jaTwoMoreDegrees);
				inv.addModel("guideTwoMoreDegreesList", guideTwoMoreDegreesList);
			}
			if (guideNotEnoughCompletionIds != null && guideNotEnoughCompletionIds.size() > 0) {
				List<WUserCache> guideNotEnoughCompletionList = new ArrayList<WUserCache>();
				for (Integer id : guideNotEnoughCompletionIds) {
					WUserCache c = results.get(id);
					if (c != null) {
						guideNotEnoughCompletionList.add(c);
					}
				}
				String jaNotEnoughCompletion = GuideUtils.getInstance().getJSONArrayForCacheList(guideNotEnoughCompletionList, 4);
				inv.addModel("jaNotEnoughCompletion", jaNotEnoughCompletion);
				inv.addModel("guideNotEnoughCompletionList", guideNotEnoughCompletionList);
			}
			if (guideSameAreaIds != null && guideSameAreaIds.size() > 0) {
				List<WUserCache> guideSameAreaList = new ArrayList<WUserCache>();
				for (int i = guideSameAreaIds.size() - 1; i >= 0; i--) {
					int id = guideSameAreaIds.get(i);
					if (twoFriendsFilter != null && twoFriendsFilter.contains(id)) {
						GuideLogger.printLog("guideSameAreaList filter id:" + id);
						guideSameAreaIds.remove(i);
						continue;
					}
					WUserCache c = results.get(id);
					if (c != null) {
						guideSameAreaList.add(c);
					}
				}
				int limit = 21;
				int count = -1;
				String json = GuideUtils.getInstance().getJSONPeopleForDefType(guideSameAreaList, "sameArea", limit, count);
				inv.addModel("sameAreaJson", json);
			}
			List<GuideSameSchool> newGssList = new ArrayList<GuideSameSchool>();
			if (gssList != null && gssList.size() > 0) {
				GuideLogger.printLog("gssList.size():" + gssList.size());
				for (GuideSameSchool gss : gssList) {
					if (gss == null) {
						continue;
					}
					GuideSameSchool newGss = new GuideSameSchool();
					int count = gss.getCount();
					List<SameSchoolPeople> sspList = gss.getSspList();
					// 新的List<SameSchoolPeople>对象，这个对象最终被封装成json
					List<SameSchoolPeople> newSspList = new ArrayList<SameSchoolPeople>();
					if (sspList != null && sspList.size() > 0) {
						for (int i = sspList.size() - 1; i >= 0; i--) {
							SameSchoolPeople ssp = sspList.get(i);
							if (ssp == null) {
								continue;
							}
							int id = ssp.getId();
							if (twoFriendsFilter != null
									&& twoFriendsFilter.contains(id)) {
								sspList.remove(i);
								// 计数减一
								count--;
								continue;
							}
							WUserCache c = results.get(id);
							if (c != null) {
								SameSchoolPeople newSsp = new SameSchoolPeople();
								newSsp.setId(id);
								newSsp.setGender(c.getGender());
								newSsp.setName(c.getName());
								newSsp.setTiny_url(c.getTiny_Url());
								newSsp.setCollegeEnrollYear(ssp.getCollegeEnrollYear());
								newSsp.setHighSchoolEnrollEnrollYear(ssp.getHighSchoolEnrollEnrollYear());
								newSsp.setJuniorSchoolEnrollYear(ssp.getJuniorSchoolEnrollYear());
								newSspList.add(newSsp);
							}
						}
					}
					GuideLogger.printLog("after count:" + count);
					newGss.setCount(count);
					newGss.setMoreLink(gss.getMoreLink());
					newGss.setSchoolId(gss.getSchoolId());
					newGss.setSchoolName(gss.getSchoolName());
					newGss.setType(gss.getType());
					newGss.setSspList(newSspList);
					newGssList.add(newGss);
				}
				int limit = 21;
				String json = GuideUtils.getInstance().sameSchoolJson(newGssList, limit);
				inv.addModel("sameSchoolJson", json);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		LogTimeUtils.logEnd(begin, "UserBatchFetcher:return");
		return instruction;
	}
}
