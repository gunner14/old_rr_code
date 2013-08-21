package com.renren.sns.lead.controllers.window;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.portal.Window;
import Ice.TimeoutException;

import com.renren.sns.guide.model.GuideIRelation;
import com.renren.sns.guide.model.GuideIRelationFriend;
import com.renren.sns.lead.core.DataProviderFactory;
import com.renren.sns.lead.dataproviders.util.StatLogger;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.commons.interceptors.throughput.MethodThroughput;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.core.control.TimeCostUtil;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveRelationsPreviewLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.reg.guide.util.LogTimeUtils;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.ObjectProvider;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;
import com.xiaonei.sns.platform.core.rose.interceptors.MergeWindowModel;
import com.xiaonei.sns.platform.core.rose.interceptors.Performance;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAdapter;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheData;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheFactory;
import com.xiaonei.xce.friendfinderinfocache.FriendFinderInfoCacheAdapter;

@MergeWindowModel
@Performance
@LoginRequired
public class DataWindowsController {
	private static final int RESULT_LIMIT = 49;
	public static final String predata_applylist = "predata_applylist";
	public static final String predata_ainviter = "predata_ainviter";
	
    private int countFriendsMore(List<GuideIRelation> list,User host){
        int ret = 0;
        if(list != null){
            for (GuideIRelation relation :  list) {
                GuideLogger.printLog(" host:"+host.getId()+" i:"+relation.getTypeName()+" c:"+relation.getCount());
                ret += relation.getCount();
            }
        }
        return ret;
    }
    
    
	@MethodThroughput(maxConcurrent = 40, instruction = "")
    public void guideGetProfileFriends(Invocation inv, Window window, CacheManager cm) {
        long begin = TimeCostUtil.logBegin();
        User host = UserProvider.getInstance().get(cm);
        //DataProviderFactory.getMapper("PreviewProfile").getData(host, 290);
        //List<GuideIRelation> list = GuideXFiveRelationsPreviewLogic.getInstance().getProfileResults(RESULT_LIMIT, host, -1);
        List<GuideIRelation> list = DataProviderFactory.getDataProvider("PreviewProfile").getData(host, 285);
        if(list == null){
        	GuideLogger.printLog(" host:"+host.getId()+" list null");
        	list = new ArrayList<GuideIRelation>();
        }
        StringBuilder sb = new StringBuilder();
        int cnt = 0;
        int innercnt = 0;
        for (GuideIRelation ir : list) {
            List<GuideIRelationFriend> aa = ir.getList();
            for (GuideIRelationFriend f : aa) {
                if(innercnt++>=RESULT_LIMIT){
                    break;
                }
                sb.append(f.getId());
                sb.append(",");
                cnt++;
            }
        }
        
        {
            String json = GuideXFiveRelationsPreviewLogic.getInstance().getJSONForFriendSelector(list);
            GuideLogger.printLog(" host:"+host.getId()+" get:"+countFriendsMore(list,host));
            inv.addModel("lead_preview_friend", json);
            inv.addModel("lead_preview_friend_count", cnt);
        }
        
        GuideLogger.printLog(" host:"+host.getId()+" cnt:"+cnt+" "+"profile_f:["+sb.toString()+"]");
        LogTimeUtils.logEnd(begin, "guideGetProfileFriends");
    }
	
