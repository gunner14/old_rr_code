
<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="com.xiaonei.platform.component.application.facade.xiaonei_reg.AppUserFacade"%><%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="com.xiaonei.wService.userGuide.adapter.UserGuideAdapter"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%>
<%
try{
	
	com.xiaonei.platform.core.model.User host = null;
	try {
		host = (com.xiaonei.platform.core.model.User)request.getAttribute("host");
		com.xiaonei.platform.component.xfeed.display.impl.FeedRetriever.getInstance().getHotFeed(host.getId(),host.getName(),0,1);//捅新鲜事
	} catch(Exception e) {
		
	}
    String url = Globals.urlGuide + "/guide?t=first";
    response.sendRedirect(url);
}
catch(Exception e){
    
}
%>
