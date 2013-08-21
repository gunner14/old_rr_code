package com.xiaonei.reg.guide.flows.xfive.controllers;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.json.JSONArray;
import org.json.JSONObject;

import com.renren.sns.lead.dataproviders.util.StatLogger;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.configs.IStepStatusDefinition;
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

@LoginRequired
@Path("morefriend")
@IgnoreUserStatusImportedValidation
public class MoreFriendController {
	private static final String SKIP_DEFAULT = "f:lead";
	private static final String SUCC_DEFAULT = "r:guide.do";
	private static final String SUCC_BROWN = "f:mfbrown";
	private static final String INIT_LEAD_AAF = "/views/guide/flowxfivedarkcoral/morefriend_darkcoral.jsp"; //口香糖
	
	private final static int thisStep = IStepStatusDefinition.STEP_MOREFRIEND;
	
	@Get
	@Post
	public String index(Invocation inv){
		String isnew = inv.getParameter("new");
		if("1".equals(isnew)){
			return "f:morefriendnew";
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
		    
		    if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
                GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
                return "r:http://reg.renren.com/sb/fillemail";
            }
		    
			int type = StepProcessLogic.getInstance().getGuideUserType(host);
			GuideLogger.printLog(" host:"+host.getId()+" type:"+type);
			switch(type){
			case IGuideUserLeadType.Lead_AAF:
			case IGuideUserLeadType.OldMobile:
			case IGuideUserLeadType.Lead_AAP:
			case IGuideUserLeadType.Lead_AAQ:
			case IGuideUserLeadType.Lead_AAR:
				String retaaf = doAAFSearch(inv,request,host,type);
				return retaaf;
			case IGuideUserLeadType.XFiveBrown:
			case IGuideUserLeadType.XFiveBrown_0:
			default:
				return SUCC_BROWN;
			/*default:
				GuideLogger.printLog(" host:"+host.getId()+" why here?",GuideLogger.WARN);
				return SUCC_DEFAULT;*/
			}
		}
		return SUCC_DEFAULT;
		
	}
	
	private static void skip(User host) {
		GuideLogger.printLog(" host:"+host.getId()+" skip:"+thisStep);
		StepStatusLogic.updateStepStatus(host.getId(), thisStep);
	}
	
	private static final String lead_preview_friend_count = "lead_preview_friend_count";
	private static final String lead_preview_friend = "lead_preview_friend";
	private static final String lead_preview_confirmlist = "lead_preview_confirmlist";
	private static final String lead_preview_flag = "lead_preview_flag";
	public static String doAAFSearch(Invocation inv, HttpServletRequest request, User host,int type) {
		String user_type = StatLogger.TYPE_AAF;
    	if(type == IGuideUserLeadType.OldMobile){
    		user_type = StatLogger.TYPE_AAO;
    	}
    	else if(type == IGuideUserLeadType.Lead_AAP){
    		user_type = StatLogger.TYPE_AAP;
    	}
    	else if(type == IGuideUserLeadType.Lead_AAQ){
    		user_type = StatLogger.TYPE_AAQ;
    	}
    	else if(type == IGuideUserLeadType.Lead_AAR){
    		user_type = StatLogger.TYPE_AAR;
    	}
    	request.setAttribute("lead_user_type_string", user_type);
    	//StringBuilder sb = new StringBuilder();
		GuideLogger.printLog(" host:"+host.getId());
    	String ret = SKIP_DEFAULT;
    	int autoskiped = 0;
	    try {
            GuideTimeCost cost = GuideTimeCost.begin();
            String skip = GuideXFiveRequestUtil.getParamString(request, "skip", host.getId());
            cost.endStep();
            if(MyStringUtils.equals(skip, "1")){
                skip(host);
                
                cost.endStep();
                cost.endFinally();
                ret = SKIP_DEFAULT;
                GuideLogger.printLog(" host:"+host.getId()+" skiped");
            }
            else{
            	String friend_srouce_flag = "cluster";
            	List<Integer> ids = null;
            	List<Integer> friendlist = getFriends(inv, host);
            	List<Integer> requestlist = getRequestFriends(inv, host);
            	List<Integer> allfromlist = new ArrayList<Integer>();
            	allfromlist.addAll(friendlist);
            	allfromlist.addAll(requestlist);
            	//ids = SocialgraphAdapter.getInstance().getFriendClusterPeople(allfromlist, 0, 49);
            	ids = null;//SocialGraphAdapter.getFriendClusterCacheAdapter().getFriendClusterPeopleForGuide(requestlist, 49);
            	GuideLogger.printLog(" host:"+host.getId()+" allfromlist:"+allfromlist);
            	if(ids == null || ids.size() == 0){
            		ids = getMoreFriendsList(inv,host);
            		friend_srouce_flag = "morefrd";
            	}
            	else {
            		GuideLogger.printLog(" host:"+host.getId()+" allfromlist:"+allfromlist+" ids:"+ids);
            	}
            	
            	if(requestlist!=null){ //放入好友列表
            		String json = GuideUtil.getJSONForFriendSelectorFromIdsForMorefriend(requestlist);
                	inv.addModel(lead_preview_confirmlist, json);
                }     
            	
            	if(ids == null)ids = new ArrayList<Integer>();
            	
            	GuideLogger.printLog(" host:"+host.getId()+" sum:"+ids.size()+" flag:"+friend_srouce_flag);
            	ids.removeAll(friendlist);
            	ids.removeAll(requestlist);
            	
            	GuideLogger.printLog(" host:"+host.getId()+" fl:"+friendlist.size()+" rl:" +requestlist.size()+" after:"+ids.size()+" flag:"+friend_srouce_flag);
            	if(ids.size() > 0){ 
            		ret = INIT_LEAD_AAF;
            	}
            	else{
            		skip(host);
            		ret = SKIP_DEFAULT;
            		autoskiped = 1;
            	}
            	
            	//推完的结果在ids里面
            	//2010-12-20 13:43:20 变成过滤
            	int good_i = 0;
            	Map<Integer, WUserCache> cmap = null; //song.bao
            	if(ids != null && ids.size() > 0){
            		if(host.getId() % 10 == 5 || host.getId() % 10 == 6){
            			cmap = GuideUtil.getUserCacheMapByIsActive(ids);
            		}
            		else{
            			cmap = GuideUtil.getUserCacheMap(ids);
            		}
            		good_i = proccessCacheMap(host,inv,cmap,friend_srouce_flag);
            	}
            	/*
            	if(ids != null && ids.size()>0){
            		//分尾号
            		Map<Integer, WUserCache> cmap = GuideUtil.getUserCacheMap(ids);
            		proccessCacheMap(host,inv,cmap,"morefrd");
            	}
            	*/
            	else{
            		//inv.addModel("lead_preview_friend_count", 0);
            	}
            	if(cmap == null){
            		cmap = new HashMap<Integer, WUserCache>();
            	}
            	inv.addModel("lead_preview_stage",host.getStage());
            	inv.addModel(lead_preview_friend_count, cmap.size());
            	inv.addModel(lead_preview_flag,friend_srouce_flag);
            	GuideLogger.printLog(" host:"+host.getId()+" ids:"+ids.size()+" cache:" +cmap.size()+" flag:"+friend_srouce_flag);
            	//StatLogger.info(host.getId(), "morefrd"+":("+cmap.size()+")"+getIntsString(ids),StatLogger.S_RST_FINDFRIEND_MF_,StatLogger.TYPE_AAF);
            	StatLogger.info(host.getId(), "autoskiped:"+autoskiped+"|"+""+friend_srouce_flag+""+":"+cmap.size()+" "+cmap.keySet()+"|ids:"+ids.size() +"|good:"+good_i+"|"+(cmap.size() == ids.size()?"normal":"abnormal!"+ids),StatLogger.S_RST_FINDFRIEND_MF_,user_type);
            }
            
        } catch (Exception e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        }
        GuideLogger.printLog(" host:"+host.getId()+" ret:"+ret);
        return ret;
	}
	
	private static int proccessCacheMap(User host, Invocation inv, Map<Integer, WUserCache> cmap, String logparam) {
		//lead_preview_friend_count
		//lead_preview_friend
		int good_i = 0;
		int hostid = host == null? 0 : host.getId();
		try {
			if(hostid % 10 == 5 || hostid % 10 == 6){ //song.bao 5 6 尾号先上 2010-12-10 09:47:48
				good_i = processMapWithActSort(inv, cmap, logparam, hostid);
			}
			else{
				good_i = processMapWithoutActSort(inv, cmap, logparam, hostid);
			}
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+hostid+" e:"+e.toString());
			e.printStackTrace();
		}
		return good_i;
	}
	private static int processMapWithoutActSort(Invocation inv, Map<Integer, WUserCache> cmap, String logparam, int hostid) {
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
				retCandidate.put(joinner);
			}
			if(i>=200)break;
			
		}
		
		GuideLogger.printLog(" host:"+hostid+" "+logparam+" i:"+i+" good_i:"+good_i);
		
		jo.put("candidate", retCandidate);
		resultstring = jo.toString();
		
		//inv.addModel(lead_preview_friend_count, i);
		inv.addModel(lead_preview_friend, resultstring);
		GuideLogger.printLog(" host:"+hostid+" all:"+i+" good:" +good_i);
		return good_i;
	}


	private static int processMapWithActSort(Invocation inv, Map<Integer, WUserCache> cmap, String logparam, int hostid) {
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
			retCandidate.put(joinner);
		}
		
		jo.put("candidate", retCandidate);
		resultstring = jo.toString();
		
		//inv.addModel(lead_preview_friend_count, i);
		inv.addModel(lead_preview_friend, resultstring);
		GuideLogger.printLog(" host:"+hostid+" all:"+i+" good:" +good_i);
		return good_i;
	}

	
	private static List<Integer> getMoreFriendsList(Invocation inv,User host) {
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
            
        } catch (Exception e) {
        	GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        } 
		return flist; //返回的才是需要的大结果
	}
	
	@SuppressWarnings("unused")
	private static String getIntsString(List<Integer> list){
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
	
	private static int []  getIntArray(List<Integer> list){
		if(list == null || list.size() == 0)return new int [0];
		int [] ints = new int [list.size()];
		for (int i = 0; i < ints.length; i++) {
			ints[i] = list.get(i);
		}
		return ints;
	}
	
	/**
	 * 我申请过谁
	 * @param inv
	 * @param host
	 * @return
	 */
	private static List<Integer> getRequestFriends(Invocation inv,User host){ 
		List<Integer> list = new ArrayList<Integer>();
		BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData> _adapter = new BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData>(new DefaultBuddyByApplicantCacheFactory());
        DefaultBuddyByApplicantCacheData _data = (DefaultBuddyByApplicantCacheData) _adapter.getBuddyByApplicantCacheData(host.getId(), 0, -1);
        for (BuddyByApplicantCacheAccepterInfo el : _data.getAccepters()) {
			list.add(el.getAccepter());
        }
        
        return list;
	}
	
	private static List<Integer> getFriends(Invocation inv,User host){
		List<Integer> list = FriendsHome.getInstance().getFriendsIds(host.getId());
		if(list == null){
			list = new ArrayList<Integer>();
		}
		return list;
	}
	
}
