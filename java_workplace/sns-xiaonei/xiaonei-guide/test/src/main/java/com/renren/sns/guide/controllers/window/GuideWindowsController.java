package com.renren.sns.guide.controllers.window;

import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;

import javax.annotation.PostConstruct;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.model.FootprintNewView;
import mop.hi.oce.domain.model.FootprintResult;
import mop.hi.oce.domain.model.Relation;
import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.WorkplaceInfo;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.portal.Window;

import org.springframework.beans.factory.annotation.Autowired;

import com.dodoyo.datum.client.DatumInfoClient;
import com.dodoyo.datum.model.RegionInfo;
import com.dodoyo.friends.relation.factory.RelationModelFactory;
import com.dodoyo.friends.relation.logicmodel.IRelationModel;
import com.dodoyo.friends.relationfriend.logic.IRelation;
import com.dodoyo.friends.utils.FriendsUtil;
import com.renren.app.share.hot.ShareHotHome;
import com.renren.app.share.hot.model.HotShare;
import com.renren.app.share.xoa.api.HotShareService;
import com.renren.photo.xoa.api.service.AlbumListService;
import com.renren.sns.guide.business.HandlerContainer;
import com.renren.sns.guide.dao.NewTaskDao;
import com.renren.sns.guide.dao.StarDAO;
import com.renren.sns.guide.factory.GuideRelationModelFactory;
import com.renren.sns.guide.logic.GuideLogic;
import com.renren.sns.guide.logic.HotStarRelation;
import com.renren.sns.guide.logic.HotStarRelationImpl;
import com.renren.sns.guide.model.AppHomeView;
import com.renren.sns.guide.model.GuideCity;
import com.renren.sns.guide.model.GuideIRelation;
import com.renren.sns.guide.model.GuideIRelationModel;
import com.renren.sns.guide.model.GuideMiddleSchool;
import com.renren.sns.guide.model.GuideNewTask;
import com.renren.sns.guide.model.GuideNewTaskView;
import com.renren.sns.guide.model.GuideSameSchool;
import com.renren.sns.guide.model.SnsFootprintResult;
import com.renren.sns.guide.model.TaskHashMap;
import com.renren.sns.guide.service.GuideRecommendLogic;
import com.renren.sns.guide.service.HomeNuomiUserConfig;
import com.renren.sns.guide.utils.GuideFriendsUtil;
import com.renren.sns.guide.utils.GuideMemCacheKeysPrefix;
import com.renren.sns.guide.utils.GuideRecommendConstants;
import com.renren.sns.guide.utils.GuideUtils;
import com.renren.sns.guide.view.UserTinyView;
import com.renren.sns.tag.service.HomeNavService;
import com.renren.sns.tag.view.cache.Site;
import com.renren.ugc.model.forum.Forum;
import com.renren.ugc.util.MiniGroupUtil;
import com.renren.xcache.XCache;
import com.renren.xoa.Method;
import com.renren.xoa.XoaClient;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFactory;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.ServiceFutureHelper;
import com.xiaonei.album.home.AlbumHome;
import com.xiaonei.blog.home.BlogHome;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.throughput.MethodThroughput;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.domain.view.HomeSpreadView;
import com.xiaonei.page.service.AdminService;
import com.xiaonei.page.service.PageSpreadService;
import com.xiaonei.platform.component.application.menu.home.RecentMenuHome;
import com.xiaonei.platform.component.doing.home.DoingHome;
import com.xiaonei.platform.component.doing.model.Doing;
import com.xiaonei.platform.component.doing.model.DoingUBB;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.component.space.home.SpaceHome;
import com.xiaonei.platform.component.xfeed.display.DisplayFeedFactory;
import com.xiaonei.platform.component.xfeed.display.FeedFactory;
import com.xiaonei.platform.component.xfeed.display.IFeedRetriever;
import com.xiaonei.platform.component.xfeed.helper.FeedDefinition;
import com.xiaonei.platform.component.xfeed.publish.FeedBuilder;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.control.TimeCostUtil;
import com.xiaonei.platform.core.menu.UserMenu;
import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.model.UserConfigInfo;
import com.xiaonei.platform.core.model.UserState;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.AppMenuItemCacheMgr;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.opt.base.UserConfigWantSeeCssMgr;
import com.xiaonei.platform.core.opt.base.UserConfirmLogic;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.opt.ice.WUserCacheAdapter;
import com.xiaonei.platform.core.opt.jdbc.home.AppMenuItemHome;
import com.xiaonei.platform.core.opt.permission.License;
import com.xiaonei.platform.core.opt.permission.define.RelationDef;
import com.xiaonei.platform.core.opt.permission.strategy.impl.ProfilePrivacy;
import com.xiaonei.platform.core.utility.StringUtils;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveIDsPreviewLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.VisitLogLogic;
import com.xiaonei.reg.guide.model.SnsFootprintView;
import com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideRemoteLogger;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.reg.guide.util.LogTimeUtils;
import com.xiaonei.reg.register.model.RegInviteMap;
import com.xiaonei.reg.usertrace.dao.GuideRecommendJadeDAO;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.opt.ice.model.SnsBuddyApplyCacheDataN;
import com.xiaonei.sns.platform.core.opt.ice.model.SnsBuddyApplyCacheWithTotalN;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.DefaultScoreCacheProvider;
import com.xiaonei.sns.platform.core.rose.cache.provider.ObjectProvider;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserBasicProvider;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserConfigInfoProvider;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;
import com.xiaonei.sns.platform.core.rose.interceptors.MergeWindowModel;
import com.xiaonei.sns.platform.core.rose.interceptors.Performance;
import com.xiaonei.sns.platform.core.rose.interceptors.ServiceCutter;
import com.xiaonei.vip.facade.member.VipFacade;
import com.xiaonei.vip.facade.member.VipInfo;
import com.xiaonei.wservice.userscore.client.ScoreUtil;
import com.xiaonei.xce.buddybyaddtimecache.BuddyByAddTimeCacheAdapter;
import com.xiaonei.xce.buddybyaddtimecache.DefaultBuddyByAddTime;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAdapter;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheData;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheFactory;
import com.xiaonei.xce.feed.FeedLogicAdapter;
import com.xiaonei.xce.feed.FeedViewAdapter;
import com.xiaonei.xce.footprint.FootprintNewAdapter;
import com.xiaonei.xce.footprintunite.FootprintView;
import com.xiaonei.xce.footprintunite.VisitFootprintAdapter;
import com.xiaonei.xce.footprintunite.VisitFootprintResult;
import com.xiaonei.xce.friendfinderinfocache.FriendFinderInfoCacheAdapter;
import com.xiaonei.xce.iprecord.IPRecordAdapter;
import com.xiaonei.xce.offerfriends.DefaultOffer;
import com.xiaonei.xce.offerfriends.OfferFriendsAdapter;
import com.xiaonei.xce.scorecache.DefaultScoreCache;
import com.xiaonei.xce.scorecache.ScoreCache;
import com.xiaonei.xce.scorecache.ScoreCacheAdapter;
import com.xiaonei.platform.component.xfeed.display.impl.FeedRetriever2;

/**
 * Guide的portal在rose框架下的总配置
 * 
 * @author wei.xiang (wei.xiang@renren-inc.com)
 * 
 */
@MergeWindowModel
@Performance
@ServiceCutter(timeout = 50)
@SuppressWarnings("deprecation")
@IgnoreUserStatusImportedValidation
public class GuideWindowsController {

    @Autowired
    ShareHotHome shareHotHome; 
    
    @Autowired
	private  GuideRecommendLogic guideRecommendLogic;
    
    @Autowired
    private GuideRecommendJadeDAO guideRecommendJadeDAO;
    
    public static final String GUIDE_BUDDY_APPLY = "guideBuddyApplyIds";

    public static final String GUIDE_HOTSTAR_KEY = "starList_IDS";
    
    public static final String GUIDE_SAME_IP = "guideSameIpIds";

    public static final String GUIDE_FRIENDS_LIST = "guideFriendsListIds";

    public static final String GUIDE_CONFIRMING_FRIENDS = "guideConfirmingFriendsIds";

    public static final String GUIDE_COMMON_FRIEND = "guideCommonFriend";

    public static final String GUIDE_VISIT_IDS = "guideVisitIds"; 
    
    public static final String GUIDE_TWO_MORE_DEGREES = "guideTwoMoreDegrees"; 
    
    public static final String GUIDE_NOT_ENOUGH_COMPLETION = "guideNotEnoughCompletion"; 
    
    public static final String GUIDE_SAME_AREA_IDS = "sameAreaIds";
    
    public static final String SAME_SCHOOL_LIST = "sameSchoolList";
    
