
<%@page import="java.util.Map"%>
<%@page import="java.util.List"%>
<%@page import="com.renren.sns.guide.logic.GuideLogic"%>
<%@page import="mop.hi.oce.domain.network.JuniorHighSchoolInfo"%>
<%@page import="mop.hi.oce.domain.network.HighSchoolInfo"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserProfileLogic"%>
<%@page import="com.xiaonei.platform.component.xfeed.display.IFeedRetriever"%>
<%@page import="com.xiaonei.reg.guide.util.GuideLogger"%>
<%@page import="com.xiaonei.reg.guide.root.logics.StepProcessLogic"%>
<%@page import="com.renren.sns.lead.dataproviders.util.StatLogger"%>
<%@page import="com.xiaonei.platform.component.application.facade.UserDefaultInstallFacade"%>
<%@page import="com.xiaonei.reg.guide.util.GuideUtil"%>
<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="com.xiaonei.platform.component.application.facade.xiaonei_reg.AppUserFacade"%><%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="com.xiaonei.wService.userGuide.adapter.UserGuideAdapter"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%>
<%
com.xiaonei.platform.core.model.User host = null;
try {
	host = (com.xiaonei.platform.core.model.User)request.getAttribute("host");  
} catch(Exception e) {
	
}

if(host != null){
	String domain = "renren.com";
    boolean isrenrenuser = true;
	try{
		domain = host.getDomain();
	    if(!MyStringUtils.equals("renren.com",domain)){
	        isrenrenuser = false;
	    }
	}
	catch(Exception e){
		
	}
	
	try{
		if(isrenrenuser){
	        com.xiaonei.platform.component.xfeed.display.impl.FeedRetriever.getInstance().getHotFeed(host.getId(),host.getName(),0,1);//捅新鲜事
	        //inshion 2010-8-2 14:34:03
	        //加发新鲜事的需求
	        //System.out.println("leadend.jsp:"+host.getId());
	        UserGuideAdapter.getInstance().push(host.getId(),UserGuideAdapter.NEW_REG_SEND_FEED,null); //新用户如有有头像会触发
	        
	        
	        //大学10年 加600039317 新生帝
	        com.xiaonei.reg.guide.root.interfaces.IInfoUniversity iinfo =  com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserInfoLogic.getInstance().getInfoUniversity(host);
	        /*if(iinfo != null && iinfo.getUniversityyear() == 2010){
	            java.util.Map<String,String> map = new java.util.HashMap<String,String>();
	            map.put("pageid","600039317");
	            com.xiaonei.wService.userGuide.adapter.UserGuideAdapter.getInstance().push(host.getId(),UserGuideAdapter.FOLLOW_PAGE,map);
	        }*/
	        if(iinfo != null && iinfo.getUniversityyear() == 2011){
	            java.util.Map<String,String> map = new java.util.HashMap<String,String>();
	            map.put("pageid","600819528");
	            com.xiaonei.wService.userGuide.adapter.UserGuideAdapter.getInstance().push(host.getId(),UserGuideAdapter.FOLLOW_PAGE,map);
	        }
	        
	        //中学加新生帝
	        if(host.getStage() == 10){
	            java.util.Map<String,String> map = new java.util.HashMap<String,String>();
	            map.put("pageid","600693482");
	            com.xiaonei.wService.userGuide.adapter.UserGuideAdapter.getInstance().push(host.getId(),UserGuideAdapter.FOLLOW_PAGE,map);
	        	
	        }
	        
	    }
	}
	catch(Exception e){
		
	}
	
	try{
		//System.out.println(host.getId()+" "+host.getStage());
	    AppUserFacade.getInstance().installDefaultApps(host,!isrenrenuser);
	    
	    String s_appid = GuideUtil.getExtra_XXXString(host,"reg_app_id");
	    int appid =  MyStringUtils.stringToInt(s_appid);
	    if(appid > 0){
		    //invite reg app
		    UserDefaultInstallFacade.getInstance().install(appid,  host.getId(), false);
	    }
	    if(host.getId() % 10 == 0){
	    	//0尾号的装小小 appid = 108245
	    	appid = 108245;
	    	UserDefaultInstallFacade.getInstance().install(appid,  host.getId(), false);
	    }
	}
	catch(Exception e){
		
	}
	
	try{
		org.apache.log4j.Logger logger = org.apache.log4j.Logger.getLogger("xnlog.guide");
		String p = "" + host.getId() + "," + "p91"+ "," + "ga0" + "," ;
		logger.info(p);
		
		int type = StepProcessLogic.getInstance().getGuideUserType(host);
		if(type == 19){
			StatLogger.info(host.getId(), ""+"type"+":"+"AAF"+"",StatLogger.S_ARV_ENTERINTOO_LN_,StatLogger.TYPE_AAF);
		}
	}
	catch(Exception e){
		
	}
}

%>

<%
//stage == 10
if(host.getStage() == 10 && host.getId() % 10 <= 5){
	GuideLogger.printLog(" host:"+host.getId()+" stage 10,do popnews");
	com.xiaonei.platform.component.xfeed.display.FeedFactory factory = com.xiaonei.platform.component.xfeed.display.FeedFactory.getInstance();
	Map<String, ? super Object> map = factory.showNews(host.getId(), host.getName(), 0, IFeedRetriever.HOME_LIMIT);
	int feedCount = (Integer) map.get("count");
	
	if(feedCount<20){
		int middleSchoolId = 0;
		int enrollYear = 0;
		HighSchoolInfo hs = GuideXFiveUserProfileLogic.getInstance().loadHighschool(host);
		
		if(hs!=null){
			middleSchoolId = hs.getHighSchoolId();
			enrollYear = hs.getEnrollYear(); 
		}
		else{
			JuniorHighSchoolInfo js = GuideXFiveUserProfileLogic.getInstance().loadJuniorschool(host);
			if(js != null){
				middleSchoolId = js.getJunHighentarySchoolId();
				enrollYear = js.getJunHighentarySchoolYear();
			}
		}
		
		List<Integer> userIdList = GuideLogic.getInstance().getPopUserMiddleSchoolList(middleSchoolId, enrollYear,8); 
		//limit视需求文档逻辑而定
		GuideLogger.printLog(" host:"+host.getId()+" mid:"+middleSchoolId+" ey:"+enrollYear+" get:"+userIdList);
		GuideLogic.getInstance().guideBuddyReplyNotifyAsync(host,300, userIdList);
	}
}


%>

<%
GuideLogger.printLog(" host:"+host.getId()+" Lead Over");
response.sendRedirect(Globals.urlGuide + "/guide?t=first");
%>
