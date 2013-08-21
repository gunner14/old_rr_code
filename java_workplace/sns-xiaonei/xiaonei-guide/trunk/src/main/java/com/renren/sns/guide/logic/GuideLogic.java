package com.renren.sns.guide.logic;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.portal.Portal;
import net.paoding.rose.web.portal.Window;

import com.renren.sns.guide.controllers.GuideController;
import com.renren.sns.guide.dao.GuideFriendsDAO;
import com.renren.sns.guide.dao.InviteListDAO;
import com.renren.sns.guide.dao.PopUserCityDAO;
import com.renren.sns.guide.dao.PopUserMiddleSchoolDAO;
import com.renren.sns.guide.dao.PopUserMiddleSchoolMapDAO;
import com.renren.sns.guide.model.GuideCity;
import com.renren.sns.guide.model.GuideMiddleSchool;
import com.renren.sns.guide.utils.GuideMemCacheKeysPrefix;
import com.renren.sns.lead.core.DataProviderFactory;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.component.friends.home.FriendsLogic;
import com.xiaonei.platform.component.friends.home.RequestFriendLogic;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.userswitch.UserSwitch;
import com.xiaonei.platform.core.userswitch.UserSwitchFactory;
import com.xiaonei.platform.core.userswitch.UserSwitchIDs;
import com.xiaonei.reg.guide.flows.xfive.controllers.ArgsController;
import com.xiaonei.reg.guide.flows.xfive.controllers.UseridChooseController;
import com.xiaonei.reg.guide.logic.recommend.GuideRecommendConditionLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.wService.menu.RecentMenuLogicAdapter;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAdapter;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheData;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheFactory;
import com.xiaonei.xce.feed.FeedLogicAdapter;

/**
 * @author: wei.xiang   
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-26 上午10:40:08
 */
public class GuideLogic {
    
	private MemCacheManager mcm ;
	
	/*
	 * 显示好友空位的好友数量的临界值
	 */
	private static final int SHOW_FRIENDS_REQUESTED = 10;
	
