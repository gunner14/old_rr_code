
<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="com.xiaonei.platform.component.application.facade.xiaonei_reg.AppUserFacade"%><%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="com.xiaonei.wService.userGuide.adapter.UserGuideAdapter"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%>
<%
try{
    String url = (String)request.getAttribute("returl");
    response.sendRedirect(url);
}
catch(Exception e){
    
}
%>
<%
%>

${returl}<br/>
${visiter.id }<br/>
${visiter.name }<br/>
