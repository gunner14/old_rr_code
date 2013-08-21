
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserPageLogic"%>
<%@page import="com.xiaonei.reg.guide.util.GuideLogger"%>
<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="com.xiaonei.platform.component.application.facade.xiaonei_reg.AppUserFacade"%><%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="com.xiaonei.wService.userGuide.adapter.UserGuideAdapter"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%>
<%!interface FsUrls{
	static final String URL_QUN = "http://qun.renren.com/qun/ugc/home/";
}
%>
<%
try{
	String the_important_fs = (String)request.getAttribute("the_important_fs");
	String whole_from_source = (String)request.getAttribute("whole_from_source");
	String fsid_extra = (String)request.getAttribute("fsid_extra");
	User host = (User)request.getAttribute("visiter");
	//it's sure that fsid_extra has something;
	String url = "";
	
	if (null == fsid_extra){
		fsid_extra = "";
	}
	String [] fsid_extra_s = fsid_extra.split("_");
	String from = "";
	String from_id = "";
	if(fsid_extra_s!=null && fsid_extra_s.length >= 2){
		from = fsid_extra_s[0];
		from_id = fsid_extra_s[1];
	}
	
	if("_AAF".equals(the_important_fs) || "_AAW".equals(the_important_fs)){
		GuideLogger.printLog(" host:"+host.getId()+" from:"+from+" from_id:"+from_id);
		if("qun".equals(from)){
			url = FsUrls.URL_QUN + from_id + "?emailInvite=true";
		}
	}
	
	if("_AAR".equals(the_important_fs)){
		if("page".equals(from)){
			GuideLogger.printLog(" host:"+host.getId()+" add page:"+from_id);
			//do add page
			//GuideXFiveUserPageLogic.getInstance().beFansofPage("600002805",host);
			java.util.Map<String,String> map = new java.util.HashMap<String,String>();
            map.put("pageid",from_id);
            com.xiaonei.wService.userGuide.adapter.UserGuideAdapter.getInstance().push(host.getId(),UserGuideAdapter.FOLLOW_PAGE,map);
		}
	}
	
	GuideLogger.printLog(" host:"+host.getId()+" url:"+url);
	if("".equals(url)){
		RequestDispatcher disp = request.getRequestDispatcher("/compld");
		//url = "/guide?t=first";
		//url = url.replace("&amp;","&");
		disp.forward(request,response);
		//response.sendRedirect(url);
	}
	else{
	    url = url.replace("&amp;","&");
    	response.sendRedirect(url);
	}
	
}
catch(Exception e){
    e.printStackTrace();
}
%>