    /**
     * 最近来访
     * 
     * @param inv
     * @param window
     * @param cm
     */
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideFootPrint(Invocation inv, Window window, CacheManager cm) {
        try {
            long beginFootPrint = LogTimeUtils.logBegin();
            Integer hostId = UserProvider.getInstance().get(cm).getId();
            SnsFootprintResult result = VisitLogLogic.getSnsFootprintResult(hostId, 0, 7);
            if(result!=null){
                List<SnsFootprintView> visitList = result.getFootprintViewList();
                if (result.getUserVisitCount() > 0) {
                    inv.addModel("viewcount", result.getUserVisitCount());
                }
                if (visitList != null && visitList.size() > 0) {
                    inv.addModel("visitList", visitList);
                }
            }            
            LogTimeUtils.logEnd(beginFootPrint, "EndFootPrint");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 正向脚印,我访问过谁
     * 
     * @param inv
     * @param window
     * @param cm
     */
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guidePositiveFootprint(Invocation inv, Window window, CacheManager cm) {
        try {
            long begin = LogTimeUtils.logBegin();
            Integer hostId = UserProvider.getInstance().get(cm).getId();
            VisitFootprintAdapter vfa = AdapterFactory.getVisitFootprintAdapter();
            if(vfa!=null){
                VisitFootprintResult result = vfa.getAll(hostId, 0, 7);
                if(result!=null){
                    List<Integer> list = new ArrayList<Integer>();
                    for (FootprintView view : result.getFootprintViews()) {
                        if(view!=null){
                            list.add(view.getVisitor());
                        }                        
                    }
                    inv.addModel(GUIDE_VISIT_IDS, list);
                }                
            }  
            LogTimeUtils.logEnd(begin, "guidePositiveFootprint");
        } catch (Exception e) {
            System.err.println("guidePositiveFootprint:" + e.toString());
        }
    }

    /**
     * X5的根据资料和IP推人
     * 
     * @param inv
     * @param window
     * @param cm
     */
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideX5MayKnown(Invocation inv, Window window, CacheManager cm) {
        User user = UserProvider.getInstance().get(cm);
        try {
            int amount = 49;
            long beginMayKnown = LogTimeUtils.logBegin();
            List<GuideIRelationModel> list = new ArrayList<GuideIRelationModel>();
            //获取各个阶段的Model
            GuideIRelationModel workerModel = GuideRelationModelFactory.create(user, amount, GuideRelationModelFactory.COWORKER_RELATION_MODEL);
            GuideIRelationModel universityModel = GuideRelationModelFactory.create(user, amount, GuideRelationModelFactory.UNIVERSITY_RELATION_MODEL);
            GuideIRelationModel collegeModel = GuideRelationModelFactory.create(user, amount, GuideRelationModelFactory.COLLEGE_RELATION_MODEL);
            GuideIRelationModel highModel = GuideRelationModelFactory.create(user, amount, GuideRelationModelFactory.HIGH_RELATION_MODEL);
            GuideIRelationModel juniorModel = GuideRelationModelFactory.create(user, amount, GuideRelationModelFactory.JUNIOR_RELATION_MODEL);
            GuideIRelationModel elementaryModel = GuideRelationModelFactory.create(user, amount, GuideRelationModelFactory.ELEMENTARY_RELATION_MODEL);
            GuideIRelationModel locationModel = GuideRelationModelFactory.create(user, amount, GuideRelationModelFactory.LOCATION_RELATION_MODEL);
            list.add(workerModel);
            list.add(universityModel);
            list.add(collegeModel);
            list.add(highModel);
            list.add(juniorModel);
            list.add(elementaryModel);
            list.add(locationModel);
            List<GuideIRelation> mayFriends = GuideFriendsUtil.findPeople(user, list);
            inv.addModel("companyAndSchool", mayFriends);
            LogTimeUtils.logEnd(beginMayKnown, "EndMayKnownX5");
            long beginSameIP = LogTimeUtils.logBegin();
            int limit = -1;//全取出来
            String ip = BizFilterUtil.getClientIP(inv.getRequest());
            int[] co = IPRecordAdapter.getInstance().getUsers(ip, limit);
            List<Integer> userIds = GuideXFiveIDsPreviewLogic.getInstance().getIntegerList(co);
            String json = GuideUtils.getInstance().getJSONPeopleMayKnowForGuide(mayFriends, userIds);
            LogTimeUtils.logEnd(beginSameIP, "EndSameIP");
            inv.addModel("mayknown", json);
        } catch (Exception e) {
            System.err.println("guideX5MayKnown:" + e.toString());
        }
    }

    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideRightPages(Invocation inv, Window window, CacheManager cm) {
        User user = UserProvider.getInstance().get(cm);
        try {
            long beginRightPublicPages = LogTimeUtils.logBegin();
            /**
             * guide页面推广位，我关注的公共主页<br>
             * 
             * 直接从缓存中取，如果没有，自动添加一个任务到一个异步队列里面去处理
             * 
             * @param user
             * @return List<PageView>
             */
            HomeSpreadView homeSpreadView = PageSpreadService.getInstance().getHomeSpreads(user.getId());
            List<Page> myPages = homeSpreadView.getMypages();
            inv.addModel("myPages", myPages);
            LogTimeUtils.logEnd(beginRightPublicPages, "EndPublicPage");
        } catch (Exception e) {
            System.err.println("guideRightPages:" + e.toString());
        }
    }

    /**
     * 好友列表
     * 
     * @param inv
     * @param window
     * @param cm
     */
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideMyFriends(Invocation inv, Window window, CacheManager cm) {// ok
        User user = UserProvider.getInstance().get(cm);        
        long begin = LogTimeUtils.logBegin();
        List<Integer> friendIds = FriendsHome.getInstance().getFriendsIds(user.getId());
        int friendsCount=0;
        if (friendIds != null && friendIds.size() != 0) {
            inv.addModel(GUIDE_FRIENDS_LIST, friendIds);
            friendsCount=friendIds.size();
        }
        inv.addModel("guideFriendsCount", friendsCount);
        LogTimeUtils.logEnd(begin, "GuideWindowsControllerGetFriendsIds");
        long begin1 = LogTimeUtils.logBegin();
        List<Integer> idList = new ArrayList<Integer>();
        BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData> _adapter = new BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData>(new DefaultBuddyByApplicantCacheFactory());
        DefaultBuddyByApplicantCacheData _data = (DefaultBuddyByApplicantCacheData) _adapter.getBuddyByApplicantCacheData(user.getId(), 0, -1);
        for (BuddyByApplicantCacheAccepterInfo el : _data.getAccepters()) {
            idList.add(el.getAccepter());
        }
        if (idList != null) {
            inv.addModel(GUIDE_CONFIRMING_FRIENDS, idList);
        }            
        LogTimeUtils.logEnd(begin1, "GuideWindowsControllerApplyFriendList");
        long begin3 = LogTimeUtils.logBegin();
        //这是二度好友的
        List<Integer> flist = FriendFinderInfoCacheAdapter.getInstance().getGuestInfoMatchFriendListBatch(GuideUtils.getInstance().getIntArray(idList), user.getId()); 
        if (idList != null) {
            inv.addModel(GUIDE_TWO_MORE_DEGREES, flist);
        }             
        LogTimeUtils.logEnd(begin3, "FriendFinderInfoCacheAdapter");        
        int friendTwoWay = 0;
        List<Integer> twoWayFriendsIds = new ArrayList<Integer>();
        if (friendIds != null) {
            friendTwoWay += friendIds.size();
            twoWayFriendsIds.addAll(friendIds);
        }
        if (idList != null) {
            friendTwoWay += idList.size();
            twoWayFriendsIds.addAll(idList);
        }            
        String idsJson=GuideUtils.getInstance().getJSONForTwoWayFriendsIds(twoWayFriendsIds);
        inv.addModel("twoWayFriendsIds", idsJson);
        inv.addModel("friendTwoWay", friendTwoWay);        
    }

    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideNewsFeed(Invocation inv, Window window, CacheManager cm) {
        long begin = LogTimeUtils.logBegin();
        User host = UserProvider.getInstance().get(cm);
        FeedFactory factory = FeedFactory.getInstance();
        try {
            Map<String, ? super Object> map = factory.showNews(host.getId(), host.getName(), 0, IFeedRetriever.HOME_LIMIT);
            if (map != null) {
                String feedString = (String) map.get("content");
                int feedCount = (Integer) map.get("count");
                inv.addModel("userFeed", feedString);
                inv.addModel("feedStart", "1");
                inv.addModel("feedEnd", feedCount);
                //GuideLoggerCountFeed.printLog("guideNewsFeed userid:"+host.getId()+
                		//" feedcount:"+feedCount+" totalcount:"+FeedRetriever2.getInstance().getUnviewedCount(host.getId()));
                guideRecommendLogic.insertGuideUserFeedCount(host.getId(), feedCount, "guide_home");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        UserConfigInfo config = UserConfigInfoProvider.getInstance().get(cm, host.getId());
        inv.addModel("feedStyle", UserConfigWantSeeCssMgr.isHomeFeedSimpleView(config));
        LogTimeUtils.logEnd(begin, "guideNewsFeed:"+host.getId());
    }

    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public String guideInitUserCounts(Invocation inv, Window window, CacheManager cm) {// ok
        return "c:guideUserCount/homeUserCount";
    }

    /**
     * 邮箱认证
     * 
     * @since 20091206
     */
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideIsConfirmed(Invocation inv, Window window, CacheManager cm) {
        try {
            long begin = LogTimeUtils.logBegin();
            User user = UserProvider.getInstance().get(cm);
            boolean emailIsConfirmed = UserConfirmLogic.getInstance().isConfirmed(user);
            inv.addModel("emailIsConfirmed", emailIsConfirmed);
            LogTimeUtils.logEnd(begin, "guideIsConfirmed");
        } catch (Exception e) {
            System.err.println("guideIsConfirmed:" + e.toString());
        }
    }

    /**
     * 可能认识的人
     * 
     * @param inv
     * @param window
     * @param cm
     */
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideMaybeFrid(Invocation inv, Window window, CacheManager cm) {// ok
        try {
            long begin = LogTimeUtils.logBegin();
            User host = UserProvider.getInstance().get(cm);
            int limit = 49;
            List<Integer> ids = new ArrayList<Integer>();
            OfferFriendsAdapter<DefaultOffer> adapter = OfferFriendsAdapter.getInstance();
            if(adapter!=null){
                adapter.rebuild(host.getId());   
                List<DefaultOffer> offers = adapter.getBestOffer(host.getId(), limit);                             
                if (offers != null) {
                    for (int i = 0; i < offers.size(); i++) {
                        ids.add(offers.get(i).getUserId());
                    }
                }
            }            
            inv.addModel(GUIDE_COMMON_FRIEND, ids); 
            StringBuffer sb=new StringBuffer();
            sb.append("[guide] "+ids.size()+" "+ids.toString());
            GuideRemoteLogger.getInstance().log(host.getId(),GuideRemoteLogger.TYPE_GETOFFERFIREND,sb.toString());            
            LogTimeUtils.logEnd(begin, "guideMaybeFrid");
        } catch (Exception e) {
            System.err.println("guideMaybeFrid:" + e.toString());
        }
    }

    /**
     * 状态
     * 
     * @param inv
     * @param window
     * @param cm
     */
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideStatus(Invocation inv, Window window, CacheManager cm) {
        long begin = LogTimeUtils.logBegin();
        User host = UserProvider.getInstance().get(cm);
        UserBasic ub = UserBasicProvider.getInstance().get(cm);
        inv.addModel("RecommendDoingUBB", DoingHome.getInstance().getDoingUBB());
        Doing doing = DoingHome.getInstance().getDoingFromCache(host.getId());
        if (ub != null) {
            if ((doing != null) && (doing.getDoTime() != null)) {
                inv.addModel("ugcstatus", doing.getHtmlContentForPubLisher());
                inv.addModel("fullstatus", doing.getContent());
            }
            inv.addModel("doingtime", ub.getDoingTime().getTime());
            inv.addModel("ubid", ub.getId());
        }
        List<DoingUBB> ubbList = null;
        try {
            ubbList = DoingHome.getInstance().getDoingUBB();
            inv.addModel("ubbList", ubbList);
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (ub != null) {
            String updateTitle = "上一条";
            String staticUrl = OpiConstants.urlStatic;
            Long doingtime = ub.getDoingTime().getTime();
            long dTime;
            if (doingtime != null) {
                try {
                    dTime = Long.valueOf(doingtime);
                    Calendar cal = Calendar.getInstance();
                    long doneMinutes = cal.getTime().getTime() - dTime;
                    if (doneMinutes < 0) {
                        doneMinutes = 0;
                    }
                    doneMinutes = doneMinutes / (1000 * 60);
                    String ugcStatus = "";
                    if (doing != null) {
                        ugcStatus = doing.getHtmlContentForPubLisher();
                        if (ugcStatus != null && ugcStatus.length() > 0) {
                            if (doneMinutes < 1) {
                                updateTitle = "刚刚更新";
                            } else if (doneMinutes < 60) {
                                updateTitle = doneMinutes + "分钟前";
                            } else if (doneMinutes < 1440) {
                                updateTitle = doneMinutes / 60 + "小时前";
                            } else if (doneMinutes < 4320) {
                                updateTitle = doneMinutes / 1440 + "天前";
                            } else {
                                updateTitle = "n天前";
                            }
                        } else {
                            updateTitle = "你在做什么";
                        }
                    }
                    String ticket;
                    int code = 0;
                    ticket = CookieManager.getInstance().getCookie(inv.getRequest(),
                            PassportManager.loginKey_ticket);
                    if (ticket != null) {
                        code = ticket.hashCode();
                    }
                    inv.addModel("pubisherCode", code);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            inv.addModel("updateTitle", updateTitle);
            inv.addModel("staticUrl", staticUrl);
        }
        // 动态修改状态里的文案
        try {
            String doingText = DoingHome.getInstance().getOnlineText();
            inv.addModel("onlineText", doingText);
        } catch (Exception e) {
            inv.addModel("onlineText", "你正在干嘛？");
            e.printStackTrace();
        }
        LogTimeUtils.logEnd(begin, "guideStatus");
    }

    /**
     * VIP等级和过期提醒
     * 
     * @param inv
     * @param window
     * @param cm
     */
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideVip(Invocation inv, Window window, CacheManager cm) {// OK
        long begin = LogTimeUtils.logBegin();
        User host = UserProvider.getInstance().get(cm);
        int level = 0;
        int isVip = -1;
        long days = 8;
        String homeTipCode = "";
        VipInfo vipinfo;
        try {
            vipinfo = VipFacade.getInstance().getVipInfo(host);
        } catch (Exception e1) {
            System.err.println("vipinfo:" + e1.toString());
            return;
        }
        if (null != vipinfo) {
            homeTipCode = vipinfo.getHomeTplCode();
        }
        if (VipFacade.getInstance().isVipMember(host)) { // vip用户用亮标显示等级
            try {
                Date now = new Date();
                long remainDay = (vipinfo.getEndTime().getTime() - now.getTime()) / (1000 * 60 * 60 * 24L);
                days = remainDay > 0 ? remainDay : 1;
            } catch (Exception e) {
                System.err.println("guideVip:" + e.toString());
            }
            level = VipFacade.getInstance().getVipLevel(host); // vip等级
            isVip = 1;
        } else if (VipFacade.getInstance().isEverVip(host)) { // 曾经是vip的用户用灰标显示等级
            level = VipFacade.getInstance().getVipLevel(host); // vip等级
            isVip = 0;
        }
        inv.addModel("days", days);
        inv.addModel("isVip", isVip);
        inv.addModel("VipLevel", level);
        inv.addModel("homeTipCode", homeTipCode);
        LogTimeUtils.logEnd(begin, "guideVip");
    }

    /**
     * 积分系统
     * 
     * @since 20091106
     * @author wei.xiang@opi-corp.com Mar 9, 2010 - 5:10:32 PM
     */
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideScore(Invocation inv, Window window, CacheManager cm) {
        try{
            long begin = LogTimeUtils.logBegin();
            User user = UserProvider.getInstance().get(cm);
            final DefaultScoreCache cache = DefaultScoreCacheProvider.getInstance().get(cm, user.getId());
            if (cache == null)
                return;
            inv.addModel("scorecache", cache);
            // 我当前的级别
            int myLevel = cache.getLevel();
            // 历史登陆天数
            int loginDays = cache.getHistoryLoginDays();
            // 连续登陆天数
            int loginCDays = cache.getContinueLoginDays();
            // 当前分数
            int currentScore = cache.getScore();
            // 下一等级分数
            int nextLevelScore = cache.getNextLevelScore();
            boolean showTip = "syshome".equals(CookieManager.getInstance().getCookie(inv.getRequest(), "loginfrom"));
            CookieManager.getInstance().saveCookie(inv.getResponse(), "loginfrom", null);
            inv.addModel("myLevel", myLevel);
            inv.addModel("loginDays", loginDays);
            inv.addModel("loginCDays", loginCDays);
            inv.addModel("currentScore", currentScore);
            inv.addModel("nextLevelScore", nextLevelScore);
            inv.addModel("showTip", showTip);
            if (cache.getAwardCount() > 0) {
                inv.addModel("showAwardCount", true);
                inv.addModel("awardSize", cache.getAwardCount());
            }
            inv.addModel("scoreIcons", ScoreUtil.level2Icons(cache.getLevel(), loginCDays));
            float[] loginscores = getCurrentAndNextLoginScore(cache, loginCDays);
            inv.addModel("loginscore", loginscores[0]);
            inv.addModel("nextloginscore", loginscores[1]);
            inv.addModel("scorelevels", com.xiaonei.wservice.userscore.client.ScoreUtil.getScoreLevel(currentScore));
            LogTimeUtils.logEnd(begin, "guideScore");
        }
        catch(Exception e){
            System.err.println("guideScore:" + e.toString());
        }        
    }

    private float[] getCurrentAndNextLoginScore(ScoreCache sc, int cdays) {
        float[] factors = { 1.1f, 1.2f, 1.4f, 1.6f, 1.8f, 2.0f };
        int vipLevel = sc.getVipLevel();
        vipLevel = vipLevel > 6 ? 6 : vipLevel < 1 ? 1 : vipLevel;
        final float fac = sc.isVipTrue() ? factors[vipLevel - 1] : 1.0f;
        if (cdays < 5) {
            return new float[] { 5 * fac, 7 * fac };
        }
        if (cdays >= 5 && cdays < 30) {
            return new float[] { 7 * fac, 10 * fac };
        } else {
            return new float[] { 10 * fac, 10 * fac };
        }
    }

    /**
     * app Invite
     * 
     * @param inv
     * @param window
     * @param cm
     */    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideAppInvite(Invocation inv, Window window, CacheManager cm) {
        User host = UserProvider.getInstance().get(cm);
        try {
            long begin = LogTimeUtils.logBegin();
            //获得邀请app和邀请人
            RegInviteMap rim = GuideUtil.getAppInviteMap(host);
            if (rim != null) {
                int inviterId = rim.getInviterId();
                User inviter = SnsAdapterFactory.getUserAdapter().get(inviterId);
                inv.addModel("appInviter", inviter);
                int inviteAppId = rim.getAppId();
                if (inviteAppId != 0) {
                    List<AppHomeView> appList = new ArrayList<AppHomeView>();
                    AppMenuItem appMenuItem = AppMenuItemCacheMgr.getInstance().get(inviteAppId);
                    if(appMenuItem!=null){
                        inv.addModel("inviteAppName", appMenuItem.getAppName());
                        inv.addModel("inviteAppIcon", appMenuItem.getAppIcon());
                        inv.addModel("inviteAppUrl", appMenuItem.getSideNavUrl());
                    }                    
                    FootprintNewAdapter adapter = AdapterFactory.getAppFootprintAdapter();
                    FootprintResult result = adapter.getAll(inviter.getId(), 0, 20);
                    if (result != null) {
                        List<FootprintNewView> list = result.getFootprintViews();
                        int appCount = 0;
                        for (FootprintNewView view : list) {
                            AppMenuItem ami= AppMenuItemHome.getInstance().get(view.getGuest());
                            if(ami!=null){
                                Integer appId = ami.getAppId();
                                //过滤掉日志、相册等站内应用和邀请的这个app                    
                                if (appId > 10000 && appId != inviteAppId) {
                                    appCount++;                                    
                                    AppHomeView ahView = new AppHomeView();
                                    ahView.setAppIcon(ami.getAppIcon());
                                    ahView.setAppName(ami.getAppName());
                                    ahView.setAppUrl(ami.getSideNavUrl());
                                    ahView.setAppId(appId);
                                    appList.add(ahView);
                                }
                                if (appCount > 3) {
                                    break;
                                }
                            }
                        }
                    }
                    if (appList == null || appList.size() == 0) {
                        List<Integer> moreAppIds = new ArrayList<Integer>();
                        moreAppIds.add(44097);
                        moreAppIds.add(53429);
                        moreAppIds.add(29127);
                        List<AppHomeView> moreAppList = new ArrayList<AppHomeView>();                             
                        for (int i = 0; i < moreAppIds.size(); i++) {
                            Integer appId = moreAppIds.get(i);
                            AppMenuItem ami = AppMenuItemCacheMgr.getInstance().get(appId);
                            AppHomeView ahv = new AppHomeView();
                            ahv.setAppIcon(ami.getAppIcon());
                            ahv.setAppName(ami.getAppName());
                            ahv.setAppUrl(ami.getSideNavUrl());
                            ahv.setAppId(appId);
                            moreAppList.add(ahv);
                        }
                        inv.addModel("moreAppList", moreAppList);
                    } else {
                        inv.addModel("inviterOtherApps", appList);
                    }
                    LogTimeUtils.logEnd(begin, "guideAppInvite");
                }
            }
        } catch (Exception e) {
            System.err.println("guideAppInvite:" + e.toString());
        }
    }

    /**
     * 人气之星
     * 
     * @param inv
     * @param window
     * @param cm
     */
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideHotStar(Invocation inv, Window window, CacheManager cm) {
        User host = UserProvider.getInstance().get(cm);
        List<HotStarRelation> list = new ArrayList<HotStarRelation>();
        try {
            long begin = LogTimeUtils.logBegin();
            //工作单位
            List<WorkplaceInfo> workInfoList = AdapterFactory.getNetworkAdapter().getWorkplaceInfoList(host.getId()); // 工作资料信息
            if (workInfoList != null) {
                for (int i = 0; i < workInfoList.size(); i++) {
                    List<UserTinyView> univStarList = new ArrayList<UserTinyView>();
                    univStarList = StarDAO.getInstance().getByWorkplaceName(workInfoList.get(i).getWorkplaceName());
                    HotStarRelationImpl hsrl = new HotStarRelationImpl();
                    hsrl.setUnivId(workInfoList.get(i).getWorkplaceId());
                    hsrl.setUnivName(workInfoList.get(i).getWorkplaceName());
                    hsrl.setType("work");
                    hsrl.setList(univStarList);
                    list.add(hsrl);
                }
            }
            //地域
            try {
                RegionInfo regionInfo = DatumInfoClient.getInstance().getRegionInfo(host); // 地域资料信息
                if (regionInfo != null) {
                    List<UserTinyView> univStarList = new ArrayList<UserTinyView>();
                    univStarList = StarDAO.getInstance().getByNetwork(regionInfo.getRegion_id());
                    HotStarRelationImpl hsrl = new HotStarRelationImpl();
                    hsrl.setUnivId(regionInfo.getRegion_id());
                    hsrl.setUnivName(regionInfo.getCity_name());
                    hsrl.setType("location");
                    hsrl.setList(univStarList);
                    list.add(hsrl);
                }
            } catch (Exception e) {
                System.err.println("RegionInfo:" + e.toString());
            }
            //大学
            List<UniversityInfo> univInfoList = AdapterFactory.getNetworkAdapter().getUniversityInfoList(host.getId()); // 大学资料信息
            if (univInfoList != null) {
                for (int i = 0; i < univInfoList.size(); i++) {
                    List<UserTinyView> univStarList = new ArrayList<UserTinyView>();
                    univStarList = StarDAO.getInstance().getByNetwork(univInfoList.get(i).getUniversityId());
                    HotStarRelationImpl hsrl = new HotStarRelationImpl();
                    hsrl.setUnivId(univInfoList.get(i).getUniversityId());
                    hsrl.setUnivName(univInfoList.get(i).getUniversityName());
                    hsrl.setType("univ");
                    hsrl.setList(univStarList);
                    list.add(hsrl);
                }
            }
            //中专技校
            List<CollegeInfo> collegeInfoList = AdapterFactory.getNetworkAdapter().getCollegeInfoList(host.getId()); // 技校资料信息
            if (collegeInfoList != null) {
                for (int i = 0; i < collegeInfoList.size(); i++) {
                    List<UserTinyView> collegeStarList = new ArrayList<UserTinyView>();
                    collegeStarList = StarDAO.getInstance().getByNetwork(collegeInfoList.get(i).getCollegeId());
                    HotStarRelationImpl hsrl = new HotStarRelationImpl();
                    hsrl.setUnivId(collegeInfoList.get(i).getCollegeId());
                    hsrl.setUnivName(collegeInfoList.get(i).getCollegeName());
                    hsrl.setType("college");
                    hsrl.setList(collegeStarList);
                    list.add(hsrl);
                }
            }
            //高中
            List<HighSchoolInfo> highInfoList = AdapterFactory.getNetworkAdapter().getHighSchoolInfoList(host.getId());// 高中资料信息
            if (highInfoList != null) {
                for (int i = 0; i < highInfoList.size(); i++) {
                    List<UserTinyView> highStarList = new ArrayList<UserTinyView>();
                    highStarList = StarDAO.getInstance().getByNetwork(highInfoList.get(i).getHighSchoolId());
                    HotStarRelationImpl hsrl = new HotStarRelationImpl();
                    hsrl.setUnivId(highInfoList.get(i).getHighSchoolId());
                    hsrl.setUnivName(highInfoList.get(i).getHighSchoolName());
                    hsrl.setType("high");
                    hsrl.setList(highStarList);
                    list.add(hsrl);
                }
            }
            //初中
            JuniorHighSchoolInfo juniorInfo = AdapterFactory.getNetworkAdapter().getJuniorHighSchoolInfo(host.getId());//初中资料信息 
            if (juniorInfo != null) {
                List<UserTinyView> juniorStarList = new ArrayList<UserTinyView>();
                juniorStarList = StarDAO.getInstance().getByNetwork(juniorInfo.getJunHighentarySchoolId());
                HotStarRelationImpl hsrl = new HotStarRelationImpl();
                hsrl.setUnivId(juniorInfo.getJunHighentarySchoolId());
                hsrl.setUnivName(juniorInfo.getJunHighentarySchoolName());
                hsrl.setType("junior");
                hsrl.setList(juniorStarList);
                list.add(hsrl);
            }
            String hotStarJson = GuideUtils.getInstance().getJSONForHotStar(list);
            LogTimeUtils.logEnd(begin, "guideHotStar");
            inv.addModel("hotStarJson", hotStarJson);
        } catch (Exception e) {
            System.err.println("guideHotStar:" + e.toString());
        }
    }

    /**
     * 获取UserBasic
     * 
     * @param inv
     * @param window
     * @param cm
     */
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideUserBasic(Invocation inv, Window window, CacheManager cm) {
        UserBasic ub = UserBasicProvider.getInstance().get(cm);
        inv.addModel("guideXFType", "true"); //给刘艳的bar用的        
        inv.addModel("userBasic", ub);
    }

    /***
     * 新手任务
     * 
     * @param inv
     * @param window
     * @param cm
     */
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideNewTask(Invocation inv, Window window, CacheManager cm) {
        User user = UserProvider.getInstance().get(cm);
        long begin = LogTimeUtils.logBegin();
        try {
            List<GuideNewTask> tempTaskList = NewTaskDao.getInstance().getTaskByUser(user.getId());
            if (tempTaskList == null || tempTaskList.size() == 0) {
                NewTaskDao.getInstance().insertNewTask(user.getId(), 1, "", 0, 0, 0);
                NewTaskDao.getInstance().insertNewTask(user.getId(), 2, "", 0, 0, 0);
                NewTaskDao.getInstance().insertNewTask(user.getId(), 3, "", 0, 0, 0);
                NewTaskDao.getInstance().insertNewTask(user.getId(), 4, "", 0, 0, 0);
                NewTaskDao.getInstance().insertNewTask(user.getId(), 5, "", 0, 0, 0);
                tempTaskList = NewTaskDao.getInstance().getTaskByUser(user.getId());
            }
            List<GuideNewTask> gntList = new ArrayList<GuideNewTask>();
            if (tempTaskList != null) {
                for (int i = 0; i < tempTaskList.size(); i++) {
                    GuideNewTask gnt = new GuideNewTask();
                    int taskId = tempTaskList.get(i).getTaskId();
                    gnt.setTaskId(taskId);
                    GuideNewTaskView gntv = TaskHashMap.getInstance().getTaskHashMap().get(taskId);
                    if (gntv != null) {
                        gnt.setTaskName(gntv.getTaskName());
                        gnt.setPraReward(gntv.getPraReward());
                        gnt.setRewardName(gntv.getRewardName());
                        gnt.setRewardScore(gntv.getRewardScore());
                        gnt.setTaskFinished(tempTaskList.get(i).getTaskFinished());
                        gntList.add(gnt);
                    }
                }
            }
            inv.addModel("gntList", gntList);
        } catch (SQLException e) {
            System.err.println("guideNewTask:" + e.toString());
        }
        LogTimeUtils.logEnd(begin, "guideNewTask");
    }
    
    public static final String GUIDE_FRIDBIRTHDAY_KEY = "FriendBirthdayId";

    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideFridBirthday(Invocation inv, Window window, CacheManager cm) {// ok
        long begin = LogTimeUtils.logBegin();
        HandlerContainer.guideFridBirthdayHandler.execute(inv, window, cm);
        LogTimeUtils.logEnd(begin, "guideFridBirthday");
    }

    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideRenrenSchool(Invocation inv, Window window, CacheManager cm) {
        long begin = LogTimeUtils.logBegin();
        User user = UserProvider.getInstance().get(cm);
        int flag = GuideUtil.getExtra_reg_school_renren(user);
        inv.addModel("GUIDE_RENREN_SCHOOL", flag);
        LogTimeUtils.logEnd(begin, "guideRenrenSchool");
    }

    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideGetRequestMap(Invocation inv, Window window, CacheManager cm) {
        long begin = LogTimeUtils.logBegin();
        User user = UserProvider.getInstance().get(cm);
        int offset = 0;
        int limit = -1;
        SnsBuddyApplyCacheWithTotalN buddyApplyCacheWithTotal = (SnsBuddyApplyCacheWithTotalN) SnsAdapterFactory
                .getBuddyApplyCacheAdapterN().getBuddyApplyCacheWithTotal(user.getId(), offset, limit);
        if ((buddyApplyCacheWithTotal != null)
                && !buddyApplyCacheWithTotal.getBuddyApplyCacheN().isEmpty()
                && (buddyApplyCacheWithTotal.getTotalCount() != 0)) {
            List<SnsBuddyApplyCacheDataN> lst = buddyApplyCacheWithTotal.getBuddyApplyCacheN();// 取得申请
            List<Integer> idList = new ArrayList<Integer>();
            for (SnsBuddyApplyCacheDataN apply : lst) {
                idList.add(apply.getApplicant());
            }
            inv.addModel(GUIDE_BUDDY_APPLY, idList);
            inv.addModel("buddyApplyCount", idList.size());
        }
        LogTimeUtils.logEnd(begin, "guideGetRequestMap");
    }

    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideAppFootprint(Invocation inv, Window window, CacheManager cm) {
        long begin = TimeCostUtil.logBegin();
        try{
            User host = UserProvider.getInstance().get(cm);
            List<AppHomeView> appList = new ArrayList<AppHomeView>();
            FootprintNewAdapter adapter = AdapterFactory.getAppFootprintAdapter();
            if(adapter!=null){
            	FootprintResult result = adapter.getAll(host.getId(), 0, -1);
            	if(result!=null){
            		List<FootprintNewView> list = result.getFootprintViews();
                    int appCount=0;
                    if (list != null) {
                        for (int i = 0; i < list.size(); i++) {
                            if(appCount>3){
                                break;
                            }
                            Integer appId = list.get(i).getGuest();
                            if (appId > 10000 || appId == 17) {  
                                AppMenuItem ami=AppMenuItemCacheMgr.getInstance().get(appId);
                                if(ami!=null){                        
                                // 过滤掉日志、相册等站内应用(17是三国风云)                                                  
                                    AppHomeView ahView = new AppHomeView();
                                    ahView.setAppIcon(ami.getAppIcon());
                                    ahView.setAppName(ami.getAppName());
                                    ahView.setAppUrl(ami.getSideNavUrl());
                                    appList.add(ahView);
                                    appCount++;
                                }
                            }                
                            
                        }
                        inv.addModel("guideAppFootprint", appList);
                    }
            	}                
            }
        }
        catch(Exception e){
            System.err.println("guideAppFootprint:" + e.toString());
        }        
        LogTimeUtils.logEnd(begin, "guideAppFootprint");
    }	
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideGetMemCache(Invocation inv, Window window, CacheManager cm) {
        long begin = TimeCostUtil.logBegin();
        User host = UserProvider.getInstance().get(cm);
        try {
            MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);            
            if ("first".equals(mem.get(GuideUtils.FIRST_TIME_GUIDE + host.getId()))) {
                inv.addModel("firstArriveGuide", true);
                if (!"clicked".equals(mem.get("guidePopInfoUpload_" + host.getId()))) {
                	int type = 0;
                	try {
						type = StepProcessLogic.getInstance().getGuideUserType(host);
					} catch (Exception e) {
						System.err.println(" host:"+host.getId()+" e:"+e.toString());
					}
                	if(type == IGuideUserLeadType.Lead_AAI){
                		inv.addModel("popInfoUpload", true);
                	}
                }
                if (!"clicked".equals(mem.get("guidePopUpload_" + host.getId()))) {
                	int type = 0;
                	try {
						type = StepProcessLogic.getInstance().getGuideUserType(host);
					} catch (Exception e) {
						System.err.println(" host:"+host.getId()+" e:"+e.toString());
					}
                	if(type == IGuideUserLeadType.Lead_AAF){
                		inv.addModel("guidePopUpload", true);
                	}                    
                }
                //inshion 2010-11-05 09:49:36 头像
                if(!GuideUtil.hasUploaded(host)){//没有头像
                	int step = StepStatusLogic.getStepStatus(host.getId());
                	if((step & StepStatusLogic.STEP_UPLOAD_HEAD) == 0){
                		inv.addModel("needOpenPortrait",true);
                	}
                }
            }
            if (!"clicked".equals(mem.get("guideHighschoolInvite_" + host.getId()))) {
                inv.addModel("disGuideHighschoolInvite", true);
            }
            if (!"clicked".equals(mem.get("guideInviteApp_" + host.getId()))) {
                inv.addModel("disGuideInviteApp", true);
            }            
            inv.addModel("defaultTrunkValue", mem.get("defaultKeyTrunk" + host.getId()));
        } catch (Exception e) {
            e.printStackTrace();
        }
        LogTimeUtils.logEnd(begin, "guideGetMemCache");
    }

    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideSendHotNews(Invocation inv, Window window, CacheManager cm) {
        long begin = TimeCostUtil.logBegin();
        User host = UserProvider.getInstance().get(cm);
        //发送热门新鲜事
        MemCacheManager mcm = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);
        Long memValue = Calendar.getInstance().getTimeInMillis() / 1000;//单位:秒
        Long expiry = 60 * 60l;//一小时,memcache的expiry不靠谱，自己实现一个
        if (mcm.get("guideNewHotShareFeed" + host.getId()) == null || (memValue - (Long) mcm.get("guideNewHotShareFeed" + host.getId()) > (expiry))) {
            int MAX = 22;            
            List<HotShare> hotShareList=shareHotHome.findFixedHotShare();  
            if(hotShareList!=null){                
                GuideLogger.printLog("hotShareList.size():" + host.getId() + ":" + hotShareList.size()); 
            }
            else{
                GuideLogger.printLog("hotShareList is null"); 
            }
            if (hotShareList != null && hotShareList.size()!= 0) {
                mcm.set("guideNewHotShareFeed" + host.getId(), memValue, (int) MemCacheKeys.hour * 1);
                FeedBuilder xbuilder = new FeedBuilder.Builder(FeedDefinition.EDM_HOTFEED, 
                        host.getId(), "", FeedDefinition.EDM_HOTFEED, FeedDefinition.EDM_HOTFEED).merge(FeedDefinition.EDM_HOTFEED).build();                      
                for (int i = 0; i < hotShareList.size() && i < 2; i++) {
                    HotShare share = hotShareList.get(i);
                    xbuilder.put("videoImage" + i, share.getThumbUrl());
                    xbuilder.put("videoID" + i, share.getShareId());                    
                    xbuilder.put("videoOwnerID" + i, share.getUserId());
                    xbuilder.put("videoOwnerName" + i, share.getUserName());
                    String title = StringUtils.substring(share.getTitle(), 0, MAX);
                    xbuilder.put("videoTitle" + i, title);
                }
                if(hotShareList.size()>=3){
                    for (int i = 2; i < hotShareList.size() && i < 4; i++) {
                        HotShare share = hotShareList.get(i);  
                        String photoLink=OpiConstants.urlShare+"/share/GetShare.do";
                        if(share!=null){
                            photoLink+="?id="+share.getShareId()+"&owner="+share.getUserId();
                            xbuilder.put("photoLink" + (i-2), photoLink);
                            String title = StringUtils.substring(share.getTitle(), 0, MAX);
                            xbuilder.put("photoTitle" + (i-2), title);
                        } 
                    }
                }     
                if(hotShareList.size()>=5){
                    for (int i = 4; i < hotShareList.size() && i < 6; i++) {
                        HotShare share = hotShareList.get(i);
                        String blogLink=OpiConstants.urlShare+"/share/GetShare.do";
                        if(share!=null){
                            blogLink+="?id="+share.getShareId()+"&owner="+share.getUserId();
                            xbuilder.put("blogLink" + (i-4), blogLink);                       
                            String title = StringUtils.substring(share.getTitle(), 0, MAX);
                            xbuilder.put("blogTitle" + (i-4), title);
                        } 
                    }  
                }                              
                xbuilder.put("configID", 17);
                xbuilder.put("userID", host.getId());
                xbuilder.setPrivate(host.getId());
                try {
                    xbuilder.publish();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
        LogTimeUtils.logEnd(begin, "guideSendHotNews");
    }

    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void getFriendListWithinTime(Invocation inv, Window window, CacheManager cm) {
        try {
            long begin = LogTimeUtils.logBegin();
            int userId = UserProvider.getInstance().get(cm).getId();//用户id
            DefaultScoreCache scoredata = ScoreCacheAdapter.getInstance().getScoreData(userId);//取用户积分对象
            if(scoredata!=null){
            	 List<Boolean> activedays = scoredata.getActiveDays();//从积分对象中取活跃天数
            	 int absentcount = 1;
                 //开始统计没上线的天数
                 for (; absentcount <= 29; ++absentcount) {
                	 if(activedays!=null){
                		 if (activedays.get(absentcount)) {
                             break;
                         }
                	 }                     
                 }
                 //至此absentcount表示上次上线那天距离今天的天数;
                 Calendar c = Calendar.getInstance();
                 c.add(Calendar.DAY_OF_MONTH, -absentcount);
                 c.set(Calendar.HOUR_OF_DAY, 0);
                 c.set(Calendar.MINUTE, 0);
                 c.set(Calendar.SECOND, 0);
                 c.set(Calendar.MILLISECOND, 0);
                 long timelastzero = c.getTimeInMillis();//表示上次登录那天的0点
                 long timenow = System.currentTimeMillis();//当前时间      
                 //用以上两个时间取好友列表
                 List<DefaultBuddyByAddTime> newfriends = BuddyByAddTimeCacheAdapter.getInstance().getFriendListWithinTime(userId, timelastzero, timenow);
                 List<Integer> userIds = new ArrayList<Integer>();
                 if(newfriends!=null){
                	 for (DefaultBuddyByAddTime one : newfriends) {
                         userIds.add(one.getUserId());
                     }  
                 }                           
                 if(userIds!=null && userIds.size()!=0){
                     String json = GuideUtils.getInstance().getJSONForGetFriendListWithinTime(userIds);
                     inv.addModel("friendListWithinTime", json);
                 }        
            } 
            LogTimeUtils.logEnd(begin, "getFriendListWithinTime");
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideIsFullInfo(Invocation inv, Window window, CacheManager cm) {
        try {
            long begin = LogTimeUtils.logBegin();
            User user = UserProvider.getInstance().get(cm);
            boolean flag = GuideUtils.getInstance().isFullInfo(user);
            inv.addModel("guideIsFullInfo", flag);
            LogTimeUtils.logEnd(begin, "guideIsFullInfo");
        } catch (Exception e) {
            System.err.println("guideIsFullInfo:" + e.toString());
        }
    }

    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideGetBlogCount(Invocation inv, Window window, CacheManager cm) {
        try {
            long begin = LogTimeUtils.logBegin();
            User user = UserProvider.getInstance().get(cm);
            int guideBlogCount = BlogHome.getInstance().getBlogCountForProfile(user.getId(),RelationDef.uuSelf);
            inv.addModel("guideBlogCount", guideBlogCount);
            LogTimeUtils.logEnd(begin, "guideBlogCount");
        } catch (Exception e) {
            System.err.println("guideBlogCount:" + e.toString());
        }
    }

    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideGetAlbumCount(Invocation inv, Window window, CacheManager cm) {
        try {
            GuideLogger.printLog("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 到这里了");
            long begin = LogTimeUtils.logBegin();
            User user = UserProvider.getInstance().get(cm);
           // final ServiceFactory fact = ServiceFactories.getFactory();
            //AlbumListService albumListService = fact.getService(AlbumListService.class);
            //final ServiceFuture<Integer> serviceFuture=albumListService.getAlbumsCount(user.getId(), user.getId(),false);
            int guideAlbumCount = AlbumHome.getInstance().getAlbumCount4Profile(user.getId(), user.getId(), Relation.uuSelf);
            //inv.addModel("guideAlbumCountbiaoshi",  ServiceFutureHelper.execute(serviceFuture, 3000));
            inv.addModel("guideAlbumCountbiaoshi",  5);
            LogTimeUtils.logEnd(begin, "guideGetAlbumCount");
        } catch (Exception e) {
            System.err.println("guideGetAlbumCount:" + e.toString());
        }
    }

    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideIsMSNInvited(Invocation inv, Window window, CacheManager cm) {
        try {
            long begin = LogTimeUtils.logBegin();
            User user = UserProvider.getInstance().get(cm);
            boolean flag = GuideLogic.getInstance().isMSNInvited(user.getId());
            inv.addModel("isMSNInvited", flag);
            LogTimeUtils.logEnd(begin, "isMSNInvited");
        } catch (Exception e) {
            System.err.println("isMSNInvited:" + e.toString());
        }
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideBarTreasure(Invocation inv, Window window, CacheManager cm) {
        try {
            long begin = LogTimeUtils.logBegin();
            User user = UserProvider.getInstance().get(cm);
            String xxx="reg_10treasure";
            int flag=GuideUtil.getExtra_XXX(user, xxx);
            boolean barTreasure=false;
            if(flag==1){
                barTreasure=true;                
            }      
            inv.addModel("BarTreasure", barTreasure);
            LogTimeUtils.logEnd(begin, "guideBarTreasure");
        } catch (Exception e) {
            System.err.println("guideBarTreasure:" + e.toString());
        }
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideRightHotStar(Invocation inv, Window window, CacheManager cm) {// ok
        try{
            long begin = LogTimeUtils.logBegin();
            HandlerContainer.gudieHotStarHandler.execute(inv, window, cm);
            LogTimeUtils.logEnd(begin, "guideRightHotStar");
        }
        catch(Exception e){
            System.err.println("guideRightHotStar:" + e.toString());
        }        
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideIsXinSheng2010(Invocation inv, Window window, CacheManager cm) {
        try {
            long begin = LogTimeUtils.logBegin();
            User user = UserProvider.getInstance().get(cm);
            List<UniversityInfo> univInfoList = AdapterFactory.getNetworkAdapter().getUniversityInfoList(user.getId()); // 大学资料信息 
            boolean isXinSheng2011 = false;
            if (univInfoList != null) {
				for (UniversityInfo universityInfo : univInfoList) {
					int enrollYear = universityInfo.getEnrollYear();
					GuideLogger.printLog(" hostid:" + user.getId() + " enrollYear:" + enrollYear);
					if (enrollYear == 2011) {
						isXinSheng2011 = true;
						break;
					}
				}
			}
            inv.addModel("isXinSheng2011", isXinSheng2011);
            LogTimeUtils.logEnd(begin, "isXinSheng2011 is:"+ isXinSheng2011);
        } catch (Exception e) {
            System.err.println("isXinSheng2011:" + e.toString());
        }
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideZhengZhouGroup(Invocation inv, Window window, CacheManager cm) {
        User user = UserProvider.getInstance().get(cm);
        UserBasic ub = UserBasicProvider.getInstance().get(cm);         
        String cityCode=ub.getCityCode();
        Integer regionId=0;
        try {
            RegionInfo regionInfo = DatumInfoClient.getInstance().getRegionInfo(user);
            if(regionInfo!=null){
                regionId=regionInfo.getRegion_id();
            }            
        } catch (Exception e) {
            System.err.println("guideZhengZhouGroup:Exception:"+e.toString());
        }
        //郑州的城市号和网络号
        if("4101".equals(cityCode) || regionId==60018001){
            inv.addModel("guideZhengZhouGroup", true);
            String url = "xoa://forum.ugc.xoa.renren.com/forum/100542";  //定位xoa服务中的资源的url
            Method method = Method.get(url);    //获取一个对url指定的资源进行GET操作的method
            //设置parameter
            method.setParam("biz", "1");   //add a parameter with the biz tag             
            XoaClient client = new XoaClient(); //构造一个XoaClient, XoaClient中的每个方法都是线程安全的，所以建议尽量复用同一个XoaClient实例。             
            /////////////异步调用方式//////////////
            try {
                Future<XoaResponse> future = client.submit(method); //提交XoaMethod
                //do something else
                XoaResponse response = future.get();    //从Future拿到XoaResponse             
                Forum forum = response.getContentAs(Forum.class);  //将XoaResponse的content还原为blog对象
                Integer memberCount=forum.getMemberCount();
                inv.addModel("memberCount", memberCount);
            } catch (InterruptedException e) {
                System.err.println("guideZhengZhouGroup:InterruptedException:"+e.toString());
            } catch (ExecutionException e) {
                System.err.println("guideZhengZhouGroup:ExecutionException:"+e.toString());
            }
        }
        
    }
    
    @Autowired
    private AdminService adminService;
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideManagePages(Invocation inv, Window window, CacheManager cm) {// ok
        User host = UserProvider.getInstance().get(cm);
        Integer limit=2;
        List<Page> list=new ArrayList<Page>();
        if(adminService!=null){
        	try{
        		list=adminService.getMngLimitPages(host.getId(), limit);
        	}
        	catch(Exception e){
        		System.err.println("guideManagePages"+e.toString());
        	}        	
        }
        inv.addModel("managePageList", list);
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideManageLoverPages(Invocation inv, Window window, CacheManager cm) {// ok
        User host = UserProvider.getInstance().get(cm);        
        List<Page> list=new ArrayList<Page>();
        if(adminService!=null){
        	try{
        		list=adminService.getHomeMyManagedLoverPages(host.getId());
        	}
        	catch(Exception e){
        		System.err.println("guideManageLoverPages"+e.toString());
        	}        	
        }
        inv.addModel("manageLoverPageList", list);
    }  

    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideIsShowNewHeaderTip(Invocation inv, Window window, CacheManager cm) {
        try {
            int userId = UserProvider.getInstance().get(cm).getId();
            MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_home);
            Boolean isShowNewHeaderTip=(mem.get("newheader_show_tip_" + userId) == null);
            inv.addModel("isShowNewHeaderTip", isShowNewHeaderTip); 
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideGetCommonMemcache(Invocation inv, Window window, CacheManager cm) {
    	 long begin = TimeCostUtil.logBegin();
         User host = UserProvider.getInstance().get(cm);
         try {
             MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_dip);            
             if (!"closed".equals(mem.get("appInvite" + host.getId()))) {
                 inv.addModel("disAppInvite", true);
             }    
             if (!"closed".equals(mem.get("homeLoverSpace" + host.getId()))) {
                 inv.addModel("disLoverSpace", true);
             }
         } catch (Exception e) {
        	 System.err.println("guideGetCommonMemcache"+e.toString());
         }
         LogTimeUtils.logEnd(begin, "guideGetCommonMemcache");
    }   
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideNotEnoughCompletion(Invocation inv, Window window, CacheManager cm) {
    	long begin = TimeCostUtil.logBegin();
    	List<Integer> list=new ArrayList<Integer>();
    	list.add(190136739);
    	list.add(223500512);
    	list.add(232686677);
    	list.add(234800486);
    	inv.addModel(GUIDE_NOT_ENOUGH_COMPLETION, list);  
    	LogTimeUtils.logEnd(begin, "guideNotEnoughCompletion");
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideGetTargetUserFriendCount(Invocation inv, Window window, CacheManager cm) {
    	long begin = TimeCostUtil.logBegin();
    	User owner = (User) ObjectProvider.getInstance().get(cm, GuideUtils.AJAX_GUIDE_USER);
    	Integer friendCount=FriendsHome.getInstance().getFriendCount(owner);
    	inv.addModel("friendCount", friendCount);
    	LogTimeUtils.logEnd(begin, "guideGetTargetUserFriendCount");
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideGetTargetUserAlbumCount(Invocation inv, Window window, CacheManager cm) {
    	long begin = TimeCostUtil.logBegin();
    	User owner = (User) ObjectProvider.getInstance().get(cm, GuideUtils.AJAX_GUIDE_USER);
    	//Integer albumCount=AlbumHome.getInstance().getCountByUserCommon(owner.getId(),RelationDef.uuNoPath);
    	 final ServiceFactory fact = ServiceFactories.getFactory();
    	AlbumListService albumListService=fact.getService(AlbumListService.class);
    	int albumCount=ServiceFutureHelper.execute(albumListService.getAlbumsCount(owner.getId(),owner.getId(),false),80);
    	inv.addModel("albumCount", albumCount);
    	LogTimeUtils.logEnd(begin, "guideGetTargetUserAlbumCount");
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideGetTargetUserBlogCount(Invocation inv, Window window, CacheManager cm) {
    	long begin = TimeCostUtil.logBegin();
    	User owner = (User) ObjectProvider.getInstance().get(cm, GuideUtils.AJAX_GUIDE_USER);
    	Integer blogCount=BlogHome.getInstance().getBlogCountForProfile(owner.getId(), RelationDef.uuFriend);
    	inv.addModel("blogCountkk", blogCount);
    	LogTimeUtils.logEnd(begin, "guideGetTargetUserBlogCount");
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideGetTargetUserDoing(Invocation inv, Window window, CacheManager cm) {
    	long begin = TimeCostUtil.logBegin();
    	User owner = (User) ObjectProvider.getInstance().get(cm, GuideUtils.AJAX_GUIDE_USER);    	
		try {
			DoingHome dh=DoingHome.getInstance();
            Integer doingCount=dh.getDoingCount(owner.getId());
			Doing doing = dh.getUserLastDoing(owner.getId());
			long doingId=0l;
	        String doingContent="";
	        String updateTitle = "上一条";
	        String htmlContentForPubLisher="";
	        if(doing != null){
	            doingId=doing.getId();
	            doingContent=doing.getHtmlContent();    
	            htmlContentForPubLisher = doing.getHtmlContentForPubLisher();
	            Long doingtime = doing.getDoTime().getTime();
	            long dTime;
	            if (doingtime != null) {
	                try {
	                    dTime = Long.valueOf(doingtime);
	                    Calendar cal = Calendar.getInstance();
	                    long doneMinutes = cal.getTime().getTime() - dTime;
	                    if (doneMinutes < 0) {
	                        doneMinutes = 0;
	                    }
	                    doneMinutes = doneMinutes / (1000 * 60);
	                    String ugcStatus = "";                        
	                    ugcStatus = doing.getHtmlContentForPubLisher();
	                    if (ugcStatus != null && ugcStatus.length() > 0) {
	                        if (doneMinutes < 1) {
	                            updateTitle = "刚刚更新";
	                        } else if (doneMinutes < 60) {
	                            updateTitle = doneMinutes + "分钟前";
	                        } else if (doneMinutes < 1440) {
	                            updateTitle = doneMinutes / 60 + "小时前";
	                        } else if (doneMinutes < 4320) {
	                            updateTitle = doneMinutes / 1440 + "天前";
	                        } else {
	                            updateTitle = "n天前";
	                        }
	                    } else {
	                        updateTitle = "你在做什么";
	                    }                        
	                } catch (Exception e) {
	                    e.printStackTrace();
	                }
	            }
	        }
	        inv.addModel("doingCountkku", doingCount);
	        inv.addModel("doingId", doingId);
	        inv.addModel("doingContent", doingContent);
	        inv.addModel("updateTitle", updateTitle);
	        inv.addModel("htmlContentForPubLisher", htmlContentForPubLisher);
		} catch (SQLException e1) {
			e1.printStackTrace();
		}         
    	LogTimeUtils.logEnd(begin, "guideGetTargetUserFriendCount");
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideIsFriendWithTargetUser(Invocation inv, Window window, CacheManager cm) {
    	long begin = TimeCostUtil.logBegin();
    	User host = UserProvider.getInstance().get(cm);
    	User owner = (User) ObjectProvider.getInstance().get(cm, GuideUtils.AJAX_GUIDE_USER);    	
    	try {
			Boolean isFriend=FriendsHome.getInstance().isFriend(host.getId(), owner.getId());
			inv.addModel("isFriend", isFriend);
		} catch (Exception e) {
			e.printStackTrace();
		}
    	LogTimeUtils.logEnd(begin, "guideGetTargetUserFriendCount");
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideShowhTargetUserProfile(Invocation inv, Window window, CacheManager cm) {
    	long begin = TimeCostUtil.logBegin();
    	User host = UserProvider.getInstance().get(cm);
    	User owner = (User) ObjectProvider.getInstance().get(cm, GuideUtils.AJAX_GUIDE_USER);    	
    	License license = null;// 许可对象，是否可以访问主页的许可           
        int path = RelationDef.uuNoPath;// 计算path, uuFriend
        int sourceControl = 0;// 被访问者的基本设置
        UserConfigInfo userUci = null;
        try {
            userUci = SnsAdapterFactory.getUserConfigInfoAdapter().getUserConfig(owner.getId());
        } catch (Exception e) {
            e.printStackTrace();
        }
        try {
            // 登录情况下，可以判断 2 人关系 path
            if (userUci != null) {
                sourceControl = userUci.getBasicConfig();
            }
            // 采用新的封装
            license = SpaceHome.getInstance().getVisitPermission(host, owner, sourceControl); 
            path = license.getPath();
        } catch (Exception e) {
            e.printStackTrace();
        }
        /*
         * 个人主页的隐私，本业务用于状态是否可回复，日志相册是否可查看
         */
        Boolean showProfile=true;
        if ((license == null) || !license.hasRight()){
            showProfile=false;
        }    
        ProfilePrivacy pp = null;            
        if (userUci != null) {
            int authValue = owner.getAuth();
            if ((userUci.getProfilePrivacy() == null) || (userUci.getProfilePrivacy().trim().length() == 0)) {
                pp = new ProfilePrivacy(path, authValue);
            } else {
                pp = new ProfilePrivacy(path, userUci.getProfilePrivacy(), authValue);
            }
        }
        if (pp != null){
        	pp.setVisitor(owner.getId());  
        	inv.addModel("showGossipBoard", pp.getShowGossipBoard());
        }            
        inv.addModel("showProfilekku", showProfile);
    	LogTimeUtils.logEnd(begin, "guideGetTargetUserFriendCount");
    }
    
	@MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideGetFeedView(Invocation inv, Window window, CacheManager cm) {
    	long begin = TimeCostUtil.logBegin();
    	User host = UserProvider.getInstance().get(cm);
        try {
            List<String> list = null;
            int realUserId = host.getId();
            String savedFeedType = "hot";
            String cookieName = "feedType";
            try {
                String value = CookieManager.getInstance().getCookie(inv.getRequest(), cookieName);
                if (value == null) {
                    // 没有cookie信息
                    list = FeedViewAdapter.getInstance().getHot(host.getId(), host.getName(), 0, 0, IFeedRetriever.HOME_LIMIT);
                   // GuideLoggerCountFeed.printLog("guideGetFeedView01 userid:"+host.getId()+" feedcount:"+
                    //list.size()+" totalcount:"+FeedRetriever2.getInstance().getUnviewedCount(host.getId()));
                    guideRecommendLogic.insertGuideUserFeedCount(host.getId(), list.size(), "guide_home");
                } else {
                    String[] entry = value.split("_");
                    if (entry != null && entry.length == 2) {
                        int cookieUserId = Integer.parseInt(entry[0]);
                        String cookieFeedType = entry[1];
                        if (cookieUserId == realUserId && "live".equals(cookieFeedType)) {
                            // 相同用户登录并且feed类型为live
                            // 下面的代码很难看，因为showLiveByUGC返回的是MAP
                            Map<String, Object> feedMap = DisplayFeedFactory.getInstance().showLiveByUGC(host.getId(),host.getName(), 0, 0, IFeedRetriever.HOME_LIMIT);
                            String resultFeedString = feedMap.get("content").toString();
                            list = new ArrayList<String>();
                            list.add(resultFeedString);
                            savedFeedType = "live";
                            //GuideLoggerCountFeed.printLog("guideGetFeedView02 userid:"+host.getId()+" feedcount:"
                            //+feedMap.get("count")+" totalcount:"+FeedRetriever2.getInstance().getUnviewedCount(host.getId()));
                            guideRecommendLogic.insertGuideUserFeedCount(host.getId(), (Integer)feedMap.get("count"), "guide_home");
                        } else {
                            // 不同用户登录或者feed类型为hot
                            list = FeedViewAdapter.getInstance().getHot(host.getId(),host.getName(), 0, 0, IFeedRetriever.HOME_LIMIT);
                            //GuideLoggerCountFeed.printLog("guideGetFeedView03 userid:"+host.getId()+" feedcount:"
                            //+list.size()+" totalcount:"+FeedRetriever2.getInstance().getUnviewedCount(host.getId()));
                            guideRecommendLogic.insertGuideUserFeedCount(host.getId(), list.size(), "guide_home");
                        }
                    } else {
                        // cookie格式错误
                        list = FeedViewAdapter.getInstance().getHot(host.getId(), host.getName(),0, 0, IFeedRetriever.HOME_LIMIT);
                        //GuideLoggerCountFeed.printLog("guideGetFeedView04 userid:"+host.getId()+" feedcount:"+
                        //list.size()+" totalcount:"+FeedRetriever2.getInstance().getUnviewedCount(host.getId()));
                        guideRecommendLogic.insertGuideUserFeedCount(host.getId(), list.size(), "guide_home");
                    }
                }
            } catch (Exception e) {
                // 出现未知异常
                list = FeedViewAdapter.getInstance().getHot(host.getId(), host.getName(), 0, 0, IFeedRetriever.HOME_LIMIT);
                //GuideLoggerCountFeed.printLog("guideGetFeedView05 userid:"+host.getId()+" feedcount:"+
                //list.size()+" totalcount:"+FeedRetriever2.getInstance().getUnviewedCount(host.getId()));
                guideRecommendLogic.insertGuideUserFeedCount(host.getId(), list.size(), "guide_home");
            }
            int cookieTime = 60 * 60 * 24 * 30;
            CookieManager.getInstance().saveCookie(inv.getResponse(), cookieName, realUserId + "_" + savedFeedType, cookieTime, "/", "guide.renren.com");
            inv.addModel("savedFeedType", savedFeedType);
            if (list != null) {
                StringBuffer feedString = new StringBuffer();
                for (int i = 0; i < list.size(); i++) {
                    feedString.append(list.get(i));
                }
                int feedCount = list.size();
                inv.addModel("userFeed", feedString.toString());
                inv.addModel("feedStart", "1");
                inv.addModel("feedEnd", feedCount);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        UserConfigInfo config = UserConfigInfoProvider.getInstance().get(cm, host.getId());
        inv.addModel("feedStyle", UserConfigWantSeeCssMgr.isHomeFeedSimpleView(config));
        LogTimeUtils.logEnd(begin, "guideGetFeedView:"+host.getId());
    }
    
    /**
     * stage==10的人气之星模块
     * @param inv
     * @param window
     * @param cm
     */
    @MethodThroughput(maxConcurrent = 50, instruction = "")
	public void guideGetPopUserMiddleSchoolList(Invocation inv, Window window, CacheManager cm) {
		boolean ownSchoolStar = true; // 自己的学校是否有有人气之星
		int enrollYear = -1; // 最后取最大的中学入学年
		int limit = 8;
		int middleSchoolId = 0;
		long begin = TimeCostUtil.logBegin();
		String middleSchoolName = "";
		User host = UserProvider.getInstance().get(cm);
		List<HighSchoolInfo> hsInfoList = AdapterFactory.getNetworkAdapter().getHighSchoolInfoList(host.getId()); // 高中资料信息
		if (hsInfoList != null) {
			for (HighSchoolInfo hsi : hsInfoList) {
				if (hsi.getEnrollYear() > enrollYear) {
					enrollYear = hsi.getEnrollYear();
					middleSchoolId = hsi.getHighSchoolId();
					middleSchoolName = hsi.getHighSchoolName();
				}
			}
		}
		if ("".equals(middleSchoolName)) {
			JuniorHighSchoolInfo junInfo = AdapterFactory.getNetworkAdapter().getJuniorHighSchoolInfo(host.getId()); // 初中资料信息
			if (junInfo != null) {
				enrollYear = junInfo.getJunHighentarySchoolYear();
				middleSchoolId = junInfo.getJunHighentarySchoolId();
				middleSchoolName = junInfo.getJunHighentarySchoolName();
			}
		}
		GuideLogger.printLog(host.getId() + ":my middleSchoolId:" + middleSchoolId);
		GuideLogger.printLog(host.getId() + ":my middleSchoolName:" + middleSchoolName);
		// 构造一个自己学校的对象
		GuideMiddleSchool myGms = new GuideMiddleSchool();
		myGms.setMiddleSchoolId(middleSchoolId);
		myGms.setMiddleSchoolName(middleSchoolName);
		GuideCity gc = GuideLogic.getInstance().getCityCodeByMiddleSchoolId(middleSchoolId);
		String cityCode = "";
		String cityName = "";
		// pop_user_middle_school_map表中可能有对应不上的信息
		if (gc != null) {
			cityCode = gc.getCityCode();
			cityName = gc.getCityName();
		}
		GuideLogger.printLog(host.getId() + ":cityCode:" + cityCode);
		GuideLogger.printLog(host.getId() + ":cityName:" + cityName);
		inv.addModel("cityName", cityName);
		//要展示的学校（有可能是城市）
		List<Integer> schoolIdList = new ArrayList<Integer>();
		List<GuideMiddleSchool> gmsList = null;//new ArrayList<GuideMiddleSchool>();
		List<Integer> userIdList = GuideLogic.getInstance().getPopUserMiddleSchoolList(middleSchoolId, enrollYear, limit);
		if (userIdList == null || userIdList.size() == 0) {
			ownSchoolStar = false;
		}
		MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);
		int cityStarLimit = 8;
		GuideLogger.printLog(host.getId() + ":mem.get(GuideUtils.FIRST_TIME_GUIDE + host.getId()):" + mem.get(GuideUtils.FIRST_TIME_GUIDE + host.getId()));
		if ("first".equals(mem.get(GuideUtils.FIRST_TIME_GUIDE + host.getId()))) {
			GuideLogger.printLog(host.getId() + ":first:" + middleSchoolId);
			if (!ownSchoolStar) {
				userIdList = GuideLogic.getInstance().getUserStarIdsByCityCode(cityCode, cityStarLimit);
				middleSchoolName = cityName;
			}
			mem.delete(GuideUtils.FIRST_TIME_GUIDE + host.getId());
			GuideLogger.printLog(host.getId() + ":after delete mem:" + mem.get(GuideUtils.FIRST_TIME_GUIDE + host.getId()));
		} else {
			int popUserCount = 5;
			gmsList = GuideLogic.getInstance().getMiddleSchoolIdsByCityCode(cityCode, popUserCount);
			if (gmsList != null) {
				for (GuideMiddleSchool gms : gmsList) {
					schoolIdList.add(gms.getMiddleSchoolId());
				}
			}
			// 如果取出来的学校不包含自己的学校，且自己的学校有人气之星
			if (!schoolIdList.contains(middleSchoolId) && ownSchoolStar) {
				// 把自己的学校对象加进来
				gmsList.add(myGms);
			}
			Random random = new Random();
			if (gmsList != null) {
				int seed = gmsList.size() + 1;
				// 要显示学校（地区）的序号
				int displayId = random.nextInt(seed);
				// 如果是最后一个，就显示地区，统一用gmsList来判断数组的位置
				if (displayId == gmsList.size()) {
					middleSchoolName = cityName;
					userIdList = GuideLogic.getInstance().getUserStarIdsByCityCode(cityCode, cityStarLimit);
				} else if (gmsList.size() > 0) {
					int schoolId = gmsList.get(displayId).getMiddleSchoolId();
					middleSchoolName = gmsList.get(displayId).getMiddleSchoolName();
					GuideLogger.printLog(host.getId() + ":display schoolId:" + schoolId);
					GuideLogger.printLog(host.getId() + ":display middleSchoolName:" + middleSchoolName);
					userIdList = GuideLogic.getInstance().getPopUserMiddleSchoolList(schoolId, enrollYear, limit);
				}
			}
		}
		inv.addModel("middleSchoolName", middleSchoolName);
		if (userIdList != null && userIdList.size() != 0) {
			List<WUserCache> userCacheList = GuideUtils.getInstance().getUserCacheListOrderByOriginal(userIdList);
			if (userCacheList != null && userCacheList.size() != 0) {
				inv.addModel("userStarList", userCacheList);
			}
		}
		LogTimeUtils.logEnd(begin, "guideGetPopUserMiddleSchoolList");
	}
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideGetTargetUserMiddleSchool(Invocation inv, Window window, CacheManager cm) {
    	long begin = TimeCostUtil.logBegin();
    	User owner = (User) ObjectProvider.getInstance().get(cm, GuideUtils.AJAX_GUIDE_USER);    
    	String middleSchoolName= "";
    	int enrollYear = 0;
    	List<HighSchoolInfo> hsInfoList = AdapterFactory.getNetworkAdapter().getHighSchoolInfoList(owner.getId()); // 高中资料信息          
    	if(hsInfoList != null ){
    		for(HighSchoolInfo hsi : hsInfoList){
    			if(hsi.getEnrollYear() > enrollYear){
    				enrollYear = hsi.getEnrollYear();
    				middleSchoolName = hsi.getHighSchoolName();
    			}
    		}
    	}
    	if("".equals(middleSchoolName)){
    		JuniorHighSchoolInfo junInfo = AdapterFactory.getNetworkAdapter().getJuniorHighSchoolInfo(owner.getId()); // 初中资料信息
    		if(junInfo!=null){
    			middleSchoolName = junInfo.getJunHighentarySchoolName();
    			enrollYear = junInfo.getJunHighentarySchoolYear();
    		}
    	}   
    	inv.addModel("middleSchoolName", middleSchoolName);
    	inv.addModel("middleSchoolYear", enrollYear);
    	LogTimeUtils.logEnd(begin, "guideGetTargetUserMiddleSchoolName");
    }
    
    /**
     * 根据学校和地区资料推人
     * 
     * @param inv
     * @param window
     * @param cm
     */
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideGetBestOfferByStage(Invocation inv, Window window, CacheManager cm) {
    	long begin = TimeCostUtil.logBegin();
    	int stage =  xce.offerfriends.HIGHSCHOOL.value;
    	User host = UserProvider.getInstance().get(cm);
    	int limit = 379;
    	List<DefaultOffer> list = OfferFriendsAdapter.getInstance().getBestOfferByStage(stage, host.getId(), limit);
    	List<Integer> sameAreaIds = new ArrayList<Integer>();
    	if(list !=null){
    		for(DefaultOffer item:list){        		
        		if(item.isSameAreaSameEnroll()){
        			sameAreaIds.add(item.getUserId());
        		}
        	}
    	}
    	GuideLogger.printLog(host.getId() + ":sameAreaIds.size()"+sameAreaIds.size());
    	inv.addModel(GUIDE_SAME_AREA_IDS, sameAreaIds);
    	LogTimeUtils.logEnd(begin, "guideGetBestOfferByStage");
    }
    
    private HotShareService hotShareService;
    
    private MemCacheManager mcm ;
    
    @PostConstruct
    public void initHotShareService() {
        ServiceFactory fact = ServiceFactories.getFactory();
        hotShareService = fact.getService(HotShareService.class);
        mcm = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
	public void transmitBlogList(Invocation inv, Window window, CacheManager cm) {
		long begin = TimeCostUtil.logBegin();
		User host = UserProvider.getInstance().get(cm);
		int blogType = 1;
		int stype = 102;
		int limit = 7;
		int stage = 10;
		int[] targets = new int[] { host.getId() };
		ServiceFuture<HotShare[]> blogSf = (ServiceFuture<HotShare[]>) hotShareService.findHotShare(blogType, stage, 0, limit);
		blogSf.submit();
		HotShare[] blogList = (HotShare[]) mcm.get(GuideMemCacheKeysPrefix.BLOG_HOT_SHARE);
		if (blogList == null) {
			blogList = blogSf.awaitUninterruptibly().getContent();
			if (blogList != null) {
				GuideLogger.printLog(host.getId() + ":blogList.length:" + blogList.length);
				mcm.set(GuideMemCacheKeysPrefix.BLOG_HOT_SHARE, blogList, (int) MemCacheKeys.hour * 7);
			}
		}
		if (blogList != null) {
			GuideLogger.printLog(host.getId() + ":blogList.length:" + blogList.length);
			for (HotShare hs : blogList) {
				if (hs == null) {
					continue;
				}
				GuideLogger.printLog(host.getId() + ":hs.getUserId():" + hs.getUserId());
				FeedLogicAdapter.getInstance().DirectedSend(hs.getShareId(), stype, hs.getUserId(), targets);
			}
		}
		LogTimeUtils.logEnd(begin, "transmitBlogList");
	}

	@MethodThroughput(maxConcurrent = 50, instruction = "")
	public void transmitPhotoList(Invocation inv, Window window, CacheManager cm) {
		long begin = TimeCostUtil.logBegin();
		User host = UserProvider.getInstance().get(cm);
		int photoType = 8;
		int stype = 103;
		int limit = 7;
		int stage = 10;
		int[] targets = new int[] { host.getId() };
		ServiceFuture<HotShare[]> photoSf = (ServiceFuture<HotShare[]>) hotShareService.findHotShare(photoType, stage, 0, limit);
		photoSf.submit();
		HotShare[] photoList = (HotShare[]) mcm.get(GuideMemCacheKeysPrefix.PHOTO_HOT_SHARE);
		if (photoList == null) {
			photoList = photoSf.awaitUninterruptibly().getContent();
			if (photoList != null) {
				GuideLogger.printLog(host.getId() + ":photoList.length:" + photoList.length);
				mcm.set(GuideMemCacheKeysPrefix.PHOTO_HOT_SHARE, photoList, (int) MemCacheKeys.hour * 7);
			}
		}
		if (photoList != null) {
			GuideLogger.printLog(host.getId() + ":photoList.length:" + photoList.length);
			for (HotShare hs : photoList) {
				if (hs == null) {
					continue;
				}
				GuideLogger.printLog(host.getId() + ":hs.getUserId():" + hs.getUserId());
				FeedLogicAdapter.getInstance().DirectedSend(hs.getShareId(), stype, hs.getUserId(), targets);
			}
		}
		LogTimeUtils.logEnd(begin, "transmitPhotoList");
	}

	@MethodThroughput(maxConcurrent = 50, instruction = "")
	public void transmitVideoList(Invocation inv, Window window, CacheManager cm) {
		long begin = TimeCostUtil.logBegin();
		User host = UserProvider.getInstance().get(cm);
		int videoType = 10;
		int limit = 7;
		int stage = 10;
		int stype = 110;
		int[] targets = new int[] { host.getId() };
		ServiceFuture<HotShare[]> videoSf = (ServiceFuture<HotShare[]>) hotShareService.findHotShare(videoType, stage, 0, limit);
		videoSf.submit();
		HotShare[] videoList = (HotShare[]) mcm.get(GuideMemCacheKeysPrefix.VIDEO_HOT_SHARE);
		if (videoList == null) {
			videoList = videoSf.awaitUninterruptibly().getContent();
			if (videoList != null) {
				GuideLogger.printLog(host.getId() + ":videoList.length:" + videoList.length);
				mcm.set(GuideMemCacheKeysPrefix.VIDEO_HOT_SHARE, videoList,(int) MemCacheKeys.hour * 7);
			}
		}
		if (videoList != null) {
			GuideLogger.printLog(host.getId() + ":videoList.length:" + videoList.length);
			for (HotShare hs : videoList) {
				if (hs == null) {
					continue;
				}
				GuideLogger.printLog(host.getId() + ":hs.getUserId():" + hs.getUserId());
				FeedLogicAdapter.getInstance().DirectedSend(hs.getShareId(),stype, hs.getUserId(), targets);
			}
		}
		LogTimeUtils.logEnd(begin, "transmitVideoList");
	}
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void transmitCityFeeds(Invocation inv, Window window, CacheManager cm) {
    	long begin = TimeCostUtil.logBegin();
    	User host = UserProvider.getInstance().get(cm);
    	int middleSchoolId = -1;
		int enrollYear = 0;
		List<HighSchoolInfo> hsInfoList = AdapterFactory.getNetworkAdapter().getHighSchoolInfoList(host.getId());
		if (hsInfoList != null) {
			for (HighSchoolInfo hsi : hsInfoList) {
				if (hsi.getEnrollYear() > enrollYear) {
					enrollYear = hsi.getEnrollYear();
					middleSchoolId = hsi.getHighSchoolId();
				}
			}
		}
		if (middleSchoolId == -1) {
			JuniorHighSchoolInfo junInfo = AdapterFactory.getNetworkAdapter().getJuniorHighSchoolInfo(host.getId());
			if (junInfo != null) {
				middleSchoolId = junInfo.getJunHighentarySchoolId();
				enrollYear = junInfo.getJunHighentarySchoolYear();
			}
		}
		int cityStarLimit = 8;
		GuideCity gc = GuideLogic.getInstance().getCityCodeByMiddleSchoolId(middleSchoolId);
		//有可能根据学校取不到城市
		if(gc!=null){
			String cityCode = gc.getCityCode();
			List<Integer> idList = GuideLogic.getInstance().getUserStarIdsByCityCode(cityCode,cityStarLimit);
			GuideLogger.printLog(host.getId() + ":city star cityCode:"+cityCode);
			GuideLogger.printLog(host.getId() + ":city star list:"+idList.size());
			GuideLogic.getInstance().guideBuddyReplyNotify(host.getId(), idList);
		}		
		LogTimeUtils.logEnd(begin, "transmitCityFeeds");
    }
    
    /**
     * 根据学校资料推人
     * 
     * @param inv
     * @param window
     * @param cm
     */
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void getSameSchool(Invocation inv, Window window, CacheManager cm) {
    	User user = UserProvider.getInstance().get(cm);
        try {
            int amount = 21;
            long begin = LogTimeUtils.logBegin();
            List<IRelationModel> list = new ArrayList<IRelationModel>();
            //获取各个阶段的Model
            IRelationModel highModel = RelationModelFactory.create(user, amount, RelationModelFactory.HIGH_RELATION_MODEL);
            IRelationModel juniorModel = RelationModelFactory.create(user, amount, RelationModelFactory.JUNIOR_RELATION_MODEL);
            list.add(highModel);
            list.add(juniorModel);
            List<IRelation> mayFriends = FriendsUtil.findPeople(user, list);
            if(mayFriends!=null){
            	GuideLogger.printLog(user.getId() + ":mayFriends.size():"+mayFriends.size());
            }
            else{
            	GuideLogger.printLog(user.getId() + ":mayFriends is null");
            }
            List<GuideSameSchool> gssList = GuideUtils.getInstance().parseGuideIRelationList(mayFriends);
            inv.addModel(SAME_SCHOOL_LIST, gssList);
            LogTimeUtils.logEnd(begin, "EndGetSameSchool");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    @Autowired
    private HomeNavService homeNavService;

    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void userOwnSites(Invocation inv, Window window, CacheManager cm) {
        int userId = UserProvider.getInstance().get(cm).getId();
        if (homeNavService != null) {
            try {
                Site[] site = homeNavService.getUserOwnSites(userId);
                GuideLogger.printLog("site == null:" + userId + ":" + (site == null));
                if (site == null) {
                    inv.addModel("show_topic", 0);
                } else {
                	inv.addModel("homeLeftSites", site);
                    inv.addModel("show_topic", 1);
                }
            } catch (Exception e) {
                System.err.print("userOwnSites" + e.toString());
                e.printStackTrace();
            }
        }
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
	public void guideMiniGroupMenu(Invocation inv, Window window, CacheManager cm) {
		try {
            int userId = UserProvider.getInstance().get(cm).getId();
            List<Integer> userCacheId = new ArrayList<Integer>();
            userCacheId.add(userId);
            List<WUserCache> userCache = WUserCacheAdapter.getInstance().getUsersCacheList(userCacheId);
            inv.addModel("miniGrouplist", MiniGroupUtil.getHomeMenu(userCache.get(0)));
        } catch (Exception e) {
            e.printStackTrace();
        }
	}
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideRecentMenuHome(Invocation inv, Window window, CacheManager cm) {
        //ArrayList<AppMenuItem> recentMenu = (ArrayList<AppMenuItem>) ((UserMenu) inv.getModel().get("userMenu")).getRecentMenu();
    	//inv.addModel("recentMenuHome", RecentMenuHome.getInstance().getMenuRecommendedApp(recentMenu));

        inv.addModel("recentMenuHome", RecentMenuHome.getInstance().getMenuRecommendedApp(
        		(ArrayList<AppMenuItem>) ((UserMenu) inv.getModel().get("userMenu")).getRecentMenu(),
        		(ArrayList<AppMenuItem>) ((UserMenu) inv.getModel().get("userMenu")).getFavoriteMenu()));
        inv.addModel("onelevelset", RecentMenuHome.getInstance().get1LevelMenuAppIdSet());
        inv.addModel("onelevelmap", RecentMenuHome.getInstance().get1LevelMenuMap());
        inv.addModel("twolevelmap", RecentMenuHome.getInstance().get2LevelMenuMap());
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void vipZone(Invocation inv, Window window, CacheManager cm) {
		User user = UserProvider.getInstance().get(cm);
		String url = "xoa://vip.xoa.renren.com/homeCenter/home";
		Method method = Method.get(url);
		UserState userState = user.getUserState();
		String content = "";
		if (user != null && userState != null) {
			// 查询cache内是不是当前用户属于VIP过期一个月的用户
			XCache<Integer> _flagcache = XCache.getCache("vas_homeview.showflag", Integer.class);
			if (user.isVipMember() || userState.isGiftMember() || _flagcache.get(String.valueOf(user.getId())) != null) {
				// vip会员，礼物会员，vip过期不足一个月的用户调用XOA
				method.setParam("uid", Integer.toString(user.getId()));
				XoaClient client = new XoaClient();
				XoaResponse response = null;
				try {
					response = client.execute(method);
					content = response.getContentAs(String.class);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
		inv.addModel("vipZoneContent", content);
		inv.addModel("timeStamp", new SimpleDateFormat("yyyyMMdd").format(new Date()));
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void homeNuomiMenu(Invocation inv, Window window, CacheManager cm) {// OK
        User host = UserProvider.getInstance().get(cm);
        inv.addModel("show_nuomi_menu", HomeNuomiUserConfig.getInstance().isNuomi(host.getId()));
    }
    
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void guideV6Commented(Invocation inv, Window window, CacheManager cm) {// ok
        int userid = UserProvider.getInstance().get(cm).getId();
        inv.getRequest().setAttribute("recommendcontentlist", 
                guideRecommendLogic.getRecommendContentList(userid,4,GuideRecommendConstants.CONTENT_TYPE_MIMISITE,"guide_home"));
    }
    
    @MethodThroughput(maxConcurrent = 50, instruction = "")
    public void homeCollectionMenuHome(Invocation inv, Window window, CacheManager cm) {
        List<AppMenuItem> favList = (ArrayList<AppMenuItem>) ((UserMenu) inv.getModel().get(
                "userMenu")).getFavoriteMenu();
        Collections.reverse(favList);
        GuideLogger.printLog("favList:"+favList);
        
        inv.getRequest().setAttribute("favList", favList);
        
        /*inv.addModel("favListsguidemm", favList);
        inv.getRequest().setAttribute("favListsguidenn", favList);
        
        inv.addModel("favListsguide1", favList);
        inv.getRequest().setAttribute("favListsguide2", favList);
        
        inv.addModel("favlistsguidemmrr", favList);
        inv.getRequest().setAttribute("favlistsguidennrr", favList);
        
        inv.addModel("favlistsguidemmrry", "addmodel");
        inv.getRequest().setAttribute("favlistsguidennrri", "reqiestset");*/

    }
}
