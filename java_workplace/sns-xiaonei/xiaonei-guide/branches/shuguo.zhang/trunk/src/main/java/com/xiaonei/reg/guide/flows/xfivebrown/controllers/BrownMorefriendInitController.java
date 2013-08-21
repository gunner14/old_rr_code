package com.xiaonei.reg.guide.flows.xfivebrown.controllers;

import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.configs.IStepStatusDefinition;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;
import com.xiaonei.xce.friendfinderinfocache.FriendFinderInfoCacheAdapter;

@LoginRequired
@Path("mfbrown")
@IgnoreUserStatusImportedValidation
public class BrownMorefriendInitController {
	
    private static final String SUCC_DEFAULT = "/views/guide/flowxfivedark/morefriend_dark.jsp";
	private static final String SKIP_DEFAULT = "f:lead";
	private final static int thisStep = IStepStatusDefinition.STEP_MOREFRIEND;
	
	@Post
	@Get
	public String index(Invocation inv){
		
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
			String skip = GuideXFiveRequestUtil.getParamString(request, "skip", host.getId());
			if(MyStringUtils.equals(skip, "1")){
				skip(host);
				
				return SKIP_DEFAULT;
			}
			
			List<Integer> mayFriends = getFriendsList(inv,host);
			if(mayFriends == null || mayFriends.size() == 0){
				skip(host);
				GuideLogger.printLog(" host:"+host.getId());
				return SKIP_DEFAULT;
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" friends:"+mayFriends.size());
				String fromsource = RegFromSourceLogic.getFromSource(host);
				int type = StepProcessLogic.getInstance().getGuideUserType(host,fromsource);
		        if(type == 13&&fromsource.contains(":PVZ:")){
		            return "/views/guide/flowxfivedarkpvz/morefriend_dark.jsp";
		        }
				
				
				return SUCC_DEFAULT;
			}
		}
		
		return SUCC_DEFAULT;
	}
	
	private void skip(User host) {
		GuideLogger.printLog(" host:"+host.getId()+" skip:"+thisStep);
		StepStatusLogic.updateStepStatus(host.getId(), thisStep);
	}

	private int []  getIntArray(List<Integer> list){
		if(list == null || list.size() == 0)return new int [0];
		int [] ints = new int [list.size()];
		for (int i = 0; i < ints.length; i++) {
			ints[i] = list.get(i);
		}
		return ints;
	}
	

	@SuppressWarnings("deprecation")
	private List<Integer> getFriendsList(Invocation inv,User host) {
		List<Integer> flist = null;
        try {
        	List<Integer> list = FriendsHome.getInstance().getApplyFriendList(host.getId());
        	
        	if(list!=null){
        		String json = GuideUtil.getJSONForFriendSelectorFromIdsForMorefriend(list);
            	inv.addModel("lead_more_friend_confirmlist", json);
            }     
        	
        	if(list != null && list.size()>0){
        		flist = FriendFinderInfoCacheAdapter.getInstance().getGuestInfoMatchFriendListBatch(getIntArray(list), host.getId());
        		if(flist.size() >= 36){
        			flist = flist.subList(0, 35);
        		}
        		inv.addModel("lead_more_friend_count", list.size());
        		String json = GuideUtil.getJSONForFriendSelectorFromIds(host.getId(),flist);
        		inv.addModel("lead_more_friend", json);
        	}
        	else{
        		inv.addModel("lead_more_friend_count", 0);
        	}
        	
        } catch (Exception e) {
        	GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        } 
		return flist;
	}


}
