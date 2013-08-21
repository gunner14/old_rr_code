package com.xiaonei.reg.guide.flows.xfive.controllers;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.WorkplaceInfo;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;

import Ice.TimeoutException;

import com.dodoyo.datum.model.RegionInfo;
import com.renren.sns.guide.model.GuideIRelation;
import com.renren.sns.guide.model.GuideIRelationFriend;
import com.renren.sns.lead.dataproviders.util.StatLogger;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.flows.xfive.formbeans.InfoForPreview;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveRelationsPreviewLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserProfileLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserSortLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAdapter;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheData;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheFactory;
import com.xiaonei.xce.friendfinderinfocache.FriendFinderInfoCacheAdapter;
import com.xiaonei.xce.offerfriends.DefaultOffer;

/**
 * PreviewInitController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 2010-09-26 18:25:17 添加 doAAASearch
 */
@LoginRequired
@Path({"preview"})
@IgnoreUserStatusImportedValidation
public class PreviewInitController {
	
	@Autowired
	GuideXFiveUserSortLogic guideXFiveUserSortLogic;
    
	private String formatYear(int year) {
		if(year != 0){
			return year+"级";
		}
		return "";
	}
	private static final int RESULT_LIMIT = 24;
	private final static int thisStep = StepStatusLogic.STEP_PREVIEW;
    private static final String SKIP_DEFAULT = "f:lead";
	private static final String SUCC_DEFAULT = "/views/guide/flowxfive/preview.jsp";
	private static final String SUCC_CORAL = "f:pvcoral";
	private static final String SUCC_BROWN = "f:pvbrown";
	private static final String INIT_LEAD_AAA = "/views/guide/flowxfivedarkchoco/preview_darkchoco.jsp"; //通讯录 曾纯强
	private static final String INIT_LEAD_AAF = "/views/guide/flowxfivedarkcoral/preview_darkcoral.jsp"; //口香糖
	private static final String INIT_LEAD_AAH = "/views/guide/flowxfivedarkcorn/preview_darkcorn.jsp"; //无头带邀请好友引导
	private static final String INIT_LEAD_AAI = "/views/guide/flowxfivedarkcrimson/preview_darkcrimson.jsp"; //一步快速引导
	
	private static final String REDIRECT_MOREFREND = "r:morefriend";
	
