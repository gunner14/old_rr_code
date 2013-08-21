package com.xiaonei.reg.guide.flows.xfive.controllers;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.Calendar;
import java.util.List;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.ice.WUserBasicAdapter;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveCommonCacheLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.reg.register.logic.additional.RegPortraitLogic;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@LoginRequired
@Path("portrait")
@IgnoreUserStatusImportedValidation
public class PortraitInitController {
	private static final String SUCC = "f:portraits"; 
	private static final String INIT_DEFAULT = "/views/guide/flowxfivedark/portrait_dark.jsp";
	private static final String INIT_LEAD_AAA = "/views/guide/flowxfivedarkchoco/portrait_darkchoco.jsp"; //通讯录 曾纯强
	private static final String INIT_LEAD_AAF = "/views/guide/flowxfivedarkcoral/portrait_darkcoral.jsp"; //口香糖引导 2010-12-2 10:07:41加上
	private static final String INIT_LEAD_AAH = "/views/guide/flowxfivedarkcorn/portrait_darkcorn.jsp"; //无头带邀请好友引导
	
	@SuppressWarnings("deprecation")
	@Get
	@Post
	public String get(Invocation inv){
		String isnew = inv.getParameter("new");
		if("1".equals(isnew)){
			return "f:portraitnew";
		}
		HttpServletRequest request = inv.getRequest();
		//HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		GuideTimeCost cost = GuideTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		if(host == null){
			return "@系统忙，请稍候再试";
		}
		cost.endStep();
		//GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - start" +" - current GDomain is:"+Globals.urlGuide);
		//cost.endStep();
		try {
		    
		    if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
                GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
                return "r:http://reg.renren.com/sb/fillemail";
            }
		    
