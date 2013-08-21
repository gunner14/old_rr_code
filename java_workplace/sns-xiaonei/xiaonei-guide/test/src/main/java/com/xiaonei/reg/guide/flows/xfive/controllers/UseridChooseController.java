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
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.flows.xfive.formbeans.InfoForPreview;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveRelationsPreviewLogic;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserProfileLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.recommend.GuideRecommendConditionLogic;
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
import com.xiaonei.xce.offerfriends.OfferFriendsAdapter;

/**
 * PreviewInitController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 2010-09-26 18:25:17 添加 doAAASearch
 */
@LoginRequired
@Path({"useridtrailchoose"})
@IgnoreUserStatusImportedValidation
public class UseridChooseController {
    
   // public static boolean isdebug=false;
  
	@Get
	@Post
	public String get(Invocation inv){
		
		User host = GuideXFiveRequestUtil.getCurrentHost(inv.getRequest());
		if(!"399323761".equals(host.getId()+"")){
			return "@ the user can not oper";
		}
		//trailNumList.add(1);
		//trailNumList.add(2);
		GuideLogger.printLog("success");
	   HttpServletRequest request=inv.getRequest();
	   String operType=request.getParameter("opertype");
	   if(operType==null || (!"increase".equals(operType) && !"reduce".equals(operType))){
		   return "@operType is not correct";
	   }
	  
	   String trailNum=request.getParameter("trailnum");
	   if(trailNum==null || !trailNum.matches("[0-9]{1}")){
		   return "@trailNum is not correct";
	   }
	   if("increase".equals(operType)){
		   GuideRecommendConditionLogic.getInstance().add(trailNum);
		   
		   return "@increase success:"+GuideRecommendConditionLogic.getInstance().getConditonList();
	   }
	   if("reduce".equals(operType)){
		   GuideRecommendConditionLogic.getInstance().del(trailNum);
		   return "@reduce success:"+GuideRecommendConditionLogic.getInstance().getConditonList();
	   }
	   
	   return "@param is not correct";
	   
	}
}