	@Get
	@Post
	public String get(Invocation inv){
		String isnew = inv.getParameter("new");
		if("1".equals(isnew)){
			return "f:previewnew";
		}
		
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		User host = GuideXFiveRequestUtil.getCurrentHost(request);		
		if(request == null || response == null){
			GuideLogger.printLog(" request or response null "+(request==null)+(response==null),GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
		}
		else{	
			try {
			    if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
	                GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
	                return "r:http://reg.renren.com/sb/fillemail";
	            }
			    
				//GuideXFiveUtil.setInitAttributes(request);
				String skip = GuideXFiveRequestUtil.getParamString(request, "skip", host.getId());
				if(MyStringUtils.equals(skip, "1")){
					skip(host);
					return SKIP_DEFAULT;
				}
				int type = StepProcessLogic.getInstance().getGuideUserType(host);
				GuideLogger.printLog(" host:"+host.getId()+" type:"+type);
				request.setAttribute("lead_user_type", type);
				switch(type){
				case IGuideUserLeadType.XFiveGeneral:
					//把默认的都牵到SUCC_BROWN 主要是为了9的 2010年3月29日17:18:49
					/*doDefault(request, host);
					return SUCC_DEFAULT;*/
					return SUCC_BROWN;					
				case IGuideUserLeadType.XFiveCoral:
				case IGuideUserLeadType.XFiveCoralB:
					return SUCC_CORAL;
				case IGuideUserLeadType.XFiveBrown:
					return SUCC_BROWN;
				case IGuideUserLeadType.Lead_AAA:
				case IGuideUserLeadType.Lead_AAE:
				    String ret = doAAASearch(inv, request,host);
				    return ret;
				case IGuideUserLeadType.Lead_AAF:
				case IGuideUserLeadType.OldMobile:
				case IGuideUserLeadType.Lead_AAP:
				case IGuideUserLeadType.Lead_AAQ:
				case IGuideUserLeadType.Lead_AAR:
					String aafret = doAAFSearch(inv, request,host,type);
				    return aafret;
				case IGuideUserLeadType.Lead_AAH:
					String aahret = doAAHSearch(inv, request,host);
				    return aahret;
				case IGuideUserLeadType.Lead_AAI:
					String aairet = doAAISearch(inv, request,host);
					return aairet;
				default:
					//2010-3-12 14:42:17 为网吧引导同步
					/*doDefault(request, host);
					return SUCC_DEFAULT;*/
					return SUCC_BROWN;
				}
				
			} catch (Exception e) {
				GuideLogger.printLog("setInitAttributes failed! err "+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
		}
		
		return SUCC_DEFAULT;
	}
	

	private String doAAISearch(Invocation inv, HttpServletRequest request,
			User host) {
		String ret = INIT_LEAD_AAI;
		try {
			User af = GuideUtil.getAFriend(host);
			if(af != null){
				Map<Integer, WUserCache> fs = FriendsHome.getInstance().getFriends(af.getId());
				if(fs != null && fs.size() >= 1){
					if(fs.containsKey(host.getId())){ //去掉自己
						fs.remove(host.getId());
					}
					String json = GuideUtil.getJSONForFriendSelectorFromCacheMapBySort(host.getId(),fs);
					inv.addModel("lead_already_friend",af);
					inv.addModel("lead_may_known",json);
					GuideLogger.printLog(" host:"+host.getId()+" foff:"+fs.size());
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" foff:0"+" af:"+af.getId());
					skip(host);
					ret = SKIP_DEFAULT;
				}
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+"  af null!",GuideLogger.WARN);
			}
		} catch (TimeoutException e) {
			e.printStackTrace();
		} catch (IndexOutOfBoundsException e) {
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return ret;
	}

	/**
	 * 我申请过谁
	 * @param inv
	 * @param host
	 * @return
	 */
	private List<Integer> getRequestFriends(Invocation inv,User host){ 
		List<Integer> list = new ArrayList<Integer>();
		BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData> _adapter = new BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData>(new DefaultBuddyByApplicantCacheFactory());
        DefaultBuddyByApplicantCacheData _data = (DefaultBuddyByApplicantCacheData) _adapter.getBuddyByApplicantCacheData(host.getId(), 0, -1);
        for (BuddyByApplicantCacheAccepterInfo el : _data.getAccepters()) {
			list.add(el.getAccepter());
        }
        
        return list;
	}
	
	private List<Integer> getFriends(Invocation inv,User host){
		List<Integer> list = FriendsHome.getInstance().getFriendsIds(host.getId());
		if(list == null){
			list = new ArrayList<Integer>();
		}
		return list;
	}
	
	@SuppressWarnings("unused")
	private List<Integer> getMoreFriendsList(Invocation inv,User host) {
		List<Integer> flist = new ArrayList<Integer>();
        try {
        	List<Integer> list = new ArrayList<Integer>();
        	//List<Integer> list = FriendsHome.getInstance().getApplyFriendList(host.getId());
        	BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData> _adapter = new BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData>(new DefaultBuddyByApplicantCacheFactory());
            DefaultBuddyByApplicantCacheData _data = (DefaultBuddyByApplicantCacheData) _adapter.getBuddyByApplicantCacheData(host.getId(), 0, -1);
            for (BuddyByApplicantCacheAccepterInfo el : _data.getAccepters()) {
				list.add(el.getAccepter());
            }
            
            //这个list是好友列表 
            flist = FriendFinderInfoCacheAdapter.getInstance().getGuestInfoMatchFriendListBatch(getIntArray(list), host.getId()); //这是二度好友的
    		if(flist.size() >= 36){
    			flist = flist.subList(0, 35);
    		}
            
        	if(flist!=null){ //放入好友列表
        		String json = GuideUtil.getJSONForFriendSelectorFromIdsForMorefriend(list);
            	inv.addModel("lead_preview_confirmlist", json);
            }     
        } catch (Exception e) {
        	GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        } 
		return flist; //返回的才是需要的大结果
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
	private String getIntsString(List<Integer> list){
		StringBuilder sb = new StringBuilder();
		sb.append("[");
		
		if(list == null || list.size() == 0){
			
		}
		else{
			for (Integer integer : list) {
				sb.append(integer).append(",");
			}
		}
		sb.append("]");
		return sb.toString();
	}
	
	private static final String lead_preview_stage = "lead_preview_stage";
	private static final String lead_preview_friend_count = "lead_preview_friend_count";
	private static final String lead_preview_friend = "lead_preview_friend";
	private static final String lead_user_type = "lead_user_type";
	
    private String doAAFSearch(Invocation inv, HttpServletRequest request, final User host, int type) {
    	String user_type = StatLogger.TYPE_AAF;
    	if(type == IGuideUserLeadType.OldMobile){
    		user_type = StatLogger.TYPE_AAO;
    	}
    	else if(type == IGuideUserLeadType.Lead_AAQ){
    		user_type = StatLogger.TYPE_AAQ;
    	}
    	else if(type == IGuideUserLeadType.Lead_AAR){
    		user_type = StatLogger.TYPE_AAR;
    	}
    	request.setAttribute("lead_user_type_string", user_type);
    	String ret = SKIP_DEFAULT;
	    try {
            GuideTimeCost cost = GuideTimeCost.begin();
            String skip = GuideXFiveRequestUtil.getParamString(request, "skip", host.getId());
            cost.endStep();
            int autoskiped = 0;
            if(MyStringUtils.equals(skip, "1")){
                skip(host);
                
                cost.endStep();
                cost.endFinally();
                ret = SKIP_DEFAULT;
                GuideLogger.printLog(" host:"+host.getId()+" skiped");
            }
            else{
            	List<Integer> ids = new ArrayList<Integer>();
            	List<GuideIRelation> profilefriends = null;
            	Map<Integer, DefaultOffer> offerFriendsMap = null;
            	int profilefriends_count = 0;
            	//ids = getMoreFriendsList(inv,host);
            	//sb.append(" more_ids:"+ids.size());//先推二度好友
            	//如果有好友
            	
            	List<Integer> friendlist = getFriends(inv, host);
            	List<Integer> requestlist = getRequestFriends(inv, host);
            	if(requestlist.size() >= 1 && !"true".equals(inv.getParameter("stay"))){ //加过好友的
            		skip(host);
            		//GuideLogger.printLog(" host:"+host.getId()+" fl:"+friendlist.size()+" rl:"+requestlist.size());
            		StatLogger.info(host.getId(), ""+"friends:"+friendlist.size()+" requests:"+requestlist.size(),StatLogger.S_ACT_ADDEDFRIEN_FI_,user_type);
            		autoskiped = 1;
            		ret = REDIRECT_MOREFREND;
            	}
            	else if(ids.size() > 0){ 
            		ret = INIT_LEAD_AAF;
            	}
            	else{ //再推可能认识的人
            		StatLogger.info(host.getId(), ""+"friends:"+friendlist.size()+" requests:"+requestlist.size(),StatLogger.S_ACT_NOADDFRIEN_FI_,user_type);
            		List<DefaultOffer> offerList = GuideUtil.getOfferFriendListByStage(host);
            		offerFriendsMap = GuideUtil.getOfferFriendMap(offerList);
            		ids = GuideUtil.getOfferFriendIdList(offerList);      		
            		cost.endStep();
            		
            		if(ids == null || ids.size() <= 0){
            			try {
							profilefriends = getProfileFriends(host);
						} catch (Exception e) {
							GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
						}
						
						if(profilefriends == null || profilefriends.size() == 0){
							GuideLogger.printLog(" host:"+host.getId());
							skip(host);
							autoskiped = 2;
							cost.endStep();
							ret = SKIP_DEFAULT;
						}
						else{
							profilefriends_count = countFriends(profilefriends, host);
							ret = INIT_LEAD_AAF;
						}
            			
            		}
            		else{
            			GuideLogger.printLog(" host:"+host.getId()+" mayfriends:"+ids.size());
            			
            			cost.endStep();
            			ret = INIT_LEAD_AAF;
            		}
            		cost.endFinally();
            	}
            	
            	GuideLogger.printLog(" host:"+host.getId()+" sum:"+ids.size());
            	//推完的结果不只在ids里面……还在profilefriends里面
            	//下面是对这两个List的操作
            	
            	ids.removeAll(friendlist);
            	ids.removeAll(requestlist);
            	GuideLogger.printLog(" host:"+host.getId()+" fl:"+friendlist.size()+" rl:" +requestlist.size()+" after:"+ids.size());
            	
            	String friend_srouce_flag = "mayknow";
            	if(ids != null && ids.size()>0){ // 这是mayknown的
            		
            	}
            	else if(profilefriends != null && profilefriends_count > 0){ // 这是profile的
            		ids = getFriendsIds(profilefriends, host);
            		GuideLogger.printLog(" host:"+host.getId()+" profilefriends:"+profilefriends.size()+" ids:" +ids);
            		friend_srouce_flag = "profrid";
            	}
            	else{
            	}
            	//合并到一个List中了
            	int good_i = 0;
            	Map<Integer, WUserCache> cmap = null; //song.bao
            	if(ids != null && ids.size() > 0){
            		if(host.getId() % 10 == 5 || host.getId() % 10 == 6){
            			cmap = GuideUtil.getUserCacheMapByIsActive(ids);
            		}
            		else{
            			cmap = GuideUtil.getUserCacheMap(ids);
            		}
            		//到这的cmap就是最终结果
            		good_i = proccessCacheMap(host,inv,cmap,friend_srouce_flag, offerFriendsMap);
            	}
            	else{
            		ids = new ArrayList<Integer>();
            		//inv.addModel(lead_preview_friend_count, 0);
            	}
            	if(cmap == null){
            		cmap = new HashMap<Integer, WUserCache>();
            	}
            	inv.addModel(lead_preview_stage,host.getStage());
            	inv.addModel(lead_preview_friend_count, cmap.size());
            	inv.addModel(lead_user_type,user_type);
            	
            	GuideLogger.printLog(" host:"+host.getId()+" ids:"+ids.size()+" cache:" +cmap.size()+" flag:"+friend_srouce_flag);
            	StatLogger.info(host.getId(), "autoskiped:"+autoskiped+" "+friend_srouce_flag+":"+cmap.size()+" "+cmap.keySet()+" ids:"+ids.size() +" good:"+good_i+" "+(cmap.size() == ids.size()?"normal":"abnormal!"+ids),StatLogger.S_RST_FINDFRIEND_PV_,user_type);
            }
            
        } catch (Exception e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        }
        GuideLogger.printLog(" host:"+host.getId()+" ret:"+ret);
        return ret;
	}

	private List<Integer> getFriendsIds(List<GuideIRelation> profilefriends,User host) {
		return GuideXFiveRelationsPreviewLogic.getInstance().getIdsFromGuideIRelationList(profilefriends);
	}

	private int proccessCacheMap(User host, Invocation inv, Map<Integer, WUserCache> cmap, String logparam, Map<Integer, DefaultOffer> offerFriendsMap) {
		//lead_preview_friend_count
		//lead_preview_friend
		int good_i = 0;
		int hostid = host == null? 0 : host.getId();
		try {
			if(hostid % 10 == 5 || hostid % 10 == 6){ //song.bao 5 6 尾号先上 2010-12-10 09:47:48
				good_i = processMapWithActSort(inv, cmap, logparam, hostid, offerFriendsMap);
			}
			else{
				good_i = processMapWithoutActSort(inv, cmap, logparam, hostid, offerFriendsMap);
			}
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+hostid+" e:"+e.toString());
			e.printStackTrace();
		}
		return good_i;
	}

	private int processMapWithoutActSort(Invocation inv, Map<Integer, WUserCache> cmap, String logparam, int hostid, Map<Integer, DefaultOffer> offerFriendsMap) {
		String resultstring = "";
		JSONObject jo = new JSONObject();
		JSONArray retCandidate = new JSONArray();
		int i = 0;
		int good_i = 0;
		for (Entry<Integer, WUserCache> cur : cmap.entrySet()) {
			i++;
			WUserCache userCache = cur.getValue();
			if(userCache != null){
				if( userCache.isActive()){ 
					good_i ++;
				}
				JSONObject joinner = new JSONObject();
				joinner.put("id", userCache.getId());
				joinner.put("head", userCache.getTiny_Url());
				joinner.put("name", userCache.getName());
				joinner.put("net", userCache.getUnivName());
				joinner.put("act", userCache.isActive()?1:0);
				if(offerFriendsMap != null){
					DefaultOffer offer = offerFriendsMap.get(userCache.getId());
					if(offer != null){
						joinner.put("type", offer.getType());
					}
				}
				retCandidate.put(joinner);
			}
			if(i>=200)break;
			
		}
		
		GuideLogger.printLog(" host:"+hostid+" "+logparam+" i:"+i+" good_i:"+good_i);
		
		jo.put("candidate", retCandidate);
		resultstring = jo.toString();
		
		/*
         * kobin  2012/05/25
         * 添加用户价值排序逻辑
         */
		long sortStartTime = System.currentTimeMillis();
        resultstring = guideXFiveUserSortLogic.sortJsonArrayByUserMark(resultstring.trim());
        long sortEndTime = System.currentTimeMillis();
        GuideLogger.printLog(">>>>>>>>GuideXFiveUserSortLogic cost time:" + (sortEndTime - sortStartTime) + "<<<<<<<<");
        
		//inv.addModel(lead_preview_friend_count, i);
		inv.addModel(lead_preview_friend, resultstring);
		GuideLogger.printLog(" host:"+hostid+" all:"+i+" good:" +good_i);
		return good_i;
	}


	private int processMapWithActSort(Invocation inv, Map<Integer, WUserCache> cmap, String logparam, int hostid, Map<Integer, DefaultOffer> offerFriendsMap) {
		String resultstring = "";
		JSONObject jo = new JSONObject();
		JSONArray retCandidate = new JSONArray();
		List<WUserCache> nogoodman = new ArrayList<WUserCache>();
		int i = 0;
		int good_i = 0;
		for (Entry<Integer, WUserCache> cur : cmap.entrySet()) {
			i++;
			WUserCache userCache = cur.getValue();
			if(userCache != null){
				if( userCache.isActive()){ 
					good_i ++;
					JSONObject joinner = new JSONObject();
					joinner.put("id", userCache.getId());
					joinner.put("head", userCache.getTiny_Url());
					joinner.put("name", userCache.getName());
					joinner.put("net", userCache.getUnivName());
					joinner.put("act", userCache.isActive()?1:0);
					if(offerFriendsMap != null){
						DefaultOffer offer = offerFriendsMap.get(userCache.getId());
						if(offer != null){
							joinner.put("type", offer.getType());
						}
					}
					retCandidate.put(joinner);
				}
				else{
					nogoodman.add(userCache);
				}
			}
			if(i>=200)break;
			
		}
		
		GuideLogger.printLog(" host:"+hostid+" "+logparam+" i:"+i+" good_i:"+good_i);
		
		for (WUserCache userCache : nogoodman) {//没头像的放后面
			JSONObject joinner = new JSONObject();
			joinner.put("id", userCache.getId());
			joinner.put("head", userCache.getTiny_Url());
			joinner.put("name", userCache.getName());
			joinner.put("net", userCache.getUnivName());
			joinner.put("act", userCache.isActive()?1:0);
			if(offerFriendsMap != null){
				DefaultOffer offer = offerFriendsMap.get(userCache.getId());
				if(offer != null){
					joinner.put("type", offer.getType());
				}
			}
			retCandidate.put(joinner);
		}
		
		jo.put("candidate", retCandidate);
		resultstring = jo.toString();
		
		/*
         * kobin  2012/05/25
         * 添加用户价值排序逻辑
         */
		long sortStartTime = System.currentTimeMillis();
        resultstring = guideXFiveUserSortLogic.sortJsonArrayByUserMark(resultstring.trim());
        long sortEndTime = System.currentTimeMillis();
        GuideLogger.printLog(">>>>>>>>GuideXFiveUserSortLogic cost time:" + (sortEndTime - sortStartTime) + "<<<<<<<<");

        //inv.addModel(lead_preview_friend_count, i);
		inv.addModel(lead_preview_friend, resultstring);
		GuideLogger.printLog(" host:"+hostid+" all:"+i+" good:" +good_i);
		return good_i;
	}


	private int countFriends(List<GuideIRelation> list,User host){
        int ret = 0;
        if(list != null){
            for (GuideIRelation relation :  list) {
                ret += relation.getCount();
            }
        }
        return ret;
    }
    
    
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

	private String doAAASearch(Invocation inv, HttpServletRequest request, User host) {
	    String ret = SKIP_DEFAULT;
	    try {
            GuideTimeCost cost = GuideTimeCost.begin();
            String skip = GuideXFiveRequestUtil.getParamString(request, "skip", host.getId());
            cost.endStep();
            if(MyStringUtils.equals(skip, "1")){
                skip(host);
                
                cost.endStep();
                cost.endFinally();
                ret = SKIP_DEFAULT;
            }
            else{
                List<GuideIRelation> mayFriends = getFriendsListForLead(inv,host);
                cost.endStep();
                
                if(mayFriends == null || countFriends(mayFriends,host) <= 0){
                    GuideLogger.printLog(" host:"+host.getId());
                    skip(host);
                    
                    cost.endStep();
                    cost.endFinally();
                    ret = SKIP_DEFAULT;
                }
                else{
                    GuideLogger.printLog(" host:"+host.getId()+" friends:"+countFriends(mayFriends,host));
                    
                    cost.endStep();
                    cost.endFinally();
                    ret = INIT_LEAD_AAA;
                }
            }
            
        } catch (Exception e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        }
        GuideLogger.printLog(" host:"+host.getId()+" ret:"+ret);
        return ret;
    }
	
	private String doAAHSearch(Invocation inv, HttpServletRequest request, User host) {
	    String ret = SKIP_DEFAULT;
	    try {
            GuideTimeCost cost = GuideTimeCost.begin();
            String skip = GuideXFiveRequestUtil.getParamString(request, "skip", host.getId());
            cost.endStep();
            if(MyStringUtils.equals(skip, "1")){
                skip(host);
                
                cost.endStep();
                cost.endFinally();
                ret = SKIP_DEFAULT;
            }
            else{
                List<GuideIRelation> mayFriends = getFriendsListForLead(inv,host);
                cost.endStep();
                
                if(mayFriends == null || countFriends(mayFriends,host) <= 0){
                    GuideLogger.printLog(" host:"+host.getId());
                    skip(host);
                    
                    cost.endStep();
                    cost.endFinally();
                    ret = SKIP_DEFAULT;
                }
                else{
                    GuideLogger.printLog(" host:"+host.getId()+" friends:"+countFriends(mayFriends,host));
                    
                    cost.endStep();
                    cost.endFinally();
                    ret = INIT_LEAD_AAH;
                }
            }
            
        } catch (Exception e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        }
        GuideLogger.printLog(" host:"+host.getId()+" ret:"+ret);
        return ret;
    }
	
   private void skip(User host) {
        GuideTimeCost cost = GuideTimeCost.begin();
        GuideLogger.printLog(" host:" + host.getId() + " skip:" + thisStep);
        StepStatusLogic.updateStepStatus(host.getId(), thisStep);
        cost.endFinally();

    }
   private List<GuideIRelation> getProfileFriends(User host) {
       List<GuideIRelation> list = GuideXFiveRelationsPreviewLogic.getInstance().getProfileResults(48, host, -1);
       StringBuilder sb = new StringBuilder();
       int cnt = 0;
       int innercnt = 0;
       for (GuideIRelation ir : list) {
           List<GuideIRelationFriend> aa = ir.getList();
           for (GuideIRelationFriend f : aa) {
               if(innercnt++>=48){
                   break;
               }
               sb.append(f.getId());
               sb.append(",");
               cnt++;
           }
       }
       GuideLogger.printLog(" host:"+host.getId()+" cnt:"+cnt+" "+"f:["+sb.toString()+"]");
       return list;
   }
   private List<GuideIRelation> getFriendsListForLead(Invocation inv,User host) {
       /*List<Integer> list = GuideXFiveIDsPreviewLogic.getInstance().getProfileResults(RESULT_LIMIT, host, -1);
       
       if(list != null){
           String json = GuideUtil.getJSONForFriendSelectorFromIds(list);
           GuideLogger.printLog(" host:"+host.getId()+" get:"+list.size());
           inv.addModel("lead_may_known", json);
       }
       else{
           GuideLogger.printLog(" host:"+host.getId()+" getlist null",GuideLogger.WARN);
       }*/
       List<GuideIRelation> list = GuideXFiveRelationsPreviewLogic.getInstance().getProfileResults(RESULT_LIMIT, host, -1);
       if (list == null) list = new ArrayList<GuideIRelation>();
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
       GuideLogger.printLog(" host:"+host.getId()+" cnt:"+cnt+" "+"f:["+sb.toString()+"]");
       String json = GuideXFiveRelationsPreviewLogic.getInstance().getJSONForFriendSelector(list);
       GuideLogger.printLog(" host:"+host.getId()+" get:"+countFriendsMore(list,host));
       inv.addModel("lead_may_known", json);
       return list;
       
   }
   

    @SuppressWarnings("unused")
	private void doDefault(HttpServletRequest request, User host) {
		try {
			GuideXFiveUtil.setInitAttributes(request);
			List<InfoForPreview> list = new ArrayList<InfoForPreview>();
			int stage = host.getStage();
			boolean haselem = false;
			GuideLogger.printLog(" host:"+host.getId()+" stage:"+host.getStage());
			GuideLogger.printLog(" host:"+host.getId());
			
			UniversityInfo univ_univ = GuideXFiveUserProfileLogic.getInstance().loadUniversity(host);
			if(univ_univ != null){
				InfoForPreview info = new InfoForPreview();
				
				StringBuilder sb = new StringBuilder();
				sb.append(univ_univ.getUniversityName());
				String dept = univ_univ.getDepartment();
				GuideLogger.printLog(" host:"+host.getId()+" dept:"+dept);
				if(dept != null && !"".equals(dept.trim()) && !dept.contains("选择院系")){
					sb.append(dept);
				}
				sb.append(formatYear(univ_univ.getEnrollYear()));

				info.setDisplayname(sb.toString());
				info.setType("U");
				info.setInfoid(""+univ_univ.getUniversityId());
				info.setDbid(""+univ_univ.getId());
				info.setUserid(host.getId());
				info.setInfoyear(univ_univ.getEnrollYear());
				list.add(info);
			}
			GuideLogger.printLog(" host:"+host.getId());
			HighSchoolInfo univ_high = GuideXFiveUserProfileLogic.getInstance().loadHighschool(host);
			if(univ_high != null){
				InfoForPreview info = new InfoForPreview();
				info.setDisplayname(univ_high.getHighSchoolName()+formatYear(univ_high.getEnrollYear()));
				info.setType("H");
				info.setInfoid(""+univ_high.getHighSchoolId());
				info.setDbid(""+univ_high.getId());
				info.setUserid(host.getId());
				info.setInfoyear(univ_high.getEnrollYear());
				list.add(info);
			}
			
			GuideLogger.printLog(" host:"+host.getId());
			JuniorHighSchoolInfo junior = GuideXFiveUserProfileLogic.getInstance().loadJuniorschool(host);
			if(junior != null){
				InfoForPreview info = new InfoForPreview();
				info.setDisplayname(junior.getJunHighentarySchoolName()+formatYear(junior.getJunHighentarySchoolYear()));
				info.setType("J");
				info.setInfoid(""+junior.getJunHighentarySchoolId());
				info.setDbid(""+0);
				info.setUserid(host.getId());
				info.setInfoyear(junior.getJunHighentarySchoolYear());
				list.add(info);
			}
			GuideLogger.printLog(" host:"+host.getId());
			CollegeInfo tech = GuideXFiveUserProfileLogic.getInstance().loadTechschool(host);
			if(tech != null){
				InfoForPreview info = new InfoForPreview();
				info.setDisplayname(tech.getCollegeName()+formatYear(tech.getEnrollYear()));
				info.setType("T");
				info.setInfoid(""+tech.getCollegeId());
				info.setDbid(""+tech.getId());
				info.setUserid(host.getId());
				info.setInfoyear(tech.getEnrollYear());
				list.add(info);
			}
			GuideLogger.printLog(" host:"+host.getId());
			ElementarySchoolInfo elem = GuideXFiveUserProfileLogic.getInstance().loadElementary(host);
			if(elem != null){
				InfoForPreview info = new InfoForPreview();
				info.setDisplayname(elem.getElementarySchoolName()+formatYear(elem.getElementarySchoolYear()));
				info.setType("E");
				info.setInfoid(""+elem.getElementarySchoolId());
				info.setDbid(""+0);
				info.setUserid(host.getId());
				info.setInfoyear(elem.getElementarySchoolYear());
				list.add(info);
				haselem = true;
			}
			WorkplaceInfo major_work = GuideXFiveUserProfileLogic.getInstance().loadWorkplace(host);
			if(major_work != null){
				InfoForPreview info = new InfoForPreview();
				info.setDisplayname(major_work.getWorkplaceName());
				info.setType("W");
				info.setInfoid(""+major_work.getWorkplaceId());
				info.setDbid(""+major_work.getId());
				info.setUserid(host.getId());
				info.setInfoyear(major_work.getJoinYear());
				list.add(info);
			}
			switch(stage){
			case Stage.STAGE_MAJOR:
				if("".equals("")){
					InfoForPreview info = new InfoForPreview();
					info.setType("I");
					info.setDisplayname("可能认识的人");
					info.setUserid(host.getId());
					list.add(info);
				}
				break;
			case Stage.STAGE_NONE:
				GuideLogger.printLog(" host:"+host.getId());
				RegionInfo none_home = GuideXFiveUserProfileLogic.getInstance().loadHomeRegion(host);
				RegionInfo none_cur =  GuideXFiveUserProfileLogic.getInstance().loadCurrentRegion(host);
				if(none_home != null && none_cur != null ){
					InfoForPreview info = new InfoForPreview();
//					if("澳门".equals(none_home.getCity_name())){
//						info.setDisplayname(none_home.getProvince_name());
//					}
//					else{
//					}
					info.setDisplayname("在"+none_cur.getProvince_name()+"的"+none_home.getProvince_name()+"人");
					info.setType("R");
					info.setInfoid(""+none_home.getRegion_id());
					info.setDbid(""+none_home.getId());
					info.setUserid(host.getId());
					list.add(info);
				}
				
				if("".equals("")){
					InfoForPreview info = new InfoForPreview();
					info.setType("P");
					info.setDisplayname("在你附近上网");
					info.setUserid(host.getId());
					list.add(info);
				}
				break;
			}
			/*
			switch(stage){
			case Stage.STAGE_HEIGHSCHOOL:
				GuideLogger.printLog(" host:"+host.getId());
				HighSchoolInfo high = GuideXFiveUserProfileLogic.getInstance().loadHighschool(host);
				if(high != null){
					InfoForPreview info = new InfoForPreview();
					info.setDisplayname(high.getHighSchoolName());
					info.setType("H");
					info.setInfoid(""+high.getHighSchoolId());
					info.setDbid(""+high.getId());
					info.setUserid(host.getId());
					list.add(info);
				}
				GuideLogger.printLog(" host:"+host.getId());
				JuniorHighSchoolInfo junior = GuideXFiveUserProfileLogic.getInstance().loadJuniorschool(host);
				if(junior != null){
					InfoForPreview info = new InfoForPreview();
					info.setDisplayname(junior.getJunHighentarySchoolName());
					info.setType("J");
					info.setInfoid(""+junior.getJunHighentarySchoolId());
					info.setDbid(""+0);
					info.setUserid(host.getId());
					list.add(info);
				}
				GuideLogger.printLog(" host:"+host.getId());
				CollegeInfo tech = GuideXFiveUserProfileLogic.getInstance().loadTechschool(host);
				if(tech != null){
					InfoForPreview info = new InfoForPreview();
					info.setDisplayname(tech.getCollegeName());
					info.setType("T");
					info.setInfoid(""+tech.getCollegeId());
					info.setDbid(""+tech.getId());
					info.setUserid(host.getId());
					list.add(info);
				}
				GuideLogger.printLog(" host:"+host.getId());
				ElementarySchoolInfo elem = GuideXFiveUserProfileLogic.getInstance().loadElementary(host);
				if(elem != null){
					InfoForPreview info = new InfoForPreview();
					info.setDisplayname(elem.getElementarySchoolName());
					info.setType("E");
					info.setInfoid(""+elem.getElementarySchoolId());
					info.setDbid(""+0);
					info.setUserid(host.getId());
					list.add(info);
					haselem = true;
				}
				
				break;
			case Stage.STAGE_UNIV:
				GuideLogger.printLog(" host:"+host.getId());
				UniversityInfo univ_univ = GuideXFiveUserProfileLogic.getInstance().loadUniversity(host);
				if(univ_univ != null){
					InfoForPreview info = new InfoForPreview();
					info.setDisplayname(univ_univ.getUniversityName());
					info.setType("U");
					info.setInfoid(""+univ_univ.getUniversityId());
					info.setDbid(""+univ_univ.getId());
					info.setUserid(host.getId());
					list.add(info);
				}
				GuideLogger.printLog(" host:"+host.getId());
				HighSchoolInfo univ_high = GuideXFiveUserProfileLogic.getInstance().loadHighschool(host);
				if(univ_high != null){
					InfoForPreview info = new InfoForPreview();
					info.setDisplayname(univ_high.getHighSchoolName());
					info.setType("H");
					info.setInfoid(""+univ_high.getHighSchoolId());
					info.setDbid(""+univ_high.getId());
					info.setUserid(host.getId());
					list.add(info);
				}
				
				break;
			case Stage.STAGE_MAJOR:
				GuideLogger.printLog(" host:"+host.getId());
				WorkplaceInfo major_work = GuideXFiveUserProfileLogic.getInstance().loadWorkplace(host);
				if(major_work != null){
					InfoForPreview info = new InfoForPreview();
					info.setDisplayname(major_work.getWorkplaceName());
					info.setType("W");
					info.setInfoid(""+major_work.getWorkplaceId());
					info.setDbid(""+major_work.getId());
					info.setUserid(host.getId());
					list.add(info);
				}
				GuideLogger.printLog(" host:"+host.getId());
//				RegionInfo major_cur = GuideXFiveUserProfileLogic.getInstance().loadCurrentRegion(host);
//				if(major_cur != null){
//					InfoForPreview info = new InfoForPreview();
//					info.setDisplayname(major_cur.getProvince_name()+major_cur.getCity_name());
//					info.setType("I");
//					info.setInfoid(""+major_cur.getRegion_id());
//					info.setDbid(""+major_cur.getId());
//					info.setUserid(host.getId());
//					list.add(info);
//				}
				if("".equals("")){
					InfoForPreview info = new InfoForPreview();
					info.setType("I");
					info.setDisplayname("可能在你身边的朋友");
					info.setUserid(host.getId());
					list.add(info);
				}
				
				break;
			case Stage.STAGE_NONE:
				GuideLogger.printLog(" host:"+host.getId());
				RegionInfo none_cur = GuideXFiveUserProfileLogic.getInstance().loadCurrentRegion(host);
				if(none_cur != null){
					InfoForPreview info = new InfoForPreview();
					info.setDisplayname(none_cur.getProvince_name()+none_cur.getCity_name());
					info.setType("R");
					info.setInfoid(""+none_cur.getRegion_id());
					info.setDbid(""+none_cur.getId());
					info.setUserid(host.getId());
					list.add(info);
				}
				
				if("".equals("")){
					InfoForPreview info = new InfoForPreview();
					info.setType("P");
					info.setDisplayname("可能认识的人");
					info.setUserid(host.getId());
					list.add(info);
				}
				break;
			default:
				GuideLogger.printLog(" host:"+host.getId());
				break;
					
			}
			*/
			request.setAttribute("infolist", list);
			
			int formtype = 0;
			switch(stage){
			case Stage.STAGE_HEIGHSCHOOL:
				if(haselem){
					formtype = 0;
				}
				else{
					formtype = 2;
				}
				break;
			case Stage.STAGE_UNIV:
				formtype = 1;
				break;
			case Stage.STAGE_MAJOR:
				formtype = 3;
				break;
			case Stage.STAGE_NONE:
				formtype = 4;
				break;
			default:
				break;
					
			}
			
			request.setAttribute("formtype", formtype);
			
		} catch (Exception e) {
			GuideLogger.printLog("setInitAttributes failed! err "+e.toString()+GuideLogger.ERROR);
			e.printStackTrace();
		}
	}

	
}