	@MethodThroughput(maxConcurrent = 40, instruction = "")
    public void guideGetOfferFriends(Invocation inv, Window window, CacheManager cm) {
        long begin = TimeCostUtil.logBegin();
        User host = UserProvider.getInstance().get(cm);
        
        List<Integer> ids = DataProviderFactory.getDataProvider("PreviewOfferfriend").getData(host, 250);
		String json = "";
		if(ids != null){
			json = GuideUtil.getJSONForFriendSelectorFromIds(host.getId(),ids);
			inv.addModel(lead_preview_offer,json);
			inv.addModel(lead_preview_offer_count,ids.size());
		}
		else{
			ids = new ArrayList<Integer>();
		}
		GuideLogger.printLog(" host:"+host.getId()+" cnt:"+ids.size()+" "+"offer_f:"+ids+"");
        LogTimeUtils.logEnd(begin, "guideGetOfferFriends");
    }
	@MethodThroughput(maxConcurrent = 40, instruction = "")
	public void guideGetInviterFriendsCluster(Invocation inv, Window window, CacheManager cm) {
		long begin = TimeCostUtil.logBegin();
		User host = UserProvider.getInstance().get(cm);
		User ainviter = (User) ObjectProvider.getInstance().get(cm, predata_ainviter);
		int cnt = 0;
		int ainviterid = 0;
		String json = "";
		if(ainviter != null ){
			ainviterid = ainviter.getId();
			List<Integer> inviterlist = new ArrayList<Integer>();
			inviterlist.add(ainviter.getId());
			List<Integer> resultlist = new ArrayList<Integer>();//SocialGraphAdapter.getFriendClusterCacheAdapter().getFriendClusterPeopleForGuide(inviterlist,  RESULT_LIMIT);
			Integer inviterid = ainviterid;
			resultlist.remove(inviterid);
			Map<Integer, WUserCache> fmap = GuideUtil.getUserCacheMap(resultlist);
			cnt = fmap.size();
			json = GuideUtil.getJSONForFriendSelectorFromCacheMap(host.getId(), fmap,resultlist);
			StatLogger.info(host.getId(), "from:"+ainviterid+" cluster:"+resultlist.size()+" "+resultlist,StatLogger.S_RST_FINDFRIEND_PV_,StatLogger.TYPE_AAJ);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" ainviter:"+null);
		}
		inv.addModel(lead_preview_offer_cluster,json);
		inv.addModel(lead_preview_offer_count_cluster,cnt);
		GuideLogger.printLog(" host:"+host.getId()+" ainviter:"+ainviterid+" retcnt:"+cnt);
		LogTimeUtils.logEnd(begin, "guideGetInviterFriendsCluster");
	}
	private static final String lead_preview_offer_cluster = "lead_preview_offer_cluster";
	private static final String lead_preview_offer_count_cluster = "lead_preview_offer_count_cluster";
	
	private static final String lead_preview_offer = "lead_preview_offer";
	private static final String lead_preview_offer_count = "lead_preview_offer_count";
	
	@MethodThroughput(maxConcurrent = 40, instruction = "")
	public void guideGetRequestFriendsCluster(Invocation inv, Window window, CacheManager cm) {
		long begin = TimeCostUtil.logBegin();
		User host = UserProvider.getInstance().get(cm);
		List<Integer> requestlist = getBuddyApplyCacheList(host);
		User af = GuideUtil.getAInviter(host); 
		requestlist.add(af.getId());
		int cnt = 0;
		String json = "";
		{
			List<Integer> resultlist = new ArrayList<Integer>();//SocialGraphAdapter.getFriendClusterCacheAdapter().getFriendClusterPeopleForGuide(requestlist, RESULT_LIMIT);
			resultlist.removeAll(requestlist);
			Map<Integer, WUserCache> fmap = GuideUtil.getUserCacheMap(resultlist);
			cnt = fmap.size();
			json = GuideUtil.getJSONForFriendSelectorFromCacheMap(host.getId(), fmap,resultlist);
			StatLogger.info(host.getId(), "from:"+requestlist+" cluster:"+resultlist.size()+" "+resultlist,StatLogger.S_RST_FINDFRIEND_MF_,StatLogger.TYPE_AAJ);
		}
		inv.addModel(lead_preview_offer_cluster,json);
		inv.addModel(lead_preview_offer_count_cluster,cnt);
		GuideLogger.printLog(" host:"+host.getId()+" requestlist:"+requestlist+" retcnt:"+cnt);
		LogTimeUtils.logEnd(begin, "guideGetRequestFriendsCluster");
	}
	
	@MethodThroughput(maxConcurrent = 40, instruction = "")
	public void guideGetInviterFriends(Invocation inv, Window window, CacheManager cm) {
		long begin = TimeCostUtil.logBegin();
		User host = UserProvider.getInstance().get(cm);
		Map<Integer, WUserCache> fs = null;
		try {
			//User af = GuideUtil.getAFriend(host);
			//User af = GuideUtil.getAInviter(host); //换成Inviter
			User af = (User) ObjectProvider.getInstance().get(cm, predata_ainviter);
			if(af != null){
				fs = FriendsHome.getInstance().getFriends(af.getId());
				if(fs != null && fs.size() >= 1){
					if(fs.containsKey(host.getId())){ //去掉自己
						fs.remove(host.getId());
					}
					String json = GuideUtil.getJSONForFriendSelectorFromCacheMapBySort(host.getId(),fs);
					inv.addModel("lead_already_friend",af);
					inv.addModel("lead_preview_inviterfriend",json);
					inv.addModel("lead_preview_inviterfriend_count",fs.size());
					GuideLogger.printLog(" host:"+host.getId()+" foff:"+fs.size());
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" foff:0"+" af:"+af.getId());
				}
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+"  af null!",GuideLogger.WARN);
			}
		} catch (TimeoutException e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
		} catch (IndexOutOfBoundsException e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
		} catch (IllegalArgumentException e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
		}
		GuideLogger.printLog(" host:"+host.getId()+" cnt:"+(fs==null?0:fs.size()));
		LogTimeUtils.logEnd(begin, "guideGetInviterFriends");
	}
	
	@MethodThroughput(maxConcurrent = 40, instruction = "")
    public void guideGetMoreFriends(Invocation inv, Window window, CacheManager cm) {
        long begin = TimeCostUtil.logBegin();
        User host = UserProvider.getInstance().get(cm);
        String json = "";
        List<Integer> flist = null;
        try {
        	List<Integer> list = getBuddyApplyCacheList(host);
        	//List<Integer> list = (List<Integer>) ObjectProvider.getInstance().get(cm, predata_applylist);
            //这个list是好友列表 
            
            flist = FriendFinderInfoCacheAdapter.getInstance().getGuestInfoMatchFriendListBatch(getIntArray(list), host.getId()); //这是二度好友的
    		if(flist.size() > RESULT_LIMIT){
    			flist = flist.subList(0, RESULT_LIMIT);
    		}
            
        	if(flist!=null){ //放入好友列表
        		json = GuideUtil.getJSONForFriendSelectorFromIdsForMorefriend(list);
            	inv.addModel("lead_preview_confirmlist", json);
            }     
        } catch (Exception e) {
        	GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        } 
        inv.addModel("lead_preview_more",json);
        LogTimeUtils.logEnd(begin, "guideGetMoreFriends");
    }

	private List<Integer> getBuddyApplyCacheList(User host) {
		List<Integer> list = new ArrayList<Integer>();
		try {
			BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData> _adapter = new BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData>(new DefaultBuddyByApplicantCacheFactory());
			DefaultBuddyByApplicantCacheData _data = (DefaultBuddyByApplicantCacheData) _adapter.getBuddyByApplicantCacheData(host.getId(), 0, -1);
			for (BuddyByApplicantCacheAccepterInfo el : _data.getAccepters()) {
				list.add(el.getAccepter());
			}
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return list;
	}

	
	private int []  getIntArray(List<Integer> list){
		if(list == null || list.size() == 0)return new int [0];
		int [] ints = new int [list.size()];
		for (int i = 0; i < ints.length; i++) {
			ints[i] = list.get(i);
		}
		return ints;
	}
	@SuppressWarnings("unused")
	private static int []   aaa(List<Integer> list){
		if(list == null || list.size() == 0)return new int [0];
		int [] ints = new int [list.size()];
		for (int i = 0; i < ints.length; i++) {
			ints[i] = list.get(i);
		}
		return ints;
	}
	
	public static void main(String[] args) {
		List<Integer> list = new ArrayList<Integer>();
		list.add(1);
		list.add(2);
		list.add(3);
		list.add(4);
		Integer r = 3;
		list.remove(r);
		//Integer [] a = new Integer[]{1,2,3,4};
		Map<Integer,Integer> aa = new HashMap<Integer, Integer>();
		aa.put(1, 2);
		aa.put(2, 2);
		aa.put(3, 2);
		System.out.println(list);
				
	}
}
