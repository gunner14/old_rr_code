
<%@page import="com.xiaonei.wService.fmwidget.common.util.MyStringUtils"%>
<%@page import="com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetDictModel"%>
<%@page import="com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetRecordModel"%>
<%@page import="com.xiaonei.wService.fmwidget.common.data.logic.FakemacWidgetDictCacheLogic"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.wService.fmwidget.adapter.FakemacWidgetAdapter"%>
<%@page import="com.xiaonei.platform.core.opt.base.PassportManager"%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>

<%
System.out.println(" app/setController.jsp");
Integer chostid = PassportManager.getInstance().getHostIdFromCookie(request, response);
if(chostid == null || chostid == 0){
    out.println("login required");
    return;
}

String id = request.getParameter("id");
int id_n = (Integer)MyStringUtils.stringToObject(id,Integer.class);
if(id_n == 0){
    out.println("app_id required");
    return;
}
String content = request.getParameter("content");
FakemacWidgetDictModel dict = new FakemacWidgetDictModel();
dict.setApp_id((Integer)MyStringUtils.stringToObject(request.getParameter("app_id"),Integer.class));
dict.setApp_description(request.getParameter("app_description"));
dict.setApp_owner(request.getParameter("app_owner"));
dict.setApp_name(request.getParameter("app_name"));
dict.setId((Integer)MyStringUtils.stringToObject(request.getParameter("id"),Integer.class));
FakemacWidgetAdapter.getInstance().updateFakemacWidgetDictById(0,id_n,dict);
FakemacWidgetDictCacheLogic.getInstance().removeDicts(chostid);
response.sendRedirect("/demo/home");
%>