	private void initMemCacheManager(){
		mcm = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide); 
	}
	
    private GuideLogic() {
    	initMemCacheManager();
    }

    private static GuideLogic instance = null;

    public static synchronized GuideLogic getInstance() {
        if (instance == null) {
            instance = new GuideLogic();
        }
        return instance;
    }
    
    /**
     * 取id向已经发好友申请，但对方并没有接受好友申请的用户id列表 不要使用，按照申请时间逆序
     * 
     * @param id
     * @return
     */
    public List<Integer> getApplyFriendListByTimeDesc(final int id) {
        List<Integer> list = null;
        try {
            list = GuideFriendsDAO.getInstance().getFriendRequestIdListByTimeDesc(id);
        } catch (SQLException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            return null;
        }
        return list;
    }
    
    /**
     * 如果能查到就是导过人，如果不能查都就是没有导过人
     */
    public boolean isMSNInvited(int userId){
    	Boolean flag=false;        
        Object o=mcm.get("isMSNInvited_"+userId);
        if(o==null){
            try {
            	List<Integer> ids = InviteListDAO.getInstance().getInviteList(userId);
                if(ids!=null && ids.size()!=0){
                    flag=true;
                }
                mcm.set("isMSNInvited_"+userId, flag, (int) MemCacheKeys.day*21);
            } catch (SQLException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }     
        }    
        else{
            flag=(Boolean)o;
        }
        return flag;
    }
    
    /**
     * 根据学校和入学年获取人气之星
     * @param middleSchoolId
     * @param enrollYear
     * @param limit
     * @return
     */
	public List<Integer> getPopUserMiddleSchoolList(int middleSchoolId,int enrollYear,int limit){
    	/*String memKey = GuideMemCacheKeysPrefix.MIDDLE_SCHOOL_STAR + middleSchoolId + "_" + enrollYear ;
    	List<Integer> list = (List<Integer>) mcm.get(memKey);
    	GuideLogger.printLog("MIDDLE_SCHOOL_STAR list ==null" + (list==null));
    	try {    		
    		if(list == null){*/
    	List<Integer> list = new ArrayList<Integer>();
		try {
			list = PopUserMiddleSchoolDAO.getInstance().getPopUserMiddleSchoolList(middleSchoolId, enrollYear, limit);
			if (list != null) {
				GuideLogger.printLog("list.size()" + list.size());
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
    			/*GuideLogger.printLog("PopUserMiddleSchoolDAO list ==null" + (list==null));
    			if(list!=null){
    				mcm.set(memKey, list, (int) MemCacheKeys.hour * 3);
    			}    			
    		}    		
		} catch (SQLException e) {
			e.printStackTrace();
		}*/
		return list;
    }
    
	/**
	 * 根据学校查城市
	 * @param middleSchoolId
	 * @return
	 */
    public GuideCity getCityCodeByMiddleSchoolId(int middleSchoolId){
    	String memKey = GuideMemCacheKeysPrefix.CITY_BY_SCHOOL + middleSchoolId;
    	GuideCity gc = (GuideCity) mcm.get(memKey);
    	try {
    		if(gc == null){
    			gc = PopUserMiddleSchoolMapDAO.getInstance().getCityCodeByMiddleSchoolId(middleSchoolId);
    			GuideLogger.printLog("gc==null" + (gc==null));
    			if(gc!=null){
    				mcm.set(memKey, gc, (int) MemCacheKeys.hour * 3);
    			}    			
    		}  
    		
		} catch (SQLException e) {
			e.printStackTrace();
		}		
		return gc;
    }
    
    /**
     * 根据城市查学校列表
     * @param cityCode
     * @param limit
     * @return
     */
	public List<GuideMiddleSchool> getMiddleSchoolIdsByCityCode(String cityCode, int limit) {
		/*
		 * String memKey = GuideMemCacheKeysPrefix.SCHOOLS_BY_CITY + cityCode ;
		 * List<GuideMiddleSchool> list = (List<GuideMiddleSchool>)
		 * mcm.get(memKey); GuideLogger.printLog("schools by city
		 * list==null:"+(list==null)); try { if(list == null){
		 */
		List<GuideMiddleSchool> list = new ArrayList<GuideMiddleSchool>();
		try {
			list = PopUserMiddleSchoolDAO.getInstance().getMiddleSchoolIdsByCityCode(cityCode, limit);
			if (list != null) {
				GuideLogger.printLog(cityCode + ":的学校个数:" + list.size());
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		/*
		 * GuideLogger.printLog("list==null:"+(list==null)); if(list!=null){
		 * GuideLogger.printLog("list.size():"+list.size()); mcm.set(memKey,
		 * list, (int) MemCacheKeys.hour * 3); } } } catch (SQLException e) {
		 * e.printStackTrace(); }
		 */
		return list;
	}
    
    /**
	 * 城市的人气之星
	 * 
	 * @param cityCode
	 * @param limit
	 * @return
	 */
    public List<Integer> getUserStarIdsByCityCode(String cityCode,int limit){
    	/*String memKey = GuideMemCacheKeysPrefix.CITY_USER_STAR + cityCode ;
    	List<Integer> list = (List<Integer>) mcm.get(memKey);    
    	GuideLogger.printLog("CITY_USER_STAR list==null:"+(list==null));
    	try {    		
    		if(list == null){*/
    	List<Integer> list = new ArrayList<Integer>();
		try {
			list = PopUserCityDAO.getInstance().getUserStarIdsByCityCode(cityCode, limit);
			if(list!=null){
				GuideLogger.printLog(cityCode+":城市人气之星的人数:"+list.size());
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
    			/*if(list!=null){
    				mcm.set(memKey, list, (int) MemCacheKeys.hour * 3);
    			}
    		}    		
		} catch (SQLException e) {
			e.printStackTrace();
		}*/
		return list;
    }
    
    public void addCommonWindows(Invocation inv,Portal portal,User host){
    	addWindow(portal, "window_guidegetcommonmemcache", "guideGetCommonMemcache");
        addWindow(portal, "window_guidegetmemcache", "guideGetMemCache");
        addWindow(portal, "window_guideisconfirmed", "guideIsConfirmed");
        addWindow(portal, "window_guidevip", "guideVip");            
        addWindow(portal, "window_guidenotenoughcompletion", "guideNotEnoughCompletion");        
        addWindow(portal, "window_guidescore", "guideScore");
        addWindow(portal, "window_guideuserbasic", "guideUserBasic"); 
        addWindow(portal, "window_userOwnSites", "userOwnSites"); 
        addWindow(portal, "window_guideMiniGroupMenu", "guideMiniGroupMenu"); 
        addWindow(portal, "window_guideRecentMenuHome", "guideRecentMenuHome"); 
        addWindow(portal, "window_homenuomimenu", "homeNuomiMenu");
        addWindow(portal, "window_guideisxinsheng2010", "guideIsXinSheng2010");
        addWindow(portal, "window_guidestatus", "guideStatus"); 
    }
    
    public void addAllStageWindows(Invocation inv,Portal portal,User host){
    	addWindow(portal, "window_guidefootprint", "guideFootPrint");   
    	if(host.getId() % 10 ==9 ){
    		addWindow(portal, "window_guidegetfriendlistwithintime", "getFriendListWithinTime");
    	}        
        addWindow(portal, "window_guideinitusercounts", "guideInitUserCounts");
        addWindow(portal, "window_guideisshownewheadertip", "guideIsShowNewHeaderTip");
        //addWindow(portal, "window_guideisxinsheng2010", "guideIsXinSheng2010");
        addWindow(portal, "window_guidemanagepages", "guideManagePages");        
        addWindow(portal, "window_guidemanageloverpages", "guideManageLoverPages"); 
        addWindow(portal, "window_guidemaybefrid", "guideMaybeFrid");
        addWindow(portal, "window_guidevipZone", "vipZone"); 
        UserSwitch switcher = UserSwitchFactory.get(UserSwitchIDs.SWITCH_ID_UGCTAB);
        if (switcher.isOn(host.getId())) {
        	GuideLogger.printLog("host.getId():" + host.getId() + "GetFeedView");
        	addWindow(portal, "window_guidegetfeedview", "guideGetFeedView");
        } else {
        	GuideLogger.printLog("host.getId():" + host.getId() + "GuideNewsFeed");
            addWindow(portal, "window_guidenewsfeed", "guideNewsFeed");//
        }
       
        String newFlowSource=RegFromSourceLogic.getFromSource(host);
        if(newFlowSource==null){
        	newFlowSource="";
        }
        //if(ifShowFriendRequestTop(inv,host)) { //
        	//addWindow(portal,"guide_v6_recommend_friend","guideMyFriends"); //addModel()不变，只需要前台在解析过程中，和以前的右侧好友列表类似即可
        //} else {
        	addWindow(portal, "window_guidemyfriends", "guideMyFriends"); 
        	addWindow(portal, "guide_v6_recommend", "guideV6Commented");
        //}

        
       /* if(ArgsController.isdebug==true){
            addWindow(portal, "guide_v6_recommend", "guideV6Commented"); 
        }
        else if(newFlowSource.trim().endsWith("_AAF") && host.getId()%100!=1 && host.getId()%100!=2 && host.getId()%100!=3
        		&& host.getId()%100!=4 && host.getId()%100!=5 && host.getId()<447938281){
            addWindow(portal, "guide_v6_recommend", "guideV6Commented"); 
        }
        else if( GuideRecommendConditionLogic.getInstance().contains(host.getId()%10) && host.getId()<447938281){
        	addWindow(portal, "guide_v6_recommend", "guideV6Commented"); 
        }*/
    }
    
    
    public void addFavList(Portal portal){
        	
        addWindow(portal, "home_CollectionMenuHome", "homeCollectionMenuHome"); 
       
    }
    
    public void addOtherWindows(Invocation inv,Portal portal,User host){
    	addWindow(portal, "window_guideappfootprint", "guideAppFootprint");
    	addWindow(portal, "window_guideappinvite", "guideAppInvite");
    	addWindow(portal, "window_guidebartreasure", "guideBarTreasure");
    	addWindow(portal, "window_guidefridbirthday", "guideFridBirthday");  
    	addWindow(portal, "window_guidegetrequestmap", "guideGetRequestMap");
    	addWindow(portal, "window_guidepositivefootprint", "guidePositiveFootprint");
    	addWindow(portal, "window_guiderenrenschool", "guideRenrenSchool");
        addWindow(portal, "window_guiderighthotstar", "guideRightHotStar");
        addWindow(portal, "window_guiderightpages", "guideRightPages");
        addWindow(portal, "window_guidesendhotnews", "guideSendHotNews");
        addWindow(portal, "window_guidezhengzhougroup", "guideZhengZhouGroup");
    }
    
    public void addWindowsForStageTen(Invocation inv,Portal portal,User host){
    	addWindow(portal, "window_guidegetpopusermiddleschoollist", "guideGetPopUserMiddleSchoolList");
    }
    
    private void addWindow(Portal portal, String name, String path) {
        try {
            Window window = portal.addWindow(name, "/window/guideWindows/" + path);
            window.set(Window.FUTURE_CANCEL_ENABLE_ATTR, false);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**
     * 导新鲜事
     */
    public void guideBuddyReplyNotify(int hostId,List<Integer> idList){
    	if (idList != null) {
            for (int i = 0; i < idList.size(); i++) {
                int fid = idList.get(i);
                //对方的隐私设置不为仅是好友可见(getBasicConfig()!= 0)
                if (SnsAdapterFactory.getUserConfigInfoAdapter().getUserConfig(fid).getBasicConfig() != 0) {
                    FeedLogicAdapter.getInstance().guideBuddyReplyNotify(hostId, fid);
                    GuideLogger.printLog(fid+" => "+hostId);
                }
            }
        }
    }
    
    public void guideBuddyReplyNotifyAsync(User host,int timeout, List<Integer> idList){
		DataProviderFactory.getDataProvider("GuideBuddyReplyNotify").getData(host, timeout, idList);
    }
    
    
//    public static boolean isV6(User host,String newFlowSource){
//    	int userid=host.getId();
//
//    	if(newFlowSource==null){
//    		newFlowSource="";
//    	}
//    	if(ArgsController.isdebug==true){
//    		//GuideLogger.printLog("test env");
//    		if(!host.getUserState().isV6()){
//
//        		SnsAdapterFactory.getUserStateAdapter().setV6On(userid);
//
//        	}
//    		return true;
//    	}
//
//    	if(host.getUserState().isV6()){
//    		//GuideLogger.printLog("already is v6 user");
//    		//int[] appIds = new int[]{166607,166613,166615,166616,166617};
//		    //RecentMenuLogicAdapter.getInstance().tracks(Integer.valueOf(host.getId()),appIds);
//    		return true;
//    	}
//    	if(newFlowSource.trim().endsWith("_AAF")){
//    		//GuideLogger.printLog("AAF user");
//        	if(!host.getUserState().isV6()){
//
//        		SnsAdapterFactory.getUserStateAdapter().setV6On(userid);
//        	}
//        	return true;
//        }
//    	//把新注册的用户都指向v6
//		if ((userid > 451294146) && (userid < 600000000)) {
//			if (!host.getUserState().isV6()) {
//
//				SnsAdapterFactory.getUserStateAdapter().setV6On(userid);
//			}
//			return true;
//		}
//        /*if(GuideRecommendConditionLogic.getInstance().contains(host.getId()%10) && host.getId()<447938281){
//        	GuideLogger.printLog("before user");
//        	if(!host.getUserState().isV6()){
//
//        		SnsAdapterFactory.getUserStateAdapter().setV6On(host.getId());
//        	}
//        	return true;
//        }*/
//        return false;
//    }
    
    /**
     * 得到当前用户所申请的所有好友的ID
     * @param inv
     * @param user
     * @return 申请过的好友list
     */
    public boolean ifShowFriendRequestTop(Invocation inv,User user) {
    	boolean result = false;
    	
        BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData> _adapter = new BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData>(new DefaultBuddyByApplicantCacheFactory());
        DefaultBuddyByApplicantCacheData data = (DefaultBuddyByApplicantCacheData) _adapter.getBuddyByApplicantCacheData(user.getId(), 0, -1);
        
        if ( data.getAccepters().size() <= SHOW_FRIENDS_REQUESTED ){
        	result = true;
        }
        return result;
    }
}
 