			int type = StepProcessLogic.getInstance().getGuideUserType(host);
			GuideLogger.printLog(" host:"+host.getId()+" type:"+type);
			request.setAttribute("lead_user_type", type);
			cost.endStep();
			request.setAttribute("sex_flag", SnsAdapterFactory.getUserAdapter().get(host.getId()).getGender());
		} catch (Exception e) {
			GuideLogger.printLog("setAttribute failed! e:"+e.toString(),GuideLogger.ERROR);
		}
		cost.endStep();
		// 跳转到head.upload域下
		String sex_flag = "1";
		String gend = "";//
		try {
			gend = SnsAdapterFactory.getUserBornAdapter().getUserBorn(host.getId()).getGender();
		} catch (Exception e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		if (User.genderFemale.equalsIgnoreCase(gend)) {
			sex_flag = "0";
		}
		cost.endStep();
		
		boolean hasHead = true;
		try {
			hasHead = RegPortraitLogic.hasUploaded(host);
		} catch (Exception e) {
			GuideLogger.printLog("hasUploaded failed!"+ host.getId(),GuideLogger.ERROR);
		}
		if(hasHead){
			try {
				GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - end - to guide - head = "+SnsAdapterFactory.getUserAdapter().get(host.getId()).getHeadUrl());
			} catch (Exception e) {
				GuideLogger.printLog("failed!"+ host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			return SUCC;
		}
		cost.endStep();
		
		String other = GuideXFiveRequestUtil.getParamString(request, "other", host.getId());
		if(!MyStringUtils.equals("1", other)){
			request.setAttribute("user", host.getId());
			request.setAttribute("name", host.getName());
			
			if(host.getStage() == 20){//大学生
				request.setAttribute("college", "true");
			}
			
			if("kaixin.com".equals(Globals.domain.toString()))
				request.setAttribute("kaixinDomain", "true");
			try {
				String mainUrl = host.getMainUrl();
				if((mainUrl != null) && (mainUrl.length() >= 3)){
					if(mainUrl.substring(0, 3).equals("0/0")){	
					}else{
						request.setAttribute("hasHead", Boolean.TRUE);
						// FIXME mainUrl取半长，这里的headUrl用fullUrl
						request.setAttribute("userMainFullUrl", host.getMainFullUrl());
					}
				}
				UserBasic basic = WUserBasicAdapter.getInstance().get(host.getId());
				if(basic != null){
					//request.setAttribute("headUrl", basic.getHead_Url());
					int year = basic.getBirthYear() + 18;
					int month = basic.getBirthMonth() - 1;
					int day = basic.getBirthDay();
					Calendar compare = Calendar.getInstance();
					Calendar now = (Calendar)compare.clone();
					compare.set(Calendar.YEAR, year);
					compare.set(Calendar.MONTH, month);
					compare.set(Calendar.DAY_OF_MONTH, day);
					if(now.compareTo(compare) < 0)//less than 18
						request.setAttribute("young", Boolean.TRUE); 
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
			
			try {
	        	//List<Integer> list = FriendsHome.getInstance().getApplyFriendList(host.getId());
	        	List<Integer> list = GuideXFiveCommonCacheLogic.getInstance().loadConfirmList(host);
	        	if(list == null){
	        		GuideLogger.printLog(" host:"+host.getId()+" cache null");
	        		list = FriendsHome.getInstance().getApplyFriendList(host.getId());
	        	}
	        	if(list!=null){
	        		String json = GuideUtil.getJSONForFriendSelectorFromIdsForPortrait(list);
	            	inv.addModel("lead_more_friend_confirmlist", json);
	            }     
	        	
	        } catch (Exception e) {
	        	GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
	            e.printStackTrace();
	        } 
	        
			
			//对用户访问的URL做encode,以传给flash用于跳转
			try{
				String url = request.getRequestURL().toString() + "?" + request.getQueryString();
				url = URLEncoder.encode(url,"UTF-8");
				request.setAttribute("encodeUrl", url);
			}catch (Exception e){
				e.printStackTrace();
			}
			
			String ret = INIT_DEFAULT;
			String fromsource = RegFromSourceLogic.getFromSource(host);
			int type = StepProcessLogic.getInstance().getGuideUserType(host,fromsource);
			String user_type_string = "___";
			GuideLogger.printLog(" host:"+host.getId()+" type:"+type);
			switch(type){
			    case IGuideUserLeadType.Lead_AAA:
			    case IGuideUserLeadType.Lead_AAE:
			        ret = INIT_LEAD_AAA;
			    break;
			    case IGuideUserLeadType.Lead_AAH:
			    	ret = INIT_LEAD_AAH;
			    break;
			    case IGuideUserLeadType.Lead_AAF:
			    case IGuideUserLeadType.Lead_ACE:
			    	user_type_string = "AAF";
			    	ret = INIT_LEAD_AAF;
				break;
			    case IGuideUserLeadType.OldMobile:
			    	user_type_string = "AAO";
			    	ret = INIT_LEAD_AAF;
				break;
			    case IGuideUserLeadType.Lead_AAP:
			    	user_type_string = "AAP";
			    	ret = INIT_LEAD_AAF;
			    break;
			    case IGuideUserLeadType.Lead_AAQ:
			    	user_type_string = "AAQ";
			    	ret = INIT_LEAD_AAF;
			    break;
			    case IGuideUserLeadType.Lead_AAR:
			    	user_type_string = "AAR";
			    	ret = INIT_LEAD_AAF;
			    break;
			    default:
			        if(type==13&&fromsource.contains(":PVZ:")){
                        return "/views/guide/flowxfivedarkpvz/portrait_dark.jsp";  
                      }
			        ret = INIT_DEFAULT;
			    user_type_string = "___";
			    break;        
			}
			inv.addModel("user_type_string",user_type_string);
			
			return ret;
		}
		
		//nav_bar=1
		String nav_bar = "";
		cost.endStep();
		try {
			GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+"try nav_bar");
			cost.endStep();
			//2009-12-21 15:53:36 查证没有ChannelUser这一说了，去掉
//			if(A090729Util.isChannnelUser(host)){
//				nav_bar = "&nav_bar=1";
//				GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+"has nav_bar");
//				cost.endStep();
//			}
//			else{
//				nav_bar = "&nav_bar=0";
//				GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+"no nav_bar");
//				cost.endStep();
//			}
//			
			nav_bar = "&nav_bar=0";
			GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+"no nav_bar");
			cost.endStep();
		} catch (Exception e) {
			GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId(),1);
		}
		
		cost.endStep();
		GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - end - sex="+sex_flag);
		cost.endFinally();
		
		String skipurl = Globals.urlGuide + "/portraits";
		try {
			skipurl = URLEncoder.encode(Globals.urlGuide + "/portraits","UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		} 
		String headurl = "http://head.upload."+Globals.domain+"/registerguide/newEntry.do?rurl="
		+ skipurl + "&sex_flag="
		+ sex_flag+nav_bar;
		GuideLogger.writeLine(this , "initPage() - hostid:"+ host.getId()+ " - headurl = "+headurl);
		
		return "r:"+headurl;
		
	}	
	
	
}
